/*
 * MD218A voice coil motor driver
 *
 *
 */

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/platform_device.h> 
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include "OV12830TRULYAF.h"
#include "../camera/kd_camera_hw.h"

#define LENS_I2C_BUSNUM 1
static struct i2c_board_info __initdata kd_lens_dev={ I2C_BOARD_INFO("OV12830TRULYAF", 0x1E)};//LINE <> <DATE20130626> <add sensor:ov8835,ov12830> wupingzhou
//static struct i2c_board_info __initdata kd_lens_dev={ I2C_BOARD_INFO("OV12830TRULYAF", 0x18)};


#define OV12830TRULYAF_DRVNAME "OV12830TRULYAF"
//#define OV12830TRULYAF_VCM_WRITE_ID           0x6c
#define OV12830TRULYAF_VCM_WRITE_ID           0x18

#define OV12830TRULYAF_DEBUG
#ifdef OV12830TRULYAF_DEBUG
#define OV12830TRULYAFDB printk
#else
#define OV12830TRULYAFDB(x,...)
#endif

//#define AF_LSC_MODE
#define AF_DLC_MODE

static spinlock_t g_OV12830TRULYAF_SpinLock;

static struct i2c_client * g_pstOV12830TRULYAF_I2Cclient = NULL;

static dev_t g_OV12830TRULYAF_devno;
static struct cdev * g_pOV12830TRULYAF_CharDrv = NULL;
static struct class *actuator_class = NULL;

static int  g_s4OV12830TRULYAF_Opened = 0;
static long g_i4MotorStatus = 0;
static long g_i4Dir = 0;
static unsigned long g_u4OV12830TRULYAF_INF = 0;
static unsigned long g_u4OV12830TRULYAF_MACRO = 1023;
static unsigned long g_u4TargetPosition = 0;
static unsigned long g_u4CurrPosition   = 0;

static int g_sr = 5;

static long g_i4Position = 0;
static unsigned long g_u4PrePosition   = 0;
static unsigned long g_u4PrePosition_Offset = 0; 
static unsigned long g_Pre_u4Src   = 7;


//extern s32 mt_set_gpio_mode(u32 u4Pin, u32 u4Mode);
//extern s32 mt_set_gpio_out(u32 u4Pin, u32 u4PinOut);
//extern s32 mt_set_gpio_dir(u32 u4Pin, u32 u4Dir);

//extern int iReadReg2( u8 * a_puBuff , u16 i2cId)
//extern int iWriteReg(u16 a_u2Addr , u32 a_u4Data , u32 a_u4Bytes , u16 i2cId);
//#define OV12830TRULYAF_write_cmos_sensor(addr, para) iWriteReg((u16) addr , (u32) para , 1, OV12830TRULYAF_VCM_WRITE_ID)
//extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);
//#define i2c_master_send(addr, para,size) iWriteRegI2C((u8 *)para , (u16) size, OV8826AF_VCM_WRITE_ID)


static int s4OV12830TRULYAF_ReadReg(unsigned short * a_pu2Result)
{
    int  i4RetValue = 0;
    char pBuff[2];

    i4RetValue = i2c_master_recv(g_pstOV12830TRULYAF_I2Cclient, pBuff , 2);
   // iReadReg2((u8*)&pBuff,OV8826AF_VCM_WRITE_ID);

    if (i4RetValue < 0) 
    {
        OV12830TRULYAFDB("[OV12830TRULYAF] I2C read failed!! \n");
        return -1;
    }

    *a_pu2Result = (((u16)pBuff[0]) << 4) + (pBuff[1] >> 4);

    return 0;
}

static int s4OV12830TRULYAF_WriteReg(u16 a_u2Data)
{
    int  i4RetValue = 0;

    char puSendCmd[2] = {(char)(a_u2Data >> 4) , (char)(((a_u2Data & 0xF) << 4)+g_sr)};

    OV12830TRULYAFDB("[OV12830TRULYAF] s4OV12830TRULYAF_WriteReg g_pstOV12830TRULYAF_I2Cclient->addr:%x\n",g_pstOV12830TRULYAF_I2Cclient->addr);
    //OV12830TRULYAFDB("[OV12830TRULYAF] g_sr %d, write %d \n", g_sr, a_u2Data);
    g_pstOV12830TRULYAF_I2Cclient->ext_flag |= I2C_A_FILTER_MSG;
    i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSendCmd, 2);
	
    if (i4RetValue < 0) 
    {
        OV12830TRULYAFDB("[OV12830TRULYAF] I2C send failed!! \n");
        return -1;
    }

	g_u4PrePosition = a_u2Data;

    return 0;
}

inline static int getOV12830TRULYAFInfo(__user stOV12830TRULYAF_MotorInfo * pstMotorInfo)
{
    stOV12830TRULYAF_MotorInfo stMotorInfo;
    stMotorInfo.u4MacroPosition   = g_u4OV12830TRULYAF_MACRO;
    stMotorInfo.u4InfPosition     = g_u4OV12830TRULYAF_INF;
    stMotorInfo.u4CurrentPosition = g_u4CurrPosition;
    stMotorInfo.bIsSupportSR      = TRUE;

	if (g_i4MotorStatus == 1)	{stMotorInfo.bIsMotorMoving = 1;}
	else						{stMotorInfo.bIsMotorMoving = 0;}

	if (g_s4OV12830TRULYAF_Opened >= 1)	{stMotorInfo.bIsMotorOpen = 1;}
	else						{stMotorInfo.bIsMotorOpen = 0;}

    if(copy_to_user(pstMotorInfo , &stMotorInfo , sizeof(stOV12830TRULYAF_MotorInfo)))
    {
        OV12830TRULYAFDB("[OV12830TRULYAF] copy to user failed when getting motor information \n");
    }

    return 0;
}

inline static int moveOV12830TRULYAF(unsigned long a_u4Position)
{
    int ret = 0;
    
    if(g_u4PrePosition > a_u4Position)
	{
		g_u4PrePosition_Offset = g_u4PrePosition - a_u4Position;
	}
    else
	{
		g_u4PrePosition_Offset = a_u4Position - g_u4PrePosition;

	}

#if 0
    if(g_u4PrePosition_Offset == 24)
	{
		g_sr = 6;
	}
    else  // Other Step
	{
		g_sr = 3;
	}
#endif


    
    if((a_u4Position > g_u4OV12830TRULYAF_MACRO) || (a_u4Position < g_u4OV12830TRULYAF_INF))
    {
        OV12830TRULYAFDB("[OV12830TRULYAF] out of range \n");
        return -EINVAL;
    }

    if (g_s4OV12830TRULYAF_Opened == 1)
    {
        unsigned short InitPos;
        ret = s4OV12830TRULYAF_ReadReg(&InitPos);
	    
        spin_lock(&g_OV12830TRULYAF_SpinLock);
        if(ret == 0)
        {
            OV12830TRULYAFDB("[OV12830TRULYAF] Init Pos %6d \n", InitPos);
            g_u4CurrPosition = (unsigned long)InitPos;
        }
        else
        {		
            g_u4CurrPosition = 0;
        }
        g_s4OV12830TRULYAF_Opened = 2;
        spin_unlock(&g_OV12830TRULYAF_SpinLock);
    }

    if (g_u4CurrPosition < a_u4Position)
    {
        spin_lock(&g_OV12830TRULYAF_SpinLock);	
        g_i4Dir = 1;
        spin_unlock(&g_OV12830TRULYAF_SpinLock);	
    }
    else if (g_u4CurrPosition > a_u4Position)
    {
        spin_lock(&g_OV12830TRULYAF_SpinLock);	
        g_i4Dir = -1;
        spin_unlock(&g_OV12830TRULYAF_SpinLock);			
    }
    else										{return 0;}

    spin_lock(&g_OV12830TRULYAF_SpinLock);    
	g_i4Position = (long)g_u4CurrPosition;
    g_u4TargetPosition = a_u4Position;
    spin_unlock(&g_OV12830TRULYAF_SpinLock);	

    //OV12830TRULYAFDB("[OV12830TRULYAF] move [curr] %d [target] %d\n", g_u4CurrPosition, g_u4TargetPosition);

            spin_lock(&g_OV12830TRULYAF_SpinLock);
            //g_sr = 6;
            g_i4MotorStatus = 0;
            spin_unlock(&g_OV12830TRULYAF_SpinLock);	
		
            if(s4OV12830TRULYAF_WriteReg((unsigned short)g_u4TargetPosition) == 0)
            {
                spin_lock(&g_OV12830TRULYAF_SpinLock);		
                g_u4CurrPosition = (unsigned long)g_u4TargetPosition;
                spin_unlock(&g_OV12830TRULYAF_SpinLock);				
            }
            else
            {
                OV12830TRULYAFDB("[OV12830TRULYAF] set I2C failed when moving the motor \n");			
                spin_lock(&g_OV12830TRULYAF_SpinLock);
                g_i4MotorStatus = -1;
                spin_unlock(&g_OV12830TRULYAF_SpinLock);				
            }

    return 0;
}

inline static int setOV12830TRULYAFInf(unsigned long a_u4Position)
{
    spin_lock(&g_OV12830TRULYAF_SpinLock);
    g_u4OV12830TRULYAF_INF = a_u4Position;
    spin_unlock(&g_OV12830TRULYAF_SpinLock);	
    return 0;
}

inline static int setOV12830TRULYAFMacro(unsigned long a_u4Position)
{
    spin_lock(&g_OV12830TRULYAF_SpinLock);
    g_u4OV12830TRULYAF_MACRO = a_u4Position;
    spin_unlock(&g_OV12830TRULYAF_SpinLock);	
    return 0;	
}

////////////////////////////////////////////////////////////////
static long OV12830TRULYAF_Ioctl(
struct file * a_pstFile,
unsigned int a_u4Command,
unsigned long a_u4Param)
{
    long i4RetValue = 0;

    switch(a_u4Command)
    {
        case OV12830TRULYAFIOC_G_MOTORINFO :
            i4RetValue = getOV12830TRULYAFInfo((__user stOV12830TRULYAF_MotorInfo *)(a_u4Param));
        break;

        case OV12830TRULYAFIOC_T_MOVETO :
            i4RetValue = moveOV12830TRULYAF(a_u4Param);
        break;
 
        case OV12830TRULYAFIOC_T_SETINFPOS :
            i4RetValue = setOV12830TRULYAFInf(a_u4Param);
        break;

        case OV12830TRULYAFIOC_T_SETMACROPOS :
            i4RetValue = setOV12830TRULYAFMacro(a_u4Param);
        break;
		
        default :
      	    OV12830TRULYAFDB("[OV12830TRULYAF] No CMD \n");
            i4RetValue = -EPERM;
        break;
    }

    return i4RetValue;
}

//Main jobs:
// 1.check for device-specified errors, device not ready.
// 2.Initialize the device if it is opened for the first time.
// 3.Update f_op pointer.
// 4.Fill data structures into private_data
//CAM_RESET
static int OV12830TRULYAF_Open(struct inode * a_pstInode, struct file * a_pstFile)
{
    OV12830TRULYAFDB("[OV12830TRULYAF] OV12830TRULYAF_Open - Start\n");

    spin_lock(&g_OV12830TRULYAF_SpinLock);
    long i4RetValue = 0;


    if(g_s4OV12830TRULYAF_Opened)
    {
        spin_unlock(&g_OV12830TRULYAF_SpinLock);
        OV12830TRULYAFDB("[OV12830TRULYAF] the device is opened \n");
        return -EBUSY;
    }

    #ifdef AF_DLC_MODE
    char puSuspendCmd[2] = {(char)(0xEC), (char)(0xA3)};
    i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSuspendCmd, 2);

    char puSuspendCmd1[2] = {(char)(0xA1), (char)(0x0D)};
    i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSuspendCmd1, 2);

    char puSuspendCmd2[2] = {(char)(0xF2), (char)(0xE8)};
    i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSuspendCmd2, 2);

    char puSuspendCmd3[2] = {(char)(0xDC), (char)(0x51)};
    i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSuspendCmd3, 2);

    #else //AF_LSC_MODE
    char puSendCmd[2] = {(char)(0xEC) , (char)(0xA3)};
   	i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSendCmd, 2);
    		
    char puSendCmd2[2] = {(char)(0xA1) , (char)(0x05)};
    i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSendCmd2, 2);
    		
    char puSendCmd3[2] = {(char)(0xF2) , (char)(0x90)};
    i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSendCmd3, 2);
    		
   	char puSendCmd4[2] = {(char)(0xDC) , (char)(0x51)};
    i4RetValue = i2c_master_send(g_pstOV12830TRULYAF_I2Cclient, puSendCmd4, 2);
    #endif

    g_s4OV12830TRULYAF_Opened = 1;
		
    spin_unlock(&g_OV12830TRULYAF_SpinLock);

    OV12830TRULYAFDB("[OV12830TRULYAF] OV12830TRULYAF_Open - End\n");

    return 0;
}

//Main jobs:
// 1.Deallocate anything that "open" allocated in private_data.
// 2.Shut down the device on last close.
// 3.Only called once on last time.
// Q1 : Try release multiple times.
static int OV12830TRULYAF_Release(struct inode * a_pstInode, struct file * a_pstFile)
{
    OV12830TRULYAFDB("[OV12830TRULYAF] OV12830TRULYAF_Release - Start\n");

    if (g_s4OV12830TRULYAF_Opened)
    {
        OV12830TRULYAFDB("[OV12830TRULYAF] feee \n");
        g_sr = 5;
	    s4OV12830TRULYAF_WriteReg(200);
        msleep(10);
	    s4OV12830TRULYAF_WriteReg(100);
        msleep(10);
            	            	    	    
        spin_lock(&g_OV12830TRULYAF_SpinLock);
        g_s4OV12830TRULYAF_Opened = 0;
        spin_unlock(&g_OV12830TRULYAF_SpinLock);

    }

    OV12830TRULYAFDB("[OV12830TRULYAF] OV12830TRULYAF_Release - End\n");

    return 0;
}

static const struct file_operations g_stOV12830TRULYAF_fops = 
{
    .owner = THIS_MODULE,
    .open = OV12830TRULYAF_Open,
    .release = OV12830TRULYAF_Release,
    .unlocked_ioctl = OV12830TRULYAF_Ioctl
};

inline static int Register_OV12830TRULYAF_CharDrv(void)
{
    struct device* vcm_device = NULL;

    OV12830TRULYAFDB("[OV12830TRULYAF] Register_OV12830TRULYAF_CharDrv - Start\n");

    //Allocate char driver no.
    if( alloc_chrdev_region(&g_OV12830TRULYAF_devno, 0, 1,OV12830TRULYAF_DRVNAME) )
    {
        OV12830TRULYAFDB("[OV12830TRULYAF] Allocate device no failed\n");

        return -EAGAIN;
    }

    //Allocate driver
    g_pOV12830TRULYAF_CharDrv = cdev_alloc();

    if(NULL == g_pOV12830TRULYAF_CharDrv)
    {
        unregister_chrdev_region(g_OV12830TRULYAF_devno, 1);

        OV12830TRULYAFDB("[OV12830TRULYAF] Allocate mem for kobject failed\n");

        return -ENOMEM;
    }

    //Attatch file operation.
    cdev_init(g_pOV12830TRULYAF_CharDrv, &g_stOV12830TRULYAF_fops);

    g_pOV12830TRULYAF_CharDrv->owner = THIS_MODULE;

    //Add to system
    if(cdev_add(g_pOV12830TRULYAF_CharDrv, g_OV12830TRULYAF_devno, 1))
    {
        OV12830TRULYAFDB("[OV12830TRULYAF] Attatch file operation failed\n");

        unregister_chrdev_region(g_OV12830TRULYAF_devno, 1);

        return -EAGAIN;
    }

//    actuator_class = class_create(THIS_MODULE, "actuatordrv");
    actuator_class = class_create(THIS_MODULE, "actuatordrv8");
    if (IS_ERR(actuator_class)) {
        int ret = PTR_ERR(actuator_class);
        OV12830TRULYAFDB("Unable to create class, err = %d\n", ret);
        return ret;            
    }

    vcm_device = device_create(actuator_class, NULL, g_OV12830TRULYAF_devno, NULL, OV12830TRULYAF_DRVNAME);

    if(NULL == vcm_device)
    {
        return -EIO;
    }
    
    OV12830TRULYAFDB("[OV12830TRULYAF] Register_OV12830TRULYAF_CharDrv - End\n");    
    return 0;
}

inline static void Unregister_OV12830TRULYAF_CharDrv(void)
{
    OV12830TRULYAFDB("[OV12830TRULYAF] Unregister_OV12830TRULYAF_CharDrv - Start\n");

    //Release char driver
    cdev_del(g_pOV12830TRULYAF_CharDrv);

    unregister_chrdev_region(g_OV12830TRULYAF_devno, 1);
    
    device_destroy(actuator_class, g_OV12830TRULYAF_devno);

    class_destroy(actuator_class);

    OV12830TRULYAFDB("[OV12830TRULYAF] Unregister_OV12830TRULYAF_CharDrv - End\n");    
}

//////////////////////////////////////////////////////////////////////

static int OV12830TRULYAF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int OV12830TRULYAF_i2c_remove(struct i2c_client *client);
static const struct i2c_device_id OV12830TRULYAF_i2c_id[] = {{OV12830TRULYAF_DRVNAME,0},{}};   
struct i2c_driver OV12830TRULYAF_i2c_driver = {                       
    .probe = OV12830TRULYAF_i2c_probe,                                   
    .remove = OV12830TRULYAF_i2c_remove,                           
    .driver.name = OV12830TRULYAF_DRVNAME,                 
    .id_table = OV12830TRULYAF_i2c_id,                             
};  

#if 0 
static int OV12830TRULYAF_i2c_detect(struct i2c_client *client, int kind, struct i2c_board_info *info) {         
    strcpy(info->type, OV12830TRULYAF_DRVNAME);                                                         
    return 0;                                                                                       
}      
#endif 
static int OV12830TRULYAF_i2c_remove(struct i2c_client *client) {
    return 0;
}

/* Kirby: add new-style driver {*/
static int OV12830TRULYAF_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int i4RetValue = 0;

    OV12830TRULYAFDB("[OV12830TRULYAF] OV12830TRULYAF_i2c_probe\n");

    /* Kirby: add new-style driver { */
    g_pstOV12830TRULYAF_I2Cclient = client;
    
    OV12830TRULYAFDB("[OV12830TRULYAF] g_pstOV12830TRULYAF_I2Cclient->addr:%x\n",g_pstOV12830TRULYAF_I2Cclient->addr);
    //g_pstOV12830TRULYAF_I2Cclient->addr = 0x6c;
    //g_pstOV12830TRULYAF_I2Cclient->addr = 0x18;    
    g_pstOV12830TRULYAF_I2Cclient->addr = (OV12830TRULYAF_VCM_WRITE_ID)   >> 1;//g_pstOV12830TRULYAF_I2Cclient->addr >> 1;//LINE <> <DATE20130626> <add sensor:ov8835,ov12830> wupingzhou
    OV12830TRULYAFDB("[OV12830TRULYAF] g_pstOV12830TRULYAF_I2Cclient->addr:%x\n",g_pstOV12830TRULYAF_I2Cclient->addr);
    
    //Register char driver
    i4RetValue = Register_OV12830TRULYAF_CharDrv();

    if(i4RetValue){

        OV12830TRULYAFDB("[OV12830TRULYAF] register char device failed!\n");

        return i4RetValue;
    }

    spin_lock_init(&g_OV12830TRULYAF_SpinLock);

    OV12830TRULYAFDB("[OV12830TRULYAF] Attached!! \n");

    return 0;
}

static int OV12830TRULYAF_probe(struct platform_device *pdev)
{
    return i2c_add_driver(&OV12830TRULYAF_i2c_driver);
}

static int OV12830TRULYAF_remove(struct platform_device *pdev)
{
    i2c_del_driver(&OV12830TRULYAF_i2c_driver);
    return 0;
}

static int OV12830TRULYAF_suspend(struct platform_device *pdev, pm_message_t mesg)
{
    return 0;
}

static int OV12830TRULYAF_resume(struct platform_device *pdev)
{
    return 0;
}

// platform structure
static struct platform_driver g_stOV12830TRULYAF_Driver = {
    .probe		= OV12830TRULYAF_probe,
    .remove	= OV12830TRULYAF_remove,
    .suspend	= OV12830TRULYAF_suspend,
    .resume	= OV12830TRULYAF_resume,
    .driver		= {
        .name	= "lens_actuator8",
        .owner	= THIS_MODULE,
    }
};

//LINE <> <DATE20130626> <add sensor:ov8835,ov12830> wupingzhou
static struct platform_device actuator_dev8 = {
	.name		  = "lens_actuator8",
	.id		  = -1,
};

static int __init OV12830TRULYAF_i2C_init(void)
{
    i2c_register_board_info(LENS_I2C_BUSNUM, &kd_lens_dev, 1);
    platform_device_register(&actuator_dev8);		//LIN//LINE <> <DATE20130626> <add sensor:ov8835,ov12830> wupingzhou

    if(platform_driver_register(&g_stOV12830TRULYAF_Driver)){
        OV12830TRULYAFDB("failed to register OV12830TRULYAF driver\n");
        return -ENODEV;
    }

    return 0;
}

static void __exit OV12830TRULYAF_i2C_exit(void)
{
	platform_driver_unregister(&g_stOV12830TRULYAF_Driver);
}

module_init(OV12830TRULYAF_i2C_init);
module_exit(OV12830TRULYAF_i2C_exit);

MODULE_DESCRIPTION("OV12830TRULYAF lens module driver");
MODULE_AUTHOR("KY Chen <ky.chen@Mediatek.com>");
MODULE_LICENSE("GPL");



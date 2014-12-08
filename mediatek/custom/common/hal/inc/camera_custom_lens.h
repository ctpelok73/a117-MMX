#ifndef __CAMERA_CUSTOM_LENS_H_
#define __CAMERA_CUSTOM_LENS_H_

//#include "../../../../inc/MediaTypes.h"
//#include "msdk_nvram_camera_exp.h"
//#include "camera_custom_nvram.h"

#define MAX_NUM_OF_SUPPORT_LENS                 16

#define DUMMY_SENSOR_ID                      0xFFFF


/* LENS ID */
#define DUMMY_LENS_ID                        0xFFFF
#define FM50AF_LENS_ID                       0x0001
#define MT9P017AF_LENS_ID                    0x0002
#define OV8825AF_LENS_ID					 0x0003
#define BU6429AF_LENS_ID					 0x0004
#define OV12830AF_LENS_ID					 0x0005
#define OV5647AF_LENS_ID					 0x0006
#define S5K3H2YXAF_LENS_ID					 0x0007
#define A5142AF_LENS_ID					 0x0008
#define IMX111_LENS_ID					 0x0009
#define AR0833AF_LENS_ID 0x000A

#define SENSOR_DRIVE_LENS_ID                 0x1000
#define BU6429AF_LENS_ID					 0x000A
#define BU6424AF_LENS_ID					 0x000B
#define AD5823_LENS_ID					   0x5823
#define AD5823AF_LENS_ID					 0x5823
#define DW9718AF_LENS_ID 					 0x9718
#define AD5820AF_LENS_ID					 0x5820
#define DW9714AF_LENS_ID 					 0x9714
#define OV8825TRULYAF_LENS_ID				 0x0006//LINE <> <DATE20130307> <add sensor:truly ov8825> wupingzhou
#define OV8826AF_LENS_ID				     0x0007//LINE <> <DATE20130408> <add sensor:sunny ov8825 MTM> wupingzhou
#define IMX179AF_LENS_ID				     0x0008	//LINE <> <20130523> <add sensor:IMX179> lishengli
#define OV8835AF_LENS_ID					 0x0009//LINE <> <DATE20130626> <add sensor:ov8835,ov12830> wupingzhou
#define OV12830AF_LENS_ID					 0x000a//LINE <> <DATE20130626> <add sensor:ov8835,ov12830> wupingzhou
#define OV12830TRULYAF_LENS_ID			     0x000b//LINE <> <DATE20130715> <add sensor:truly ov12830> wupingzhou

/* AF LAMP THRESHOLD*/
#define AF_LAMP_LV_THRES 60


typedef UINT32(*PFUNC_GETLENSDEFAULT)(VOID*, UINT32);

typedef struct
{
    UINT32 SensorId;
    UINT32 LensId;
    UINT8  LensDrvName[32];
    UINT32 (*getLensDefault)(VOID *pDataBuf, UINT32 size);

} MSDK_LENS_INIT_FUNCTION_STRUCT, *PMSDK_LENS_INIT_FUNCTION_STRUCT;


UINT32 GetLensInitFuncList(PMSDK_LENS_INIT_FUNCTION_STRUCT pLensList);

#endif /* __MSDK_LENS_EXP_H */





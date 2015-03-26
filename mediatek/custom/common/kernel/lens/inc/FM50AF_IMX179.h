#ifndef _FM50AF_IMX179_H
#define _FM50AF_IMX179_H

#include <linux/ioctl.h>
//#include "kd_imgsensor.h"

#define FM50AF_IMX179_MAGIC 'A'
//IOCTRL(inode * ,file * ,cmd ,arg )


//Structures
typedef struct {
//current position
unsigned long u4CurrentPosition;
//macro position
unsigned long u4MacroPosition;
//Infiniti position
unsigned long u4InfPosition;
//Motor Status
bool          bIsMotorMoving;
//Motor Open?
bool          bIsMotorOpen;
//Support SR?
bool          bIsSupportSR;
} stFM50AF_IMX179_MotorInfo;

//Control commnad
//S means "set through a ptr"
//T means "tell by a arg value"
//G means "get by a ptr"             
//Q means "get by return a value"
//X means "switch G and S atomically"
//H means "switch T and Q atomically"
#define FM50AF_IMX179IOC_G_MOTORINFO _IOR(FM50AF_IMX179_MAGIC,0,stFM50AF_IMX179_MotorInfo)

#define FM50AF_IMX179IOC_T_MOVETO _IOW(FM50AF_IMX179_MAGIC,1,unsigned long)

#define FM50AF_IMX179IOC_T_SETINFPOS _IOW(FM50AF_IMX179_MAGIC,2,unsigned long)

#define FM50AF_IMX179IOC_T_SETMACROPOS _IOW(FM50AF_IMX179_MAGIC,3,unsigned long)

#else
#endif

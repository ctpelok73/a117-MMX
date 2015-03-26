#include <utils/Log.h>
#include <utils/Errors.h>
#include <fcntl.h>
#include <math.h>

#include "MediaHal.h"

//#include "lens_custom_cfg.h"
//#include "msdk_lens_exp.h"
#include "camera_custom_lens.h"
//#include "lens.h"
//nclude "image_sensor.h"
#include "kd_imgsensor.h"

extern PFUNC_GETLENSDEFAULT pDummy_getDefaultData;

#if defined(SENSORDRIVE)
extern PFUNC_GETLENSDEFAULT pSensorDrive_getDefaultData;
#endif

#if defined(FM50AF_IMX179)
extern PFUNC_GETLENSDEFAULT pFM50AF_getDefaultData;
#endif

#if defined(FM50AF)
extern PFUNC_GETLENSDEFAULT pFM50AF_getDefaultData;
#endif

#if defined(MT9P017AF)
extern PFUNC_GETLENSDEFAULT pMT9P017AF_getDefaultData;
#endif

#if defined(OV8825AF)
extern PFUNC_GETLENSDEFAULT pOV8825AF_getDefaultData;
#endif

#if defined(OV12830AF)
extern PFUNC_GETLENSDEFAULT pOV12830AF_getDefaultData;
#endif

#if defined(OV5647AF)
extern PFUNC_GETLENSDEFAULT pOV5647AF_getDefaultData;
#endif

#if defined(S5K3H2YXAF)
extern PFUNC_GETLENSDEFAULT pS5K3H2YXAF_getDefaultData;
#endif

#if defined(A5142AF)
extern PFUNC_GETLENSDEFAULT pA5142AF_getDefaultData;
#endif

#if defined(IMX111AF)
extern PFUNC_GETLENSDEFAULT pIMX111AF_getDefaultData;
#endif

#if defined(AR0833AF)
extern PFUNC_GETLENSDEFAULT pAR0833AF_getDefaultData;
#endif

MSDK_LENS_INIT_FUNCTION_STRUCT LensList[MAX_NUM_OF_SUPPORT_LENS] =
{
	{DUMMY_SENSOR_ID, DUMMY_LENS_ID, "Dummy", pDummy_getDefaultData},

#if defined(SENSORDRIVE)
	{DUMMY_SENSOR_ID, SENSOR_DRIVE_LENS_ID, "kd_camera_hw", pSensorDrive_getDefaultData},	

    //  for backup lens, need assign correct SensorID
    //{OV5642_SENSOR_ID, SENSOR_DRIVE_LENS_ID, "kd_camera_hw", pSensorDrive_getDefaultData},
#endif

#if defined(FM50AF_IMX179)
		{FM50AF_IMX179_SENSOR_ID, FM50AF_IMX179_LENS_ID, "OV12830AF", pFM50AF_IMX179_getDefaultData},
#endif

#if defined(OV8825AF)
		{OV8825_SENSOR_ID, OV8825AF_LENS_ID, "OV8825AF", pOV8825AF_getDefaultData},
#endif

#if defined(OV12830AF)
		{OV12830_SENSOR_ID, OV12830AF_LENS_ID, "OV12830AF", pOV12830AF_getDefaultData},
#endif

#if defined(FM50AF)
	{DUMMY_SENSOR_ID, FM50AF_LENS_ID, "FM50AF", pFM50AF_getDefaultData},
#endif

#if defined(FM50AF)
	{OV5648MIPI_SENSOR_ID,  FM50AF_LENS_ID,        "FM50AF", pFM50AF_getDefaultData},
#endif

#if defined(OV5647AF)
		{OV5647_SENSOR_ID, OV5647AF_LENS_ID, "OV5647AF", pOV5647AF_getDefaultData},
#endif

    //  for new added lens, need assign correct SensorID
#if defined(MT9P017AF)
	{MT9P017MIPI_SENSOR_ID, MT9P017AF_LENS_ID, "MT9P017AF", pMT9P017AF_getDefaultData},
#endif

#if defined(S5K3H2YXAF)
	{S5K3H2YX_SENSOR_ID, S5K3H2YXAF_LENS_ID, "S5K3H2YXAF", pS5K3H2YXAF_getDefaultData},
#endif

#if defined(A5142AF)
	{A5142MIPI_SENSOR_ID, A5142AF_LENS_ID, "A5142AF", pA5142AF_getDefaultData},
	//{A5142MIPI_SENSOR_ID, DUMMY_LENS_ID, "Dummy", pDummy_getDefaultData},
#endif

#if defined(IMX111AF)
	{IMX111_SENSOR_ID, IMX111_LENS_ID, "IMX111AF", pIMX111AF_getDefaultData},
	//{IMX111_SENSOR_ID, DUMMY_LENS_ID, "Dummy", pDummy_getDefaultData},
#endif

#if defined(AR0833AF)
	{AR0833_SENSOR_ID, AR0833AF_LENS_ID, "AR0833AF", pAR0833AF_getDefaultData},
#endif

};

UINT32 GetLensInitFuncList(PMSDK_LENS_INIT_FUNCTION_STRUCT pLensList)
{
    memcpy(pLensList, &LensList[0], sizeof(MSDK_LENS_INIT_FUNCTION_STRUCT)* MAX_NUM_OF_SUPPORT_LENS);
    return MHAL_NO_ERROR;
} // GetLensInitFuncList()







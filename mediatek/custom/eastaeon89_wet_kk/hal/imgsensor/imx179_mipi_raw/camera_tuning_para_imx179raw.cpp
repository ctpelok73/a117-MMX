#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_imx179raw.h"
#include "camera_info_imx179raw.h"
#include "camera_custom_AEPlinetable.h"
const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    },
    ISPPca:{
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
        },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
        }
    }},
    ISPCcmPoly22:{
        79225,    // i4R_AVG
        18403,    // i4R_STD
        114750,    // i4B_AVG
        32053,    // i4B_STD
        {  // i4P00[9]
            4162500, -1725000, 120000, -585000, 3225000, -80000, 272500, -1802500, 4090000
        },
        {  // i4P10[9]
            1032222, -1066863, 42450, 66160, -203342, 128705, 31906, 219923, -251829
        },
        {  // i4P01[9]
            793663, -832795, 46090, -132028, -201920, 330830, 23994, -374954, 350960
        },
        {  // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1160,    // u4MinGain, 1024 base = 1x
            8192,    // u4MaxGain, 16x
            50,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            14,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            14,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            14,    // u4CapExpUnit 
            23,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            20,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            200,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 170, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 90, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            TRUE,    // bEnableCaptureThres
            TRUE,    // bEnableVideoThres
            TRUE,    // bEnableStrobeThres
            47,    // u4AETarget
            47,    // u4StrobeAETarget
            50,    // u4InitIndex
            10,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -6,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            32,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            160,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            160,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            75    // u4FlatnessStrength
        }
    },
    // AWB NVRAM
    {
        // AWB calibration data
        {
            // rUnitGain (unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rGoldenGain (golden sample gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                1025,    // i4R
                512,    // i4G
                718    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                48,    // i4X
                -456    // i4Y
            },
            // Horizon
            {
                -460,    // i4X
                -466    // i4Y
            },
            // A
            {
                -334,    // i4X
                -460    // i4Y
            },
            // TL84
            {
                -177,    // i4X
                -425    // i4Y
            },
            // CWF
            {
                -127,    // i4X
                -522    // i4Y
            },
            // DNP
            {
                -29,    // i4X
                -429    // i4Y
            },
            // D65
            {
                131,    // i4X
                -381    // i4Y
            },
            // DF
            {
                79,    // i4X
                -478    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                -24,    // i4X
                -458    // i4Y
            },
            // Horizon
            {
                -527,    // i4X
                -389    // i4Y
            },
            // A
            {
                -402,    // i4X
                -402    // i4Y
            },
            // TL84
            {
                -241,    // i4X
                -392    // i4Y
            },
            // CWF
            {
                -207,    // i4X
                -496    // i4Y
            },
            // DNP
            {
                -96,    // i4X
                -419    // i4Y
            },
            // D65
            {
                70,    // i4X
                -397    // i4Y
            },
            // DF
            {
                3,    // i4X
                -520    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                1013,    // i4R
                512,    // i4G
                890    // i4B
            },
            // Horizon 
            {
                516,    // i4R
                512,    // i4G
                1792    // i4B
            },
            // A 
            {
                607,    // i4R
                512,    // i4G
                1499    // i4B
            },
            // TL84 
            {
                717,    // i4R
                512,    // i4G
                1156    // i4B
            },
            // CWF 
            {
                874,    // i4R
                512,    // i4G
                1233    // i4B
            },
            // DNP 
            {
                879,    // i4R
                512,    // i4G
                952    // i4B
            },
            // D65 
            {
                1025,    // i4R
                512,    // i4G
                718    // i4B
            },
            // DF 
            {
                1090,    // i4R
                512,    // i4G
                879    // i4B
            }
        },
        // Rotation matrix parameter
        {
            9,    // i4RotationAngle
            253,    // i4Cos
            40    // i4Sin
        },
        // Daylight locus parameter
        {
            -174,    // i4SlopeNumerator
            128    // i4SlopeDenominator
        },
        // AWB light area
        {
            // Strobe:FIXME
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            },
            // Tungsten
            {
            -291,    // i4RightBound
            -941,    // i4LeftBound
            -345,    // i4UpperBound
            -445    // i4LowerBound
            },
            // Warm fluorescent
            {
            -291,    // i4RightBound
            -941,    // i4LeftBound
            -445,    // i4UpperBound
            -565    // i4LowerBound
            },
            // Fluorescent
            {
            -146,    // i4RightBound
            -291,    // i4LeftBound
            -327,    // i4UpperBound
            -444    // i4LowerBound
            },
            // CWF
            {
            -146,    // i4RightBound
            -291,    // i4LeftBound
            -444,    // i4UpperBound
            -546    // i4LowerBound
            },
            // Daylight
            {
            95,    // i4RightBound
            -146,    // i4LeftBound
            -317,    // i4UpperBound
            -477    // i4LowerBound
            },
            // Shade
            {
            455,    // i4RightBound
            95,    // i4LeftBound
            -317,    // i4UpperBound
            -477    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            95,    // i4RightBound
            -146,    // i4LeftBound
            -477,    // i4UpperBound
            -565    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            455,    // i4RightBound
            -941,    // i4LeftBound
            -292,    // i4UpperBound
            -565    // i4LowerBound
            },
            // Daylight
            {
            120,    // i4RightBound
            -146,    // i4LeftBound
            -317,    // i4UpperBound
            -477    // i4LowerBound
            },
            // Cloudy daylight
            {
            220,    // i4RightBound
            45,    // i4LeftBound
            -317,    // i4UpperBound
            -477    // i4LowerBound
            },
            // Shade
            {
            320,    // i4RightBound
            45,    // i4LeftBound
            -317,    // i4UpperBound
            -477    // i4LowerBound
            },
            // Twilight
            {
            -146,    // i4RightBound
            -306,    // i4LeftBound
            -317,    // i4UpperBound
            -477    // i4LowerBound
            },
            // Fluorescent
            {
            120,    // i4RightBound
            -341,    // i4LeftBound
            -342,    // i4UpperBound
            -546    // i4LowerBound
            },
            // Warm fluorescent
            {
            -302,    // i4RightBound
            -502,    // i4LeftBound
            -342,    // i4UpperBound
            -546    // i4LowerBound
            },
            // Incandescent
            {
            -302,    // i4RightBound
            -502,    // i4LeftBound
            -317,    // i4UpperBound
            -477    // i4LowerBound
            },
            // Gray World
            {
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
            }
        },
        // PWB default gain	
        {
            // Daylight
            {
            933,    // i4R
            512,    // i4G
            817    // i4B
            },
            // Cloudy daylight
            {
            1099,    // i4R
            512,    // i4G
            652    // i4B
            },
            // Shade
            {
            1162,    // i4R
            512,    // i4G
            603    // i4B
            },
            // Twilight
            {
            734,    // i4R
            512,    // i4G
            1135    // i4B
            },
            // Fluorescent
            {
            899,    // i4R
            512,    // i4G
            1001    // i4B
            },
            // Warm fluorescent
            {
            648,    // i4R
            512,    // i4G
            1572    // i4B
            },
            // Incandescent
            {
            602,    // i4R
            512,    // i4G
            1491    // i4B
            },
            // Gray World
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        // AWB preference color	
        {
            // Tungsten
            {
            0,    // i4SliderValue
            7524    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5975    // i4OffsetThr
            },
            // Shade
            {
            50,    // i4SliderValue
            344    // i4OffsetThr
            },
            // Daylight WB gain
            {
            882,    // i4R
            512,    // i4G
            584    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            480,    // i4R
            512,    // i4G
            520    // i4B
            },
            // Preference gain: warm fluorescent
            {
            500,    // i4R
            512,    // i4G
            520    // i4B
            },
            // Preference gain: fluorescent
            {
            498,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            520,    // i4R
            512,    // i4G
            525    // i4B
            },
            // Preference gain: daylight
            {
            512,    // i4R
            512,    // i4G
            504    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            512,    // i4R
            512,    // i4G
            504    // i4B
            }
        },
        {// CCT estimation
            {// CCT
                2300,    // i4CCT[0]
                2850,    // i4CCT[1]
                4100,    // i4CCT[2]
                5100,    // i4CCT[3]
                6500    // i4CCT[4]
            },
            {// Rotated X coordinate
                -597,    // i4RotatedXCoordinate[0]
                -472,    // i4RotatedXCoordinate[1]
                -311,    // i4RotatedXCoordinate[2]
                -166,    // i4RotatedXCoordinate[3]
                0    // i4RotatedXCoordinate[4]
            }
        }
    },
    {0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T)};

    if (CameraDataType > CAMERA_DATA_AE_PLINETABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        default:
            break;
    }
    return 0;
}}; // NSFeature



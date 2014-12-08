/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved. 
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov12830raw_truly.h"
#include "camera_info_ov12830raw_truly.h"
#include "camera_custom_AEPlinetable.h"
//#include "camera_isp_lsc_ov12830raw_truly.h"

const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE_TRULY =
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
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    	}
    },
    ISPPca: {
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
        63900,    // i4R_AVG
        14309,    // i4R_STD
        94833,    // i4B_AVG
        22962,    // i4B_STD
        {  // i4P00[9]
            5066667, -2710000, 210000, -680000, 4033333, -793333, -193333, -2643333, 5396667
        },
        {  // i4P10[9]
            1631477, -684057, -940194, -759904, 1221647, -461744, -153077, 1946446, -1793369
        },
        {  // i4P01[9]
            1340273, -417921, -909853, -858538, 1136165, -277627, -248516, 1038305, -789789
        },
        { // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        { // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        { // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }        
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE_TRULY =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1195,   // u4MinGain, 1024 base =  1x
            16384,  // u4MaxGain, 16x
            100,     // u4MiniISOGain, ISOxx
            128,    // u4GainStepUnit, 1x/8
            33,     // u4PreExpUnit
            30,     // u4PreMaxFrameRate
            33,     // u4VideoExpUnit
            30,     // u4VideoMaxFrameRate
            1024,   // u4Video2PreRatio, 1024 base = 1x
            17,    // u4CapExpUnit 
            18,    // u4CapMaxFrameRate
            1024,   // u4Cap2PreRatio, 1024 base = 1x
            24,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
         },
         // rHistConfig
        {
            2,   // u4HistHighThres
            32,  // u4HistLowThres
            2,   // u4MostBrightRatio
            1,   // u4MostDarkRatio
            160, // u4CentralHighBound
            20,  // u4CentralLowBound
            {240, 230, 220, 210, 200}, // u4OverExpThres[AE_CCT_STRENGTH_NUM]
            {86, 108, 128, 148, 200},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 39}       // u4BlackLightThres[AE_CCT_STRENGTH_NUM]
        },        
		// rCCTConfig
		{
			TRUE,			 // bEnableBlackLight
			TRUE,			 // bEnableHistStretch
			FALSE,			 // bEnableAntiOverExposure
			TRUE,			 // bEnableTimeLPF
			FALSE,			  // bEnableCaptureThres
			FALSE,			  // bEnableVideoThres
			FALSE,			  // bEnableStrobeThres
			47, 			   // u4AETarget
			47, 			   // u4StrobeAETarget
			50, 			   // u4InitIndex
			36, 				// u4BackLightWeight
			37, 			   // u4HistStretchWeight
			4,				   // u4AntiOverExpWeight
			4,				   // u4BlackLightStrengthIndex
            4,    // u4HistStretchStrengthIndex
			2,				   // u4AntiOverExpStrengthIndex
			2,				   // u4TimeLPFStrengthIndex
			{1, 3, 5, 7, 8}, // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM]
			90, 			   // u4InDoorEV = 9.0, 10 base
			-8,    // i4BVOffset delta BV = value/10 
			64, 				// u4PreviewFlareOffset
			64, 				// u4CaptureFlareOffset
			5,				   // u4CaptureFlareThres
			64, 				// u4VideoFlareOffset
			5,				   // u4VideoFlareThres
			64, 				// u4StrobeFlareOffset
			2,				   // u4StrobeFlareThres
			8,				   // u4PrvMaxFlareThres
			0,				   // u4PrvMinFlareThres
			8,				   // u4VideoMaxFlareThres
			0,				   // u4VideoMinFlareThres			  
			8, 			   // u4FlatnessThres			   // 10 base for flatness condition.
			75				   // u4FlatnessStrength
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
                801,    // i4R
                512,    // i4G
                682    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                7,    // i4X
                -322    // i4Y
            },
            // Horizon
            {
                -439,    // i4X
                -265    // i4Y
            },
            // A
            {
                -314,    // i4X
                -287    // i4Y
            },
            // TL84
            {
                -180,    // i4X
                -275    // i4Y
            },
            // CWF
            {
                -146,    // i4X
                -362    // i4Y
            },
            // DNP
            {
                -92,    // i4X
                -301    // i4Y
            },
            // D65
            {
                59,    // i4X
                -271    // i4Y
            },
            // DF
            {
                -22,    // i4X
                -350    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                2,    // i4X
                -322    // i4Y
            },
            // Horizon
            {
                -443,    // i4X
                -258    // i4Y
            },
            // A
            {
                -318,    // i4X
                -282    // i4Y
            },
            // TL84
            {
                -184,    // i4X
                -272    // i4Y
            },
            // CWF
            {
                -152,    // i4X
                -360    // i4Y
            },
            // DNP
            {
                -97,    // i4X
                -300    // i4Y
            },
            // D65
            {
                55,    // i4X
                -272    // i4Y
            },
            // DF
            {
                -27,    // i4X
                -350    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                800,    // i4R
                512,    // i4G
                784    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                648,    // i4G
                1679    // i4B
            },
            // A 
            {
                512,    // i4R
                531,    // i4G
                1198    // i4B
            },
            // TL84 
            {
                582,    // i4R
                512,    // i4G
                948    // i4B
            },
            // CWF 
            {
                685,    // i4R
                512,    // i4G
                1018    // i4B
            },
            // DNP 
            {
                679,    // i4R
                512,    // i4G
                871    // i4B
            },
            // D65 
            {
                801,    // i4R
                512,    // i4G
                682    // i4B
            },
            // DF 
            {
                798,    // i4R
                512,    // i4G
                847    // i4B
            }
        },
        // Rotation matrix parameter
        {
            1,    // i4RotationAngle
            256,    // i4Cos
            4    // i4Sin
        },
        // Daylight locus parameter
        {
            -130,    // i4SlopeNumerator
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
            -234,    // i4RightBound
            -884,    // i4LeftBound
            -215,    // i4UpperBound
            -320    // i4LowerBound
            },
            // Warm fluorescent
            {
            -234,    // i4RightBound
            -884,    // i4LeftBound
            -320,    // i4UpperBound
            -440    // i4LowerBound
            },
            // Fluorescent
            {
            -147,    // i4RightBound
            -234,    // i4LeftBound
            -206,    // i4UpperBound
            -316    // i4LowerBound
            },
            // CWF
            {
            -147,    // i4RightBound
            -234,    // i4LeftBound
            -316,    // i4UpperBound
            -410    // i4LowerBound
            },
            // Daylight
            {
            80,    // i4RightBound
            -147,    // i4LeftBound
            -192,    // i4UpperBound
            -372    // i4LowerBound
            },
            // Shade
            {
            440,    // i4RightBound
            80,    // i4LeftBound
            -192,    // i4UpperBound
            -352    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            80,    // i4RightBound
            -147,    // i4LeftBound
            -372,    // i4UpperBound
            -492    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            440,    // i4RightBound
            -884,    // i4LeftBound
            0,    // i4UpperBound
            -492    // i4LowerBound
            },
            // Daylight
            {
            105,    // i4RightBound
            -147,    // i4LeftBound
            -192,    // i4UpperBound
            -372    // i4LowerBound
            },
            // Cloudy daylight
            {
            205,    // i4RightBound
            30,    // i4LeftBound
            -192,    // i4UpperBound
            -372    // i4LowerBound
            },
            // Shade
            {
            305,    // i4RightBound
            30,    // i4LeftBound
            -192,    // i4UpperBound
            -372    // i4LowerBound
            },
            // Twilight
            {
            -147,    // i4RightBound
            -307,    // i4LeftBound
            -192,    // i4UpperBound
            -372    // i4LowerBound
            },
            // Fluorescent
            {
            105,    // i4RightBound
            -284,    // i4LeftBound
            -222,    // i4UpperBound
            -410    // i4LowerBound
            },
            // Warm fluorescent
            {
            -218,    // i4RightBound
            -418,    // i4LeftBound
            -222,    // i4UpperBound
            -410    // i4LowerBound
            },
            // Incandescent
            {
            -218,    // i4RightBound
            -418,    // i4LeftBound
            -192,    // i4UpperBound
            -372    // i4LowerBound
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
            734,    // i4R
            512,    // i4G
            767    // i4B
            },
            // Cloudy daylight
            {
            882,    // i4R
            512,    // i4G
            634    // i4B
            },
            // Shade
            {
            943,    // i4R
            512,    // i4G
            592    // i4B
            },
            // Twilight
            {
            558,    // i4R
            512,    // i4G
            1018    // i4B
            },
            // Fluorescent
            {
            702,    // i4R
            512,    // i4G
            882    // i4B
            },
            // Warm fluorescent
            {
            518,    // i4R
            512,    // i4G
            1208    // i4B
            },
            // Incandescent
            {
            494,    // i4R
            512,    // i4G
            1154    // i4B
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
            5845    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            4937    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1341    // i4OffsetThr
            },
            // Daylight WB gain
            {
            654,    // i4R
            512,    // i4G
            841    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            517,    // i4R
            519,    // i4G
            504    // i4B
            },
            // Preference gain: warm fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight
            {
            506,    // i4R
            512,    // i4G
            509    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            500,    // i4R
            512,    // i4G
            500    // i4B
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
                -498,    // i4RotatedXCoordinate[0]
                -373,    // i4RotatedXCoordinate[1]
                -239,    // i4RotatedXCoordinate[2]
                -152,    // i4RotatedXCoordinate[3]
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
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE_TRULY,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE_TRULY,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE_TRULY,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        default:
            break;
    }
    return 0;
}};  //  NSFeature



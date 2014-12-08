#ifndef TOUCHPANEL_H__
#define TOUCHPANEL_H__

/* Pre-defined definition */

/* Register */
//#define FD_ADDR_MAX    	0xE9
//#define FD_ADDR_MIN    	0xDD
//#define FD_BYTE_COUNT 	6

#define CUSTOM_MAX_WIDTH (720)		//add by lishengli 20130514
#define CUSTOM_MAX_HEIGHT (1280)

//#define TPD_UPDATE_FIRMWARE

#define TPD_HAVE_BUTTON
#define TPD_BUTTON_HEIGH        (140)				//add by lishengli 20130514
#define TPD_KEY_COUNT           3
#define TPD_KEYS                { KEY_MENU, KEY_HOMEPAGE, KEY_BACK}		//add by lishengli 20130514
#define TPD_KEYS_DIM		{{140,1350,150,TPD_BUTTON_HEIGH},\
							{380,1350,150,TPD_BUTTON_HEIGH},\
							{600,1350,150,TPD_BUTTON_HEIGH}}
/*
#define TPD_KEYS_DIM		{{145,1330,120,TPD_BUTTON_HEIGH},\
							{360,1330,120,TPD_BUTTON_HEIGH},\
							{600,1330,120,TPD_BUTTON_HEIGH}}
*/							

#define TPD_POWER_SOURCE         MT65XX_POWER_LDO_VGP4
#define LCD_X           720			//add by lishengli 20130514
#define LCD_Y           1280

#define TPD_UPDATE_FIRMWARE
#define HAVE_TOUCH_KEY

//#define TPD_HAVE_CALIBRATION
//#define TPD_CALIBRATION_MATRIX  {2680,0,0,0,2760,0,0,0};
//#define TPD_WARP_START
//#define TPD_WARP_END

//#define TPD_RESET_ISSUE_WORKAROUND
//#define TPD_MAX_RESET_COUNT 3
//#define TPD_WARP_Y(y) ( TPD_Y_RES - 1 - y )
//#define TPD_WARP_X(x) ( x )




//LINE<JIRA_ID><DATE20130422><BUG_INFO>zenghaihui
#if 0
#define CTP_DBG(fmt, arg...) \
	printk("[CTP-S3202] %s (line:%d) :" fmt "\r\n", __func__, __LINE__, ## arg)
#else
#define CTP_DBG(fmt, arg...) do {} while (0)
#endif


#endif /* TOUCHPANEL_H__ */

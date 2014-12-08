#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include "musbfsh_core.h"
#include "musbfsh_mt65xx.h"
#include "mach/emi_mpu.h"
#include "usb.h" 

#define FRA (48)
#define PARA (25)

bool musbfsh_power = false;

#ifdef MTK_ICUSB_SUPPORT

void mt65xx_usb11_mac_phy_dump(void);
void reset_usb11_phy_power_negotiation_status(void);
void musbfsh_root_disc_procedure(void);
void set_usb11_enabled(void);
void mt65xx_usb11_enable_clk_pll_pw(void);
void musbfsh_mac_init(void);

enum PHY_VOLTAGE_TYPE g_phy_voltage;
extern struct musbfsh  *g_musbfsh;
extern struct my_attr skip_mac_init_attr;

int common_regs_16[] = {0x00,0x01,0x02,0x04,0x06,0x08,0x0A,0x0B,0x0C,0x0E,0x0F};
int version_regs_16[] = {0x60,0x61,0x62,0x63,0x64,0x66,0x6C,0x6E};
int hwconfig_regs_16[] = {0x70,0x72,0x74,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F};
int toggle_regs_32[] = {0x80,0x84};
int level1_regs_32[] = {0xA0,0xA4,0xA8,0xAC};
int epcsr_regs_16_1[] = {0x102,0x108,0x10B,0x10F};
int epcsr_regs_16_2[] = {0x100,0x102,0x104,0x106,0x108,0x10A,0x10B,0x10C,0x10D,0x10F};
int dma_regs_32_1[] = {0x200,0x210,0x220};
int dma_regs_32_2[] = {0x204,0x208,0x20C,0x280,0x284};
int hhub_regs_32[] = {0x480,0x482,0x484,0x486};
int debug_regs_32[] = {0x600,0x604,0x605,0x606,0x610,0x612,0x614,0x616,0x620,0x622,0x624,0x626,0x628,0x62A,
	0x62B,0x62D,0x630,0x632,0x634,0x636,0x640,0x684,0x688,0x690,0x700};
int phy_regs_32[] = {0x00, 0x04, 0x08, 0x10, 0x14, 0x18, 0x1c, 0x20, 0x60, 0x64, 0x68, 0x6c, 0x70, 0x74, 0x78, 0x7c, 0xfc};
int phy_regs_32_port0[] = {0x00, 0x04, 0x08, 0x18};

void mt65xx_usb11_mac_phy_dump(void)
{
#if 1
	struct musbfsh *musbfsh = g_musbfsh;
	int i, j;

	MYDBG("");
	i = 0; j = 0;
	WARNING("MAC register dump!\n");
	printk("common register region:\n");
	for (i=0; i<sizeof(common_regs_16)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", common_regs_16[i], musbfsh_readw(musbfsh->mregs,common_regs_16[i]));
	}

	printk("OTG/D FIFO/Version registers region:\n");
	for (i=0; i<sizeof(version_regs_16)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", version_regs_16[i], musbfsh_readw(musbfsh->mregs,version_regs_16[i]));
	}
	printk("HW configuration registers region:\n");
	for (i=0; i<sizeof(hwconfig_regs_16)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", hwconfig_regs_16[i], musbfsh_readw(musbfsh->mregs,hwconfig_regs_16[i]));
	}

	printk("Toggle registers region:\n");
	for (i=0; i<sizeof(toggle_regs_32)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", toggle_regs_32[i], musbfsh_readl(musbfsh->mregs,toggle_regs_32[i]));
	}

	printk("Level1 registers region:\n");
	for (i=0; i<sizeof(level1_regs_32)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", level1_regs_32[i], musbfsh_readl(musbfsh->mregs,level1_regs_32[i]));
	}

	printk("EP CSR registers region:\n");
	for (i=0; i<sizeof(epcsr_regs_16_1)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", epcsr_regs_16_1[i], musbfsh_readw(musbfsh->mregs,epcsr_regs_16_1[i]));
	}

	for (i=0; i<=sizeof(epcsr_regs_16_2)/sizeof(int); i++) {
		for (j=1; j<=5; j++) {
			printk("offset=%x, value=%x\n", epcsr_regs_16_2[i]+j*0x10, musbfsh_readw(musbfsh->mregs,epcsr_regs_16_2[i]+j*0x10));
		}
	}

	printk("DMA registers region:\n");
	for (i=0; i<sizeof(dma_regs_32_1)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", dma_regs_32_1[i], musbfsh_readl(musbfsh->mregs,dma_regs_32_1[i]));
	}
	for (i=0; i<=sizeof(dma_regs_32_2)/sizeof(int); i++) {
		for (j=0; j<4; j++) {
			printk("offset=%x, value=%x\n", dma_regs_32_2[i]+j*0x10, musbfsh_readl(musbfsh->mregs,dma_regs_32_2[i]+j*0x10));
		}
	}

	printk("Rx count registers region:\n");
	for (i=1; i<=5; i++) {
		printk("offset=%x, value=%x\n", 0x300+i*4, musbfsh_readl(musbfsh->mregs,0x300+i*4));
	}
	printk("Debug registers region:\n");
	for (i=0; i<sizeof(debug_regs_32)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", debug_regs_32[i], musbfsh_readl(musbfsh->mregs,debug_regs_32[i]));
	}

	WARNING("PHY register dump!\n");
	printk("PHY registers region:\n");
	for (i=0; i<sizeof(phy_regs_32)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", phy_regs_32[i], __raw_readl(USB11_PHY_ADDR+phy_regs_32[i]));
	}

	WARNING("PHY register dump PORT0!\n");
	printk("PHY registers region PORT0:\n");
	for (i=0; i<sizeof(phy_regs_32_port0)/sizeof(int); i++) {
		printk("offset=%x, value=%x\n", phy_regs_32_port0[i], __raw_readl(USB_SIF_BASE+0x800+phy_regs_32_port0[i]));
	}
	MYDBG("");
#endif
}

void mt65xx_usb11_mac_reset_and_phy_stress_set(void)
{
	struct timeval tv_begin, tv_end;
	do_gettimeofday(&tv_begin);

	MYDBG("");

#if 0
	musbfsh_root_disc_procedure();
	mdelay(2000);					// wait disc procedure done
#else
	MYDBG("skip musbfsh_root_disc_procedure()\n"); 
#endif

	reset_usb11_phy_power_negotiation_status();

	//mt65xx_usb11_mac_phy_dump();

#if 1
	USB11PHY_SET8(0x69, 0x40);
	mdelay(100);
	USB11PHY_CLR8(0x69, 0x40);
	mdelay(100);
#else
	MYDBG("skip mac reset bit for testing\n");
#endif

	//clear 3.3V bias off ==> 3.3V
	USB11PHY_CLR8(0x15, 0x04);
	mdelay(2);

	//mt65xx_usb11_mac_phy_dump();

	do_gettimeofday(&tv_end);
	MYDBG("time spent, sec : %d, usec : %d\n", (int)(tv_end.tv_sec - tv_begin.tv_sec), (int)(tv_end.tv_usec - tv_begin.tv_usec));
}

void mt65xx_usb11_mac_phy_babble_clear(struct musbfsh *musbfsh)
{
	USB11PHY_SET8(0x6D, force_usb11_avalid | force_usb11_bvalid | force_usb11_sessend | force_usb11_vbusvalid);

	USB11PHY_CLR8(0x6C, RG_USB11_AVALID | RG_USB11_BVALID | RG_USB11_VBUSVALID);
	USB11PHY_SET8(0x6C, RG_USB11_SESSEND);
}

void mt65xx_usb11_mac_phy_babble_recover(struct musbfsh *musbfsh)
{
	USB11PHY_SET8(0x6D, force_usb11_avalid | force_usb11_bvalid | force_usb11_sessend | force_usb11_vbusvalid);

	USB11PHY_CLR8(0x6C, RG_USB11_SESSEND);
	USB11PHY_SET8(0x6C, RG_USB11_AVALID | RG_USB11_BVALID | RG_USB11_VBUSVALID);
}


int mt65xx_check_usb11_clk_status(void)
{
	int status = __raw_readl(0xf0003018);

	if(status & (0x1<<11))
	{
		//		MYDBG("status : %x, usb11 clk off\n", status);
		return 1;
	}
	else
	{
		//		MYDBG("status : %x, usb11 clk on\n", status);
		return 0;
	}
}



void usb11_phy_set_test(void)
{
	char result;

	MYDBG("");

	result = USB11PHY_READ8(0x12);
	MYDBG("result : %x\n", result);

	//RG_USB20_ICUSB_EN, b0
	USB11PHY_CLR8(0x12, 0x01);

	result = USB11PHY_READ8(0x12);
	MYDBG("result : %x\n", result);

	//RG_USB20_ICUSB_EN, b1
	USB11PHY_SET8(0x12, 0x01);

	result = USB11PHY_READ8(0x12);
	MYDBG("result : %x\n", result);

}

enum PHY_VOLTAGE_TYPE get_usb11_phy_voltage(void)
{
	MYDBG("");
	dump_stack();
	return g_phy_voltage;
}

void set_usb_phy_voltage(enum PHY_VOLTAGE_TYPE phy_volt)
{
	MYDBG("");
	g_phy_voltage = phy_volt;
}

void mt65xx_usb11_disable_clk_pll_pw(void)
{
	MYDBG("");
#if 0
	MYDBG("skip---\n"); 
#else
	mt65xx_usb11_clock_enable(false); // disable MAC before PHY to avoid funny interrupts like VBUS_ERR, if UPLL is occupied by port0

	disable_pll(UNIVPLL, "USB11");

	hwPowerDown(MT65XX_POWER_LDO_VUSB, "USB11");
#endif

	musbfsh_power = false;
}
void mt65xx_usb11_enable_clk_pll_pw(void)
{
	MYDBG("");
#if 0
	MYDBG("skip---\n"); 
#else
	hwPowerOn(MT65XX_POWER_LDO_VUSB, VOL_3300, "USB11");

	enable_pll(UNIVPLL, "USB11");
	udelay(300);

	mt65xx_usb11_clock_enable(true); // disable MAC before PHY to avoid funny interrupts like VBUS_ERR, if UPLL is occupied by port0
#endif

	musbfsh_power = true;
}

void usb11_hs_slew_rate_cal(void);
void mt65xx_usb11_phy_poweron_common(int icusb, int bias_off)
{
	MYDBG("icusb : %d, bias_off : %d\n", icusb, bias_off);
	//set smae value with 40nm, RG_USB20_DISCD
#if 0
	USB11PHY_SET8(0x16, 0x20);
#endif

	//switch to USB function. (system register, force ip into usb mode.
	USB11PHY_CLR8(0x6b, 0x04);
	USB11PHY_CLR8(0x6e, 0x01);

	// from mt65xx_usb11_phy_recover()
#if 1
	//4 6. RG_DPPULLDOWN = 1'b0
	USB11PHY_CLR8(0x68, 0x40);
	//4 7. RG_DMPULLDOWN = 1'b0
	USB11PHY_CLR8(0x68, 0x80);
	//4 8. RG_XCVRSEL = 2'b00
	USB11PHY_CLR8(0x68, 0x30);
	//4 9. RG_TERMSEL = 1'b0
	USB11PHY_CLR8(0x68, 0x04);
	//4 10. RG_DATAIN[3:0] = 4'b0000	
	USB11PHY_CLR8(0x69, 0x3c);

	//4 11. force_dp_pulldown = 1b'0
	USB11PHY_CLR8(0x6a, 0x10);
	//4 12. force_dm_pulldown = 1b'0
	USB11PHY_CLR8(0x6a, 0x20);
	//4 13. force_xcversel = 1b'0
	USB11PHY_CLR8(0x6a, 0x08);
	//4 14. force_termsel = 1b'0	
	USB11PHY_CLR8(0x6a, 0x02);
	//4 15. force_datain = 1b'0
	USB11PHY_CLR8(0x6a, 0x80);
#endif

	//RG_USB20_BC11_SW_EN, b0
	USB11PHY_CLR8(0x1a, 0x80);

	//RG_USB20_INTR_E, b1	
	USB11PHY_SET8(0x00, 0x20);

	//RG_SUSPENDM, b1
	USB11PHY_SET8(0x68, 0x08);

	//set force suspendm.
	USB11PHY_SET8(0x6a, 0x04);

	mdelay(2);

	// IC USB special
#if 1
	if(icusb)
	{	
		MYDBG("");
		//RG_USB20_ICUSB_EN, b1
		USB11PHY_SET8(0x12, 0x01);

		if(bias_off)
		{
			MYDBG("");
			//set 3.3V bias off ==> 1.8V
			USB11PHY_SET8(0x15, 0x04);
		}
		else
		{
			MYDBG("");
			//clear 3.3V bias off ==> 3.3V
			USB11PHY_CLR8(0x15, 0x04);
		}
	}
#endif

	// from mt65xx_usb11_phy_recover()
#if 1
	//4 17. RG_USB20_OTG_VBUSSCMP_EN 1'b1   
	USB11PHY_SET8(0x1a, 0x10);
#endif

	// from ROM code set up
#if 1
	//RG_SET		
	USB11PHY_SET8(0x6C, 0x2C);	
	USB11PHY_CLR8(0x6C, 0x10);

	// force setting, skip iddig
	USB11PHY_SET8(0x6D, 0x3C);
	USB11PHY_CLR8(0x6D, 0x02);

#endif	

	if(!icusb)
	{
		MYDBG("");
		usb11_hs_slew_rate_cal();
	}

	mdelay(2);

	//release force suspendm.
	USB11PHY_CLR8(0x6a, 0x04);
	//	mt65xx_usb11_mac_phy_dump();


	if(skip_mac_init_attr.value)
	{
		MYDBG("perform musbfsh_mac_init()\n");
		musbfsh_mac_init();
	}
	else
	{
		MYDBG("not perform musbfsh_mac_init()\n");
	}

	//	mt65xx_usb11_mac_phy_dump();
}

void mt65xx_usb11_phy_savecurrent_common(int icusb, int bias_off)
{
	MYDBG("icusb : %d, bias_off : %d\n", icusb, bias_off);
	if(icusb)
	{
		//switch to USB function. (system register, force ip into usb mode.
		USB11PHY_CLR8(0x6b, 0x04);
		USB11PHY_CLR8(0x6e, 0x01);

		//release force suspendm.
		USB11PHY_CLR8(0x6a, 0x04);

		//RG_USB20_BC11_SW_EN, b0
		USB11PHY_CLR8(0x1a, 0x80);

		//RG_USB20_ICUSB_EN, b0
		USB11PHY_CLR8(0x12, 0x01);

		if(bias_off)
		{
			//set 3.3V bias off ==> 3.3V
			USB11PHY_SET8(0x15, 0x04);
		}
		else
		{
			//clear 3.3V bias off ==> 3.3V
			USB11PHY_CLR8(0x15, 0x04);
		}

		//force suspendm = 1.
		USB11PHY_SET8(0x6a, 0x04);

		udelay(1);
	}
	else	// from mt65xx_usb11_phy_savecurrent()
	{
		MYDBG("");

		//4 1. swtich to USB function. (system register, force ip into usb mode.
		USB11PHY_CLR8(0x6b, 0x04);
		USB11PHY_CLR8(0x6e, 0x01);

		//4 2. release force suspendm.
		USB11PHY_CLR8(0x6a, 0x04);
		//4 3. RG_DPPULLDOWN./RG_DMPULLDOWN.
		USB11PHY_SET8(0x68, 0xc0);
		//4 4. RG_XCVRSEL[1:0] =2'b01.
		USB11PHY_CLR8(0x68, 0x30);
		USB11PHY_SET8(0x68, 0x10);
		//4 5. RG_TERMSEL = 1'b1
		USB11PHY_SET8(0x68, 0x04);
		//4 6. RG_DATAIN[3:0]=4'b0000
		USB11PHY_CLR8(0x69, 0x3c);
		//4 7.force_dp_pulldown, force_dm_pulldown, force_xcversel,force_termsel.
		USB11PHY_SET8(0x6a, 0xba);

		//4 8.RG_USB20_BC11_SW_EN 1'b0
		USB11PHY_CLR8(0x1a, 0x80);
		//4 9.RG_USB20_OTG_VBUSSCMP_EN 1'b0
		USB11PHY_CLR8(0x1a, 0x10);
		//4 10. delay 800us.
		udelay(800);
		//4 11. rg_usb20_pll_stable = 1
		USB11PHY_SET8(0x63, 0x02);

		udelay(1);
		//4 12.  force suspendm = 1.
		USB11PHY_SET8(0x6a, 0x04);

		USB11PHY_CLR8(0x6C, 0x2C);
		USB11PHY_SET8(0x6C, 0x10);
		USB11PHY_CLR8(0x6D, 0x3C);	

		//4 13.  wait 1us
		udelay(1);
	}
}

void mt65xx_usb11_phy_recover_common(int icusb, int bias_off)
{
	MYDBG("icusb : %d, bias_off : %d\n", icusb, bias_off);
	if(icusb)
	{	
		//switch to USB function. (system register, force ip into usb mode.
		USB11PHY_CLR8(0x6b, 0x04);
		USB11PHY_CLR8(0x6e, 0x01);


		//RG_USB20_BC11_SW_EN 1'b0
		USB11PHY_CLR8(0x1a, 0x80);

		//RG_USB20_ICUSB_EN, b1
		USB11PHY_SET8(0x12, 0x01);

		if(bias_off)
		{
			//set 3.3V bias off ==> 3.3V
			USB11PHY_SET8(0x15, 0x04);
		}
		else
		{
			//clear 3.3V bias off ==> 3.3V
			USB11PHY_CLR8(0x15, 0x04);
		}

		//release force suspendm.
		USB11PHY_CLR8(0x6a, 0x04);

		//wait 1us
		udelay(1);
	}
	else			// from mt65xx_usb11_phy_recover()
	{	
		MYDBG("");

		//4 3. force_uart_en = 1'b0
		USB11PHY_CLR8(0x6b, 0x04);
		//4 4. RG_UART_EN = 1'b0
		USB11PHY_CLR8(0x6e, 0x1);
		//4 5. Release force suspendm.
		USB11PHY_CLR8(0x6a, 0x04);

		//4 6. RG_DPPULLDOWN = 1'b0
		USB11PHY_CLR8(0x68, 0x40);
		//4 7. RG_DMPULLDOWN = 1'b0
		USB11PHY_CLR8(0x68, 0x80);
		//4 8. RG_XCVRSEL = 2'b00
		USB11PHY_CLR8(0x68, 0x30);
		//4 9. RG_TERMSEL = 1'b0
		USB11PHY_CLR8(0x68, 0x04);
		//4 10. RG_DATAIN[3:0] = 4'b0000	
		USB11PHY_CLR8(0x69, 0x3c);

		//4 11. force_dp_pulldown = 1b'0
		USB11PHY_CLR8(0x6a, 0x10);
		//4 12. force_dm_pulldown = 1b'0
		USB11PHY_CLR8(0x6a, 0x20);
		//4 13. force_xcversel = 1b'0
		USB11PHY_CLR8(0x6a, 0x08);
		//4 14. force_termsel = 1b'0	
		USB11PHY_CLR8(0x6a, 0x02);
		//4 15. force_datain = 1b'0
		USB11PHY_CLR8(0x6a, 0x80);

		//4 16. RG_USB20_BC11_SW_EN 1'b0
		USB11PHY_CLR8(0x1a, 0x80);


		//4 17. RG_USB20_OTG_VBUSSCMP_EN 1'b1   
		USB11PHY_SET8(0x1a, 0x10);

		USB11PHY_SET8(0x6C, 0x2C);	
		USB11PHY_SET8(0x6D, 0x3C);	 
		//<4> 18. wait 800 usec.
		udelay(800);

		usb11_hs_slew_rate_cal();
	}
}

void mt65xx_usb11_phy_poweron_volt_30(void)
{
	MYDBG("");
	mt65xx_usb11_phy_poweron_common(1, 0);
}

void mt65xx_usb11_phy_savecurrent_volt_30(void)
{
	MYDBG("");
	mt65xx_usb11_phy_savecurrent_common(1, 0);
}
void mt65xx_usb11_phy_recover_volt_30(void)
{
	MYDBG("");
	mt65xx_usb11_phy_recover_common(1, 0);
}

void mt65xx_usb11_phy_poweron_volt_18(void)
{
	MYDBG("");
	mt65xx_usb11_phy_poweron_common(1, 1);
}
void mt65xx_usb11_phy_savecurrent_volt_18(void)
{
	MYDBG("");
	mt65xx_usb11_phy_savecurrent_common(1, 1);
}
void mt65xx_usb11_phy_recover_volt_18(void)
{
	MYDBG("");
	mt65xx_usb11_phy_recover_common(1, 1);
}

void mt65xx_usb11_phy_poweron_volt_50(void)
{
	MYDBG("");
	//copy from mt65xx_usb11_phy_recover()
#if 0
	//4 3. force_uart_en = 1'b0
	USB11PHY_CLR8(0x6b, 0x04);
	//4 4. RG_UART_EN = 1'b0
	USB11PHY_CLR8(0x6e, 0x1);
	//4 5. Release force suspendm.
	USB11PHY_CLR8(0x6a, 0x04);

	//4 6. RG_DPPULLDOWN = 1'b0
	USB11PHY_CLR8(0x68, 0x40);
	//4 7. RG_DMPULLDOWN = 1'b0
	USB11PHY_CLR8(0x68, 0x80);
	//4 8. RG_XCVRSEL = 2'b00
	USB11PHY_CLR8(0x68, 0x30);
	//4 9. RG_TERMSEL = 1'b0
	USB11PHY_CLR8(0x68, 0x04);
	//4 10. RG_DATAIN[3:0] = 4'b0000	
	USB11PHY_CLR8(0x69, 0x3c);

	//4 11. force_dp_pulldown = 1b'0
	USB11PHY_CLR8(0x6a, 0x10);
	//4 12. force_dm_pulldown = 1b'0
	USB11PHY_CLR8(0x6a, 0x20);
	//4 13. force_xcversel = 1b'0
	USB11PHY_CLR8(0x6a, 0x08);
	//4 14. force_termsel = 1b'0	
	USB11PHY_CLR8(0x6a, 0x02);
	//4 15. force_datain = 1b'0
	USB11PHY_CLR8(0x6a, 0x80);

	//4 16. RG_USB20_BC11_SW_EN 1'b0
	USB11PHY_CLR8(0x1a, 0x80);


	//4 17. RG_USB20_OTG_VBUSSCMP_EN 1'b1   
	USB11PHY_SET8(0x1a, 0x10);

	USB11PHY_SET8(0x6C, 0x2C);	
	USB11PHY_SET8(0x6D, 0x3C);	 
	//<4> 18. wait 800 usec.
	udelay(800);

	usb11_hs_slew_rate_cal();

	MYDBG("perform musbfsh_mac_init()\n");
	musbfsh_mac_init();

#else
	MYDBG("");
	mt65xx_usb11_phy_poweron_common(0, 0);

#endif
}
void mt65xx_usb11_phy_recover_volt_50(void)
{
	MYDBG("");
	mt65xx_usb11_phy_recover_common(0, 0);
}

void mt65xx_usb11_phy_savecurrent_volt_50(void)
{
	MYDBG("");
	mt65xx_usb11_phy_savecurrent_common(0, 0);
}
#endif

void usb11_hs_slew_rate_cal(void) {

	unsigned long data;
	unsigned long x;
	unsigned char value;
	unsigned long start_time, timeout;
	unsigned int timeout_flag = 0;
	//4 s1:enable usb ring oscillator.
	USB11PHY_WRITE8(0x15,0x80);

	//4 s2:wait 1us.
	udelay(1);

	//4 s3:enable free run clock
	USB11PHY_WRITE8 (0xf00-0x900+0x11,0x01);
	//4 s4:setting cyclecnt.
	USB11PHY_WRITE8 (0xf00-0x900+0x01,0x04);
	//4 s5:enable frequency meter
	USB11PHY_SET8 (0xf00-0x900+0x03,0x05);

	//4 s6:wait for frequency valid.
	start_time = jiffies;
	timeout = jiffies + 3 * HZ;
	while(!(USB11PHY_READ8(0xf00-0x900+0x10)&0x1)){
		if(time_after(jiffies, timeout)){
			timeout_flag = 1;
			break;
		}
	}

	//4 s7: read result.
	if(timeout_flag){
		printk("[USBPHY] Slew Rate Calibration: Timeout\n");
		value = 0x4;
	}
	else{
		data = USB11PHY_READ32 (0xf00-0x900+0x0c);
		x = ((1024*FRA*PARA)/data);
		value = (unsigned char)(x/1000);
		if((x-value*1000)/100>=5)
			value += 1;
		// printk("[USB11PHY]slew calibration:FM_OUT =%d, x=%d,value=%d\n",data,x,value);
	}

	//4 s8: disable Frequency and run clock.
	USB11PHY_CLR8 (0xf00-0x900+0x03,0x05);//disable frequency meter
	USB11PHY_CLR8 (0xf00-0x900+0x11,0x01);//disable free run clock

	//4 s9: 
	USB11PHY_WRITE8(0x15,value<<4);

	//4 s10:disable usb ring oscillator.
	USB11PHY_CLR8(0x15,0x80);  
}


void mt65xx_usb11_phy_poweron(void)
{
#ifdef MTK_ICUSB_SUPPORT
	enum PHY_VOLTAGE_TYPE phy_volt;
	struct timeval tv_begin, tv_end;
#endif

	WARNING("mt65xx_usb11_phy_poweron++\r\n");
#ifdef MTK_ICUSB_SUPPORT
	phy_volt = get_usb11_phy_voltage();
	do_gettimeofday(&tv_begin);

	if(phy_volt == VOL_33)
	{
		mt65xx_usb11_phy_poweron_volt_30();
	}
	else if(phy_volt == VOL_18)
	{
		mt65xx_usb11_phy_poweron_volt_18();
	}
	else if(phy_volt == VOL_50)
	{
		mt65xx_usb11_phy_poweron_volt_50();
	}
	else
	{
		MYDBG("");
	}

	do_gettimeofday(&tv_end);
	MYDBG("time spent, sec : %d, usec : %d\n", (unsigned int)(tv_end.tv_sec - tv_begin.tv_sec), (unsigned int)(tv_end.tv_usec - tv_begin.tv_usec));
#else
	//move pll to func calling mt65xx_usb11_phy_poweron() 
#if 0	
	enable_pll(UNIVPLL, "USB11");
	//udelay(100); // PHY power stable time	
#if 0
	/* reverse preloader's sin @mt6575_usbphy.c */
	USB11PHY_CLR8(U1PHTCR2+3, force_usb11_avalid | force_usb11_bvalid | force_usb11_sessend | force_usb11_vbusvalid);
	USB11PHY_CLR8(U1PHTCR2+2, RG_USB11_AVALID | RG_USB11_BVALID | RG_USB11_SESSEND | RG_USB11_VBUSVALID);
	USB11PHY_CLR8(U1PHYCR1+2, force_usb11_en_fs_ls_rcv | force_usb11_en_fs_ls_tx);
	/**************************************/

	USB11PHY_SET8(U1PHYCR0+1, RG_USB11_FSLS_ENBGRI);

	USB11PHY_SET8(U1PHTCR2+3, force_usb11_avalid | force_usb11_sessend | force_usb11_vbusvalid);
	USB11PHY_SET8(U1PHTCR2+2, RG_USB11_AVALID | RG_USB11_VBUSVALID);
	USB11PHY_CLR8(U1PHTCR2+2, RG_USB11_SESSEND);
#endif


#if 0
	//HQA LOOPBACK TEST. <FS>
	USB11PHY_CLR8(0x1a, 0x80);
	USB11PHY_CLR8(0x68, 0x08);

	USB11PHY_CLR8(0x68, 0x03);
	USB11PHY_SET8(0x68, 0x10);

	USB11PHY_SET8(0x68, 0x04);
	USB11PHY_CLR8(0x69, 0x03);
	USB11PHY_CLR8(0x69, 0x3C);

	USB11PHY_SET8(0x68, 0x80);

	USB11PHY_SET8(0x6a, 0x04);
	USB11PHY_SET8(0x6a, 0x01);
	USB11PHY_SET8(0x6a, 0x08);
	USB11PHY_SET8(0x6a, 0x02);

	USB11PHY_SET8(0x6a, 0x40);
	USB11PHY_SET8(0x6a, 0x80);
	USB11PHY_SET8(0x6a, 0x30);

	USB11PHY_SET8(0x68, 0x08);
	udelay(50);

	USB11PHY_SET8(0x63, 0x02);
	udelay(1);

	USB11PHY_SET8(0x63, 0x02);
	USB11PHY_SET8(0x63, 0x04);
	USB11PHY_CLR8(0x63, 0x08);

#endif



#if 0 
	//HQA LOOPBACK TEST. <HS>
	USB11PHY_CLR8(0x1a, 0x80);
	USB11PHY_CLR8(0x68, 0x08);

	USB11PHY_CLR8(0x68, 0x03);
	USB11PHY_CLR8(0x68, 0x30);

	USB11PHY_CLR8(0x68, 0x04);
	USB11PHY_CLR8(0x69, 0x03);
	USB11PHY_CLR8(0x69, 0x3C);

	USB11PHY_CLR8(0x68, 0xC0);

	USB11PHY_SET8(0x6a, 0x04);
	USB11PHY_SET8(0x6a, 0x01);
	USB11PHY_SET8(0x6a, 0x08);
	USB11PHY_SET8(0x6a, 0x02);

	USB11PHY_SET8(0x6a, 0x40);
	USB11PHY_SET8(0x6a, 0x80);
	USB11PHY_SET8(0x6a, 0x30);

	USB11PHY_SET8(0x68, 0x08);
	udelay(50);

	USB11PHY_SET8(0x63, 0x02);
	udelay(1);

	USB11PHY_SET8(0x63, 0x02);
	USB11PHY_SET8(0x63, 0x04);
	USB11PHY_CLR8(0x63, 0x08);

	usb11_hs_slew_rate_cal();

#endif

	udelay(50);
#endif

	USB11PHY_CLR8(0x6b, 0x04);
	USB11PHY_CLR8(0x6e, 0x01);

	USB11PHY_CLR8(0x1a, 0x80);

	/* remove in MT6588 ?????
	   USBPHY_CLR8(0x02, 0x7f);
	   USBPHY_SET8(0x02, 0x09);
	   USBPHY_CLR8(0x22, 0x03);
	 */

	USB11PHY_CLR8(0x6a, 0x04);
	//USBPHY_SET8(0x1b, 0x08);

	//force VBUS Valid		
	USB11PHY_SET8(0x6C, 0x2C);	
	USB11PHY_SET8(0x6D, 0x3C);		

	udelay(800);
#endif

}

void mt65xx_usb11_phy_savecurrent(void)
{
#ifdef MTK_ICUSB_SUPPORT
	enum PHY_VOLTAGE_TYPE phy_volt; 
#endif

	WARNING("mt65xx_usb11_phy_savecurrent++\r\n");
#ifdef MTK_ICUSB_SUPPORT
	phy_volt = get_usb11_phy_voltage();
	if(phy_volt == VOL_33)
	{
		mt65xx_usb11_phy_savecurrent_volt_30();
	}
	else if(phy_volt == VOL_18)
	{
		mt65xx_usb11_phy_savecurrent_volt_18();
	}
	else if(phy_volt == VOL_50)
	{
		mt65xx_usb11_phy_savecurrent_volt_50();
	}
	else
	{
		MYDBG("");
	}
#else
#if 0
	USB11PHY_SET8(U1PHTCR2+3, force_usb11_avalid | force_usb11_sessend | force_usb11_vbusvalid);
	USB11PHY_CLR8(U1PHTCR2+2, RG_USB11_AVALID | RG_USB11_VBUSVALID);
	USB11PHY_SET8(U1PHTCR2+2, RG_USB11_SESSEND);

	USB11PHY_CLR8(U1PHYCR0+1, RG_USB11_FSLS_ENBGRI);

	USB11PHY_SET8(U1PHYCR1+2, force_usb11_en_fs_ls_rcv | force_usb11_en_fs_ls_tx);
	USB11PHY_CLR8(U1PHYCR1+3, RG_USB11_EN_FS_LS_RCV | RG_USB11_EN_FS_LS_TX);
#endif

	//4 1. swtich to USB function. (system register, force ip into usb mode.
	USB11PHY_CLR8(0x6b, 0x04);
	USB11PHY_CLR8(0x6e, 0x01);

	//4 2. release force suspendm.
	USB11PHY_CLR8(0x6a, 0x04);
	//4 3. RG_DPPULLDOWN./RG_DMPULLDOWN.
	USB11PHY_SET8(0x68, 0xc0);
	//4 4. RG_XCVRSEL[1:0] =2'b01.
	USB11PHY_CLR8(0x68, 0x30);
	USB11PHY_SET8(0x68, 0x10);
	//4 5. RG_TERMSEL = 1'b1
	USB11PHY_SET8(0x68, 0x04);
	//4 6. RG_DATAIN[3:0]=4'b0000
	USB11PHY_CLR8(0x69, 0x3c);
	//4 7.force_dp_pulldown, force_dm_pulldown, force_xcversel,force_termsel.
	USB11PHY_SET8(0x6a, 0xba);

	//4 8.RG_USB20_BC11_SW_EN 1'b0
	USB11PHY_CLR8(0x1a, 0x80);
	//4 9.RG_USB20_OTG_VBUSSCMP_EN 1'b0
	USB11PHY_CLR8(0x1a, 0x10);
	//4 10. delay 800us.
	udelay(800);
	//4 11. rg_usb20_pll_stable = 1
	USB11PHY_SET8(0x63, 0x02);

	udelay(1);
	//4 12.  force suspendm = 1.
	USB11PHY_SET8(0x6a, 0x04);

	USB11PHY_CLR8(0x6C, 0x2C);
	USB11PHY_SET8(0x6C, 0x10);
	USB11PHY_CLR8(0x6D, 0x3C);	

	//4 13.  wait 1us
	udelay(1);

	//move 4.14 to func calling mt65xx_usb11_phy_savecurrent()
#if 0
	//4 14. turn off internal 48Mhz PLL.
	disable_pll(UNIVPLL, "USB11");
#endif
#endif
}

void mt65xx_usb11_phy_recover(void)
{
#ifdef MTK_ICUSB_SUPPORT
	enum PHY_VOLTAGE_TYPE phy_volt; 
#endif

	WARNING("mt65xx_usb11_phy_recover++\r\n");

#ifdef MTK_ICUSB_SUPPORT
	phy_volt = get_usb11_phy_voltage();
	if(phy_volt == VOL_33)
	{
		mt65xx_usb11_phy_recover_volt_30();
	}
	else if(phy_volt == VOL_18)
	{
		mt65xx_usb11_phy_recover_volt_18();
	}
	else if(phy_volt == VOL_50)
	{
		mt65xx_usb11_phy_recover_volt_50();
	}
	else
	{
		MYDBG("");
	}
#else

	//move 4.1, 4.2 to func calling mt65xx_usb11_phy_recover()
#if 0	
	//4 1. turn on USB reference clock. 	
	enable_pll(UNIVPLL, "USB11");

#if 0
	USB11PHY_SET8(U1PHTCR2+3, force_usb11_avalid | force_usb11_sessend | force_usb11_vbusvalid);
	USB11PHY_SET8(U1PHTCR2+2, RG_USB11_AVALID | RG_USB11_VBUSVALID);
	USB11PHY_CLR8(U1PHTCR2+2, RG_USB11_SESSEND);

	USB11PHY_CLR8(U1PHYCR1+2, force_usb11_en_fs_ls_rcv | force_usb11_en_fs_ls_tx);
	USB11PHY_CLR8(U1PHYCR1+3, RG_USB11_EN_FS_LS_RCV | RG_USB11_EN_FS_LS_TX);

	USB11PHY_SET8(U1PHYCR0+1, RG_USB11_FSLS_ENBGRI);

	udelay(100);
#endif

	//4 2. wait 50 usec.
	udelay(50);
#endif
	//4 3. force_uart_en = 1'b0
	USB11PHY_CLR8(0x6b, 0x04);
	//4 4. RG_UART_EN = 1'b0
	USB11PHY_CLR8(0x6e, 0x1);
	//4 5. Release force suspendm.
	USB11PHY_CLR8(0x6a, 0x04);

	//4 6. RG_DPPULLDOWN = 1'b0
	USB11PHY_CLR8(0x68, 0x40);
	//4 7. RG_DMPULLDOWN = 1'b0
	USB11PHY_CLR8(0x68, 0x80);
	//4 8. RG_XCVRSEL = 2'b00
	USB11PHY_CLR8(0x68, 0x30);
	//4 9. RG_TERMSEL = 1'b0
	USB11PHY_CLR8(0x68, 0x04);
	//4 10. RG_DATAIN[3:0] = 4'b0000	
	USB11PHY_CLR8(0x69, 0x3c);

	//4 11. force_dp_pulldown = 1b'0
	USB11PHY_CLR8(0x6a, 0x10);
	//4 12. force_dm_pulldown = 1b'0
	USB11PHY_CLR8(0x6a, 0x20);
	//4 13. force_xcversel = 1b'0
	USB11PHY_CLR8(0x6a, 0x08);
	//4 14. force_termsel = 1b'0	
	USB11PHY_CLR8(0x6a, 0x02);
	//4 15. force_datain = 1b'0
	USB11PHY_CLR8(0x6a, 0x80);

	//4 16. RG_USB20_BC11_SW_EN 1'b0
	USB11PHY_CLR8(0x1a, 0x80);


	//4 17. RG_USB20_OTG_VBUSSCMP_EN 1'b1   
	USB11PHY_SET8(0x1a, 0x10);

	USB11PHY_SET8(0x6C, 0x2C);	
	USB11PHY_SET8(0x6D, 0x3C);	 
	//<4> 18. wait 800 usec.
	udelay(800);

	usb11_hs_slew_rate_cal();
#endif
}

static bool clock_enabled = false;

static int enable_cnt = 0;			
static int disable_cnt = 0;

void mt65xx_usb11_clock_enable(bool enable)
{
	//WARNING("mt65xx_usb11_clock_enable++\r\n");
	dump_stack();
	MYDBG("enable_cnt : %d, disable_cnt : %d\n", enable_cnt, disable_cnt);
	if(enable){
		enable_cnt++;
		if(clock_enabled)//already enable
		{
			MYDBG("");
			return;
		}
		else{
			MYDBG("");
			enable_clock (MT_CG_PERI0_USB1, "USB11");
			clock_enabled = true;
		}
	}
	else{
		disable_cnt++;
		if(!clock_enabled)//already disabled.
		{
			MYDBG("");
			return;
		}
		else{
			MYDBG("");
			disable_clock (MT_CG_PERI0_USB1, "USB11");
			clock_enabled = false;
		}
	}
	return;
}

int mt65xx_usb11_poweron(int on){
	static bool recover = false;
	WARNING("mt65xx_usb11_poweron++\r\n");

	if(on){
		if(musbfsh_power) {
			return 1; //already powered on
		} else{
			mt65xx_usb11_clock_enable (true);	            	
			if(!recover){
				enable_pll(UNIVPLL, "USB11");
				udelay(50);
				mt65xx_usb11_phy_recover();
				recover = true;
			} else {
				enable_pll(UNIVPLL, "USB11");
				udelay(50);

				mt65xx_usb11_phy_recover();
			}
			MYDBG("");
			dump_stack();

			musbfsh_power = true;
		}
	} else{
		if(!musbfsh_power) {
			return 1; //already power off
		} else{
			mt65xx_usb11_phy_savecurrent();		
			disable_pll(UNIVPLL, "USB11");			
			mt65xx_usb11_clock_enable(false);

			MYDBG("");
			dump_stack();

			musbfsh_power = false;
		}
	}
	return 0;
}

void mt65xx_usb11_vbus(struct musbfsh *musbfsh, int is_on)
{
	// static int oned = 0;  
	WARNING("mt65xx_usb11_vbus++,is_on=%d\r\n",is_on);
#if 0
	// mt_set_gpio_mode(GPIO67,0);//should set GPIO_OTG_DRVVBUS_PIN as gpio mode. 
	mt_set_gpio_dir(GPIO_OTG_DRVVBUS_PIN,GPIO_DIR_OUT);
	if(is_on){
		if(oned)
			return;
		else{
			mt_set_gpio_out(GPIO_OTG_DRVVBUS_PIN,GPIO_OUT_ONE);
			oned = 1;
		}
	} else {
		if(!oned)
			return;
		else{
			mt_set_gpio_out(GPIO_OTG_DRVVBUS_PIN,GPIO_OUT_ZERO);
			oned = 0;
		}
	}
#endif
	return;
}

void musbfs_check_mpu_violation(u32 addr, int wr_vio)
{
	void __iomem *mregs;

	// mregs = (unsigned char __iomem*)USB_BASE;
	mregs = (void __iomem*)USB_BASE;

	printk(KERN_CRIT "MUSB checks EMI MPU violation.\n");
	printk(KERN_CRIT "addr = 0x%x, %s violation.\n", addr, wr_vio? "Write": "Read");
	printk(KERN_CRIT "POWER = 0x%x,DEVCTL= 0x%x.\n", musbfsh_readb(mregs, MUSBFSH_POWER), musbfsh_readb((void __iomem*)USB11_BASE, MUSBFSH_DEVCTL));
	printk(KERN_CRIT "DMA_CNTLch0 0x%04x,DMA_ADDRch0 0x%08x,DMA_COUNTch0 0x%08x \n",musbfsh_readw(mregs, 0x204),musbfsh_readl(mregs,0x208),musbfsh_readl(mregs,0x20C));
	printk(KERN_CRIT "DMA_CNTLch1 0x%04x,DMA_ADDRch1 0x%08x,DMA_COUNTch1 0x%08x \n",musbfsh_readw(mregs, 0x214),musbfsh_readl(mregs,0x218),musbfsh_readl(mregs,0x21C));
	printk(KERN_CRIT "DMA_CNTLch2 0x%04x,DMA_ADDRch2 0x%08x,DMA_COUNTch2 0x%08x \n",musbfsh_readw(mregs, 0x224),musbfsh_readl(mregs,0x228),musbfsh_readl(mregs,0x22C));
	printk(KERN_CRIT "DMA_CNTLch3 0x%04x,DMA_ADDRch3 0x%08x,DMA_COUNTch3 0x%08x \n",musbfsh_readw(mregs, 0x234),musbfsh_readl(mregs,0x238),musbfsh_readl(mregs,0x23C));
	printk(KERN_CRIT "DMA_CNTLch4 0x%04x,DMA_ADDRch4 0x%08x,DMA_COUNTch4 0x%08x \n",musbfsh_readw(mregs, 0x244),musbfsh_readl(mregs,0x248),musbfsh_readl(mregs,0x24C));
	printk(KERN_CRIT "DMA_CNTLch5 0x%04x,DMA_ADDRch5 0x%08x,DMA_COUNTch5 0x%08x \n",musbfsh_readw(mregs, 0x254),musbfsh_readl(mregs,0x258),musbfsh_readl(mregs,0x25C));
	printk(KERN_CRIT "DMA_CNTLch6 0x%04x,DMA_ADDRch6 0x%08x,DMA_COUNTch6 0x%08x \n",musbfsh_readw(mregs, 0x264),musbfsh_readl(mregs,0x268),musbfsh_readl(mregs,0x26C));
	printk(KERN_CRIT "DMA_CNTLch7 0x%04x,DMA_ADDRch7 0x%08x,DMA_COUNTch7 0x%08x \n",musbfsh_readw(mregs, 0x274),musbfsh_readl(mregs,0x278),musbfsh_readl(mregs,0x27C));        
}

int musbfsh_platform_init(struct musbfsh *musbfsh)
{
	INFO("musbfsh_platform_init++\n");
	if(!musbfsh){
		ERR("musbfsh_platform_init,error,musbfsh is NULL");
		return -1;
	}
	musbfsh->board_set_vbus = mt65xx_usb11_vbus;
	musbfsh->board_set_power = mt65xx_usb11_poweron;
	hwPowerOn(MT65XX_POWER_LDO_VUSB, VOL_3300, "USB11"); // don't need to power on PHY for every resume

#ifdef MTK_ICUSB_SUPPORT
	/* only power_on, clk, pll for MAC,  no phy power on */
	printk(KERN_WARNING "%s, only clk and pll, no phy_power_on\n", __func__);
	mt65xx_usb11_clock_enable (true);
	enable_pll(UNIVPLL, "USB11");
#else
	mt65xx_usb11_poweron(true);
#endif
	emi_mpu_notifier_register(MST_ID_MMPERI_2, musbfs_check_mpu_violation);  
	return 0;
}

int musbfsh_platform_exit(struct musbfsh *musbfsh)
{
	INFO("musbfsh_platform_exit++\r\n");
	mt65xx_usb11_poweron(false);
	hwPowerDown(MT65XX_POWER_LDO_VUSB, "USB11"); // put it here because we can't shutdown PHY power during suspend
	return 0;
}

void musbfsh_platform_enable(struct musbfsh *musbfsh)
{
	INFO("musbfsh_platform_enable++\r\n");
}

void musbfsh_platform_disable(struct musbfsh *musbfsh)
{
	INFO("musbfsh_platform_disable++\r\n");
}

void musbfsh_hcd_release (struct device *dev) 
{
	INFO("musbfsh_hcd_release++,dev = 0x%08X.\n", (uint32_t)dev);
}  
//-------------------------------------------------------------------------





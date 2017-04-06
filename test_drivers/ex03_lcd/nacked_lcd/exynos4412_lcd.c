#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/dma-mapping.h>
#include <linux/compiler.h>

#include <linux/io.h>

#define __HARXON_DEBUG__ 1
#define FB_NAME "harxon_fb"

#ifdef __HARXON_DEBUG__
#define harxon_debug() printk(KERN_EMERG "func(%s):line%d+++\n", __func__, __LINE__)
#define Debug(KERN_EMERG,arg...) printk(KERN_EMERG,##arg)
#else
#define harxon_degug()
#define Debug(KERN_EMERG,arg...)
#endif

struct EXYNOS4412_LCDBLK_CFG{
	unsigned long lcdblk_cfg;
	unsigned long lcdblk_cfg2;
};

struct EXYNOS4412_GPIO{
		unsigned long gpf0con;
		unsigned long gpf0dat;
		unsigned long gpf0pud;
		unsigned long gpf0drv;
		unsigned long gpf0conpdn;
		unsigned long gpf0pudpdn;
		
		unsigned long gpf1con;
		unsigned long gpf1dat;
		unsigned long gpf1pud;
		unsigned long gpf1drv;
		unsigned long gpf1conpdn;
		unsigned long gpf1pudpdn;
		
		unsigned long gpf2con;
		unsigned long gpf2dat;
		unsigned long gpf2pud;
		unsigned long gpf2drv;
		unsigned long gpf2conpdn;
		unsigned long gpf2pudpdn;
		
		unsigned long gpf3con;
		unsigned long gpf3dat;
		unsigned long gpf3pud;
		unsigned long gpf3drv;
		unsigned long gpf3conpdn;
		unsigned long gpf3pudpdn;
		
};


struct EXYNOS4412_LCD_REGISTER{
	
		unsigned long vidcon0;	/*配置视频格式和显示使能和关闭.*/
		unsigned long vidcon1;	/*规定RGB接口的控制信号.*/
		unsigned long vidcon2;	/*规定输出数据控制格式*/
		unsigned long vidcon3; /*规定图像增强控制.*/
		
		unsigned long vidtcon0;	/*6.配置视频输出时钟和决定显示尺寸*/
		unsigned long vidtcon1;
		unsigned long vidtcon2;
		unsigned long vidtcon3;
		
		unsigned long wincon0;	/*窗口特性设置*/
		unsigned long wincon1;	/*窗口特性设置*/
		unsigned long wincon2;	/*窗口特性设置*/
		unsigned long wincon3;	/*窗口特性设置*/
		
		unsigned long wincon4;	/*窗口特性设置*/
		unsigned long shadowcon;
		unsigned long winchmap2;
		unsigned long vidosd0a;
		
		unsigned long vidosd0b;
		unsigned long vidosd0c;
		unsigned long vidosd1a;
		unsigned long vidosd1b;
		
		unsigned long vidosd1c;
		unsigned long vidosd2a;
		unsigned long vidosd2b;
		unsigned long vidosd2c;

		
		unsigned long vidosd3a;
		unsigned long vidosd3b;
		unsigned long vidosd3c;
		unsigned long vidosd4a;
		
		unsigned long vidosd4b;
		unsigned long vidosd4c;
		unsigned long vindw00add0b0;	/*VIDWxxADDx: Specifies source image address setting.*/
		unsigned long vindw00add0b1;
		
		unsigned long vindw00add0b2;
		unsigned long vindw01add0b0;
		unsigned long vindw01add0b1;
		unsigned long vindw01add0b2;
		
		unsigned long vindw02add0b0;
		unsigned long vindw02add0b1;
		unsigned long vindw02add0b2;
		unsigned long vindw03add0b0;

		unsigned long vindw03add0b1;
		unsigned long vindw03add0b2;
		unsigned long vindw04add0b0;
		unsigned long vindw04add0b1;
		
		unsigned long vindw04add0b2;
		unsigned long vindw00add1b0;
		unsigned long vindw00add1b1;
		unsigned long vindw00add1b2;
		
		unsigned long vindw01add1b0;
		unsigned long vindw01add1b1;
		unsigned long vindw01add1b2;		
		unsigned long vindw02add1b0;
		
		unsigned long vindw02add1b1;
		unsigned long vindw02add1b2;
		unsigned long vindw03add1b0;
		unsigned long vindw03add1b1;
		
		unsigned long vindw03add1b2;
		unsigned long vindw04add1b0;
		unsigned long vindw04add1b1;		
		unsigned long vindw04add1b2;

		unsigned long vidw00add2;
		unsigned long vidw01add2;
		unsigned long vidw02add2;
		unsigned long vidw03add2;
		
		unsigned long vidw04add2;
		/*
		//1. VIDCON0: Configures video output format and displays enable/disable.
		//2. VIDCON1: Specifies RGB I/F control signal.
		//3. VIDCON2: Specifies output data format control.
		//4. VIDCON3: Specifies image enhancement control.
		5. I80IFCONx: Specifies CPU interface control signal.
		//6. VIDTCONx: Configures video output timing and determines the size of display.
		//7. WINCONx: Specifies each window feature setting.
		8. VIDOSDxA, VIDOSDxB: Specifies window position setting.
		9. VIDOSDxC, D: Specifies On Screen Display (OSD) size setting.
		10. VIDWxALPHA0/1: Specifies alpha value setting.
		11. BLENDEQx: Specifies blending equation setting.
		12. VIDWxxADDx: Specifies source image address setting.
		13. WxKEYCONx: Specifies color key setting register.
		14. WxKEYALPHA: Specifies color key alpha value setting.
		15. WINxMAP: Specifies window color control.
		16. GAMMALUT_xx: Specifies gamma value setting.
		17. COLORGAINCON: Specifies color gain value setting.
		18. HUExxx: Specifies Hue coefficient and offset value setting.
		19. WPALCON: Specifies palette control register.
		20. WxRTQOSCON: Specifies RTQoS control register.
		21. WxPDATAxx: Specifies window palette data of each index.
		22. SHDOWCON: Specifies shadow control register.
		23. WxRTQOSCON: Specifies QoS control register.
		*/
};

static volatile struct EXYNOS4412_LCDBLK_CFG* lcdblk_cfg;
static volatile void __iomem* clk_src_lcd0;
static volatile void __iomem* clk_drv_lcd;
static volatile void __iomem* gpd0_con;
static volatile void __iomem* gpd0_dat;
static volatile struct EXYNOS4412_GPIO* fb_gpio;
static volatile struct EXYNOS4412_LCD_REGISTER* fb_reg;

static struct fb_info* info = NULL;
static u32 a_pseudo_palette[16] = {};

int exynos4412_fb_setcolreg(unsigned regno, unsigned red, unsigned green, unsigned blue, unsigned transp, struct fb_info *info){
	
		if(regno >= 16)return -1;
			
				a_pseudo_palette[regno] = ((red & 0x01f) << info->var.red.offset) |((green & 0x03f) << info->var.green.offset) |((blue & 0x01f) << info ->var.blue.offset);
				
				return 0;
}


static struct fb_ops exynos4412_lcd_fops = {
	.owner		= THIS_MODULE,
		.fb_setcolreg	= exynos4412_fb_setcolreg,
		/*
		.fb_fillrect	= cfb_fillrect,
		.fb_copyarea	= cfb_copyarea,
		.fb_imageblit	= cfb_imageblit,
		*/

};

static int exynos4412_lcd_init(void){
	/*
	 *	1. 分配fb_info结构体
	 *	2. 设置该结构体
	 *	3. 硬件操作
	 *	4. 注册fb_info结构体
	 */
	
		//1. 分配fb_info结构体
		harxon_debug();
		info = framebuffer_alloc(0, NULL);
		
		//2. 设置fb_info
		//2.1 固定参数
		strcpy(info->fix.id, FB_NAME);		/* identification string eg "TT Builtin" */
		//info->fix.smem_start = ;	/* Start of frame buffer mem *//* (physical address) */
		info->fix.smem_len = 1024*(5+6+5)*600/8;			/*参考LCD手册 Length of frame buffer mem */
		info->fix.visual = FB_VISUAL_TRUECOLOR;			/*TFT屏是真彩色 see FB_VISUAL_*		*/ 
		info->fix.line_length = 1024*16/8;		/* length of a line in bytes    */
	
		harxon_debug();
		
		//2.2 可变参数
		info->var.xres = 1024;			/* visible resolution		*/
		info->var.yres = 600;
		info->var.xres_virtual = info->var.xres;		/* virtual resolution		*/
		info->var.yres_virtual = info->var.yres;
		
		info->var.bits_per_pixel = 5+6+5;		/* guess what			*/
		info->var.red.offset = 6+5;		/* bitfield in fb mem if true color, */	/* else only length is significant *//* beginning of bitfield	*/
		info->var.red.length = 5;			/* length of bitfield		*/
		info->var.green.offset = 5;
		info->var.green.length = 6;
		info->var.blue.offset = 0;
		info->var.blue.length = 5;
		
		harxon_debug();
		
		info->fbops = &exynos4412_lcd_fops;
		//	char __iomem *screen_base;	/* Virtual address */
		
		harxon_debug();
		
		info->screen_size = info->fix.smem_len;	/* Amount of ioremapped VRAM or 0 */ 
		info->pseudo_palette = a_pseudo_palette;		/* Fake palette of 16 colors */ 
		
		//3.1 硬件操作:
		/*显示控制寄存器，exynos手册12章节*/
		/*
		 *	LCDBLK_CFG = 0x1001_0000 + 0x0210
		 * 	FIMDBYPASS_LBLK0	bit[1]	1 = FIMD Bypass
		 *
		 *	LCDBLK_CFG2 = 0x1001_0000 + 0x0214
		 * MIE0_DISPON	bit[0]	 1 = PWM outpupt enable
		 */
		lcdblk_cfg = ioremap(0x10010000 + 0x0210, 8);
		lcdblk_cfg->lcdblk_cfg |= (1<<1);
		lcdblk_cfg->lcdblk_cfg2 |= (1<<0);
		 
		/*特殊寄存器SFR，设置LCD时钟, exynos4412手册7章节*/
		/*
		 *	CLK_SRC_LCD0 = 0x1003_0000 + 0xC234
		 * FIMD0_SEL	bit[3:0]	0110 = SCLKMPLL_USER_T
		 *	
		 */
		clk_src_lcd0 = ioremap(0x10030000 + 0xC234, 4);
		writel(readl(clk_src_lcd0) & ~(0xf<<0), clk_src_lcd0);
		writel(readl(clk_src_lcd0) | (0x6<<0), clk_src_lcd0);
		
		/*
		 *	CLK_DIV_LCD = 0x1003_0000 + 0xC534
		 *
		 * FIMD0_RATIO	bit[3:0]	0 = FIMD0_RATIO; SCLK_FIMD0 = MOUTFIMD0/(FIMD0_RATIO + 1)	
		 * 
		 */
		clk_drv_lcd = ioremap(0x10030000 + 0xC534, 4);
		writel(readl(clk_drv_lcd) & ~(0xf<<0), clk_drv_lcd);

		/*
		 * LCD的PWM使能输出 
		 * GPD0CON = 0x1140_0000 + 0x00A0
		 * 		bit[7:4]	0x1 = Output； 0x3 = LCD_PWM
		 * 	
		 * GPD0DAT = 0x1140_0000 + 0x00A4
		 *		bit[1] 0x1 = When configuring as output port then pin state should be same as
		 * corresponding bit. 
		 */
		 gpd0_con = ioremap(0x11400000 + 0x00A0, 8);
		 gpd0_dat = gpd0_con + 1;
		 writel((readl(gpd0_con) & ~(0xf<<4)) | (1<<4), gpd0_con);
		
		/*	
		 *	GPF0[0:3] LCD_HSYNC/ LCD_VSYNC/ LCD_DEN/ LCD_VOTCLK
		 *	GPF0[4:7] LCD_VD[0:3]
		 *	GPF0CON = 0x1140_0000 + 0x0180
		 *	GPF0DAT = 0x1140_0000 + 0x0184
		 *
		 *	GPF1[0:7] LCD_VD[4:11]
		 *	GPF1CON = 0x1140_0000 + 0x01A0
		 *	GPF1DAT = 0x1140_0000 + 0x01A4
		 *	
		 *	GPF2[0:7] LCD_VD[12:19]
		 *	GPF2CON = 0x1140_0000 + 0x01C0
		 *	GPF2DAT = 0x1140_0000 + 0x01C4
		 *
		 *	GPF3[0:3] LCD_VD[20:23]
		 *	GPF3CON = 0x1140_0000 + 0x01E0
		 *	GPF3DAT = 0x1140_0000 + 0x01E4
		 */
		fb_gpio = ioremap(0x11400000 + 0x0180, sizeof(struct EXYNOS4412_GPIO));
		Debug("sizeof(exynos4412_gpio_t):%d\n", sizeof(struct EXYNOS4412_LCD_REGISTER));
		harxon_debug();
		fb_gpio->gpf0con = 0x22222222;	/*参考exynos4412手册的GPIO，配置对应LCD管脚*/
		harxon_debug();
		fb_gpio->gpf1con = 0x22222222;
		harxon_debug();
		fb_gpio->gpf2con = 0x22222222;
		harxon_debug();
		fb_gpio->gpf3con = 0x00222222;
		harxon_debug();
		
		fb_reg = ioremap(0x11C00000 + 0x0000, sizeof(struct EXYNOS4412_LCD_REGISTER));
		harxon_debug();
		/*
		 *	VIDCON0	配置视频格式和显示使能和关闭.
		 *	VIDOUT 	bit[28:26]	000 = RGB interface
		 *	RGSPSEL	bit[18]	0 = RGB parallel format; 1 = RGB serial format
		 * PNRMODE	bit[17]	0 = Normal: RGBORDER[2] atVIDCON3; 1 = Invert: to RGBORDER[2] atVIDCON3
		 *	CLKVAL_F	bit[13:6] VCLK = FIMD * SCLK/(CLKVAL+1), where CLKVAL>=1(VCLK <= 80MHZ)
		 *						 		(160/10 - 1)
		 *	VCLKFREE	bit[5]	1 = Free-run mode
		 *	ENVID  [1]	0 = Disables the video output and display control signal; 1 = Enables the video output and display control signal
		 *	ENVID_F	[0]	0 = Disables the video output and display control signal； 1 = Enables the video output and display control signal
		 */
		fb_reg->vidcon0 = ((1<<18)| (15<<16)) & ~(3<<0);
		harxon_debug();
		
		/*
		 *	VIDCON1	规定RGB接口的控制信号.
		 *	FIXVCLK	bit[10:9]	01 = VCLK running
		 *	IHSYNC	bit[6]	1 = Inverted
		 *	IVSYNC	bit[5]	1 = Inverted
		 */
		fb_reg->vidcon1 = (1<<9)|(3<<5);
		harxon_debug();
		
		/*
		 *	VIDCON2	 规定输出数据控制格式
		 *	VIDCON3	 规定图像增强控制.
		 *	简单图片无需设置
		 */
		
		/*
		 * VIDTCON0 	配置视频输出时钟和决定显示尺寸
		 * VBPD	bit[23:16]	22 =  垂直后肩 = 垂直同步信号结束到新的一帧数据开始传递需要多少行
		 * VFPD bit[15:8]	12 = 垂直前肩 = 一帧数据结束到垂直同步信号的开始需要多少行
		 * VSPW	bit[7:0]	10 = 垂直同步脉冲宽度(行为单位)
		 */
		fb_reg->vidtcon0 = (22<<16)|(12<<8)|(10<<0);
		harxon_debug();
		/*
		 * VIDTCON1 	配置视频输出时钟和决定显示尺寸
		 * HBPD	bit[23:16]	160 = 水平后肩 = 垂直同步信号结束到新的一帧数据开始传递需要多少行
		 * HFPD bit[15:8]	160 = 水平前肩 = 一帧数据结束到垂直同步信号的开始需要多少行
		 * HSPW	bit[7:0]	10 = 水平同步脉冲宽度(VCLK为单位)
		 */
		fb_reg->vidtcon1 = (160<<16)|(160<<8)|(10<<0);
		harxon_debug();
		
		/*
		 * VIDTCON2	配置视频输出时钟和决定显示尺寸
		 * 600 * 1024
		 * LINEVAL  [21:11]	599  = lineval; VBPD + 1 < LINEVAL
		 * HOZVAL	[10:0]	1023 = hozval
		 */
		fb_reg->vidtcon2 = (599<<11)|(1023<<0);
		harxon_debug();
		
		/*
		 * WINCON0 窗口特性配置
		 * ENLOCAL_F	bit[16]	半字转换
		 * BPPMODE_F	bit[5:2]	0101 = 16 BPP (non-palletized, R:5-G:6-B:5)
		 * ENWIN_F	bit[0]	0 = Disables the video output and video control signal
		 */
		fb_reg->wincon0 = (1<<16)|(5<<2)|(0<<0);
		harxon_debug();

		/*
		 * SHADOWCON 图层通道的使能
		 * C0_EN_F  bit[0]	Enables Channel 0; 1 = Enables
		 */
		fb_reg->shadowcon = (1<<0);
		 
		
		/*	设置屏幕坐标和尺寸
		 *	VIDOSD0A
		 *	OSD_LeftTopX_F  [21:11]	Specifies the horizontal screen coordinate for left top pixel of OSD image.
		 *	OSD_LeftTopY_F  [10:0]	Specifies the vertical screen coordinate for left top pixel of OSD image.
		 *
		 *	VIDOSD0C
		 *	OSDSIZE  [23:0]	Specifies the Window Size For example, Height*Width (number of word)
		 */
		 fb_reg->vidosd0a = (1023<<11)|(599<<0);
		 fb_reg->vidosd0c = (1024*600);
		/*
		 * VIDWxxADDx: Specifies source image address setting.
		 *	
		 *VBASEU_F	[31:0]	Specifies A[31:0] of the start address for video frame buffer
		 *VBASEL_F	[31:0]	Specifies A[31:0] of the end address for video frame buffer.
		 *VBASEL = VBASEU + (PAGEWIDTH + OFFSIZE)*(LINEVAL + 1)
		 *PAGEWIDTH_F  [12:0]	Specifies virtual screen page width (number of byte).
		 */	 
		fb_reg->vindw00add0b0 = info->fix.smem_start;
		fb_reg->vindw00add1b0 = info->fix.smem_start + info->fix.smem_len;
		fb_reg->vidw00add2 = info->fix.line_length;


		/*3.2 申请虚拟DMA显存，传递信息给LCD控制器*/
		
		info->screen_base = dma_alloc_writecombine(NULL, info->fix.smem_len, (dma_addr_t*)(&(info->fix.smem_start)), GFP_KERNEL);


		 
		/*3.3 开启LCD */
		fb_reg->wincon0 |= (1<<0);
		//Enables the video output and display control signal
		fb_reg->vidcon0 |= (3<<0);
		
		writel(readl(gpd0_dat) | (1<<1), gpd0_dat);
		
		
		harxon_debug();
		
		/*4. 注册LCD*/
		register_framebuffer(info);	/*major==29, minor=0, /dev/fb0 */
		harxon_debug();
		
		return 0;
}
static void exynos4412_lcd_exit(void){
	
		harxon_debug();	
		
		unregister_framebuffer(info);
		
		dma_free_writecombine(NULL, info->fix.smem_len, &info->screen_base, info->fix.smem_start);
		
		iounmap(fb_reg);
		iounmap(fb_gpio);
		iounmap(gpd0_con);
		iounmap(clk_drv_lcd);
		iounmap(clk_src_lcd0);
		iounmap(lcdblk_cfg);
		
		framebuffer_release(info);
		
}
module_init(exynos4412_lcd_init);
module_exit(exynos4412_lcd_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("EXYNOS4412-LCD BASE DRIVER");

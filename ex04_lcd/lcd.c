/* refer to linux/drivers/video/s3c-fb.c
 *
 * Harxon write samsung SoC Framebuffer driver - lcd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software FoundatIon.
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/pm_runtime.h>
#include <linux/platform_data/video_s3c.h>

#include <video/samsung_fimd.h>

#include "lcd.h"
static void lcd_setup_gpio(void){
	
	;//
}
//platform data 
static struct s3c_fb_pd_win  pd_win[S3C_FB_MAX_WIN] = {
	[0] = {
		.default_bpp = 0,
		.max_bpp = 0,
		.xres = 0,
		.yres = 0,
		.virtual_x = 0,
		.virtual_y = 0,
	},
	[1] = {},
	[2] = {},
	[3] = {},
	[4] = {},
};
static struct fb_videomode fb_vtiming = {
		.name = "",
		.refresh = 0,
		.xres = 0,
		.yres = 0,
		.pixclock = 0,
		.left_margin = 0,
		.right_margin = 0,
		.upper_margin = 0,
		.lower_margin = 0,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = 0,
		.vmode = 0,
		.flag = 0,
};

static struct s3c_fb_platdata pd[] = {
	{
		.setup_gpio = lcd_setup_gpio,
		.win[0] = &pd_win[0],
		.win[1] = &pd_win[1],
		.win[2] = &pd_win[2],
		.win[3] = &pd_win[3],
		.win[4] = &pd_win[4],
		.vtiming = &fb_vtiming,
		.vidcon0 = 0,
		.vidcon1 = 0,
	},
	{},
};

#define VALID_BPP124 (VALID_BPP(1) | VALID_BPP(2) | VALID_BPP(4))
#define VALID_BPP1248 (VALID_BPP124 | VALID_BPP(8))

static struct s3c_fb_win_variant s3c_fb_data_s5p_wins[] = {
	[0] = {
		.has_osd_c	= 1,
		//支持设置窗口0的OSD的偏移量
		.osd_size_off	= 0x8,
		//登记过的调色板的大小
		.palette_sz	= 256,
		//像素深度的设置
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
	[1] = {
		.has_osd_c	= 1,
		.has_osd_d	= 1,
		.osd_size_off	= 0xc,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
	[2] = {
		.has_osd_c	= 1,
		.has_osd_d	= 1,
		.osd_size_off	= 0xc,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
	[3] = {
		.has_osd_c	= 1,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
	[4] = {
		.has_osd_c	= 1,
		.has_osd_alpha	= 1,
		.palette_sz	= 256,
		.valid_bpp	= (VALID_BPP1248 | VALID_BPP(13) |
				   VALID_BPP(15) | VALID_BPP(16) |
				   VALID_BPP(18) | VALID_BPP(19) |
				   VALID_BPP(24) | VALID_BPP(25) |
				   VALID_BPP(32)),
	},
};


static struct s3c_fb_driverdata s3c_fb_data_exynos4 = {
	.variant = {
		//初始化的窗口数量为5
		.nr_windows	= 5,
		//vidtcon @配置视频输出时间和确定的显示大小的寄存器偏移地址
		.vidtcon	= VIDTCON0,
		//wincon @指定win0窗口特性...
		.wincon		= WINCON(0),
		//winmap @指定win0窗口颜色...
		.winmap		= WINxMAP(0),
		//keycon @指定颜色键设置寄存器W1KEYCON0  
		.keycon		= WKEYCON,
		//osd @指定视频窗口 0 的位置控制寄存器VIDOSD0A
		.osd		= VIDOSD_BASE,
		.osd_stride	= 16,
		//buf_start @指定窗口 0 的缓冲区起始地址寄存器，buffer0
		.buf_start	= VIDW_BUF_START(0),
		//buf_size @指定窗口 0 的缓冲大小寄存器
		.buf_size	= VIDW_BUF_SIZE(0),
		//buf_end @指定窗口 0 的缓冲结束地址寄存器，buffer0
		.buf_end	= VIDW_BUF_END(0),

		.palette = {
			//Win0 PalRam @指定 0 到 255 项调色板数据
			[0] = 0x2400,
			//Win1 PalRam @指定 0 到 255 项调色板数据
			[1] = 0x2800,
			[2] = 0x2c00,
			[3] = 0x3000,
			[4] = 0x3400,
		},

		.has_shadowcon	= 1,
		.has_blendcon	= 1,
		.has_fixvclk	= 1,
	},
	.win[0]	= &s3c_fb_data_s5p_wins[0],
	.win[1]	= &s3c_fb_data_s5p_wins[1],
	.win[2]	= &s3c_fb_data_s5p_wins[2],
	.win[3]	= &s3c_fb_data_s5p_wins[3],
	.win[4]	= &s3c_fb_data_s5p_wins[4],
};


/**
 * s3c_fb_validate_win_bpp - validate the bits-per-pixel for this mode.
 * @win: The device window.
 * @bpp: The bit depth.
 */
static bool s3c_fb_validate_win_bpp(struct s3c_fb_win *win, unsigned int bpp)
{
	return win->variant.valid_bpp & VALID_BPP(bpp);
}

/**
 * s3c_fb_check_var() - framebuffer layer request to verify a given mode.
 * @var: The screen information to verify.
 * @info: The framebuffer device.
 *
 * Framebuffer layer call to verify the given information and allow us to
 * update various information depending on the hardware capabilities.
 */
static int s3c_fb_check_var(struct fb_var_screeninfo *var,
			    struct fb_info *info)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;

	dev_dbg(sfb->dev, "checking parameters\n");

	var->xres_virtual = max(var->xres_virtual, var->xres);
	var->yres_virtual = max(var->yres_virtual, var->yres);

	if (!s3c_fb_validate_win_bpp(win, var->bits_per_pixel)) {
		dev_dbg(sfb->dev, "win %d: unsupported bpp %d\n",
			win->index, var->bits_per_pixel);
		return -EINVAL;
	}

	/* always ensure these are zero, for drop through cases below */
	var->transp.offset = 0;
	var->transp.length = 0;

	switch (var->bits_per_pixel) {
	case 1:
	case 2:
	case 4:
	case 8:
		if (sfb->variant.palette[win->index] != 0) {
			/* non palletised, A:1,R:2,G:3,B:2 mode */
			var->red.offset		= 5;
			var->green.offset	= 2;
			var->blue.offset	= 0;
			var->red.length		= 2;
			var->green.length	= 3;
			var->blue.length	= 2;
			var->transp.offset	= 7;
			var->transp.length	= 1;
		} else {
			var->red.offset	= 0;
			var->red.length	= var->bits_per_pixel;
			var->green	= var->red;
			var->blue	= var->red;
		}
		break;

	case 19:
		/* 666 with one bit alpha/transparency */
		var->transp.offset	= 18;
		var->transp.length	= 1;
		/* drop through */
	case 18:
		var->bits_per_pixel	= 32;

		/* 666 format */
		var->red.offset		= 12;
		var->green.offset	= 6;
		var->blue.offset	= 0;
		var->red.length		= 6;
		var->green.length	= 6;
		var->blue.length	= 6;
		break;

	case 16:
		/* 16 bpp, 565 format */
		var->red.offset		= 11;
		var->green.offset	= 5;
		var->blue.offset	= 0;
		var->red.length		= 5;
		var->green.length	= 6;
		var->blue.length	= 5;
		break;

	case 32:
	case 28:
	case 25:
		var->transp.length	= var->bits_per_pixel - 24;
		var->transp.offset	= 24;
		/* drop through */
	case 24:
		/* our 24bpp is unpacked, so 32bpp */
		var->bits_per_pixel	= 32;
		var->red.offset		= 16;
		var->red.length		= 8;
		var->green.offset	= 8;
		var->green.length	= 8;
		var->blue.offset	= 0;
		var->blue.length	= 8;
		break;

	default:
		dev_err(sfb->dev, "invalid bpp\n");
		return -EINVAL;
	}

	dev_dbg(sfb->dev, "%s: verified parameters\n", __func__);
	return 0;
}

/**
 * s3c_fb_calc_pixclk() - calculate the divider to create the pixel clock.
 * @sfb: The hardware state.
 * @pixclock: The pixel clock wanted, in picoseconds.
 *
 * Given the specified pixel clock, work out the necessary divider to get
 * close to the output frequency.
 */
static int s3c_fb_calc_pixclk(struct s3c_fb *sfb, unsigned int pixclk)
{
	unsigned long clk;
	unsigned long long tmp;
	unsigned int result;

	if (sfb->variant.has_clksel)
		clk = clk_get_rate(sfb->bus_clk);
	else
		clk = clk_get_rate(sfb->lcd_clk);

	tmp = (unsigned long long)clk;
	tmp *= pixclk;

	do_div(tmp, 1000000000UL);
	result = (unsigned int)tmp / 1000;

	dev_dbg(sfb->dev, "pixclk=%u, clk=%lu, div=%d (%lu)\n",
		pixclk, clk, result, result ? clk / result : clk);

	return result;
}

/**
 * s3c_fb_align_word() - align pixel count to word boundary
 * @bpp: The number of bits per pixel
 * @pix: The value to be aligned.
 *
 * Align the given pixel count so that it will start on an 32bit word
 * boundary.
 */
static int s3c_fb_align_word(unsigned int bpp, unsigned int pix)
{
	int pix_per_word;

	if (bpp > 16)
		return pix;

	pix_per_word = (8 * 32) / bpp;
	return ALIGN(pix, pix_per_word);
}

/**
 * vidosd_set_size() - set OSD size for a window
 *
 * @win: the window to set OSD size for
 * @size: OSD size register value
 */
static void vidosd_set_size(struct s3c_fb_win *win, u32 size)
{
	struct s3c_fb *sfb = win->parent;

	/* OSD can be set up if osd_size_off != 0 for this window */
	if (win->variant.osd_size_off)
		writel(size, sfb->regs + OSD_BASE(win->index, sfb->variant)
				+ win->variant.osd_size_off);
}

/**
 * vidosd_set_alpha() - set alpha transparency for a window
 *
 * @win: the window to set OSD size for
 * @alpha: alpha register value
 */
static void vidosd_set_alpha(struct s3c_fb_win *win, u32 alpha)
{
	struct s3c_fb *sfb = win->parent;

	if (win->variant.has_osd_alpha)
		writel(alpha, sfb->regs + VIDOSD_C(win->index, sfb->variant));
}

/**
 * shadow_protect_win() - disable updating values from shadow registers at vsync
 *
 * @win: window to protect registers for
 * @protect: 1 to protect (disable updates)
 */
static void shadow_protect_win(struct s3c_fb_win *win, bool protect)
{
	struct s3c_fb *sfb = win->parent;
	u32 reg;

	if (protect) {
		if (sfb->variant.has_prtcon) {
			writel(PRTCON_PROTECT, sfb->regs + PRTCON);
		} else if (sfb->variant.has_shadowcon) {
			reg = readl(sfb->regs + SHADOWCON);
			writel(reg | SHADOWCON_WINx_PROTECT(win->index),
				sfb->regs + SHADOWCON);
		}
	} else {
		if (sfb->variant.has_prtcon) {
			writel(0, sfb->regs + PRTCON);
		} else if (sfb->variant.has_shadowcon) {
			reg = readl(sfb->regs + SHADOWCON);
			writel(reg & ~SHADOWCON_WINx_PROTECT(win->index),
				sfb->regs + SHADOWCON);
		}
	}
}

/**
 * s3c_fb_enable() - Set the state of the main LCD output
 * @sfb: The main framebuffer state.
 * @enable: The state to set.
 */
static void s3c_fb_enable(struct s3c_fb *sfb, int enable)
{
	u32 vidcon0 = readl(sfb->regs + VIDCON0);

	if (enable && !sfb->output_on)
		pm_runtime_get_sync(sfb->dev);

	if (enable) {
		vidcon0 |= VIDCON0_ENVID | VIDCON0_ENVID_F;
	} else {
		/* see the note in the framebuffer datasheet about
		 * why you cannot take both of these bits down at the
		 * same time. */

		if (vidcon0 & VIDCON0_ENVID) {
			vidcon0 |= VIDCON0_ENVID;
			vidcon0 &= ~VIDCON0_ENVID_F;
		}
	}

	writel(vidcon0, sfb->regs + VIDCON0);

	if (!enable && sfb->output_on)
		pm_runtime_put_sync(sfb->dev);

	sfb->output_on = enable;
}

/**
 * s3c_fb_set_par() - framebuffer request to set new framebuffer state.
 * @info: The framebuffer to change.
 *
 * Framebuffer layer request to set a new mode for the specified framebuffer
 */
static int s3c_fb_set_par(struct fb_info *info)
{
	struct fb_var_screeninfo *var = &info->var;
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;
	void __iomem *regs = sfb->regs;
	void __iomem *buf = regs;
	int win_no = win->index;
	u32 alpha = 0;
	u32 data;
	u32 pagewidth;

	dev_dbg(sfb->dev, "setting framebuffer parameters\n");

	pm_runtime_get_sync(sfb->dev);

	shadow_protect_win(win, 1);

	switch (var->bits_per_pixel) {
	case 32:
	case 24:
	case 16:
	case 12:
		info->fix.visual = FB_VISUAL_TRUECOLOR;
		break;
	case 8:
		if (win->variant.palette_sz >= 256)
			info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
		else
			info->fix.visual = FB_VISUAL_TRUECOLOR;
		break;
	case 1:
		info->fix.visual = FB_VISUAL_MONO01;
		break;
	default:
		info->fix.visual = FB_VISUAL_PSEUDOCOLOR;
		break;
	}

	info->fix.line_length = (var->xres_virtual * var->bits_per_pixel) / 8;

	info->fix.xpanstep = info->var.xres_virtual > info->var.xres ? 1 : 0;
	info->fix.ypanstep = info->var.yres_virtual > info->var.yres ? 1 : 0;

	/* disable the window whilst we update it */
	writel(0, regs + WINCON(win_no));

	if (!sfb->output_on)
		s3c_fb_enable(sfb, 1);

	/* write the buffer address */

	/* start and end registers stride is 8 */
	buf = regs + win_no * 8;

	writel(info->fix.smem_start, buf + sfb->variant.buf_start);

	data = info->fix.smem_start + info->fix.line_length * var->yres;
	writel(data, buf + sfb->variant.buf_end);

	pagewidth = (var->xres * var->bits_per_pixel) >> 3;
	data = VIDW_BUF_SIZE_OFFSET(info->fix.line_length - pagewidth) |
	       VIDW_BUF_SIZE_PAGEWIDTH(pagewidth) |
	       VIDW_BUF_SIZE_OFFSET_E(info->fix.line_length - pagewidth) |
	       VIDW_BUF_SIZE_PAGEWIDTH_E(pagewidth);
	writel(data, regs + sfb->variant.buf_size + (win_no * 4));

	/* write 'OSD' registers to control position of framebuffer */

	data = VIDOSDxA_TOPLEFT_X(0) | VIDOSDxA_TOPLEFT_Y(0) |
	       VIDOSDxA_TOPLEFT_X_E(0) | VIDOSDxA_TOPLEFT_Y_E(0);
	writel(data, regs + VIDOSD_A(win_no, sfb->variant));

	data = VIDOSDxB_BOTRIGHT_X(s3c_fb_align_word(var->bits_per_pixel,
						     var->xres - 1)) |
	       VIDOSDxB_BOTRIGHT_Y(var->yres - 1) |
	       VIDOSDxB_BOTRIGHT_X_E(s3c_fb_align_word(var->bits_per_pixel,
						     var->xres - 1)) |
	       VIDOSDxB_BOTRIGHT_Y_E(var->yres - 1);

	writel(data, regs + VIDOSD_B(win_no, sfb->variant));

	data = var->xres * var->yres;

	alpha = VIDISD14C_ALPHA1_R(0xf) |
		VIDISD14C_ALPHA1_G(0xf) |
		VIDISD14C_ALPHA1_B(0xf);

	vidosd_set_alpha(win, alpha);
	vidosd_set_size(win, data);

	/* Enable DMA channel for this window */
	if (sfb->variant.has_shadowcon) {
		data = readl(sfb->regs + SHADOWCON);
		data |= SHADOWCON_CHx_ENABLE(win_no);
		writel(data, sfb->regs + SHADOWCON);
	}

	data = WINCONx_ENWIN;
	sfb->enabled |= (1 << win->index);

	/* note, since we have to round up the bits-per-pixel, we end up
	 * relying on the bitfield information for r/g/b/a to work out
	 * exactly which mode of operation is intended. */

	switch (var->bits_per_pixel) {
	case 1:
		data |= WINCON0_BPPMODE_1BPP;
		data |= WINCONx_BITSWP;
		data |= WINCONx_BURSTLEN_4WORD;
		break;
	case 2:
		data |= WINCON0_BPPMODE_2BPP;
		data |= WINCONx_BITSWP;
		data |= WINCONx_BURSTLEN_8WORD;
		break;
	case 4:
		data |= WINCON0_BPPMODE_4BPP;
		data |= WINCONx_BITSWP;
		data |= WINCONx_BURSTLEN_8WORD;
		break;
	case 8:
		if (var->transp.length != 0)
			data |= WINCON1_BPPMODE_8BPP_1232;
		else
			data |= WINCON0_BPPMODE_8BPP_PALETTE;
		data |= WINCONx_BURSTLEN_8WORD;
		data |= WINCONx_BYTSWP;
		break;
	case 16:
		if (var->transp.length != 0)
			data |= WINCON1_BPPMODE_16BPP_A1555;
		else
			data |= WINCON0_BPPMODE_16BPP_565;
		data |= WINCONx_HAWSWP;
		data |= WINCONx_BURSTLEN_16WORD;
		break;
	case 24:
	case 32:
		if (var->red.length == 6) {
			if (var->transp.length != 0)
				data |= WINCON1_BPPMODE_19BPP_A1666;
			else
				data |= WINCON1_BPPMODE_18BPP_666;
		} else if (var->transp.length == 1)
			data |= WINCON1_BPPMODE_25BPP_A1888
				| WINCON1_BLD_PIX;
		else if ((var->transp.length == 4) ||
			(var->transp.length == 8))
			data |= WINCON1_BPPMODE_28BPP_A4888
				| WINCON1_BLD_PIX | WINCON1_ALPHA_SEL;
		else
			data |= WINCON0_BPPMODE_24BPP_888;

		data |= WINCONx_WSWP;
		data |= WINCONx_BURSTLEN_16WORD;
		break;
	}

	/* Enable the colour keying for the window below this one */
	if (win_no > 0) {
		u32 keycon0_data = 0, keycon1_data = 0;
		void __iomem *keycon = regs + sfb->variant.keycon;

		keycon0_data = ~(WxKEYCON0_KEYBL_EN |
				WxKEYCON0_KEYEN_F |
				WxKEYCON0_DIRCON) | WxKEYCON0_COMPKEY(0);

		keycon1_data = WxKEYCON1_COLVAL(0xffffff);

		keycon += (win_no - 1) * 8;

		writel(keycon0_data, keycon + WKEYCON0);
		writel(keycon1_data, keycon + WKEYCON1);
	}

	writel(data, regs + sfb->variant.wincon + (win_no * 4));
	writel(0x0, regs + sfb->variant.winmap + (win_no * 4));

	/* Set alpha value width */
	if (sfb->variant.has_blendcon) {
		data = readl(sfb->regs + BLENDCON);
		data &= ~BLENDCON_NEW_MASK;
		if (var->transp.length > 4)
			data |= BLENDCON_NEW_8BIT_ALPHA_VALUE;
		else
			data |= BLENDCON_NEW_4BIT_ALPHA_VALUE;
		writel(data, sfb->regs + BLENDCON);
	}

	shadow_protect_win(win, 0);

	pm_runtime_put_sync(sfb->dev);

	return 0;
}

/**
 * s3c_fb_update_palette() - set or schedule a palette update.
 * @sfb: The hardware information.
 * @win: The window being updated.
 * @reg: The palette index being changed.
 * @value: The computed palette value.
 *
 * Change the value of a palette register, either by directly writing to
 * the palette (this requires the palette RAM to be disconnected from the
 * hardware whilst this is in progress) or schedule the update for later.
 *
 * At the moment, since we have no VSYNC interrupt support, we simply set
 * the palette entry directly.
 */
static void s3c_fb_update_palette(struct s3c_fb *sfb,
				  struct s3c_fb_win *win,
				  unsigned int reg,
				  u32 value)
{
	void __iomem *palreg;
	u32 palcon;

	palreg = sfb->regs + sfb->variant.palette[win->index];

	dev_dbg(sfb->dev, "%s: win %d, reg %d (%p): %08x\n",
		__func__, win->index, reg, palreg, value);

	win->palette_buffer[reg] = value;

	palcon = readl(sfb->regs + WPALCON);
	writel(palcon | WPALCON_PAL_UPDATE, sfb->regs + WPALCON);

	if (win->variant.palette_16bpp)
		writew(value, palreg + (reg * 2));
	else
		writel(value, palreg + (reg * 4));

	writel(palcon, sfb->regs + WPALCON);
}

static inline unsigned int chan_to_field(unsigned int chan,
					 struct fb_bitfield *bf)
{
	chan &= 0xffff;
	chan >>= 16 - bf->length;
	return chan << bf->offset;
}

/**
 * s3c_fb_setcolreg() - framebuffer layer request to change palette.
 * @regno: The palette index to change.
 * @red: The red field for the palette data.
 * @green: The green field for the palette data.
 * @blue: The blue field for the palette data.
 * @trans: The transparency (alpha) field for the palette data.
 * @info: The framebuffer being changed.
 */
static int s3c_fb_setcolreg(unsigned regno,
			    unsigned red, unsigned green, unsigned blue,
			    unsigned transp, struct fb_info *info)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;
	unsigned int val;

	dev_dbg(sfb->dev, "%s: win %d: %d => rgb=%d/%d/%d\n",
		__func__, win->index, regno, red, green, blue);

	pm_runtime_get_sync(sfb->dev);

	switch (info->fix.visual) {
	case FB_VISUAL_TRUECOLOR:
		/* true-colour, use pseudo-palette */

		if (regno < 16) {
			u32 *pal = info->pseudo_palette;

			val  = chan_to_field(red,   &info->var.red);
			val |= chan_to_field(green, &info->var.green);
			val |= chan_to_field(blue,  &info->var.blue);

			pal[regno] = val;
		}
		break;

	case FB_VISUAL_PSEUDOCOLOR:
		if (regno < win->variant.palette_sz) {
			val  = chan_to_field(red, &win->palette.r);
			val |= chan_to_field(green, &win->palette.g);
			val |= chan_to_field(blue, &win->palette.b);

			s3c_fb_update_palette(sfb, win, regno, val);
		}

		break;

	default:
		pm_runtime_put_sync(sfb->dev);
		return 1;	/* unknown type */
	}

	pm_runtime_put_sync(sfb->dev);
	return 0;
}

/**
 * s3c_fb_blank() - blank or unblank the given window
 * @blank_mode: The blank state from FB_BLANK_*
 * @info: The framebuffer to blank.
 *
 * Framebuffer layer request to change the power state.
 */
static int s3c_fb_blank(int blank_mode, struct fb_info *info)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;
	unsigned int index = win->index;
	u32 wincon;
	u32 output_on = sfb->output_on;

	dev_dbg(sfb->dev, "blank mode %d\n", blank_mode);

	pm_runtime_get_sync(sfb->dev);

	wincon = readl(sfb->regs + sfb->variant.wincon + (index * 4));

	switch (blank_mode) {
	case FB_BLANK_POWERDOWN:
		wincon &= ~WINCONx_ENWIN;
		sfb->enabled &= ~(1 << index);
		/* fall through to FB_BLANK_NORMAL */

	case FB_BLANK_NORMAL:
		/* disable the DMA and display 0x0 (black) */
		shadow_protect_win(win, 1);
		writel(WINxMAP_MAP | WINxMAP_MAP_COLOUR(0x0),
		       sfb->regs + sfb->variant.winmap + (index * 4));
		shadow_protect_win(win, 0);
		break;

	case FB_BLANK_UNBLANK:
		shadow_protect_win(win, 1);
		writel(0x0, sfb->regs + sfb->variant.winmap + (index * 4));
		shadow_protect_win(win, 0);
		wincon |= WINCONx_ENWIN;
		sfb->enabled |= (1 << index);
		break;

	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	default:
		pm_runtime_put_sync(sfb->dev);
		return 1;
	}

	shadow_protect_win(win, 1);
	writel(wincon, sfb->regs + sfb->variant.wincon + (index * 4));

	/* Check the enabled state to see if we need to be running the
	 * main LCD interface, as if there are no active windows then
	 * it is highly likely that we also do not need to output
	 * anything.
	 */
	s3c_fb_enable(sfb, sfb->enabled ? 1 : 0);
	shadow_protect_win(win, 0);

	pm_runtime_put_sync(sfb->dev);

	return output_on == sfb->output_on;
}

/**
 * s3c_fb_pan_display() - Pan the display.
 *
 * Note that the offsets can be written to the device at any time, as their
 * values are latched at each vsync automatically. This also means that only
 * the last call to this function will have any effect on next vsync, but
 * there is no need to sleep waiting for it to prevent tearing.
 *
 * @var: The screen information to verify.
 * @info: The framebuffer device.
 */
static int s3c_fb_pan_display(struct fb_var_screeninfo *var,
			      struct fb_info *info)
{
	struct s3c_fb_win *win	= info->par;
	struct s3c_fb *sfb	= win->parent;
	void __iomem *buf	= sfb->regs + win->index * 8;
	unsigned int start_boff, end_boff;

	pm_runtime_get_sync(sfb->dev);

	/* Offset in bytes to the start of the displayed area */
	start_boff = var->yoffset * info->fix.line_length;
	/* X offset depends on the current bpp */
	if (info->var.bits_per_pixel >= 8) {
		start_boff += var->xoffset * (info->var.bits_per_pixel >> 3);
	} else {
		switch (info->var.bits_per_pixel) {
		case 4:
			start_boff += var->xoffset >> 1;
			break;
		case 2:
			start_boff += var->xoffset >> 2;
			break;
		case 1:
			start_boff += var->xoffset >> 3;
			break;
		default:
			dev_err(sfb->dev, "invalid bpp\n");
			pm_runtime_put_sync(sfb->dev);
			return -EINVAL;
		}
	}
	/* Offset in bytes to the end of the displayed area */
	end_boff = start_boff + info->var.yres * info->fix.line_length;

	/* Temporarily turn off per-vsync update from shadow registers until
	 * both start and end addresses are updated to prevent corruption */
	shadow_protect_win(win, 1);

	writel(info->fix.smem_start + start_boff, buf + sfb->variant.buf_start);
	writel(info->fix.smem_start + end_boff, buf + sfb->variant.buf_end);

	shadow_protect_win(win, 0);

	pm_runtime_put_sync(sfb->dev);
	return 0;
}

/**
 * s3c_fb_enable_irq() - enable framebuffer interrupts
 * @sfb: main hardware state
 */
static void s3c_fb_enable_irq(struct s3c_fb *sfb)
{
	void __iomem *regs = sfb->regs;
	u32 irq_ctrl_reg;

	if (!test_and_set_bit(S3C_FB_VSYNC_IRQ_EN, &sfb->irq_flags)) {
		/* IRQ disabled, enable it */
		irq_ctrl_reg = readl(regs + VIDINTCON0);

		irq_ctrl_reg |= VIDINTCON0_INT_ENABLE;
		irq_ctrl_reg |= VIDINTCON0_INT_FRAME;

		irq_ctrl_reg &= ~VIDINTCON0_FRAMESEL0_MASK;
		irq_ctrl_reg |= VIDINTCON0_FRAMESEL0_VSYNC;
		irq_ctrl_reg &= ~VIDINTCON0_FRAMESEL1_MASK;
		irq_ctrl_reg |= VIDINTCON0_FRAMESEL1_NONE;

		writel(irq_ctrl_reg, regs + VIDINTCON0);
	}
}

/**
 * s3c_fb_disable_irq() - disable framebuffer interrupts
 * @sfb: main hardware state
 */
static void s3c_fb_disable_irq(struct s3c_fb *sfb)
{
	void __iomem *regs = sfb->regs;
	u32 irq_ctrl_reg;

	if (test_and_clear_bit(S3C_FB_VSYNC_IRQ_EN, &sfb->irq_flags)) {
		/* IRQ enabled, disable it */
		irq_ctrl_reg = readl(regs + VIDINTCON0);

		irq_ctrl_reg &= ~VIDINTCON0_INT_FRAME;
		irq_ctrl_reg &= ~VIDINTCON0_INT_ENABLE;

		writel(irq_ctrl_reg, regs + VIDINTCON0);
	}
}

static irqreturn_t s3c_fb_irq(int irq, void *dev_id)
{
	struct s3c_fb *sfb = dev_id;
	void __iomem  *regs = sfb->regs;
	u32 irq_sts_reg;

	spin_lock(&sfb->slock);

	irq_sts_reg = readl(regs + VIDINTCON1);

	if (irq_sts_reg & VIDINTCON1_INT_FRAME) {

		/* VSYNC interrupt, accept it */
		writel(VIDINTCON1_INT_FRAME, regs + VIDINTCON1);

		sfb->vsync_info.count++;
		wake_up_interruptible(&sfb->vsync_info.wait);
	}

	/* We only support waiting for VSYNC for now, so it's safe
	 * to always disable irqs here.
	 */
	s3c_fb_disable_irq(sfb);

	spin_unlock(&sfb->slock);
	return IRQ_HANDLED;
}

/**
 * s3c_fb_wait_for_vsync() - sleep until next VSYNC interrupt or timeout
 * @sfb: main hardware state
 * @crtc: head index.
 */
static int s3c_fb_wait_for_vsync(struct s3c_fb *sfb, u32 crtc)
{
	unsigned long count;
	int ret;

	if (crtc != 0)
		return -ENODEV;

	pm_runtime_get_sync(sfb->dev);

	count = sfb->vsync_info.count;
	s3c_fb_enable_irq(sfb);
	ret = wait_event_interruptible_timeout(sfb->vsync_info.wait,
				       count != sfb->vsync_info.count,
				       msecs_to_jiffies(VSYNC_TIMEOUT_MSEC));

	pm_runtime_put_sync(sfb->dev);

	if (ret == 0)
		return -ETIMEDOUT;

	return 0;
}

static int s3c_fb_ioctl(struct fb_info *info, unsigned int cmd,
			unsigned long arg)
{
	struct s3c_fb_win *win = info->par;
	struct s3c_fb *sfb = win->parent;
	int ret;
	u32 crtc;

	switch (cmd) {
	case FBIO_WAITFORVSYNC:
		if (get_user(crtc, (u32 __user *)arg)) {
			ret = -EFAULT;
			break;
		}

		ret = s3c_fb_wait_for_vsync(sfb, crtc);
		break;
	default:
		ret = -ENOTTY;
	}

	return ret;
}

static struct fb_ops s3c_fb_ops = {
	.owner		= THIS_MODULE,
	.fb_check_var	= s3c_fb_check_var,
	.fb_set_par	= s3c_fb_set_par,
	.fb_blank	= s3c_fb_blank,
	.fb_setcolreg	= s3c_fb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
	.fb_pan_display	= s3c_fb_pan_display,
	.fb_ioctl	= s3c_fb_ioctl,
};

/**
 * s3c_fb_missing_pixclock() - calculates pixel clock
 * @mode: The video mode to change.
 *
 * Calculate the pixel clock when none has been given through platform data.
 */
static void s3c_fb_missing_pixclock(struct fb_videomode *mode)
{
	u64 pixclk = 1000000000000ULL;
	u32 div;

	div  = mode->left_margin + mode->hsync_len + mode->right_margin +
	       mode->xres;
	div *= mode->upper_margin + mode->vsync_len + mode->lower_margin +
	       mode->yres;
	div *= mode->refresh ? : 60;

	do_div(pixclk, div);

	mode->pixclock = pixclk;
}

/**
 * s3c_fb_alloc_memory() - allocate display memory for framebuffer window
 * @sfb: The base resources for the hardware.
 * @win: The window to initialise memory for.
 *
 * Allocate memory for the given framebuffer.
 */
static int s3c_fb_alloc_memory(struct s3c_fb *sfb, struct s3c_fb_win *win)
{
	struct s3c_fb_pd_win *windata = win->windata;
	unsigned int real_size, virt_size, size;
	struct fb_info *fbi = win->fbinfo;
	dma_addr_t map_dma;

	dev_dbg(sfb->dev, "allocating memory for display\n");

	real_size = windata->xres * windata->yres;
	virt_size = windata->virtual_x * windata->virtual_y;

	dev_dbg(sfb->dev, "real_size=%u (%u.%u), virt_size=%u (%u.%u)\n",
		real_size, windata->xres, windata->yres,
		virt_size, windata->virtual_x, windata->virtual_y);

	size = (real_size > virt_size) ? real_size : virt_size;
	size *= (windata->max_bpp > 16) ? 32 : windata->max_bpp;
	size /= 8;

	fbi->fix.smem_len = size;
	size = PAGE_ALIGN(size);

	dev_dbg(sfb->dev, "want %u bytes for window\n", size);

	fbi->screen_base = dma_alloc_writecombine(sfb->dev, size,
						  &map_dma, GFP_KERNEL);
	if (!fbi->screen_base)
		return -ENOMEM;

	dev_dbg(sfb->dev, "mapped %x to %p\n",
		(unsigned int)map_dma, fbi->screen_base);

	memset(fbi->screen_base, 0x0, size);
	fbi->fix.smem_start = map_dma;

	return 0;
}

/**
 * s3c_fb_free_memory() - free the display memory for the given window
 * @sfb: The base resources for the hardware.
 * @win: The window to free the display memory for.
 *
 * Free the display memory allocated by s3c_fb_alloc_memory().
 */
static void s3c_fb_free_memory(struct s3c_fb *sfb, struct s3c_fb_win *win)
{
	struct fb_info *fbi = win->fbinfo;

	if (fbi->screen_base)
		dma_free_writecombine(sfb->dev, PAGE_ALIGN(fbi->fix.smem_len),
			      fbi->screen_base, fbi->fix.smem_start);
}

/**
 * s3c_fb_release_win() - release resources for a framebuffer window.
 * @win: The window to cleanup the resources for.
 *
 * Release the resources that where claimed for the hardware window,
 * such as the framebuffer instance and any memory claimed for it.
 */
static void s3c_fb_release_win(struct s3c_fb *sfb, struct s3c_fb_win *win)
{
	u32 data;

	if (win->fbinfo) {
		if (sfb->variant.has_shadowcon) {
			data = readl(sfb->regs + SHADOWCON);
			data &= ~SHADOWCON_CHx_ENABLE(win->index);
			data &= ~SHADOWCON_CHx_LOCAL_ENABLE(win->index);
			writel(data, sfb->regs + SHADOWCON);
		}
		unregister_framebuffer(win->fbinfo);
		if (win->fbinfo->cmap.len)
			fb_dealloc_cmap(&win->fbinfo->cmap);
		s3c_fb_free_memory(sfb, win);
		framebuffer_release(win->fbinfo);
	}
}

/**
 * s3c_fb_probe_win() - register an hardware window
 * @sfb: The base resources for the hardware
 * @variant: The variant information for this window.
 * @res: Pointer to where to place the resultant window.
 *
 * Allocate and do the basic initialisation for one of the hardware's graphics
 * windows.
 */
static int s3c_fb_probe_win(struct s3c_fb *sfb, unsigned int win_no,
			    struct s3c_fb_win_variant *variant,
			    struct s3c_fb_win **res)
{
	struct fb_var_screeninfo *var;
	struct fb_videomode initmode;
	struct s3c_fb_pd_win *windata;
	struct s3c_fb_win *win;
	struct fb_info *fbinfo;
	int palette_size;
	int ret;

	dev_dbg(sfb->dev, "probing window %d, variant %p\n", win_no, variant);

	init_waitqueue_head(&sfb->vsync_info.wait);

	palette_size = variant->palette_sz * 4;

	fbinfo = framebuffer_alloc(sizeof(struct s3c_fb_win) +
				   palette_size * sizeof(u32), sfb->dev);
	if (!fbinfo) {
		dev_err(sfb->dev, "failed to allocate framebuffer\n");
		return -ENOENT;
	}

	windata = sfb->pdata->win[win_no];
	initmode = *sfb->pdata->vtiming;

	WARN_ON(windata->max_bpp == 0);
	WARN_ON(windata->xres == 0);
	WARN_ON(windata->yres == 0);

	win = fbinfo->par;
	*res = win;
	var = &fbinfo->var;
	win->variant = *variant;
	win->fbinfo = fbinfo;
	win->parent = sfb;
	win->windata = windata;
	win->index = win_no;
	win->palette_buffer = (u32 *)(win + 1);

	ret = s3c_fb_alloc_memory(sfb, win);
	if (ret) {
		dev_err(sfb->dev, "failed to allocate display memory\n");
		return ret;
	}

	/* setup the r/b/g positions for the window's palette */
	if (win->variant.palette_16bpp) {
		/* Set RGB 5:6:5 as default */
		win->palette.r.offset = 11;
		win->palette.r.length = 5;
		win->palette.g.offset = 5;
		win->palette.g.length = 6;
		win->palette.b.offset = 0;
		win->palette.b.length = 5;

	} else {
		/* Set 8bpp or 8bpp and 1bit alpha */
		win->palette.r.offset = 16;
		win->palette.r.length = 8;
		win->palette.g.offset = 8;
		win->palette.g.length = 8;
		win->palette.b.offset = 0;
		win->palette.b.length = 8;
	}

	/* setup the initial video mode from the window */
	initmode.xres = windata->xres;
	initmode.yres = windata->yres;
	fb_videomode_to_var(&fbinfo->var, &initmode);

	fbinfo->fix.type	= FB_TYPE_PACKED_PIXELS;
	fbinfo->fix.accel	= FB_ACCEL_NONE;
	fbinfo->var.activate	= FB_ACTIVATE_NOW;
	fbinfo->var.vmode	= FB_VMODE_NONINTERLACED;
	fbinfo->var.bits_per_pixel = windata->default_bpp;
	fbinfo->fbops		= &s3c_fb_ops;
	fbinfo->flags		= FBINFO_FLAG_DEFAULT;
	fbinfo->pseudo_palette  = &win->pseudo_palette;

	/* prepare to actually start the framebuffer */

	ret = s3c_fb_check_var(&fbinfo->var, fbinfo);
	if (ret < 0) {
		dev_err(sfb->dev, "check_var failed on initial video params\n");
		return ret;
	}

	/* create initial colour map */

	ret = fb_alloc_cmap(&fbinfo->cmap, win->variant.palette_sz, 1);
	if (ret == 0)
		fb_set_cmap(&fbinfo->cmap, fbinfo);
	else
		dev_err(sfb->dev, "failed to allocate fb cmap\n");

	s3c_fb_set_par(fbinfo);

	dev_dbg(sfb->dev, "about to register framebuffer\n");

	/* run the check_var and set_par on our configuration. */

	ret = register_framebuffer(fbinfo);
	if (ret < 0) {
		dev_err(sfb->dev, "failed to register framebuffer\n");
		return ret;
	}

	dev_info(sfb->dev, "window %d: fb %s\n", win_no, fbinfo->fix.id);

	return 0;
}

/**
 * s3c_fb_set_rgb_timing() - set video timing for rgb interface.
 * @sfb: The base resources for the hardware.
 *
 * Set horizontal and vertical lcd rgb interface timing.
 */
static void s3c_fb_set_rgb_timing(struct s3c_fb *sfb)
{
	struct fb_videomode *vmode = sfb->pdata->vtiming;
	void __iomem *regs = sfb->regs;
	int clkdiv;
	u32 data;

	if (!vmode->pixclock)
		s3c_fb_missing_pixclock(vmode);

	clkdiv = s3c_fb_calc_pixclk(sfb, vmode->pixclock);

	data = sfb->pdata->vidcon0;
	data &= ~(VIDCON0_CLKVAL_F_MASK | VIDCON0_CLKDIR);

	if (clkdiv > 1)
		data |= VIDCON0_CLKVAL_F(clkdiv-1) | VIDCON0_CLKDIR;
	else
		data &= ~VIDCON0_CLKDIR;	/* 1:1 clock */

	if (sfb->variant.is_2443)
		data |= (1 << 5);
	writel(data, regs + VIDCON0);

	data = VIDTCON0_VBPD(vmode->upper_margin - 1) |
	       VIDTCON0_VFPD(vmode->lower_margin - 1) |
	       VIDTCON0_VSPW(vmode->vsync_len - 1);
	writel(data, regs + sfb->variant.vidtcon);

	data = VIDTCON1_HBPD(vmode->left_margin - 1) |
	       VIDTCON1_HFPD(vmode->right_margin - 1) |
	       VIDTCON1_HSPW(vmode->hsync_len - 1);
	writel(data, regs + sfb->variant.vidtcon + 4);

	data = VIDTCON2_LINEVAL(vmode->yres - 1) |
	       VIDTCON2_HOZVAL(vmode->xres - 1) |
	       VIDTCON2_LINEVAL_E(vmode->yres - 1) |
	       VIDTCON2_HOZVAL_E(vmode->xres - 1);
	writel(data, regs + sfb->variant.vidtcon + 8);
}

/**
 * s3c_fb_clear_win() - clear hardware window registers.
 * @sfb: The base resources for the hardware.
 * @win: The window to process.
 *
 * Reset the specific window registers to a known state.
 */
static void s3c_fb_clear_win(struct s3c_fb *sfb, int win)
{
	void __iomem *regs = sfb->regs;
	u32 reg;

	writel(0, regs + sfb->variant.wincon + (win * 4));
	writel(0, regs + VIDOSD_A(win, sfb->variant));
	writel(0, regs + VIDOSD_B(win, sfb->variant));
	writel(0, regs + VIDOSD_C(win, sfb->variant));

	if (sfb->variant.has_shadowcon) {
		reg = readl(sfb->regs + SHADOWCON);
		reg &= ~(SHADOWCON_WINx_PROTECT(win) |
			SHADOWCON_CHx_ENABLE(win) |
			SHADOWCON_CHx_LOCAL_ENABLE(win));
		writel(reg, sfb->regs + SHADOWCON);
	}
}

static int s3c_fb_probe(struct platform_device *pdev)
{
//	const struct platform_device_id *platid;
	struct s3c_fb_driverdata *fbdrv;
	struct device *dev = &pdev->dev;
//	struct s3c_fb_platdata *pd;
	struct s3c_fb *sfb;
	struct resource *res;
	int win;
	int ret = 0;
	u32 reg;

#if 0
	platid = platform_get_device_id(pdev);
	fbdrv = (struct s3c_fb_driverdata *)platid->driver_data;
#endif 

	fbdrv = &s3c_fb_data_exynos4;
	
	if (fbdrv->variant.nr_windows > S3C_FB_MAX_WIN) {
		dev_err(dev, "too many windows, cannot attach\n");
		return -EINVAL;
	}

#if 0
	pd = dev_get_platdata(&pdev->dev);
	if (!pd) {
		dev_err(dev, "no platform data specified\n");
		return -EINVAL;
	}
#endif 

	sfb = devm_kzalloc(dev, sizeof(struct s3c_fb), GFP_KERNEL);
	if (!sfb) {
		dev_err(dev, "no memory for framebuffers\n");
		return -ENOMEM;
	}

	dev_dbg(dev, "allocate new framebuffer %p\n", sfb);

	sfb->dev = dev;
	sfb->pdata = pd;
	sfb->variant = fbdrv->variant;

	spin_lock_init(&sfb->slock);

	sfb->bus_clk = devm_clk_get(dev, "lcd");
	if (IS_ERR(sfb->bus_clk)) {
		dev_err(dev, "failed to get bus clock\n");
		return PTR_ERR(sfb->bus_clk);
	}

	clk_prepare_enable(sfb->bus_clk);

	if (!sfb->variant.has_clksel) {
		sfb->lcd_clk = devm_clk_get(dev, "sclk_fimd");
		if (IS_ERR(sfb->lcd_clk)) {
			dev_err(dev, "failed to get lcd clock\n");
			ret = PTR_ERR(sfb->lcd_clk);
			goto err_bus_clk;
		}

		clk_prepare_enable(sfb->lcd_clk);
	}

	pm_runtime_enable(sfb->dev);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	sfb->regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(sfb->regs)) {
		ret = PTR_ERR(sfb->regs);
		goto err_lcd_clk;
	}

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(dev, "failed to acquire irq resource\n");
		ret = -ENOENT;
		goto err_lcd_clk;
	}
	sfb->irq_no = res->start;
	ret = devm_request_irq(dev, sfb->irq_no, s3c_fb_irq,
			  0, "s3c_fb", sfb);
	if (ret) {
		dev_err(dev, "irq request failed\n");
		goto err_lcd_clk;
	}

	dev_dbg(dev, "got resources (regs %p), probing windows\n", sfb->regs);

	platform_set_drvdata(pdev, sfb);
	pm_runtime_get_sync(sfb->dev);

	/* setup gpio and output polarity controls */

	pd->setup_gpio();

	writel(pd->vidcon1, sfb->regs + VIDCON1);

	/* set video clock running at under-run */
	if (sfb->variant.has_fixvclk) {
		reg = readl(sfb->regs + VIDCON1);
		reg &= ~VIDCON1_VCLK_MASK;
		reg |= VIDCON1_VCLK_RUN;
		writel(reg, sfb->regs + VIDCON1);
	}

	/* zero all windows before we do anything */

	for (win = 0; win < fbdrv->variant.nr_windows; win++)
		s3c_fb_clear_win(sfb, win);

	/* initialise colour key controls */
	for (win = 0; win < (fbdrv->variant.nr_windows - 1); win++) {
		void __iomem *regs = sfb->regs + sfb->variant.keycon;

		regs += (win * 8);
		writel(0xffffff, regs + WKEYCON0);
		writel(0xffffff, regs + WKEYCON1);
	}

	s3c_fb_set_rgb_timing(sfb);

	/* we have the register setup, start allocating framebuffers */

	for (win = 0; win < fbdrv->variant.nr_windows; win++) {
		if (!pd->win[win])
			continue;

		ret = s3c_fb_probe_win(sfb, win, fbdrv->win[win],
				       &sfb->windows[win]);
		if (ret < 0) {
			dev_err(dev, "failed to create window %d\n", win);
			for (; win >= 0; win--)
				s3c_fb_release_win(sfb, sfb->windows[win]);
			goto err_pm_runtime;
		}
	}

	platform_set_drvdata(pdev, sfb);
	pm_runtime_put_sync(sfb->dev);

	return 0;

err_pm_runtime:
	pm_runtime_put_sync(sfb->dev);

err_lcd_clk:
	pm_runtime_disable(sfb->dev);

	if (!sfb->variant.has_clksel)
		clk_disable_unprepare(sfb->lcd_clk);

err_bus_clk:
	clk_disable_unprepare(sfb->bus_clk);

	return ret;
}

/**
 * s3c_fb_remove() - Cleanup on module finalisation
 * @pdev: The platform device we are bound to.
 *
 * Shutdown and then release all the resources that the driver allocated
 * on initialisation.
 */
static int s3c_fb_remove(struct platform_device *pdev)
{
	struct s3c_fb *sfb = platform_get_drvdata(pdev);
	int win;

	pm_runtime_get_sync(sfb->dev);

	for (win = 0; win < S3C_FB_MAX_WIN; win++)
		if (sfb->windows[win])
			s3c_fb_release_win(sfb, sfb->windows[win]);

	if (!sfb->variant.has_clksel)
		clk_disable_unprepare(sfb->lcd_clk);

	clk_disable_unprepare(sfb->bus_clk);

	pm_runtime_put_sync(sfb->dev);
	pm_runtime_disable(sfb->dev);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int s3c_fb_suspend(struct device *dev)
{
	struct s3c_fb *sfb = dev_get_drvdata(dev);
	struct s3c_fb_win *win;
	int win_no;

	pm_runtime_get_sync(sfb->dev);

	for (win_no = S3C_FB_MAX_WIN - 1; win_no >= 0; win_no--) {
		win = sfb->windows[win_no];
		if (!win)
			continue;

		/* use the blank function to push into power-down */
		s3c_fb_blank(FB_BLANK_POWERDOWN, win->fbinfo);
	}

	if (!sfb->variant.has_clksel)
		clk_disable_unprepare(sfb->lcd_clk);

	clk_disable_unprepare(sfb->bus_clk);

	pm_runtime_put_sync(sfb->dev);

	return 0;
}

static int s3c_fb_resume(struct device *dev)
{
	struct s3c_fb *sfb = dev_get_drvdata(dev);
	struct s3c_fb_platdata *pd = sfb->pdata;
	struct s3c_fb_win *win;
	int win_no;
	u32 reg;

	pm_runtime_get_sync(sfb->dev);

	clk_prepare_enable(sfb->bus_clk);

	if (!sfb->variant.has_clksel)
		clk_prepare_enable(sfb->lcd_clk);

	/* setup gpio and output polarity controls */
	pd->setup_gpio();
	writel(pd->vidcon1, sfb->regs + VIDCON1);

	/* set video clock running at under-run */
	if (sfb->variant.has_fixvclk) {
		reg = readl(sfb->regs + VIDCON1);
		reg &= ~VIDCON1_VCLK_MASK;
		reg |= VIDCON1_VCLK_RUN;
		writel(reg, sfb->regs + VIDCON1);
	}

	/* zero all windows before we do anything */
	for (win_no = 0; win_no < sfb->variant.nr_windows; win_no++)
		s3c_fb_clear_win(sfb, win_no);

	for (win_no = 0; win_no < sfb->variant.nr_windows - 1; win_no++) {
		void __iomem *regs = sfb->regs + sfb->variant.keycon;
		win = sfb->windows[win_no];
		if (!win)
			continue;

		shadow_protect_win(win, 1);
		regs += (win_no * 8);
		writel(0xffffff, regs + WKEYCON0);
		writel(0xffffff, regs + WKEYCON1);
		shadow_protect_win(win, 0);
	}

	s3c_fb_set_rgb_timing(sfb);

	/* restore framebuffers */
	for (win_no = 0; win_no < S3C_FB_MAX_WIN; win_no++) {
		win = sfb->windows[win_no];
		if (!win)
			continue;

		dev_dbg(dev, "resuming window %d\n", win_no);
		s3c_fb_set_par(win->fbinfo);
	}

	pm_runtime_put_sync(sfb->dev);

	return 0;
}
#endif

#ifdef CONFIG_PM_RUNTIME
static int s3c_fb_runtime_suspend(struct device *dev)
{
	struct s3c_fb *sfb = dev_get_drvdata(dev);

	if (!sfb->variant.has_clksel)
		clk_disable_unprepare(sfb->lcd_clk);

	clk_disable_unprepare(sfb->bus_clk);

	return 0;
}

static int s3c_fb_runtime_resume(struct device *dev)
{
	struct s3c_fb *sfb = dev_get_drvdata(dev);
	struct s3c_fb_platdata *pd = sfb->pdata;

	clk_prepare_enable(sfb->bus_clk);

	if (!sfb->variant.has_clksel)
		clk_prepare_enable(sfb->lcd_clk);

	/* setup gpio and output polarity controls */
	pd->setup_gpio();
	writel(pd->vidcon1, sfb->regs + VIDCON1);

	return 0;
}
#endif
static struct platform_device_id s3c_fb_driver_ids[] = {
	{
		.name		= "exynos4-fb",
		.driver_data	= (unsigned long)&s3c_fb_data_exynos4,
	}, 
	{},
};
MODULE_DEVICE_TABLE(platform, s3c_fb_driver_ids);

static const struct dev_pm_ops s3cfb_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(s3c_fb_suspend, s3c_fb_resume)
	SET_RUNTIME_PM_OPS(s3c_fb_runtime_suspend, s3c_fb_runtime_resume,
			   NULL)
};

static struct of_device_id s3c_device_id_ptr[]= {
	{.compatible = "exynos4412,lcd",},
	{},
};

static struct platform_driver s3c_fb_driver = {
	.probe		= s3c_fb_probe,
	.remove		= s3c_fb_remove,
	.id_table	= s3c_fb_driver_ids,
	.driver		= {
		.name	= "s3c-fb",
		.owner	= THIS_MODULE,
		.pm	= &s3cfb_pm_ops,
		.of_match_table = of_match_ptr(s3c_device_id_ptr),
	},
};

module_platform_driver(s3c_fb_driver);

MODULE_AUTHOR("Harxon");
MODULE_DESCRIPTION("Exynos4412 lcd Framebuffer driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:s3c-fb");


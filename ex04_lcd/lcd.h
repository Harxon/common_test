#ifndef __LCD_H__
#define __LCD_H__
/* This driver will export a number of framebuffer interfaces depending
 * on the configuration passed in via the platform data. Each fb instance
 * maps to a hardware window. Currently there is no support for runtime
 * setting of the alpha-blending functions that each window has, so only
 * window 0 is actually useful.
 *
 *此驱动程序将依据平台数据的配置信息提供一组帧缓冲区接口。每个 fb 实例
 *和硬件窗口一一对应。目前尚不支持运行时设置 每个窗口的alpha 混合的功能，
 *因此，实际上只有窗口 0 是有用的。
 *
 * Window 0 is treated specially, it is used for the basis of the LCD
 * output timings and as the control for the output power-down state.
 *
 *窗口 0 特殊对待，它用于液晶显示器的基础
 *计时信息输出以及输出电源关闭状态的控制。
*/

/* note, the previous use of <mach/regs-fb.h> to get platform specific data
 * has been replaced by using the platform device name to pick the correct
 * configuration data for the system.
 *
 *请注意，以前使用的 <mach/regs-fb.h> 以获取平台特定数据的方式
 *已更换为通过使用平台设备名称来匹配正确的系统的配置信息
*/

#ifdef CONFIG_FB_S3C_DEBUG_REGWRITE
#undef writel
#define writel(v, r) do { \
	pr_debug("%s: %08x => %p\n", __func__, (unsigned int)v, r); \
	__raw_writel(v, r); \
} while (0)
#endif /* FB_S3C_DEBUG_REGWRITE */

/* irq_flags bits */
#define S3C_FB_VSYNC_IRQ_EN	0

#define VSYNC_TIMEOUT_MSEC 50


#define VALID_BPP(x) (1 << ((x) - 1))

#define OSD_BASE(win, variant) ((variant).osd + ((win) * (variant).osd_stride))
#define VIDOSD_A(win, variant) (OSD_BASE(win, variant) + 0x00)
#define VIDOSD_B(win, variant) (OSD_BASE(win, variant) + 0x04)
#define VIDOSD_C(win, variant) (OSD_BASE(win, variant) + 0x08)
#define VIDOSD_D(win, variant) (OSD_BASE(win, variant) + 0x0C)

/**
 * struct s3c_fb_variant - fb variant information
 * @is_2443: Set if S3C2443/S3C2416 style hardware.
 * @nr_windows: The number of windows.
 * @vidtcon: The base for the VIDTCONx registers
 * @wincon: The base for the WINxCON registers.
 * @winmap: The base for the WINxMAP registers.
 * @keycon: The abse for the WxKEYCON registers.
 * @buf_start: Offset of buffer start registers.
 * @buf_size: Offset of buffer size registers.
 * @buf_end: Offset of buffer end registers.
 * @osd: The base for the OSD registers.
 * @palette: Address of palette memory, or 0 if none.
 * @has_prtcon: Set if has PRTCON register.
 * @has_shadowcon: Set if has SHADOWCON register.
 * @has_blendcon: Set if has BLENDCON register.
 * @has_clksel: Set if VIDCON0 register has CLKSEL bit.
 * @has_fixvclk: Set if VIDCON1 register has FIXVCLK bits.
 */
struct s3c_fb_variant {
	unsigned int	is_2443:1;
	unsigned short	nr_windows;
	unsigned int	vidtcon;
	unsigned short	wincon;
	unsigned short	winmap;
	unsigned short	keycon;
	unsigned short	buf_start;
	unsigned short	buf_end;
	unsigned short	buf_size;
	unsigned short	osd;
	unsigned short	osd_stride;
	unsigned short	palette[S3C_FB_MAX_WIN];

	unsigned int	has_prtcon:1;
	unsigned int	has_shadowcon:1;
	unsigned int	has_blendcon:1;
	unsigned int	has_clksel:1;
	unsigned int	has_fixvclk:1;
};

/**
 * struct s3c_fb_win_variant
 * @has_osd_c: Set if has OSD C register.
 * @has_osd_d: Set if has OSD D register.
 * @has_osd_alpha: Set if can change alpha transparency for a window.
 * @palette_sz: Size of palette in entries.
 * @palette_16bpp: Set if palette is 16bits wide.
 * @osd_size_off: If != 0, supports setting up OSD for a window; the appropriate
 *                register is located at the given offset from OSD_BASE.
 * @valid_bpp: 1 bit per BPP setting to show valid bits-per-pixel.
 *
 * valid_bpp bit x is set if (x+1)BPP is supported.
 */
struct s3c_fb_win_variant {
	unsigned int	has_osd_c:1;
	unsigned int	has_osd_d:1;
	unsigned int	has_osd_alpha:1;
	unsigned int	palette_16bpp:1;
	unsigned short	osd_size_off;
	unsigned short	palette_sz;
	u32		valid_bpp;
};

/**
 * struct s3c_fb_driverdata - per-device type driver data for init time.
 * @variant: The variant information for this driver.
 * @win: The window information for each window.
 */
struct s3c_fb_driverdata {
	struct s3c_fb_variant	variant;
	struct s3c_fb_win_variant *win[S3C_FB_MAX_WIN];
};

/**
 * struct s3c_fb_palette - palette information
 * @r: Red bitfield.
 * @g: Green bitfield.
 * @b: Blue bitfield.
 * @a: Alpha bitfield.
 */
struct s3c_fb_palette {
	struct fb_bitfield	r;
	struct fb_bitfield	g;
	struct fb_bitfield	b;
	struct fb_bitfield	a;
};

/**
 * struct s3c_fb_win - per window private data for each framebuffer.
 * @windata: The platform data supplied for the window configuration.
 * @parent: The hardware that this window is part of.
 * @fbinfo: Pointer pack to the framebuffer info for this window.
 * @varint: The variant information for this window.
 * @palette_buffer: Buffer/cache to hold palette entries.
 * @pseudo_palette: For use in TRUECOLOUR modes for entries 0..15/
 * @index: The window number of this window.
 * @palette: The bitfields for changing r/g/b into a hardware palette entry.
 */
struct s3c_fb_win {
	struct s3c_fb_pd_win	*windata;
	struct s3c_fb		*parent;
	struct fb_info		*fbinfo;
	struct s3c_fb_palette	 palette;
	struct s3c_fb_win_variant variant;

	u32			*palette_buffer;
	u32			 pseudo_palette[16];
	unsigned int		 index;
};

/**
 * struct s3c_fb_vsync - vsync information
 * @wait:	a queue for processes waiting for vsync
 * @count:	vsync interrupt count
 */
struct s3c_fb_vsync {
	wait_queue_head_t	wait;
	unsigned int		count;
};

/**
 * struct s3c_fb - overall hardware state of the hardware
 * @slock: The spinlock protection for this data structure.
 * @dev: The device that we bound to, for printing, etc.
 * @bus_clk: The clk (hclk) feeding our interface and possibly pixclk.
 * @lcd_clk: The clk (sclk) feeding pixclk.
 * @regs: The mapped hardware registers.
 * @variant: Variant information for this hardware.
 * @enabled: A bitmask of enabled hardware windows.
 * @output_on: Flag if the physical output is enabled.
 * @pdata: The platform configuration data passed with the device.
 * @windows: The hardware windows that have been claimed.
 * @irq_no: IRQ line number
 * @irq_flags: irq flags
 * @vsync_info: VSYNC-related information (count, queues...)
 */
struct s3c_fb {
	spinlock_t		slock;
	struct device		*dev;
	struct clk		*bus_clk;
	struct clk		*lcd_clk;
	void __iomem		*regs;
	struct s3c_fb_variant	 variant;

	unsigned char		 enabled;
	bool			 output_on;

	struct s3c_fb_platdata	*pdata;
	struct s3c_fb_win	*windows[S3C_FB_MAX_WIN];

	int			 irq_no;
	unsigned long		 irq_flags;
	struct s3c_fb_vsync	 vsync_info;
};
#endif

/*
 * drivers/amlogic/media/osd/osd_fb.c
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

/* Linux Headers */
#include <linux/version.h>
#include <linux/compat.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/spinlock.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/console.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_reserved_mem.h>
#include <linux/uaccess.h>
#include <linux/dma-mapping.h>
#include <ion/ion.h>
#include <meson_ion.h>
#include <linux/fs.h>
#include <linux/cma.h>
#include <linux/dma-contiguous.h>
#include <linux/clk.h>
#include <linux/amlogic/cpu_version.h>
/* Amlogic Headers */
#include <linux/amlogic/media/vout/vinfo.h>
#include <linux/amlogic/media/vout/vout_notify.h>
#ifdef CONFIG_INSTABOOT
#include <linux/amlogic/instaboot/instaboot.h>
#endif
#ifdef CONFIG_AMLOGIC_VPU
#include <linux/amlogic/media/vpu/vpu.h>
#endif
/* Local Headers */
#include "osd.h"
#include "osd_fb.h"
#include "osd_hw.h"
#include "osd_log.h"
#include "osd_sync.h"
#include "osd_io.h"
#include "osd_rdma.h"
#include "osd_virtual.h"
static __u32 var_screeninfo[5];
static struct osd_device_data_s osd_meson_dev;

#define MAX_VPU_CLKC_CLK 500000000
#define CUR_VPU_CLKC_CLK 200000000
struct osd_info_s osd_info = {
	.index = 0,
	.osd_reverse = 0,
};

const struct color_bit_define_s default_color_format_array[] = {
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_02_PAL4, 0, 0,
		0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0,
		FB_VISUAL_PSEUDOCOLOR, 2,
	},
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_04_PAL16, 0, 1,
		0, 4, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0,
		FB_VISUAL_PSEUDOCOLOR, 4,
	},
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_08_PAL256, 0, 2,
		0, 8, 0, 0, 8, 0, 0, 8, 0, 0, 0, 0,
		FB_VISUAL_PSEUDOCOLOR, 8,
	},
	/*16 bit color*/
	{
		COLOR_INDEX_16_655, 0, 4,
		10, 6, 0, 5, 5, 0, 0, 5, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_844, 1, 4,
		8, 8, 0, 4, 4, 0, 0, 4, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_6442, 2, 4,
		10, 6, 0, 6, 4, 0, 2, 4, 0, 0, 2, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_4444_R, 3, 4,
		12, 4, 0, 8, 4, 0, 4, 4, 0, 0, 4, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_4642_R, 7, 4,
		12, 4, 0, 6, 6, 0, 2, 4, 0, 0, 2, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_1555_A, 6, 4,
		10, 5, 0, 5, 5, 0, 0, 5, 0, 15, 1, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_4444_A, 5, 4,
		8, 4, 0, 4, 4, 0, 0, 4, 0, 12, 4, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	{
		COLOR_INDEX_16_565, 4, 4,
		11, 5, 0, 5, 6, 0, 0, 5, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 16
	},
	/*24 bit color*/
	INVALID_BPP_ITEM,
	INVALID_BPP_ITEM,
	{
		COLOR_INDEX_24_6666_A, 4, 7,
		12, 6, 0, 6, 6, 0, 0, 6, 0, 18, 6, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_6666_R, 3, 7,
		18, 6, 0, 12, 6, 0, 6, 6, 0, 0, 6, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_8565, 2, 7,
		11, 5, 0, 5, 6, 0, 0, 5, 0, 16, 8, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_5658, 1, 7,
		19, 5, 0, 13, 6, 0, 8, 5, 0, 0, 8, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_888_B, 5, 7,
		0, 8, 0, 8, 8, 0, 16, 8, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	{
		COLOR_INDEX_24_RGB, 0, 7,
		16, 8, 0, 8, 8, 0, 0, 8, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 24
	},
	/*32 bit color RGBX */
	{
		COLOR_INDEX_32_BGRX, 3, 5,
		8, 8, 0, 16, 8, 0, 24, 8, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_XBGR, 2, 5,
		0, 8, 0, 8, 8, 0, 16, 8, 0, 24, 0, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_RGBX, 0, 5,
		24, 8, 0, 16, 8, 0, 8, 8, 0, 0, 0, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_XRGB, 1, 5,
		16, 8, 0, 8, 8, 0, 0, 8, 0, 24, 0, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	/*32 bit color RGBA */
	{
		COLOR_INDEX_32_BGRA, 3, 5,
		8, 8, 0, 16, 8, 0, 24, 8, 0, 0, 8, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_ABGR, 2, 5,
		0, 8, 0, 8, 8, 0, 16, 8, 0, 24, 8, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_RGBA, 0, 5,
		24, 8, 0, 16, 8, 0, 8, 8, 0, 0, 8, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	{
		COLOR_INDEX_32_ARGB, 1, 5,
		16, 8, 0, 8, 8, 0, 0, 8, 0, 24, 8, 0,
		FB_VISUAL_TRUECOLOR, 32
	},
	/*YUV color*/
	{COLOR_INDEX_YUV_422, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16},
};

static struct fb_var_screeninfo fb_def_var[] = {
	{
		.xres            = 1920,
		.yres            = 1080,
		.xres_virtual    = 1920,
		.yres_virtual    = 2160,
		.xoffset         = 0,
		.yoffset         = 0,
		.bits_per_pixel = 32,
		.grayscale       = 0,
		.red             = {0, 0, 0},
		.green           = {0, 0, 0},
		.blue            = {0, 0, 0},
		.transp          = {0, 0, 0},
		.nonstd          = 0,
		.activate        = FB_ACTIVATE_NOW,
		.height          = -1,
		.width           = -1,
		.accel_flags     = 0,
		.pixclock        = 0,
		.left_margin     = 0,
		.right_margin    = 0,
		.upper_margin    = 0,
		.lower_margin    = 0,
		.hsync_len       = 0,
		.vsync_len       = 0,
		.sync            = 0,
		.vmode           = FB_VMODE_NONINTERLACED,
		.rotate          = 0,
	},
	{
		.xres			 = 32,
		.yres			 = 32,
		.xres_virtual	 = 32,
		.yres_virtual	 = 32,
		.xoffset		 = 0,
		.yoffset		 = 0,
		.bits_per_pixel = 32,
		.grayscale		 = 0,
		.red			 = {0, 0, 0},
		.green			 = {0, 0, 0},
		.blue			 = {0, 0, 0},
		.transp          = {0, 0, 0},
		.nonstd          = 0,
		.activate		 = FB_ACTIVATE_NOW,
		.height          = -1,
		.width           = -1,
		.accel_flags     = 0,
		.pixclock		 = 0,
		.left_margin	 = 0,
		.right_margin	 = 0,
		.upper_margin	 = 0,
		.lower_margin	 = 0,
		.hsync_len		 = 0,
		.vsync_len		 = 0,
		.sync			 = 0,
		.vmode			 = FB_VMODE_NONINTERLACED,
		.rotate          = 0,
	},
	{
		.xres			 = 32,
		.yres			 = 32,
		.xres_virtual	 = 32,
		.yres_virtual	 = 32,
		.xoffset		 = 0,
		.yoffset		 = 0,
		.bits_per_pixel = 32,
		.grayscale		 = 0,
		.red			 = {0, 0, 0},
		.green			 = {0, 0, 0},
		.blue			 = {0, 0, 0},
		.transp          = {0, 0, 0},
		.nonstd          = 0,
		.activate		 = FB_ACTIVATE_NOW,
		.height          = -1,
		.width           = -1,
		.accel_flags     = 0,
		.pixclock		 = 0,
		.left_margin	 = 0,
		.right_margin	 = 0,
		.upper_margin	 = 0,
		.lower_margin	 = 0,
		.hsync_len		 = 0,
		.vsync_len		 = 0,
		.sync			 = 0,
		.vmode			 = FB_VMODE_NONINTERLACED,
		.rotate          = 0,
	},
	{
		.xres			 = 32,
		.yres			 = 32,
		.xres_virtual	 = 32,
		.yres_virtual	 = 32,
		.xoffset		 = 0,
		.yoffset		 = 0,
		.bits_per_pixel = 32,
		.grayscale		 = 0,
		.red			 = {0, 0, 0},
		.green			 = {0, 0, 0},
		.blue			 = {0, 0, 0},
		.transp          = {0, 0, 0},
		.nonstd          = 0,
		.activate		 = FB_ACTIVATE_NOW,
		.height          = -1,
		.width           = -1,
		.accel_flags     = 0,
		.pixclock		 = 0,
		.left_margin	 = 0,
		.right_margin	 = 0,
		.upper_margin	 = 0,
		.lower_margin	 = 0,
		.hsync_len		 = 0,
		.vsync_len		 = 0,
		.sync			 = 0,
		.vmode			 = FB_VMODE_NONINTERLACED,
		.rotate          = 0,
	}
};

static struct fb_fix_screeninfo fb_def_fix = {
	.id         = "OSD FB",
	.xpanstep   = 1,
	.ypanstep   = 1,
	.type       = FB_TYPE_PACKED_PIXELS,
	.visual     = FB_VISUAL_TRUECOLOR,
	.accel      = FB_ACCEL_NONE,
};

#ifdef CONFIG_AMLOGIC_LEGACY_EARLY_SUSPEND
#include <linux/amlogic/pm.h>
static struct early_suspend early_suspend;
static int early_suspend_flag;
#endif
#ifdef CONFIG_SCREEN_ON_EARLY
static int early_resume_flag;
#endif
static bool b_alloc_mem;
static bool b_reserved_mem;
static bool fb_map_flag;
static struct reserved_mem fb_rmem = {.base = 0, .size = 0};

static struct delayed_work osd_dwork;
static int osd_shutdown_flag;

unsigned int osd_log_level;
unsigned int osd_log_module = 1;

int int_viu_vsync = -ENXIO;
int int_viu2_vsync = -ENXIO;
int int_rdma = INT_RDMA;
struct osd_fb_dev_s *gp_fbdev_list[OSD_COUNT] = {};
static u32 fb_memsize[HW_OSD_COUNT + 1];
static struct page *osd_page[HW_OSD_COUNT + 1];
static phys_addr_t fb_rmem_paddr[OSD_COUNT];
static void __iomem *fb_rmem_vaddr[OSD_COUNT];
static size_t fb_rmem_size[OSD_COUNT];
static phys_addr_t fb_rmem_afbc_paddr[OSD_COUNT][OSD_MAX_BUF_NUM];
static void __iomem *fb_rmem_afbc_vaddr[OSD_COUNT][OSD_MAX_BUF_NUM];
static size_t fb_rmem_afbc_size[OSD_COUNT][OSD_MAX_BUF_NUM];

struct ion_client *fb_ion_client;

struct ion_handle *fb_ion_handle[OSD_COUNT][OSD_MAX_BUF_NUM];
#if 0
= {
	{NULL, NULL, NULL}, {NULL, NULL, NULL}
};
#endif

static int osd_cursor(struct fb_info *fbi, struct fb_cursor *var);

static int osd_set_fb_var(int index, const struct vinfo_s *vinfo)
{
	if ((vinfo->width < 0) || (vinfo->height < 0)) {
		pr_err("invalid vinfo\n");
		return 1;
	}

	fb_def_var[index].xres = vinfo->width;
	fb_def_var[index].yres = vinfo->height;
	fb_def_var[index].xres_virtual = vinfo->width;
	fb_def_var[index].yres_virtual = vinfo->height * 2;
	fb_def_var[index].bits_per_pixel = 32;

	return 0;
}

static void osd_set_fb_parameters(int index, const struct vinfo_s *vinfo)
{
	osd_set_free_scale_enable_hw(index, 0);
	osd_set_free_scale_mode_hw(index, 1);
	osd_set_free_scale_axis_hw(index, 0, 0, vinfo->width, vinfo->height);
	osd_set_window_axis_hw(index, 0, 0, vinfo->width, vinfo->height);
	osd_enable_hw(index, 1);
}

phys_addr_t get_fb_rmem_paddr(int index)
{
	if (index < 0 || index > 1)
		return 0;
	return fb_rmem_paddr[index];
}

void __iomem *get_fb_rmem_vaddr(int index)
{
	if (index < 0 || index > 1)
		return 0;
	return fb_rmem_vaddr[index];
}

size_t get_fb_rmem_size(int index)
{
	if (index < 0 || index > 1)
		return 0;
	return fb_rmem_size[index];
}


static void osddev_setup(struct osd_fb_dev_s *fbdev)
{
	if (fbdev->fb_mem_paddr) {
		mutex_lock(&fbdev->lock);
		osd_setup_hw(fbdev->fb_info->node,
			     &fbdev->osd_ctl,
			     fbdev->fb_info->var.xoffset,
			     fbdev->fb_info->var.yoffset,
			     fbdev->fb_info->var.xres,
			     fbdev->fb_info->var.yres,
			     fbdev->fb_info->var.xres_virtual,
			     fbdev->fb_info->var.yres_virtual,
			     fbdev->osd_ctl.disp_start_x,
			     fbdev->osd_ctl.disp_start_y,
			     fbdev->osd_ctl.disp_end_x,
			     fbdev->osd_ctl.disp_end_y,
			     fbdev->fb_mem_paddr, /*phys_addr_t -> u32*/
			     fbdev->fb_mem_afbc_paddr, /* afbc phys_addr_t* */
			     fbdev->color);
		mutex_unlock(&fbdev->lock);
	}
}

static void osddev_update_disp_axis(struct osd_fb_dev_s *fbdev,
					int mode_change)
{
	osd_update_disp_axis_hw(fbdev->fb_info->node,
				fbdev->osd_ctl.disp_start_x,
				fbdev->osd_ctl.disp_end_x,
				fbdev->osd_ctl.disp_start_y,
				fbdev->osd_ctl.disp_end_y,
				fbdev->fb_info->var.xoffset,
				fbdev->fb_info->var.yoffset,
				mode_change);
}

static int osddev_setcolreg(unsigned int regno, u16 red, u16 green, u16 blue,
			    u16 transp, struct osd_fb_dev_s *fbdev)
{
	struct fb_info *info = fbdev->fb_info;

	if ((fbdev->color->color_index == COLOR_INDEX_02_PAL4) ||
	    (fbdev->color->color_index == COLOR_INDEX_04_PAL16) ||
	    (fbdev->color->color_index == COLOR_INDEX_08_PAL256)) {
		mutex_lock(&fbdev->lock);
		osd_setpal_hw(fbdev->fb_info->node, regno, red, green,
				blue, transp);
		mutex_unlock(&fbdev->lock);
	}
	if (info->fix.visual == FB_VISUAL_TRUECOLOR) {
		u32 v, r, g, b, a;

		if (regno >= 16)
			return 1;
		r = red    >> (16 - info->var.red.length);
		g = green  >> (16 - info->var.green.length);
		b = blue   >> (16 - info->var.blue.length);
		a = transp >> (16 - info->var.transp.length);
		v = (r << info->var.red.offset)   |
		    (g << info->var.green.offset) |
		    (b << info->var.blue.offset)  |
		    (a << info->var.transp.offset);
		((u32 *)(info->pseudo_palette))[regno] = v;
	}
	return 0;
}

const struct color_bit_define_s *
_find_color_format(struct fb_var_screeninfo *var)
{
	u32 upper_margin, lower_margin, i, level;
	const struct color_bit_define_s *found = NULL;
	const struct color_bit_define_s *color = NULL;

	level = (var->bits_per_pixel - 1) / 8;
	switch (level) {
	case 0:
		upper_margin = COLOR_INDEX_08_PAL256;
		lower_margin = COLOR_INDEX_02_PAL4;
		break;
	case 1:
		upper_margin = COLOR_INDEX_16_565;
		lower_margin = COLOR_INDEX_16_655;
		break;
	case 2:
		upper_margin = COLOR_INDEX_24_RGB;
		lower_margin = COLOR_INDEX_24_6666_A;
		break;
	case 3:
		if ((var->nonstd != 0)
		    && (var->transp.length == 0)) {
			/* RGBX Mode */
			upper_margin = COLOR_INDEX_32_XRGB;
			lower_margin = COLOR_INDEX_32_BGRX;
		} else {
			upper_margin = COLOR_INDEX_32_ARGB;
			lower_margin = COLOR_INDEX_32_BGRA;
		}
		break;
	case 4:
		upper_margin = COLOR_INDEX_YUV_422;
		lower_margin = COLOR_INDEX_YUV_422;
		break;
	default:
		osd_log_err("unsupported color format.");
		return NULL;
	}
	/*
	 * if not provide color component length
	 * then we find the first depth match.
	 */

	if ((var->nonstd != 0) && (level == 3)
	    && (var->transp.length == 0)) {
		/* RGBX Mode */
		for (i = upper_margin; i >= lower_margin; i--) {
			color = &default_color_format_array[i];
			if ((color->red_length == var->red.length) &&
			    (color->green_length == var->green.length) &&
			    (color->blue_length == var->blue.length) &&
			    (color->transp_offset == var->transp.offset) &&
			    (color->green_offset == var->green.offset) &&
			    (color->blue_offset == var->blue.offset) &&
			    (color->red_offset == var->red.offset)) {
				found = color;
				break;
			}
			color--;
		}
	} else if ((var->red.length == 0) || (var->green.length == 0)
	    || (var->blue.length == 0) ||
	    var->bits_per_pixel != (var->red.length + var->green.length +
		    var->blue.length + var->transp.length)) {
		osd_log_dbg(MODULE_BASE, "not provide color length, use default color\n");
		found = &default_color_format_array[upper_margin];
	} else {
		for (i = upper_margin; i >= lower_margin; i--) {
			color = &default_color_format_array[i];
			if ((color->red_length == var->red.length) &&
			    (color->green_length == var->green.length) &&
			    (color->blue_length == var->blue.length) &&
			    (color->transp_length == var->transp.length) &&
			    (color->transp_offset == var->transp.offset) &&
			    (color->green_offset == var->green.offset) &&
			    (color->blue_offset == var->blue.offset) &&
			    (color->red_offset == var->red.offset)) {
				found = color;
				break;
			}
			color--;
		}
	}
	return found;
}

static void __init _fbdev_set_default(struct osd_fb_dev_s *fbdev, int index)
{
	/* setup default value */
	fbdev->fb_info->var = fb_def_var[index];
	fbdev->fb_info->fix = fb_def_fix;
	fbdev->color = _find_color_format(&fbdev->fb_info->var);
}

static int osd_check_fbsize(struct fb_var_screeninfo *var, struct fb_info *info)
{
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)info->par;

	if (osd_meson_dev.afbc_type && osd_get_afbc(fbdev->fb_index)) {
		/* fb_afbc_len = fbdev->fb_afbc_len[0] * ((info->node == 0) ?
		 *		OSD_MAX_BUF_NUM : 1);
		 */
		if (var->xres_virtual * var->yres_virtual *
				var->bits_per_pixel / 8 >
				fbdev->fb_afbc_len[0] * OSD_MAX_BUF_NUM) {
			osd_log_err("afbc no enough memory for %d*%d*%d\n",
					var->xres,
					var->yres,
					var->bits_per_pixel);
			return  -ENOMEM;
		}
	} else {
		if (var->xres_virtual * var->yres_virtual *
				var->bits_per_pixel / 8 > fbdev->fb_len) {
			osd_log_err("no enough memory for %d*%d*%d\n",
					var->xres,
					var->yres,
					var->bits_per_pixel);
			return  -ENOMEM;
		}
	}
	return 0;
}
static int osd_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	struct fb_fix_screeninfo *fix;
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)info->par;
	const struct color_bit_define_s *color_format_pt;

	fix = &info->fix;
	color_format_pt = _find_color_format(var);
	if (color_format_pt == NULL || color_format_pt->color_index == 0)
		return -EFAULT;

	osd_log_dbg(MODULE_BASE, "select color format :index %d, bpp %d\n",
		    color_format_pt->color_index,
		    color_format_pt->bpp);
	fbdev->color = color_format_pt;
	var->red.offset = color_format_pt->red_offset;
	var->red.length = color_format_pt->red_length;
	var->red.msb_right = color_format_pt->red_msb_right;
	var->green.offset  = color_format_pt->green_offset;
	var->green.length  = color_format_pt->green_length;
	var->green.msb_right = color_format_pt->green_msb_right;
	var->blue.offset   = color_format_pt->blue_offset;
	var->blue.length   = color_format_pt->blue_length;
	var->blue.msb_right = color_format_pt->blue_msb_right;
	var->transp.offset = color_format_pt->transp_offset;
	var->transp.length = color_format_pt->transp_length;
	var->transp.msb_right = color_format_pt->transp_msb_right;
	var->bits_per_pixel = color_format_pt->bpp;
	osd_log_dbg(MODULE_BASE, "rgba(L/O):%d/%d-%d/%d-%d/%d-%d/%d\n",
		    var->red.length, var->red.offset,
		    var->green.length, var->green.offset,
		    var->blue.length, var->blue.offset,
		    var->transp.length, var->transp.offset);
	fix->visual = color_format_pt->color_type;
	/* adjust memory length. */
	fix->line_length =
		CANVAS_ALIGNED(var->xres_virtual * var->bits_per_pixel / 8);
	osd_log_dbg(MODULE_BASE, "xvirtual=%d, bpp:%d, line_length=%d\n",
		var->xres_virtual, var->bits_per_pixel, fix->line_length);

	if (var->xres_virtual < var->xres)
		var->xres_virtual = var->xres;
	if (var->yres_virtual < var->yres)
		var->yres_virtual = var->yres;
	var->left_margin = var->right_margin = 0;
	var->upper_margin = var->lower_margin = 0;
	if (var->xres + var->xoffset > var->xres_virtual)
		var->xoffset = var->xres_virtual - var->xres;
	if (var->yres + var->yoffset > var->yres_virtual)
		var->yoffset = var->yres_virtual - var->yres;
	return 0;
}

static int osd_set_par(struct fb_info *info)
{
	const struct vinfo_s *vinfo = NULL;
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)info->par;
	struct osd_ctl_s *osd_ctrl = &fbdev->osd_ctl;
	u32 virt_end_x, virt_end_y;
	u32 output_index;

	output_index = get_output_device_id(fbdev->fb_index);

	if (fbdev->fb_index < osd_hw.osd_meson_dev.viu1_osd_count) {
		vinfo = get_current_vinfo();
		if (!vinfo) {
			osd_log_err("current vinfo NULL\n");
			return -1;
		}
	} else {
#ifdef CONFIG_AMLOGIC_VOUT2_SERVE
		vinfo = get_current_vinfo2();
#endif
		if (!vinfo) {
			osd_log_err("current vinfo NULL\n");
			return -1;
		}
	}
	virt_end_x = osd_ctrl->disp_start_x + info->var.xres;
	virt_end_y = osd_ctrl->disp_start_y + info->var.yres;
	if (virt_end_x > vinfo->width)
		osd_ctrl->disp_end_x = vinfo->width - 1;
	else
		osd_ctrl->disp_end_x = virt_end_x - 1;
	if (virt_end_y  > vinfo->height)
		osd_ctrl->disp_end_y = vinfo->height - 1;
	else
		osd_ctrl->disp_end_y = virt_end_y - 1;
	osddev_setup((struct osd_fb_dev_s *)info->par);

	return 0;
}

static int
osd_setcolreg(unsigned int regno, unsigned int red, unsigned int green,
		unsigned int blue, unsigned int transp, struct fb_info *info)
{
	return osddev_setcolreg(regno, red, green, blue,
				transp, (struct osd_fb_dev_s *)info->par);
}

static int
osd_setcmap(struct fb_cmap *cmap, struct fb_info *info)
{
	int count, index, r;
	u16 *red, *green, *blue, *transp;
	u16 trans = 0xffff;

	red     = cmap->red;
	green   = cmap->green;
	blue    = cmap->blue;
	transp  = cmap->transp;
	index   = cmap->start;

	for (count = 0; count < cmap->len; count++) {
		if (transp)
			trans = *transp++;
		r = osddev_setcolreg(index++, *red++, *green++, *blue++, trans,
				     (struct osd_fb_dev_s *)info->par);
		if (r != 0)
			return r;
	}
	return 0;
}

static int osd_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
{
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)info->par;
	void __user *argp = (void __user *)arg;
	u32 src_colorkey;/* 16 bit or 24 bit */
	u32 srckey_enable;
	u32 gbl_alpha;
	u32 osd_order;
	u32 blank;
	u32 capbility;
	s32 osd_axis[4] = {0};
	s32 osd_dst_axis[4] = {0};
	u32 block_windows[8] = {0};
	u32 block_mode;
	u32 hwc_enable;
	int ret = 0;
	s32 vsync_timestamp;
	s64 vsync_timestamp_64;
	u32 flush_rate;
	int out_fen_fd;
	int xoffset, yoffset;
	struct fb_sync_request_s sync_request;
	struct fb_dmabuf_export dmaexp;
	struct fb_cursor cursor;
	struct do_hwc_cmd_s do_hwc_cmd;

	pr_info("+++++++ %s(%d) cmd: 0x%08x, FBIOGET_DMABUF: 0x%08x\n", __FUNCTION__, __LINE__, cmd, FBIOGET_DMABUF);

	switch (cmd) {
	case  FBIOPUT_OSD_SRCKEY_ENABLE:
		ret = copy_from_user(&srckey_enable, argp, sizeof(u32));
		break;
	case  FBIOPUT_OSD_SRCCOLORKEY:
		ret = copy_from_user(&src_colorkey, argp, sizeof(u32));
		break;
	case FBIOPUT_OSD_SET_GBL_ALPHA:
		ret = copy_from_user(&gbl_alpha, argp, sizeof(u32));
		break;
	case FBIOPUT_OSD_SCALE_AXIS:
		ret = copy_from_user(&osd_axis, argp, 4 * sizeof(s32));
		break;
	case FBIOPUT_OSD_SYNC_ADD:
	case FBIOPUT_OSD_SYNC_RENDER_ADD:
		ret = copy_from_user(&sync_request, argp,
				sizeof(struct fb_sync_request_s));
		break;
	// Only wait for vsync when not HW decoding.
	case FBIO_WAITFORVSYNC:
		if (get_vpu_mem_pd_vmod(VPU_VIU_VD1)) {
			if (info->node < osd_meson_dev.viu1_osd_count)
				vsync_timestamp = (s32)osd_wait_vsync_event();
			else
				vsync_timestamp = (s32)osd_wait_vsync_event_viu2();
		} else {
			vsync_timestamp = 0;
		}
		ret = copy_to_user(argp, &vsync_timestamp, sizeof(s32));
		break;
	case FBIO_WAITFORVSYNC_64:
		if (get_vpu_mem_pd_vmod(VPU_VIU_VD1)) {
			if (info->node < osd_meson_dev.viu1_osd_count)
				vsync_timestamp_64 = osd_wait_vsync_event();
			else
				vsync_timestamp_64 = osd_wait_vsync_event_viu2();
		} else {
			vsync_timestamp_64 = 0;
		}
		ret = copy_to_user(argp, &vsync_timestamp_64, sizeof(s64));
		break;
	case FBIOGET_OSD_SCALE_AXIS:
	case FBIOPUT_OSD_ORDER:
	case FBIOGET_OSD_ORDER:
	case FBIOGET_OSD_GET_GBL_ALPHA:
	case FBIOPUT_OSD_2X_SCALE:
	case FBIOPUT_OSD_ENABLE_3D_MODE:
	case FBIOPUT_OSD_FREE_SCALE_ENABLE:
	case FBIOPUT_OSD_FREE_SCALE_MODE:
	case FBIOPUT_OSD_FREE_SCALE_WIDTH:
	case FBIOPUT_OSD_FREE_SCALE_HEIGHT:
	case FBIOGET_OSD_BLOCK_WINDOWS:
	case FBIOGET_OSD_BLOCK_MODE:
	case FBIOGET_OSD_FREE_SCALE_AXIS:
	case FBIOGET_OSD_WINDOW_AXIS:
	case FBIOPUT_OSD_REVERSE:
	case FBIOPUT_OSD_ROTATE_ON:
	case FBIOPUT_OSD_ROTATE_ANGLE:
	case FBIOGET_OSD_CAPBILITY:
		break;
	case FBIOGET_DMABUF:
	case FBIOGET_OSD_DMABUF:
		ret = copy_from_user(&dmaexp, argp,
				sizeof(struct fb_dmabuf_export));
		break;
	case FBIOPUT_OSD_BLOCK_MODE:
		ret = copy_from_user(&block_mode, argp, sizeof(u32));
		break;
	case FBIOPUT_OSD_BLOCK_WINDOWS:
		ret = copy_from_user(&block_windows, argp, 8 * sizeof(u32));
		break;
	case FBIOPUT_OSD_FREE_SCALE_AXIS:
		ret = copy_from_user(&osd_axis, argp, 4 * sizeof(s32));
		break;
	case FBIOPUT_OSD_WINDOW_AXIS:
		ret = copy_from_user(&osd_dst_axis, argp, 4 * sizeof(s32));
		break;
	case FBIOPUT_OSD_CURSOR:
		ret = copy_from_user(&cursor, argp, sizeof(cursor));
		break;
	case FBIOPUT_OSD_HWC_ENABLE:
		ret = copy_from_user(&hwc_enable, argp, sizeof(u32));
		break;
	case FBIOPUT_OSD_DO_HWC:
		ret = copy_from_user(&do_hwc_cmd, argp,
				sizeof(struct do_hwc_cmd_s));
		break;
	case FBIOPUT_OSD_BLANK:
		ret = copy_from_user(&blank, argp, sizeof(u32));
		break;
	default:
		osd_log_err("command 0x%x not supported (%s)\n",
				cmd, current->comm);
		return -1;
	}
	mutex_lock(&fbdev->lock);
	switch (cmd) {
	case FBIOPUT_OSD_ORDER:
		osd_set_order_hw(info->node, arg);
		break;
	case FBIOGET_OSD_ORDER:
		osd_get_order_hw(info->node, &osd_order);
		ret = copy_to_user(argp, &osd_order, sizeof(u32));
		break;
	case FBIOPUT_OSD_FREE_SCALE_ENABLE:
		osd_set_free_scale_enable_hw(info->node, arg);
		break;
	case FBIOPUT_OSD_FREE_SCALE_MODE:
		osd_set_free_scale_mode_hw(info->node, arg);
		break;
	case FBIOPUT_OSD_ENABLE_3D_MODE:
		osd_enable_3d_mode_hw(info->node, arg);
		break;
	case FBIOPUT_OSD_2X_SCALE:
		/*
		 * high 16 bits for h_scale_enable;
		 * low 16 bits for v_scale_enable
		 */
		osd_set_2x_scale_hw(info->node, arg & 0xffff0000 ? 1 : 0,
				arg & 0xffff ? 1 : 0);
		break;
	case FBIOGET_OSD_FLUSH_RATE:
		osd_get_flush_rate_hw(info->node, &flush_rate);
		if (copy_to_user(argp, &flush_rate, sizeof(u32)))
			return -EFAULT;
		break;
	case FBIOPUT_OSD_REVERSE:
		if (arg >= REVERSE_MAX)
			arg = REVERSE_FALSE;
		osd_set_reverse_hw(info->node, arg, 1);
		break;
	case FBIOPUT_OSD_ROTATE_ON:
		break;
	case FBIOPUT_OSD_ROTATE_ANGLE:
		break;
	case FBIOPUT_OSD_SRCCOLORKEY:
		switch (fbdev->color->color_index) {
		case COLOR_INDEX_16_655:
		case COLOR_INDEX_16_844:
		case COLOR_INDEX_16_565:
		case COLOR_INDEX_24_888_B:
		case COLOR_INDEX_24_RGB:
		case COLOR_INDEX_YUV_422:
			osd_log_dbg(MODULE_BASE,
				"set osd color key 0x%x\n", src_colorkey);
			fbdev->color_key = src_colorkey;
			osd_set_color_key_hw(info->node,
				fbdev->color->color_index, src_colorkey);
			break;
		default:
			break;
		}
		break;
	case FBIOPUT_OSD_SRCKEY_ENABLE:
		switch (fbdev->color->color_index) {
		case COLOR_INDEX_16_655:
		case COLOR_INDEX_16_844:
		case COLOR_INDEX_16_565:
		case COLOR_INDEX_24_888_B:
		case COLOR_INDEX_24_RGB:
		case COLOR_INDEX_YUV_422:
			osd_log_dbg(MODULE_BASE, "set osd color key %s\n",
					srckey_enable ? "enable" : "disable");
			if (srckey_enable != 0) {
				fbdev->enable_key_flag |= KEYCOLOR_FLAG_TARGET;
				if (!(fbdev->enable_key_flag &
						KEYCOLOR_FLAG_ONHOLD)) {
					osd_srckey_enable_hw(info->node, 1);
					fbdev->enable_key_flag |=
						KEYCOLOR_FLAG_CURRENT;
				}
			} else {
				osd_srckey_enable_hw(info->node, 0);
				fbdev->enable_key_flag &= ~(KEYCOLOR_FLAG_TARGET
						| KEYCOLOR_FLAG_CURRENT);
			}
			break;
		default:
			break;
		}
		break;
	case FBIOPUT_OSD_SET_GBL_ALPHA:
		osd_set_gbl_alpha_hw(info->node, gbl_alpha);
		break;
	case  FBIOGET_OSD_GET_GBL_ALPHA:
		gbl_alpha = osd_get_gbl_alpha_hw(info->node);
		ret = copy_to_user(argp, &gbl_alpha, sizeof(u32));
		break;
	case FBIOGET_OSD_SCALE_AXIS:
		osd_get_scale_axis_hw(info->node, &osd_axis[0],
			&osd_axis[1], &osd_axis[2], &osd_axis[3]);
		ret = copy_to_user(argp, &osd_axis, 4 * sizeof(s32));
		break;
	case FBIOPUT_OSD_SCALE_AXIS:
		osd_set_scale_axis_hw(info->node, osd_axis[0], osd_axis[1],
				osd_axis[2], osd_axis[3]);
		break;
	case FBIOGET_OSD_BLOCK_WINDOWS:
		osd_get_block_windows_hw(info->node, block_windows);
		ret = copy_to_user(argp, &block_windows, 8 * sizeof(u32));
		break;
	case FBIOPUT_OSD_BLOCK_WINDOWS:
		osd_set_block_windows_hw(info->node, block_windows);
		break;
	case FBIOPUT_OSD_BLOCK_MODE:
		osd_set_block_mode_hw(info->node, block_mode);
		break;
	case FBIOGET_OSD_BLOCK_MODE:
		osd_get_block_mode_hw(info->node, &block_mode);
		ret = copy_to_user(argp, &block_mode, sizeof(u32));
		break;
	case FBIOGET_OSD_FREE_SCALE_AXIS:
		osd_get_free_scale_axis_hw(info->node, &osd_axis[0],
			&osd_axis[1], &osd_axis[2], &osd_axis[3]);
		ret = copy_to_user(argp, &osd_axis, 4 * sizeof(s32));
		break;
	case FBIOGET_OSD_WINDOW_AXIS:
		osd_get_window_axis_hw(info->node, &osd_dst_axis[0],
			&osd_dst_axis[1], &osd_dst_axis[2], &osd_dst_axis[3]);
		ret = copy_to_user(argp, &osd_dst_axis, 4 * sizeof(s32));
		break;
	case FBIOPUT_OSD_FREE_SCALE_AXIS:
		osd_set_free_scale_axis_hw(info->node, osd_axis[0],
			osd_axis[1], osd_axis[2], osd_axis[3]);
		break;
	case FBIOPUT_OSD_WINDOW_AXIS:
		osd_set_window_axis_hw(info->node, osd_dst_axis[0],
			osd_dst_axis[1], osd_dst_axis[2], osd_dst_axis[3]);
		break;
	case FBIOPUT_OSD_SYNC_ADD:
		out_fen_fd =
			osd_sync_request(info->node, info->var.yres,
					&sync_request);
		if ((sync_request.sync_req.magic & 0xfffffffe) ==
			(FB_SYNC_REQUEST_MAGIC & 0xfffffffe)) {
			sync_request.sync_req.out_fen_fd = out_fen_fd;
			xoffset = sync_request.sync_req.xoffset;
			yoffset = sync_request.sync_req.yoffset;
			ret = copy_to_user(argp, &sync_request,
				sizeof(struct fb_sync_request_s));
		} else {
			sync_request.sync_req_old.out_fen_fd = out_fen_fd;
			xoffset = sync_request.sync_req_old.xoffset;
			yoffset = sync_request.sync_req_old.yoffset;
			ret = copy_to_user(argp, &sync_request,
				sizeof(struct fb_sync_request_s));
		}
		if (out_fen_fd  < 0) {
			/* fence create fail. */
			ret = -1;
		} else {
			info->var.xoffset = xoffset;
			info->var.yoffset = yoffset;
		}
		break;
	case FBIOPUT_OSD_SYNC_RENDER_ADD:
	{
		ion_phys_addr_t addr;
		size_t len;
		u32 phys_addr;

		if ((sync_request.sync_req_render.magic & 0xfffffffe) ==
			(FB_SYNC_REQUEST_RENDER_MAGIC_V1 & 0xfffffffe)) {
			struct sync_req_render_s *sync_request_render;

			sync_request_render =
				&sync_request.sync_req_render;
			ret = meson_ion_share_fd_to_phys(fb_ion_client,
				sync_request_render->shared_fd,
				&addr, &len);
			if (ret == 0) {
				if (sync_request_render->type ==
					GE2D_COMPOSE_MODE) {
					phys_addr = addr +
					sync_request_render->yoffset
						* info->fix.line_length;
				} else
					phys_addr = addr;
			} else
				phys_addr = 0;

			sync_request_render->out_fen_fd =
				osd_sync_request_render(info->node,
				info->var.yres,
				sync_request_render, phys_addr, len);
			osd_get_screen_info(info->node,
				&info->screen_base, &info->screen_size);
			ret = copy_to_user(argp,
				&sync_request,
				sizeof(struct fb_sync_request_s));
			if (sync_request_render->out_fen_fd  < 0) {
				/* fence create fail. */
				ret = -1;
			} else {
				info->var.xoffset =
					sync_request_render->xoffset;
				info->var.yoffset =
					sync_request_render->yoffset;
			}
		}
		break;
	}
	case FBIOGET_DMABUF:
	case FBIOGET_OSD_DMABUF:
#ifdef CONFIG_ION
		if (osd_get_afbc(info->node)) {
			if (dmaexp.buffer_idx > OSD_MAX_BUF_NUM - 1)
				dmaexp.buffer_idx = OSD_MAX_BUF_NUM - 1;
			dmaexp.fd =
				ion_share_dma_buf_fd(
				fb_ion_client,
				fb_ion_handle[info->node][dmaexp.buffer_idx]);
		} else {
			dmaexp.fd =
					ion_share_dma_buf_fd(
					fb_ion_client,
					fb_ion_handle[info->node][0]);
		}
		dmaexp.flags = O_CLOEXEC;

		ret = copy_to_user(argp, &dmaexp, sizeof(dmaexp))
			? -EFAULT : 0;
		break;
#endif
	case FBIOPUT_OSD_CURSOR:
#ifdef CONFIG_AMLOGIC_MEDIA_FB_OSD2_CURSOR
		osd_cursor(info, &cursor);
		ret = copy_to_user(argp, &cursor, sizeof(cursor));
#else
		ret = EINVAL;
#endif
		break;
	case FBIOPUT_OSD_HWC_ENABLE:
		osd_set_hwc_enable(info->node, hwc_enable);
		ret = 0;
		break;
	case FBIOPUT_OSD_DO_HWC:
		do_hwc_cmd.out_fen_fd =
			osd_sync_do_hwc(info->node, &do_hwc_cmd);
		ret = copy_to_user(argp,
			&do_hwc_cmd,
			sizeof(struct do_hwc_cmd_s));
		if (do_hwc_cmd.out_fen_fd >= 0)
			ret = 0;
		break;
	case FBIOPUT_OSD_BLANK:
		osd_set_enable_hw(info->node, (blank != 0) ? 0 : 1);
		ret = 0;
		break;
	case FBIOGET_OSD_CAPBILITY:
		capbility = osd_get_capbility(info->node);
		ret = copy_to_user(argp, &capbility, sizeof(u32));
	default:
		break;
	}
	mutex_unlock(&fbdev->lock);
	return  ret;
}

#ifdef CONFIG_COMPAT
struct fb_cursor_user32 {
	__u16 set;		/* what to set */
	__u16 enable;		/* cursor on/off */
	__u16 rop;		/* bitop operation */
	compat_caddr_t mask;
	struct fbcurpos hot;	/* cursor hot spot */
	struct fb_image image;	/* Cursor image */
};

static int osd_compat_cursor(struct fb_info *info, unsigned long arg)
{
	unsigned long ret;
	struct fb_cursor_user __user *ucursor;
	struct fb_cursor_user32 __user *ucursor32;
	struct fb_cursor cursor;
	void __user *argp;
	__u32 data;

	ucursor = compat_alloc_user_space(sizeof(*ucursor));
	ucursor32 = compat_ptr(arg);

	if (copy_in_user(&ucursor->set, &ucursor32->set, 3 * sizeof(__u16)))
		return -EFAULT;
	if (get_user(data, &ucursor32->mask) ||
	    put_user(compat_ptr(data), &ucursor->mask))
		return -EFAULT;
	if (copy_in_user(&ucursor->hot, &ucursor32->hot, 2 * sizeof(__u16)))
		return -EFAULT;

	argp = (void __user *)ucursor;
	if (copy_from_user(&cursor, argp, sizeof(cursor)))
		return -EFAULT;

	if (!lock_fb_info(info))
		return -ENODEV;
	if (!info->fbops->fb_cursor)
		return -EINVAL;
	console_lock();
	ret = info->fbops->fb_cursor(info, &cursor);
	console_unlock();
	unlock_fb_info(info);

	if (ret == 0 && copy_to_user(argp, &cursor, sizeof(cursor)))
		return -EFAULT;

	return ret;
}

static int osd_compat_ioctl(struct fb_info *info,
		unsigned int cmd, unsigned long arg)
{
	unsigned long ret;

	arg = (unsigned long)compat_ptr(arg);
	/* handle fbio cursor command for 32-bit app */
	if ((cmd & 0xFFFF) == (FBIOPUT_OSD_CURSOR & 0xFFFF))
		ret = osd_compat_cursor(info, arg);
	else
		ret = osd_ioctl(info, cmd, arg);

	return ret;
}
#endif

static int malloc_osd_memory(struct fb_info *info)
{
	int j = 0;
	int ret = 0;
	u32 fb_index;
	int logo_index = -1;
	struct osd_fb_dev_s *fbdev;
	struct fb_fix_screeninfo *fix = NULL;
	struct fb_var_screeninfo *var = NULL;
	struct platform_device *pdev = NULL;
	phys_addr_t base = 0;
	unsigned long size = 0;
	unsigned long fb_memsize_total  = 0;
#ifdef CONFIG_CMA
	struct cma *cma = NULL;
#endif

#ifdef CONFIG_CMA
	if (fb_rmem.base) {
		base = fb_rmem.base;
		size = fb_rmem.size;
	} else {
		cma = dev_get_cma_area(info->device);
		if (cma) {
			base = cma_get_base(cma);
			size = cma_get_size(cma);
			pr_info("%s, cma:%p\n", __func__, cma);
		}
	}
#else
	base = fb_rmem.base;
	size = fb_rmem.size;
#endif

	osd_log_info("%s, %d, base:%pa, size:%ld\n",
		__func__, __LINE__, &base, size);
	fbdev = (struct osd_fb_dev_s *)info->par;
	pdev = fbdev->dev;
	fb_index = fbdev->fb_index;
	if (osd_meson_dev.osd_count <= fb_index)
		return -1;
	fix = &info->fix;
	var = &info->var;
	if (!fb_ion_client)
		fb_ion_client = meson_ion_client_create(-1, "meson-fb");
	for (j = 0; j <= osd_meson_dev.osd_count; j++)
		fb_memsize_total += fb_memsize[j];
	/* read cma/fb-reserved memory first */
	if ((b_reserved_mem == true) &&
		((fb_memsize_total <= size) &&
		(fb_memsize[fb_index + 1] > 0))) {
		fb_rmem_size[fb_index] = fb_memsize[fb_index + 1];
		if (fb_index == DEV_OSD0)
			fb_rmem_paddr[fb_index] = base + fb_memsize[0];
		else if (fb_index == DEV_OSD1) {
			fb_rmem_paddr[fb_index] =
				base + fb_memsize[0] + fb_memsize[1];
		} else if (fb_index == DEV_OSD2) {
			fb_rmem_paddr[fb_index] = base + fb_memsize[0]
				+ fb_memsize[1] + fb_memsize[2];
		} else if (fb_index == DEV_OSD3) {
			fb_rmem_paddr[fb_index] = base + fb_memsize[0]
				+ fb_memsize[1] + fb_memsize[2]
				+ fb_memsize[3];
		}
		pr_info("%s, %d, fb_index=%d,fb_rmem_size=%zu\n",
			__func__, __LINE__, fb_index,
			fb_rmem_size[fb_index]);
		if ((fb_rmem_paddr[fb_index] > 0) &&
			(fb_rmem_size[fb_index] > 0)) {
#ifdef CONFIG_CMA
			if (fb_rmem.base) {
				if (fb_map_flag)
					fb_rmem_vaddr[fb_index] =
						phys_to_virt(
						fb_rmem_paddr[fb_index]);
				else
					fb_rmem_vaddr[fb_index] =
						ioremap_wc(
						fb_rmem_paddr[fb_index],
						fb_rmem_size[fb_index]);
				if (!fb_rmem_vaddr[fb_index])
					osd_log_err("fb[%d] ioremap error",
					fb_index);
				pr_info("%s, reserved mem\n", __func__);
			} else {
				osd_page[fb_index+1] =
					dma_alloc_from_contiguous(
					info->device,
					fb_rmem_size[fb_index]
					>> PAGE_SHIFT,
					0);
				if (!osd_page[fb_index+1]) {
					pr_err("allocate buffer failed:%zu\n",
						fb_rmem_size[fb_index]);
					return -ENOMEM;
				}
				fb_rmem_vaddr[fb_index] =
					page_address(osd_page[fb_index+1]);
				if (!fb_rmem_vaddr[fb_index])
					osd_log_err("fb[%d] get page_address error",
					fb_index);
				pr_info("%s, cma mem\n", __func__);
			}
#else
			if (fb_map_flag)
				fb_rmem_vaddr[fb_index] =
					phys_to_virt(fb_rmem_paddr[fb_index]);
			else
				fb_rmem_vaddr[fb_index] =
					ioremap_wc(
					fb_rmem_paddr[fb_index],
					fb_rmem_size[fb_index]);
			if (!fb_rmem_vaddr[fb_index])
				osd_log_err("fb[%d] ioremap error", fb_index);
			pr_info("%s, reserved mem\n", __func__);
#endif
			osd_log_dbg(MODULE_BASE, "fb_index=%d dma_alloc=%zu\n",
				fb_index, fb_rmem_size[fb_index]);
		}
	} else {
#ifdef CONFIG_AMLOGIC_ION
		pr_info("use ion buffer for fb memory, fb_index=%d\n",
			fb_index);
		if (osd_meson_dev.afbc_type && osd_get_afbc(fb_index)) {
			pr_info("OSD%d as afbcd mode,afbc_type=%d\n",
				fb_index, osd_meson_dev.afbc_type);
			for (j = 0; j < OSD_MAX_BUF_NUM; j++) {
				fb_ion_handle[fb_index][j] =
				ion_alloc(fb_ion_client,
					PAGE_ALIGN(
					fb_memsize[fb_index + 1]/
					OSD_MAX_BUF_NUM),
					0,
					(1 << ION_HEAP_TYPE_DMA),
					0);
				if (IS_ERR(fb_ion_handle[fb_index][j])) {
					osd_log_err("%s: size=%x, FAILED.\n",
					__func__, fb_memsize[fb_index + 1]);
					return -ENOMEM;
				}
				ret = ion_phys(fb_ion_client,
					fb_ion_handle[fb_index][j],
					(ion_phys_addr_t *)
					&fb_rmem_afbc_paddr[fb_index][j],
					(size_t *)
					&fb_rmem_afbc_size[fb_index][j]);
				fb_rmem_afbc_vaddr[fb_index][j] =
					ion_map_kernel(fb_ion_client,
						fb_ion_handle[fb_index][j]);
				dev_alert(&pdev->dev,
					"create ion_client %p, handle=%p\n",
					fb_ion_client,
					fb_ion_handle[fb_index][j]);
				dev_alert(&pdev->dev,
					"ion memory(%d): created fb at 0x%p, size %lu MiB\n",
					fb_index,
					(void *)fb_rmem_afbc_paddr
					[fb_index][j],
					(unsigned long)fb_rmem_afbc_size
					[fb_index][j] / SZ_1M);
				fbdev->fb_afbc_len[j] =
					fb_rmem_afbc_size[fb_index][j];
				fbdev->fb_mem_afbc_paddr[j] =
					fb_rmem_afbc_paddr[fb_index][j];
				fbdev->fb_mem_afbc_vaddr[j] =
					fb_rmem_afbc_vaddr[fb_index][j];
				if	(!fbdev->fb_mem_afbc_vaddr[j]) {
					osd_log_err("failed to ioremap afbc frame buffer\n");
					return -1;
				}
				osd_log_info(" %d, phy: 0x%p, vir:0x%p, size=%dK\n\n",
					fb_index,
					(void *)
					fbdev->fb_mem_afbc_paddr[j],
					fbdev->fb_mem_afbc_vaddr[j],
					fbdev->fb_afbc_len[j] >> 10);
			}
			fb_rmem_paddr[fb_index] =
				fb_rmem_afbc_paddr[fb_index][0];
			fb_rmem_vaddr[fb_index] =
				fb_rmem_afbc_vaddr[fb_index][0];
			fb_rmem_size[fb_index] =
				fb_rmem_afbc_size[fb_index][0];
		} else {
			fb_ion_handle[fb_index][0] =
				ion_alloc(fb_ion_client,
					fb_memsize[fb_index + 1],
					0,
					(1 << ION_HEAP_TYPE_DMA),
					0);
			if (IS_ERR(fb_ion_handle[fb_index][0])) {
				osd_log_err("%s: size=%x, FAILED.\n",
					__func__, fb_memsize[fb_index + 1]);
				return -ENOMEM;
			}
			ret = ion_phys(fb_ion_client,
				fb_ion_handle[fb_index][0],
				(ion_phys_addr_t *)
				&fb_rmem_paddr[fb_index],
				(size_t *)&fb_rmem_size[fb_index]);
			fb_rmem_vaddr[fb_index] =
				ion_map_kernel(fb_ion_client,
					fb_ion_handle[fb_index][0]);
			dev_notice(&pdev->dev,
				"create ion_client %p, handle=%p\n",
				fb_ion_client,
				fb_ion_handle[fb_index][0]);
			dev_notice(&pdev->dev,
				"ion memory(%d): created fb at 0x%p, size %ld MiB\n",
				fb_index,
				(void *)fb_rmem_paddr[fb_index],
				(unsigned long)
				fb_rmem_size[fb_index] / SZ_1M);
		}
#endif
	}
	fbdev->fb_len = fb_rmem_size[fb_index];
	fbdev->fb_mem_paddr = fb_rmem_paddr[fb_index];
	fbdev->fb_mem_vaddr = fb_rmem_vaddr[fb_index];
	if (!fbdev->fb_mem_vaddr) {
		osd_log_err("failed to ioremap frame buffer\n");
		return -ENOMEM;
	}
	osd_log_info("Frame buffer memory assigned at");
	osd_log_info(" %d, phy: 0x%p, vir:0x%p, size=%dK\n\n",
		fb_index, (void *)fbdev->fb_mem_paddr,
		fbdev->fb_mem_vaddr, fbdev->fb_len >> 10);
	if (osd_meson_dev.afbc_type && osd_get_afbc(fb_index)) {
		for (j = 0; j < OSD_MAX_BUF_NUM; j++) {
			fbdev->fb_afbc_len[j] =
				fb_rmem_afbc_size[fb_index][j];
				fbdev->fb_mem_afbc_paddr[j] =
					fb_rmem_afbc_paddr[fb_index][j];
			fbdev->fb_mem_afbc_vaddr[j] =
				fb_rmem_afbc_vaddr[fb_index][j];
			if (!fbdev->fb_mem_afbc_vaddr[j]) {
				osd_log_err("failed to ioremap afbc frame buffer\n");
				return -ENOMEM;
			}
			osd_log_info(" %d, phy: 0x%p, vir:0x%p, size=%dK\n\n",
				fb_index,
				(void *)
				fbdev->fb_mem_afbc_paddr[j],
				fbdev->fb_mem_afbc_vaddr[j],
				fbdev->fb_afbc_len[j] >> 10);
		}
	}
	fix->smem_start = fbdev->fb_mem_paddr;
	fix->smem_len = fbdev->fb_len;
	info->screen_base = (char __iomem *)fbdev->fb_mem_vaddr;
	info->screen_size = fix->smem_len;
	osd_hw.screen_base[fb_index] = fbdev->fb_mem_paddr;
	osd_hw.screen_size[fb_index] = fix->smem_len;
	osd_backup_screen_info(fb_index,
		osd_hw.screen_base[fb_index], osd_hw.screen_size[fb_index]);
	logo_index = osd_get_logo_index();
	osd_log_info("logo_index=%x,fb_index=%d\n",
		logo_index, fb_index);
	if (osd_check_fbsize(var, info))
		return -ENOMEM;
	/* clear osd buffer if not logo layer */
	if (((logo_index < 0) || (logo_index != fb_index)) ||
		(osd_meson_dev.cpu_id == __MESON_CPU_MAJOR_ID_AXG) ||
		(osd_meson_dev.cpu_id >= __MESON_CPU_MAJOR_ID_G12A)) {
		osd_log_info("---------------clear fb%d memory %p\n",
			fb_index, fbdev->fb_mem_vaddr);
		if (fbdev->fb_mem_vaddr)
			memset(fbdev->fb_mem_vaddr, 0x0, fbdev->fb_len);
		if (osd_meson_dev.afbc_type && osd_get_afbc(fb_index)) {
			for (j = 0; j < OSD_MAX_BUF_NUM; j++) {
				if (j > 0) {
					osd_log_info(
						"---------------clear fb%d memory %p\n",
						fb_index,
						fbdev->fb_mem_afbc_vaddr[j]);
					memset(fbdev->fb_mem_afbc_vaddr[j],
					       0x0,
					       fbdev->fb_afbc_len[j]);
				}
			}
		} else {
			/* two case in one
			 * 1. the big buffer ion alloc
			 * 2. reserved memory
			 */
			if (fb_rmem_vaddr[fb_index])
				memset(fb_rmem_vaddr[fb_index],
					0x0,
					fb_rmem_size[fb_index]);
		}
		/* setup osd if not logo layer */
		osddev_setup(fbdev);
	}
	return 0;
}

static int osd_open(struct fb_info *info, int arg)
{
	u32 fb_index;
	struct osd_fb_dev_s *fbdev;
	struct fb_fix_screeninfo *fix = NULL;
	int ret = 0;

	fbdev = (struct osd_fb_dev_s *)info->par;
	fbdev->open_count++;
	osd_log_dbg(MODULE_BASE, "osd_open index=%d,open_count=%d\n",
		fbdev->fb_index, fbdev->open_count);
	if (info->screen_base != NULL)
		return 0;

	fb_index = fbdev->fb_index;
	if ((osd_meson_dev.has_viu2)
		&& (fb_index == osd_meson_dev.viu2_index)) {
		int vpu_clkc_rate;
		if (osd_get_logo_index() != LOGO_DEV_VIU2_OSD0) {
			/* select mux0, if select mux1, mux0 must be set */
			clk_prepare_enable(osd_meson_dev.vpu_clkc);
			clk_set_rate(osd_meson_dev.vpu_clkc, CUR_VPU_CLKC_CLK);
			vpu_clkc_rate = clk_get_rate(osd_meson_dev.vpu_clkc);
			osd_log_info("vpu clkc clock is %d MHZ\n",
				vpu_clkc_rate/1000000);
		}
		osd_init_viu2();
	}
	if (osd_meson_dev.osd_count <= fb_index)
		return -1;
	if (b_alloc_mem) {
		/* alloc mem when osd_open */
		if (info->screen_base == NULL) {
			ret = malloc_osd_memory(info);
			if (ret < 0)
				return ret;
		}

	} else {
		/* move alloc mem when osd_mmap */
		fix = &info->fix;

		fb_rmem_size[fb_index] = fb_memsize[fb_index + 1];
		pr_info("%s, %d, fb_index=%d,fb_rmem_size=%zu\n",
			__func__, __LINE__, fb_index, fb_rmem_size[fb_index]);

		fix->smem_start = 0;
		fix->smem_len = fb_rmem_size[fb_index];
		if (!fb_ion_client)
			fb_ion_client = meson_ion_client_create(-1, "meson-fb");
	}
	if (get_logo_loaded()) {
		s32 logo_index;

		logo_index = osd_get_logo_index();
		if (logo_index < 0) {
			osd_log_info("set logo loaded\n");
			set_logo_loaded();
		}
	}
	return 0;
}

static int osd_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	unsigned long mmio_pgoff;
	unsigned long start;
	u32 len;
	int ret = 0;
	static int mem_alloced;

	if (!b_alloc_mem) {
		/* alloc mem when osd_open */
		if (!mem_alloced) {
			ret = malloc_osd_memory(info);
			if (ret < 0)
				return ret;
			mem_alloced = 1;
		}

	}
	start = info->fix.smem_start;
	len = info->fix.smem_len;
	mmio_pgoff = PAGE_ALIGN((start & ~PAGE_MASK) + len) >> PAGE_SHIFT;
	if (vma->vm_pgoff >= mmio_pgoff) {
		if (info->var.accel_flags) {
			return -EINVAL;
		}

		vma->vm_pgoff -= mmio_pgoff;
		start = info->fix.mmio_start;
		len = info->fix.mmio_len;
	}

	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	return vm_iomap_memory(vma, start, len);
}

static int is_new_page(unsigned long addr, unsigned long pos)
{
	static ulong pre_addr;
	u32 offset;
	int ret = 0;

	/* ret == 0 : in same page*/
	if (pos == 0)
		ret = 1;
	else {
		offset = pre_addr & ~PAGE_MASK;
		if ((offset + addr - pre_addr) >= PAGE_SIZE)
			ret = 1;
	}
	pre_addr = addr;
	return ret;
}

static ssize_t osd_read(struct fb_info *info, char __user *buf,
	size_t count, loff_t *ppos)
{
	u32 fb_index;
	struct osd_fb_dev_s *fbdev;
	unsigned long p = *ppos;
	unsigned long total_size;
	static u8 *vaddr;
	ulong phys;
	u32 offset, npages;
	struct page **pages = NULL;
	struct page *pages_array[2] = {};
	pgprot_t pgprot;
	u8 *buffer, *dst;
	u8 __iomem *src;
	int i, c, cnt = 0, err = 0;

	fbdev = (struct osd_fb_dev_s *)info->par;
	fb_index = fbdev->fb_index;
	total_size = osd_hw.screen_size[fb_index];
	if (total_size == 0)
		total_size = info->fix.smem_len;

	if (p >= total_size)
		return 0;

	if (count >= total_size)
		count = total_size;

	if (count + p > total_size)
		count = total_size - p;
	if (count <= PAGE_SIZE) {
		/* small than one page, need not vmalloc */
		npages = PAGE_ALIGN(count) / PAGE_SIZE;
		phys = osd_hw.screen_base[fb_index] + p;
		if (is_new_page(phys, p)) {
			/* new page, need call vmap*/
			offset = phys & ~PAGE_MASK;
			if ((offset + count) > PAGE_SIZE)
				npages++;
			for (i = 0; i < npages; i++) {
				pages_array[i] = phys_to_page(phys);
				phys += PAGE_SIZE;
			}
			/*nocache*/
			pgprot = pgprot_writecombine(PAGE_KERNEL);
			if (vaddr) {
				/*  unmap prevois vaddr */
				vunmap(vaddr);
				vaddr = NULL;
			}
			vaddr = vmap(pages_array, npages, VM_MAP, pgprot);
			if (!vaddr) {
				pr_err("the phy(%lx) vmaped fail, size: %d\n",
					phys, npages << PAGE_SHIFT);
				return -ENOMEM;
			}
			src = (u8 __iomem *) (vaddr);
		} else {
			/* in same page just get vaddr + p*/
			src = (u8 __iomem *) (vaddr + (p & ~PAGE_MASK));
		}
	} else {
		npages = PAGE_ALIGN(count) / PAGE_SIZE;
		phys = osd_hw.screen_base[fb_index] + p;
		offset = phys & ~PAGE_MASK;
		if ((offset + count) > PAGE_SIZE)
			npages++;
		pages = vmalloc(sizeof(struct page *) * npages);
		if (!pages)
			return -ENOMEM;
		for (i = 0; i < npages; i++) {
			pages[i] = phys_to_page(phys);
			phys += PAGE_SIZE;
		}
		/*nocache*/
		pgprot = pgprot_writecombine(PAGE_KERNEL);
		if (vaddr) {
			/*unmap prevois vaddr */
			vunmap(vaddr);
			vaddr = NULL;
		}
		vaddr = vmap(pages, npages, VM_MAP, pgprot);
		if (!vaddr) {
			pr_err("the phy(%lx) vmaped fail, size: %d\n",
				phys, npages << PAGE_SHIFT);
			vfree(pages);
			return -ENOMEM;
		}
		vfree(pages);
		src = (u8 __iomem *) (vaddr);
	}

	buffer = kmalloc((count > PAGE_SIZE) ? PAGE_SIZE : count,
			 GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;
	/* osd_sync(info); */

	while (count) {
		c  = (count > PAGE_SIZE) ? PAGE_SIZE : count;
		dst = buffer;
		fb_memcpy_fromfb(dst, src, c);
		dst += c;
		src += c;

		if (copy_to_user(buf, buffer, c)) {
			err = -EFAULT;
			break;
		}
		*ppos += c;
		buf += c;
		cnt += c;
		count -= c;
	}

	kfree(buffer);

	return (err) ? err : cnt;

}

static ssize_t osd_write(struct fb_info *info, const char __user *buf,
	size_t count, loff_t *ppos)
{
	u32 fb_index;
	struct osd_fb_dev_s *fbdev;
	unsigned long p = *ppos;
	unsigned long total_size;
	static u8 *vaddr;
	ulong phys;
	u32 offset, npages;
	struct page **pages = NULL;
	struct page *pages_array[2] = {};
	pgprot_t pgprot;
	u8 *buffer, *src;
	u8 __iomem *dst;
	int i, c, cnt = 0, err = 0;

	fbdev = (struct osd_fb_dev_s *)info->par;
	fb_index = fbdev->fb_index;
	total_size = osd_hw.screen_size[fb_index];

	if (total_size == 0)
		total_size = info->fix.smem_len;

	if (p > total_size)
		return -EFBIG;

	if (count > total_size) {
		err = -EFBIG;
		count = total_size;
	}

	if (count + p > total_size) {
		if (!err)
			err = -ENOSPC;

		count = total_size - p;
	}
	if (count <= PAGE_SIZE) {
		/* small than one page, need not vmalloc */
		npages = PAGE_ALIGN(count) / PAGE_SIZE;
		phys = osd_hw.screen_base[fb_index] + p;
		if (is_new_page(phys, p)) {
			/* new page, need call vmap*/
			offset = phys & ~PAGE_MASK;
			if ((offset + count) > PAGE_SIZE)
				npages++;
			for (i = 0; i < npages; i++) {
				pages_array[i] = phys_to_page(phys);
				phys += PAGE_SIZE;
			}
			/*nocache*/
			pgprot = pgprot_writecombine(PAGE_KERNEL);
			if (vaddr) {
				/* unmap prevois vaddr */
				vunmap(vaddr);
				vaddr = NULL;
			}
			vaddr = vmap(pages_array, npages, VM_MAP, pgprot);
			if (!vaddr) {
				pr_err("the phy(%lx) vmaped fail, size: %d\n",
					phys, npages << PAGE_SHIFT);
				return -ENOMEM;
			}
			dst = (u8 __iomem *) (vaddr);
		} else {
			/* in same page just get vaddr + p*/
			dst = (u8 __iomem *) (vaddr + (p & ~PAGE_MASK));
		}
	} else {
		npages = PAGE_ALIGN(count) / PAGE_SIZE;
		phys = osd_hw.screen_base[fb_index] + p;
		offset = phys & ~PAGE_MASK;
		if ((offset + count) > PAGE_SIZE)
			npages++;
		pages = vmalloc(sizeof(struct page *) * npages);
		if (!pages)
			return -ENOMEM;
		for (i = 0; i < npages; i++) {
			pages[i] = phys_to_page(phys);
			phys += PAGE_SIZE;
		}
		/*nocache*/
		pgprot = pgprot_writecombine(PAGE_KERNEL);
		if (vaddr) {
			/*  unmap prevois vaddr */
			vunmap(vaddr);
			vaddr = NULL;
		}
		vaddr = vmap(pages, npages, VM_MAP, pgprot);
		if (!vaddr) {
			pr_err("the phy(%lx) vmaped fail, size: %d\n",
				phys, npages << PAGE_SHIFT);
			vfree(pages);
			return -ENOMEM;
		}
		vfree(pages);
		dst = (u8 __iomem *) (vaddr);

	}
	buffer = kmalloc((count > PAGE_SIZE) ? PAGE_SIZE : count,
			 GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;

	/* osd_sync() */

	while (count) {
		c = (count > PAGE_SIZE) ? PAGE_SIZE : count;
		src = buffer;

		if (copy_from_user(src, buf, c)) {
			err = -EFAULT;
			break;
		}

		fb_memcpy_tofb(dst, src, c);
		dst += c;
		*ppos += c;
		buf += c;
		cnt += c;
		count -= c;
	}

	kfree(buffer);

	return (cnt) ? cnt : err;

}

static int osd_release(struct fb_info *info, int arg)
{
	struct osd_fb_dev_s *fbdev;
	int err = 0;

	fbdev = (struct osd_fb_dev_s *)info->par;

	if (!fbdev->open_count) {
		err = -EINVAL;
		osd_log_info("osd already released. index=%d\n",
			fbdev->fb_index);
		goto done;
	} else if (fbdev->open_count == 1) {
	osd_log_info("osd_release now.index=%d,open_count=%d\n",
		fbdev->fb_index, fbdev->open_count);
	}
	fbdev->open_count--;
done:
	return err;
}

int osd_blank(int blank_mode, struct fb_info *info)
{
	osd_enable_hw(info->node, (blank_mode != 0) ? 0 : 1);
	return 0;
}

static int osd_pan_display(struct fb_var_screeninfo *var,
			   struct fb_info *fbi)
{
	osd_pan_display_hw(fbi->node, var->xoffset, var->yoffset);
	osd_log_dbg(MODULE_BASE, "osd_pan_display:=>osd%d xoff=%d, yoff=%d\n",
			fbi->node, var->xoffset, var->yoffset);
	return 0;
}

static int osd_cursor(struct fb_info *fbi, struct fb_cursor *var)
{
	s16 startx = 0, starty = 0;
	struct osd_fb_dev_s *fb_dev = gp_fbdev_list[1];
	u32 output_index;

	if (fb_dev) {
		startx = fb_dev->osd_ctl.disp_start_x;
		starty = fb_dev->osd_ctl.disp_start_y;
	}
	output_index = get_output_device_id(fbi->node);
	if (osd_hw.hwc_enable[output_index])
		osd_cursor_hw_no_scale(fbi->node, (s16)var->hot.x,
			(s16)var->hot.y, (s16)startx, (s16)starty,
			fbi->var.xres, fbi->var.yres);
	else
		osd_cursor_hw(fbi->node, (s16)var->hot.x,
			(s16)var->hot.y, (s16)startx, (s16)starty,
			fbi->var.xres, fbi->var.yres);

	return 0;
}

static int osd_sync(struct fb_info *info)
{
	return 0;
}


/* fb_ops structures */
static struct fb_ops osd_ops = {
	.owner          = THIS_MODULE,
	.fb_check_var   = osd_check_var,
	.fb_set_par     = osd_set_par,
	.fb_setcolreg   = osd_setcolreg,
	.fb_setcmap     = osd_setcmap,
	.fb_fillrect    = cfb_fillrect,
	.fb_copyarea    = cfb_copyarea,
	.fb_imageblit   = cfb_imageblit,
#ifdef CONFIG_FB_SOFT_CURSOR
	.fb_cursor      = soft_cursor,
#endif
	.fb_cursor      = osd_cursor,
	.fb_ioctl       = osd_ioctl,
#ifdef CONFIG_COMPAT
	.fb_compat_ioctl = osd_compat_ioctl,
#endif
	.fb_open        = osd_open,
	.fb_mmap        = osd_mmap,
	.fb_read        = osd_read,
	.fb_write       = osd_write,
	.fb_blank       = osd_blank,
	.fb_pan_display = osd_pan_display,
	.fb_sync        = osd_sync,
	.fb_release		= osd_release,
};

static void set_default_display_axis(struct fb_var_screeninfo *var,
		struct osd_ctl_s *osd_ctrl, const struct vinfo_s *vinfo)
{
	u32  virt_end_x = osd_ctrl->disp_start_x + var->xres;
	u32  virt_end_y = osd_ctrl->disp_start_y + var->yres;

	if (virt_end_x > vinfo->width) {
		/* screen axis */
		osd_ctrl->disp_end_x = vinfo->width - 1;
	} else {
		/* screen axis */
		osd_ctrl->disp_end_x = virt_end_x - 1;
	}
	if (virt_end_y > vinfo->height)
		osd_ctrl->disp_end_y = vinfo->height - 1;
	else {
		/* screen axis */
		osd_ctrl->disp_end_y = virt_end_y - 1;
	}
}

int osd_notify_callback(struct notifier_block *block, unsigned long cmd,
			void *para)
{
	struct vinfo_s *vinfo;
	struct osd_fb_dev_s *fb_dev;
	int  i, blank;
	struct disp_rect_s *disp_rect;

	vinfo = get_current_vinfo();
	if (!vinfo) {
		osd_log_err("current vinfo NULL\n");
		return -1;
	}
	osd_log_info("current vmode=%s, cmd: 0x%lx\n",
		vinfo->name, cmd);
	if ((!strcmp(vinfo->name, "invalid")) ||
		(!strcmp(vinfo->name, "null")))
		return -1;
	osd_hw.vinfo_width[VIU1] = vinfo->width;
	osd_hw.vinfo_height[VIU1] = vinfo->field_height;
	osd_hw.field_out_en[VIU1] = is_interlaced(vinfo);
	switch (cmd) {
	case  VOUT_EVENT_MODE_CHANGE:
		set_osd_logo_freescaler();
#if 0 /*def LINE_INT_WORK_AROUND */
		if (osd_hw.osd_meson_dev.cpu_id == __MESON_CPU_MAJOR_ID_G12B &&
			is_meson_rev_b())
			set_reset_rdma_trigger_line();
#endif
		if ((osd_meson_dev.osd_ver == OSD_NORMAL)
			|| (osd_meson_dev.osd_ver == OSD_SIMPLE)
			|| (osd_hw.hwc_enable[VIU1] == 0)) {
		for (i = 0; i < osd_meson_dev.viu1_osd_count; i++) {
			fb_dev = gp_fbdev_list[i];
			if (fb_dev == NULL)
				continue;
			set_default_display_axis(&fb_dev->fb_info->var,
					&fb_dev->osd_ctl, vinfo);
			console_lock();
			osddev_update_disp_axis(fb_dev, 1);

			osd_set_antiflicker_hw(DEV_OSD1, vinfo,
				gp_fbdev_list[DEV_OSD1]->fb_info->var.yres);
			osd_reg_write(VPP_POSTBLEND_H_SIZE, vinfo->width);
			console_unlock();
		}
		}
		break;
	case VOUT_EVENT_OSD_BLANK:
		blank = *(int *)para;
		for (i = 0; i < osd_meson_dev.viu1_osd_count; i++) {
			fb_dev = gp_fbdev_list[i];
			if (fb_dev == NULL)
				continue;
			console_lock();
			osd_blank(blank, fb_dev->fb_info);
			console_unlock();
		}
		break;
	case VOUT_EVENT_OSD_DISP_AXIS:
		if ((osd_meson_dev.osd_ver == OSD_NORMAL)
			|| (osd_meson_dev.osd_ver == OSD_SIMPLE)
			|| (osd_hw.hwc_enable[VIU1] == 0)) {

		disp_rect = (struct disp_rect_s *)para;
		for (i = 0; i < osd_meson_dev.viu1_osd_count; i++) {
			if (!disp_rect)
				break;

			/* vout serve send only two layer axis */
			if (i >= 2)
				break;

			fb_dev = gp_fbdev_list[i];
			/*
			 * if osd layer preblend,
			 * it's position is controlled by vpp.
			if (fb_dev->preblend_enable)
				break;
			*/
			fb_dev->osd_ctl.disp_start_x = disp_rect->x;
			fb_dev->osd_ctl.disp_start_y = disp_rect->y;
			osd_log_dbg(MODULE_BASE, "set disp axis: x:%d y:%d w:%d h:%d\n",
				    disp_rect->x, disp_rect->y,
				    disp_rect->w, disp_rect->h);
			if (disp_rect->x + disp_rect->w > vinfo->width)
				fb_dev->osd_ctl.disp_end_x = vinfo->width - 1;
			else
				fb_dev->osd_ctl.disp_end_x =
					fb_dev->osd_ctl.disp_start_x +
					disp_rect->w - 1;
			if (disp_rect->y + disp_rect->h > vinfo->height)
				fb_dev->osd_ctl.disp_end_y = vinfo->height - 1;
			else
				fb_dev->osd_ctl.disp_end_y =
					fb_dev->osd_ctl.disp_start_y +
					disp_rect->h - 1;
			disp_rect++;
			osd_log_dbg(MODULE_BASE, "new disp axis: x0:%d y0:%d x1:%d y1:%d\n",
				    fb_dev->osd_ctl.disp_start_x,
				    fb_dev->osd_ctl.disp_start_y,
				    fb_dev->osd_ctl.disp_end_x,
				    fb_dev->osd_ctl.disp_end_y);
			console_lock();
			osddev_update_disp_axis(fb_dev, 0);
			console_unlock();
		}
		}
		break;
	}
	return 0;
}

int osd_notify_callback_viu2(struct notifier_block *block, unsigned long cmd,
			void *para)
{
	struct vinfo_s *vinfo = NULL;
	struct osd_fb_dev_s *fb_dev;
	int  i, blank;
	struct disp_rect_s *disp_rect;

#ifdef CONFIG_AMLOGIC_VOUT2_SERVE
	vinfo = get_current_vinfo2();
#endif

	if (!vinfo) {
		osd_log_err("current vinfo NULL\n");
		return -1;
	}
	osd_log_info("current vmode=%s, cmd: 0x%lx\n",
		vinfo->name, cmd);
	if (!strcmp(vinfo->name, "invalid"))
		return -1;
	osd_hw.vinfo_width[VIU2] = vinfo->width;
	osd_hw.vinfo_height[VIU2] = vinfo->field_height;
	i = osd_meson_dev.viu2_index;
	switch (cmd) {
	case  VOUT_EVENT_MODE_CHANGE:
		set_viu2_format(vinfo->viu_color_fmt);
		fb_dev = gp_fbdev_list[i];
		set_default_display_axis(&fb_dev->fb_info->var,
			&fb_dev->osd_ctl, vinfo);
		console_lock();
		osddev_update_disp_axis(fb_dev, 1);
		osd_set_antiflicker_hw(i, vinfo,
		gp_fbdev_list[i]->fb_info->var.yres);
		osd_reg_write(VPP2_POSTBLEND_H_SIZE, vinfo->width);
		console_unlock();
		break;
	case VOUT_EVENT_OSD_BLANK:
		blank = *(int *)para;
		fb_dev = gp_fbdev_list[i];
		console_lock();
		osd_blank(blank, fb_dev->fb_info);
		console_unlock();
		break;
	case VOUT_EVENT_OSD_DISP_AXIS:
		disp_rect = (struct disp_rect_s *)para;
		if (!disp_rect)
			break;
		fb_dev = gp_fbdev_list[i];
		/*
		 * if osd layer preblend,
		 * it's position is controlled by vpp.
		 * if (fb_dev->preblend_enable)
		 *	break;
		 */
		fb_dev->osd_ctl.disp_start_x = disp_rect->x;
		fb_dev->osd_ctl.disp_start_y = disp_rect->y;
		osd_log_dbg(MODULE_BASE, "set disp axis: x:%d y:%d w:%d h:%d\n",
			disp_rect->x, disp_rect->y,
			disp_rect->w, disp_rect->h);
		if (disp_rect->x + disp_rect->w > vinfo->width)
			fb_dev->osd_ctl.disp_end_x = vinfo->width - 1;
		else
			fb_dev->osd_ctl.disp_end_x =
				fb_dev->osd_ctl.disp_start_x +
				disp_rect->w - 1;
		if (disp_rect->y + disp_rect->h > vinfo->height)
			fb_dev->osd_ctl.disp_end_y = vinfo->height - 1;
		else
			fb_dev->osd_ctl.disp_end_y =
				fb_dev->osd_ctl.disp_start_y +
				disp_rect->h - 1;
		osd_log_dbg(MODULE_BASE, "new disp axis: x0:%d y0:%d x1:%d y1:%d\n",
			fb_dev->osd_ctl.disp_start_x,
			fb_dev->osd_ctl.disp_start_y,
			fb_dev->osd_ctl.disp_end_x,
			fb_dev->osd_ctl.disp_end_y);
		console_lock();
		osddev_update_disp_axis(fb_dev, 0);
		console_unlock();
		break;
	}
	return 0;
}

static struct notifier_block osd_notifier_nb = {
	.notifier_call	= osd_notify_callback,
};

#ifdef CONFIG_AMLOGIC_VOUT2_SERVE
static struct notifier_block osd_notifier_nb2 = {
	.notifier_call	= osd_notify_callback_viu2,
};
#endif

static ssize_t store_enable_3d(struct device *device,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	fbdev->enable_3d = res;
	osd_enable_3d_mode_hw(fb_info->node, fbdev->enable_3d);
	return count;
}

static ssize_t show_enable_3d(struct device *device,
			      struct device_attribute *attr,
			      char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;

	return snprintf(buf, PAGE_SIZE, "3d_enable:[0x%x]\n", fbdev->enable_3d);
}

static ssize_t store_color_key(struct device *device,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 16, &res);
	switch (fbdev->color->color_index) {
	case COLOR_INDEX_16_655:
	case COLOR_INDEX_16_844:
	case COLOR_INDEX_16_565:
	case COLOR_INDEX_24_888_B:
	case COLOR_INDEX_24_RGB:
	case COLOR_INDEX_YUV_422:
		fbdev->color_key = res;
		osd_set_color_key_hw(fb_info->node, fbdev->color->color_index,
				     fbdev->color_key);
		break;
	default:
		break;
	}

	return count;
}

static ssize_t show_color_key(struct device *device,
			      struct device_attribute *attr,
			      char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;

	return snprintf(buf, PAGE_SIZE, "0x%x\n", fbdev->color_key);
}

static ssize_t store_enable_key(struct device *device,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	mutex_lock(&fbdev->lock);
	if (res != 0) {
		fbdev->enable_key_flag |= KEYCOLOR_FLAG_TARGET;
		if (!(fbdev->enable_key_flag & KEYCOLOR_FLAG_ONHOLD)) {
			osd_srckey_enable_hw(fb_info->node, 1);
			fbdev->enable_key_flag |= KEYCOLOR_FLAG_CURRENT;
		}
	} else {
		fbdev->enable_key_flag &=
			~(KEYCOLOR_FLAG_TARGET | KEYCOLOR_FLAG_CURRENT);
		osd_srckey_enable_hw(fb_info->node, 0);
	}
	mutex_unlock(&fbdev->lock);
	return count;
}

static ssize_t show_enable_key(struct device *device,
			       struct device_attribute *attr,
			       char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;

	return snprintf(buf, PAGE_SIZE, (fbdev->enable_key_flag
			& KEYCOLOR_FLAG_TARGET) ? "1\n" : "0\n");
}

static ssize_t store_enable_key_onhold(struct device *device,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	mutex_lock(&fbdev->lock);
	if (res != 0) {
		/* hold all the calls to enable color key */
		fbdev->enable_key_flag |= KEYCOLOR_FLAG_ONHOLD;
		fbdev->enable_key_flag &= ~KEYCOLOR_FLAG_CURRENT;
		osd_srckey_enable_hw(fb_info->node, 0);
	} else {
		fbdev->enable_key_flag &= ~KEYCOLOR_FLAG_ONHOLD;
		/*
		 * if target and current mistach
		 * then recover the pending key settings
		 */
		if (fbdev->enable_key_flag & KEYCOLOR_FLAG_TARGET) {
			if ((fbdev->enable_key_flag & KEYCOLOR_FLAG_CURRENT)
					== 0) {
				fbdev->enable_key_flag |= KEYCOLOR_FLAG_CURRENT;
				osd_srckey_enable_hw(fb_info->node, 1);
			}
		} else {
			if (fbdev->enable_key_flag & KEYCOLOR_FLAG_CURRENT) {
				fbdev->enable_key_flag &=
					~KEYCOLOR_FLAG_CURRENT;
				osd_srckey_enable_hw(fb_info->node, 0);
			}
		}
	}
	mutex_unlock(&fbdev->lock);
	return count;
}

static ssize_t show_enable_key_onhold(struct device *device,
				      struct device_attribute *attr,
				      char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;

	return snprintf(buf, PAGE_SIZE,
			(fbdev->enable_key_flag & KEYCOLOR_FLAG_ONHOLD) ?
			"1\n" : "0\n");
}

static int parse_para(const char *para, int para_num, int *result)
{
	char *token = NULL;
	char *params, *params_base;
	int *out = result;
	int len = 0, count = 0;
	int res = 0;
	int ret = 0;

	if (!para)
		return 0;

	params = kstrdup(para, GFP_KERNEL);
	params_base = params;
	token = params;
	if (!token)
		return 0;
	len = strlen(token);
	do {
		token = strsep(&params, " ");
		while (token && (isspace(*token)
				|| !isgraph(*token)) && len) {
			token++;
			len--;
		}
		if ((len == 0) || (!token))
			break;
		ret = kstrtoint(token, 0, &res);
		if (ret < 0)
			break;
		len = strlen(token);
		*out++ = res;
		count++;
	} while ((token) && (count < para_num) && (len > 0));

	kfree(params_base);
	return count;
}

static ssize_t show_free_scale_axis(struct device *device,
				    struct device_attribute *attr,
				    char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int x, y, w, h;

	osd_get_free_scale_axis_hw(fb_info->node, &x, &y, &w, &h);
	return snprintf(buf, PAGE_SIZE, "%d %d %d %d\n", x, y, w, h);
}

static ssize_t store_free_scale_axis(struct device *device,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int parsed[4];

	if (likely(parse_para(buf, 4, parsed) == 4))
		osd_set_free_scale_axis_hw(fb_info->node,
				parsed[0], parsed[1], parsed[2], parsed[3]);
	else
		osd_log_err("set free scale axis error\n");
	return count;
}

static ssize_t show_scale_axis(struct device *device,
			       struct device_attribute *attr,
			       char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int x0, y0, x1, y1;

	osd_get_scale_axis_hw(fb_info->node, &x0, &y0, &x1, &y1);
	return snprintf(buf, PAGE_SIZE, "%d %d %d %d\n", x0, y0, x1, y1);
}

static ssize_t store_scale_axis(struct device *device,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int parsed[4];

	if (likely(parse_para(buf, 4, parsed) == 4))
		osd_set_scale_axis_hw(fb_info->node,
				parsed[0], parsed[1], parsed[2], parsed[3]);
	else
		osd_log_err("set scale axis error\n");
	return count;
}


static ssize_t show_scale_width(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	unsigned int free_scale_width = 0;

	osd_get_free_scale_width_hw(fb_info->node, &free_scale_width);
	return snprintf(buf, PAGE_SIZE, "free_scale_width:%d\n",
			free_scale_width);
}

static ssize_t show_scale_height(struct device *device,
				 struct device_attribute *attr,
				 char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	unsigned int free_scale_height = 0;

	osd_get_free_scale_height_hw(fb_info->node, &free_scale_height);
	return snprintf(buf, PAGE_SIZE, "free_scale_height:%d\n",
			free_scale_height);
}

static ssize_t store_free_scale(struct device *device,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	unsigned int free_scale_enable = 0;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	free_scale_enable = res;
	osd_set_free_scale_enable_hw(fb_info->node, free_scale_enable);
	return count;
}

static ssize_t show_free_scale(struct device *device,
			       struct device_attribute *attr,
			       char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	unsigned int free_scale_enable = 0;

	osd_get_free_scale_enable_hw(fb_info->node, &free_scale_enable);
	return snprintf(buf, PAGE_SIZE, "free_scale_enable:[0x%x]\n",
			free_scale_enable);
}

static ssize_t store_freescale_mode(struct device *device,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	unsigned int free_scale_mode = 0;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	free_scale_mode = res;
	osd_set_free_scale_mode_hw(fb_info->node, free_scale_mode);
	return count;
}

static ssize_t show_freescale_mode(struct device *device,
				   struct device_attribute *attr,
				   char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	unsigned int free_scale_mode = 0;
	char *help_info = "free scale mode:\n"
			  "    0: VPU free scaler\n"
			  "    1: OSD free scaler\n"
			  "    2: OSD super scaler\n";

	osd_get_free_scale_mode_hw(fb_info->node, &free_scale_mode);
	return snprintf(buf, PAGE_SIZE, "%scurrent free_scale_mode:%d\n",
			help_info, free_scale_mode);
}

static ssize_t store_scale(struct device *device, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	fbdev->scale = res;
	osd_set_2x_scale_hw(fb_info->node, fbdev->scale & 0xffff0000 ? 1 : 0,
			    fbdev->scale & 0xffff ? 1 : 0);
	return count;
}

static ssize_t show_scale(struct device *device, struct device_attribute *attr,
			  char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;

	return snprintf(buf, PAGE_SIZE, "scale:[0x%x]\n", fbdev->scale);
}

static ssize_t show_window_axis(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int x0, y0, x1, y1;

	osd_get_window_axis_hw(fb_info->node, &x0, &y0, &x1, &y1);
	return snprintf(buf, PAGE_SIZE, "%d %d %d %d\n",
			x0, y0, x1, y1);
}

static ssize_t store_window_axis(struct device *device,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	s32 parsed[4];

	if (likely(parse_para(buf, 4, parsed) == 4))
		osd_set_window_axis_hw(fb_info->node,
				parsed[0], parsed[1], parsed[2], parsed[3]);
	else
		osd_log_err("set window axis error\n");
	return count;
}

static ssize_t show_debug(struct device *device, struct device_attribute *attr,
			  char *buf)
{
	char *help = osd_get_debug_hw();

	return snprintf(buf, strlen(help), "%s", help);
}

static ssize_t store_debug(struct device *device, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	int ret = -EINVAL;

	ret = osd_set_debug_hw(buf);
	if (ret == 0)
		ret = count;

	return ret;
}

static ssize_t show_afbcd(struct device *device, struct device_attribute *attr,
			  char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 enable;

	enable = osd_get_afbc(fb_info->node);

	return snprintf(buf, PAGE_SIZE, "%d\n", enable);
}

static ssize_t store_afbcd(struct device *device, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	u32 res = 0;
	int ret = 0;
	struct fb_info *fb_info = dev_get_drvdata(device);

	ret = kstrtoint(buf, 0, &res);
	osd_log_info("afbc: %d\n", res);

	osd_set_afbc(fb_info->node, res);

	return count;
}

static ssize_t osd_clear(struct device *device, struct device_attribute *attr,
		const char *buf, size_t count)
{
	u32 res = 0;
	int ret = 0;
	struct fb_info *fb_info = dev_get_drvdata(device);

	ret = kstrtoint(buf, 0, &res);
	osd_log_info("clear: osd %d\n", fb_info->node);

	if (res)
		osd_set_clear(fb_info->node);

	return count;
}

static ssize_t show_log_level(struct device *device,
			  struct device_attribute *attr,
			  char *buf)
{
	return snprintf(buf, 40, "%d\n", osd_log_level);
}

static ssize_t store_log_level(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	osd_log_info("log_level: %d->%d\n", osd_log_level, res);
	osd_log_level = res;

	return count;
}

static ssize_t show_log_module(struct device *device,
			  struct device_attribute *attr,
			  char *buf)
{
	return snprintf(buf, 40, "0x%x\n", osd_log_module);
}

static ssize_t store_log_module(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	osd_log_info("log_module: 0x%x->0x%x\n", osd_log_module, res);
	osd_log_module = res;

	return count;
}

static ssize_t store_order(struct device *device, struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	fbdev->order = res;
	osd_set_order_hw(fb_info->node, fbdev->order);
	return count;
}

static ssize_t show_order(struct device *device, struct device_attribute *attr,
			  char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct osd_fb_dev_s *fbdev = (struct osd_fb_dev_s *)fb_info->par;

	osd_get_order_hw(fb_info->node, &(fbdev->order));
	return snprintf(buf, PAGE_SIZE, "order:[0x%x]\n", fbdev->order);
}

static ssize_t show_block_windows(struct device *device,
				  struct device_attribute *attr,
				  char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 wins[8];

	osd_get_block_windows_hw(fb_info->node, wins);
	return snprintf(buf, PAGE_SIZE,
			"0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
			wins[0], wins[1], wins[2], wins[3],
			wins[4], wins[5], wins[6], wins[7]);
}

static ssize_t store_block_windows(struct device *device,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int parsed[8];

	if (likely(parse_para(buf, 8, parsed) == 8))
		osd_set_block_windows_hw(fb_info->node, parsed);
	else
		osd_log_err("set block windows error\n");
	return count;
}

static ssize_t show_block_mode(struct device *device,
			       struct device_attribute *attr,
			       char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 mode;

	osd_get_block_mode_hw(fb_info->node, &mode);
	return snprintf(buf, PAGE_SIZE, "0x%x\n", mode);
}

static ssize_t store_block_mode(struct device *device,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 mode;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	mode = res;
	osd_set_block_mode_hw(fb_info->node, mode);
	return count;
}

static ssize_t show_flush_rate(struct device *device,
			       struct device_attribute *attr,
			       char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 flush_rate = 0;

	osd_get_flush_rate_hw(fb_info->node, &flush_rate);
	return snprintf(buf, PAGE_SIZE, "flush_rate:[%d]\n", flush_rate);
}

static ssize_t show_osd_reverse(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	char *str[4] = {"NONE", "ALL", "X_REV", "Y_REV"};
	struct fb_info *fb_info = dev_get_drvdata(device);
	unsigned int osd_reverse = 0;

	osd_get_reverse_hw(fb_info->node, &osd_reverse);
	if (osd_reverse >= REVERSE_MAX)
		osd_reverse = REVERSE_FALSE;
	return snprintf(buf, PAGE_SIZE, "osd_reverse:[%s]\n",
			str[osd_reverse]);
}

static ssize_t store_osd_reverse(struct device *device,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	unsigned int osd_reverse = 0;
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	osd_reverse = res;
	if (osd_reverse >= REVERSE_MAX)
		osd_reverse = REVERSE_FALSE;
	osd_set_reverse_hw(fb_info->node, osd_reverse, 1);
	return count;
}

static ssize_t show_antiflicker(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	unsigned int osd_antiflicker = 0;
	struct fb_info *fb_info = dev_get_drvdata(device);

	osd_get_antiflicker_hw(fb_info->node, &osd_antiflicker);
	return snprintf(buf, PAGE_SIZE, "osd_antifliter:[%s]\n",
			osd_antiflicker ? "ON" : "OFF");
}

static ssize_t store_antiflicker(struct device *device,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct vinfo_s *vinfo = NULL;
	unsigned int osd_antiflicker = 0;
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;
	u32 output_index;

	ret = kstrtoint(buf, 0, &res);
	osd_antiflicker = res;
	output_index = get_output_device_id(fb_info->node);
	if (output_index == VIU1)
		vinfo = get_current_vinfo();
#ifdef CONFIG_AMLOGIC_VOUT2_SERVE
	else if (output_index == VIU2)
		vinfo = get_current_vinfo2();
#endif
	if (!vinfo) {
		osd_log_err("get current vinfo NULL\n");
		return 0;
	}
	if (osd_antiflicker == 2)
		osd_set_antiflicker_hw(fb_info->node,
				vinfo, fb_info->var.yres);
	else
		osd_set_antiflicker_hw(fb_info->node,
				vinfo, fb_info->var.yres);
	return count;
}

static ssize_t show_ver_clone(struct device *device,
			      struct device_attribute *attr,
			      char *buf)
{
	unsigned int osd_clone = 0;
	struct fb_info *fb_info = dev_get_drvdata(device);

	osd_get_clone_hw(fb_info->node, &osd_clone);
	return snprintf(buf, PAGE_SIZE, "osd_clone:[%s]\n",
			osd_clone ? "ON" : "OFF");
}

static ssize_t store_ver_clone(struct device *device,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	unsigned int osd_clone = 0;
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	osd_clone = res;
	osd_set_clone_hw(fb_info->node, osd_clone);
	return count;
}

static ssize_t store_ver_update_pan(struct device *device,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);

	osd_set_update_pan_hw(fb_info->node);
	return count;
}

static ssize_t show_reset_status(struct device *device,
	struct device_attribute *attr, char *buf)
{
	u32 status = osd_get_reset_status();

	return snprintf(buf, PAGE_SIZE, "0x%x\n", status);
}

/* Todo: how to use uboot logo */
static ssize_t free_scale_switch(struct device *device,
	struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 res = 0;
	int ret = 0;
	unsigned int free_scale_enable = 0;

	ret = kstrtoint(buf, 0, &res);
	free_scale_enable = res;

	if (osd_hw.osd_meson_dev.osd_ver == OSD_HIGH_ONE)
		osd_switch_free_scale(
			(fb_info->node == DEV_OSD0) ? DEV_OSD0 : DEV_OSD1,
			1, free_scale_enable, fb_info->node, 1,
			free_scale_enable);
	else
		osd_switch_free_scale(
			(fb_info->node == DEV_OSD0) ? DEV_OSD1 : DEV_OSD0,
			0, 0, fb_info->node, 1,
			free_scale_enable);

	osd_log_info("free_scale_switch to fb%d, mode: 0x%x\n",
		fb_info->node, free_scale_enable);
	return count;
}

static ssize_t show_osd_deband(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	u32 osd_deband_enable;

	osd_get_deband(&osd_deband_enable);
	return snprintf(buf, 40, "%d\n",
		osd_deband_enable);
}

static ssize_t store_osd_deband(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	osd_set_deband(res);

	return count;
}

static ssize_t show_osd_fps(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 osd_fps;

	osd_get_fps(fb_info->node, &osd_fps);
	return snprintf(buf, 40, "%d\n",
		osd_fps);
}

static ssize_t store_osd_fps(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	osd_set_fps(fb_info->node, res);

	return count;
}

static void parse_param(char *buf_orig, char **parm)
{
	char *ps, *token;
	unsigned int n = 0;
	char delim1[3] = " ";
	char delim2[2] = "\n";

	ps = buf_orig;
	strcat(delim1, delim2);
	while (1) {
		token = strsep(&ps, delim1);
		if (token == NULL)
			break;
		if (*token == '\0')
			continue;
		parm[n++] = token;
	}
}



static ssize_t store_osd_reg(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	char *buf_orig, *parm[8] = {NULL};
	long val = 0;
	unsigned int reg_addr, reg_val;

	if (!buf)
		return count;
	buf_orig = kstrdup(buf, GFP_KERNEL);
	parse_param(buf_orig, (char **)&parm);
	if (!strcmp(parm[0], "rv")) {
		if (kstrtoul(parm[1], 16, &val) < 0)
			return -EINVAL;
		reg_addr = val;
		reg_val = osd_reg_read(reg_addr);
		pr_info("reg[0x%04x]=0x%08x\n", reg_addr, reg_val);
	} else if (!strcmp(parm[0], "wv")) {
		if (kstrtoul(parm[1], 16, &val) < 0)
			return -EINVAL;
		reg_addr = val;
		if (kstrtoul(parm[2], 16, &val) < 0)
			return -EINVAL;
		reg_val = val;
		osd_reg_write(reg_addr, reg_val);
	}
	kfree(buf_orig);
	buf_orig = NULL;
	return count;
}

static ssize_t show_osd_display_debug(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 osd_display_debug_enable;

	osd_get_display_debug(fb_info->node, &osd_display_debug_enable);
	return snprintf(buf, 40, "%d\n",
		osd_display_debug_enable);
}

static ssize_t store_osd_display_debug(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	if (ret < 0)
		return -EINVAL;
	osd_set_display_debug(fb_info->node, res);

	return count;
}


static ssize_t show_osd_background_size(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	struct display_flip_info_s disp_info;

	osd_get_background_size(fb_info->node, &disp_info);
	return snprintf(buf, 80, "%d %d %d %d %d %d %d %d\n",
		disp_info.background_w,
		disp_info.background_h,
		disp_info.fullscreen_w,
		disp_info.fullscreen_h,
		disp_info.position_x,
		disp_info.position_y,
		disp_info.position_w,
		disp_info.position_h);
}

static ssize_t store_osd_background_size(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int parsed[8];

	if (likely(parse_para(buf, 8, parsed) == 8))
		osd_set_background_size(
			    fb_info->node,
				(struct display_flip_info_s *)&parsed);
	else
		osd_log_err("set background size error\n");

	return count;
}

static ssize_t show_osd_hdr_mode(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	u32 hdr_used;

	osd_get_hdr_used(&hdr_used);
	return snprintf(buf, 40, "%d\n", hdr_used);
}

static ssize_t store_osd_hdr_mode(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	if (ret < 0)
		return -EINVAL;
	osd_set_hdr_used(res);

	return count;
}

static ssize_t show_osd_afbc_format(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 format[2];

	osd_get_afbc_format(fb_info->node, &format[0], &format[1]);

	return snprintf(buf, PAGE_SIZE, "%d %d\n", format[0], format[1]);
}

static ssize_t store_osd_afbc_format(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int parsed[2];

	if (likely(parse_para(buf, 2, parsed) == 2))
		osd_set_afbc_format(fb_info->node,
				parsed[0], parsed[1]);
	else
		osd_log_err("set afbc_format size error\n");

	return count;
}

static ssize_t show_osd_hwc_enalbe(struct device *device,
				struct device_attribute *attr,
				char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 hwc_enalbe;

	osd_get_hwc_enable(fb_info->node, &hwc_enalbe);
	return snprintf(buf, 40, "%d\n",
		hwc_enalbe);
}

static ssize_t store_osd_hwc_enalbe(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	if (ret < 0)
		return -EINVAL;
	osd_set_hwc_enable(fb_info->node, res);

	return count;
}

static ssize_t store_do_hwc(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	if (ret < 0)
		return -EINVAL;
	if (res)
		osd_do_hwc(fb_info->node);

	return count;
}

static ssize_t show_osd_urgent(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	u32 urgent[2];

	osd_get_urgent_info(&urgent[0], &urgent[1]);

	return snprintf(buf, PAGE_SIZE, "%d %d\n", urgent[0], urgent[1]);
}

static ssize_t store_osd_urgent(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	int parsed[2];

	if (likely(parse_para(buf, 2, parsed) == 2))
		osd_set_urgent_info(parsed[0], parsed[1]);
	else
		osd_log_err("set osd_set_urgent_info size error\n");

	return count;
}

static ssize_t store_osd_single_step_mode(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	if (ret < 0)
		return -EINVAL;
	osd_set_single_step_mode(fb_info->node, res);

	return count;
}

static ssize_t store_osd_single_step(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	if (ret < 0)
		return -EINVAL;
	osd_set_single_step(fb_info->node, res);

	return count;
}

static ssize_t show_osd_rotate(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 rotate;

	osd_get_rotate(fb_info->node, &rotate);

	return snprintf(buf, PAGE_SIZE, "%d\n", rotate);
}

static ssize_t store_osd_rotate(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int res = 0;
	int ret = 0;

	ret = kstrtoint(buf, 0, &res);
	if (ret < 0)
		return -EINVAL;

	osd_set_rotate(fb_info->node, res);
	return count;
}

static ssize_t show_afbc_err_cnt(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	u32 err_cnt;

	osd_get_afbc_err_cnt(&err_cnt);

	return snprintf(buf, PAGE_SIZE, "%d\n", err_cnt);
}

static ssize_t show_osd_dimm(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 osd_dimm[2];

	osd_get_dimm_info(fb_info->node, &osd_dimm[0], &osd_dimm[1]);

	return snprintf(buf, PAGE_SIZE, "%d, 0x%x\n", osd_dimm[0], osd_dimm[1]);
}

static ssize_t store_osd_dimm(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int parsed[2];

	if (likely(parse_para(buf, 2, parsed) == 2))
		osd_set_dimm_info(fb_info->node, parsed[0], parsed[1]);
	else
		osd_log_err("set store_osd_dimm size error\n");
	return count;
}

static ssize_t show_osd_plane_alpha(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	u32 plane_alpha;

	plane_alpha = osd_get_gbl_alpha_hw(fb_info->node);

	return snprintf(buf, PAGE_SIZE, "0x%x\n", plane_alpha);
}

static ssize_t store_osd_plane_alpha(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int plane_alpha;
	int ret = 0;

	ret = kstrtoint(buf, 0, &plane_alpha);
	if (ret < 0)
		return -EINVAL;

	osd_set_gbl_alpha_hw(fb_info->node, plane_alpha);

	return count;
}

static ssize_t show_osd_status(struct device *device,
			       struct device_attribute *attr,
			       char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);

	return snprintf(buf, PAGE_SIZE, "osd[%d] enable: %d\n",
				fb_info->node, osd_hw.enable[fb_info->node]);
}

static ssize_t show_osd_line_n_rdma(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	int  line_n_rdma;

	line_n_rdma = osd_get_line_n_rdma();

	return snprintf(buf, PAGE_SIZE, "0x%x\n", line_n_rdma);
}

static ssize_t store_osd_line_n_rdma(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	int line_n_rdma;
	int ret = 0;

	ret = kstrtoint(buf, 0, &line_n_rdma);
	if (ret < 0)
		return -EINVAL;

	osd_set_line_n_rdma(line_n_rdma);

	return count;
}

static ssize_t show_osd_hold_line(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int hold_line;

	hold_line = osd_get_hold_line(fb_info->node);

	return snprintf(buf, PAGE_SIZE, "0x%x\n", hold_line);
}

static ssize_t store_osd_hold_line(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int hold_line;
	int ret = 0;

	ret = kstrtoint(buf, 0, &hold_line);
	if (ret < 0)
		return -EINVAL;

	osd_set_hold_line(fb_info->node, hold_line);

	return count;
}

static ssize_t show_osd_blend_bypass(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	int  blend_bypass;

	blend_bypass = osd_get_blend_bypass();

	return snprintf(buf, PAGE_SIZE, "0x%x\n", blend_bypass);
}

static ssize_t store_osd_blend_bypass(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct fb_info *fb_info = dev_get_drvdata(device);
	int blend_bypass;
	int ret = 0;

	ret = kstrtoint(buf, 0, &blend_bypass);
	if (ret < 0)
		return -EINVAL;

	osd_set_blend_bypass(fb_info->node, blend_bypass);
	return count;
}

static ssize_t show_rdma_trace_enable(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%x\n", osd_hw.rdma_trace_enable);
}

static ssize_t store_rdma_trace_enable(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	int trace_enable;
	int ret = 0;

	ret = kstrtoint(buf, 0, &trace_enable);
	if (ret < 0)
		return -EINVAL;
	osd_hw.rdma_trace_enable = trace_enable;
	return count;
}

static ssize_t show_rdma_trace_reg(
	struct device *device, struct device_attribute *attr,
	char *buf)
{
	int i;
	char reg_info[16];
	char *trace_info = NULL;

	trace_info = kmalloc(osd_hw.rdma_trace_num * 16 + 1, GFP_KERNEL);
	if (!trace_info)
		return 0;
	for (i = 0; i < osd_hw.rdma_trace_num; i++) {
		sprintf(reg_info, "0x%x", osd_hw.rdma_trace_reg[i]);
		strcat(trace_info, reg_info);
		strcat(trace_info, " ");
	}
	i = snprintf(buf, PAGE_SIZE, "%s\n", trace_info);
	kfree(trace_info);
	trace_info = NULL;
	return i;
}

static ssize_t store_rdma_trace_reg(
	struct device *device, struct device_attribute *attr,
	const char *buf, size_t count)
{
	int parsed[MAX_TRACE_NUM];
	int i = 0, num = 0;

	for (i  = 0; i < MAX_TRACE_NUM; i++)
		osd_hw.rdma_trace_reg[i] = 0;
	num = parse_para(buf, MAX_TRACE_NUM, parsed);
	if (num <= MAX_TRACE_NUM) {
		osd_hw.rdma_trace_num = num;
		for (i  = 0; i < num; i++) {
			osd_hw.rdma_trace_reg[i] = parsed[i];
			pr_info("trace reg:0x%x\n", osd_hw.rdma_trace_reg[i]);
		}
	}
	return count;
}

static inline  int str2lower(char *str)
{
	while (*str != '\0') {
		*str = tolower(*str);
		str++;
	}
	return 0;
}

static struct para_osd_info_s para_osd_info[OSD_END + 2] = {
	/* head */
	{
		"head", OSD_INVALID_INFO,
		OSD_END + 1, 1,
		0, OSD_END + 1
	},
	/* dev */
	{
		"osd0",	DEV_OSD0,
		OSD_FIRST_GROUP_START - 1, OSD_FIRST_GROUP_START + 1,
		OSD_FIRST_GROUP_START, OSD_SECOND_GROUP_START - 1
	},
	{
		"osd1",	DEV_OSD1,
		OSD_FIRST_GROUP_START, OSD_FIRST_GROUP_START + 2,
		OSD_FIRST_GROUP_START, OSD_SECOND_GROUP_START - 1
	},
	{
		"all", DEV_ALL,
		OSD_FIRST_GROUP_START + 1, OSD_FIRST_GROUP_START + 3,
		OSD_FIRST_GROUP_START, OSD_SECOND_GROUP_START - 1
	},
	/* reverse_mode */
	{
		"false", REVERSE_FALSE,
		OSD_SECOND_GROUP_START - 1, OSD_SECOND_GROUP_START + 1,
		OSD_SECOND_GROUP_START, OSD_END
	},
	{
		"true", REVERSE_TRUE,
		OSD_SECOND_GROUP_START, OSD_SECOND_GROUP_START + 2,
		OSD_SECOND_GROUP_START, OSD_END
	},
	{
		"x_rev", REVERSE_X,
		OSD_SECOND_GROUP_START + 1, OSD_SECOND_GROUP_START + 3,
		OSD_SECOND_GROUP_START, OSD_END
	},
	{
		"y_rev", REVERSE_Y,
		OSD_SECOND_GROUP_START + 2, OSD_SECOND_GROUP_START + 4,
		OSD_SECOND_GROUP_START, OSD_END
	},
	{
		"tail", OSD_INVALID_INFO, OSD_END,
		0, 0,
		OSD_END + 1
	},
};

static inline int install_osd_reverse_info(struct osd_info_s *init_osd_info,
		char *para)
{
	u32 i = 0;
	static u32 tail = OSD_END + 1;
	u32 first = para_osd_info[0].next_idx;

	for (i = first; i < tail; i = para_osd_info[i].next_idx) {
		if (strcmp(para_osd_info[i].name, para) == 0) {
			u32 group_start = para_osd_info[i].cur_group_start;
			u32 group_end = para_osd_info[i].cur_group_end;
			u32	prev = para_osd_info[group_start].prev_idx;
			u32  next = para_osd_info[group_end].next_idx;

			switch (para_osd_info[i].cur_group_start) {
			case OSD_FIRST_GROUP_START:
				init_osd_info->index = para_osd_info[i].info;
				break;
			case OSD_SECOND_GROUP_START:
				init_osd_info->osd_reverse =
					para_osd_info[i].info;
				break;
			}
			para_osd_info[prev].next_idx = next;
			para_osd_info[next].prev_idx = prev;
			return 0;
		}
	}
	return 0;
}

static int __init osd_info_setup(char *str)
{
	char	*ptr = str;
	char	sep[2];
	char	*option;
	int count = 2;
	char find = 0;
	struct osd_info_s *init_osd_info;

	if (str == NULL)
		return -EINVAL;

	init_osd_info = &osd_info;
	memset(init_osd_info, 0, sizeof(struct osd_info_s));
	do {
		if (!isalpha(*ptr) && !isdigit(*ptr)) {
			find = 1;
			break;
		}
	} while (*++ptr != '\0');
	if (!find)
		return -EINVAL;
	sep[0] = *ptr;
	sep[1] = '\0';
	while ((count--) && (option = strsep(&str, sep))) {
		str2lower(option);
		install_osd_reverse_info(init_osd_info, option);
	}
	return 0;
}

__setup("osd_reverse=", osd_info_setup);
static struct device_attribute osd_attrs[] = {
	__ATTR(scale, 0664,
			show_scale, store_scale),
	__ATTR(order, 0664,
			show_order, store_order),
	__ATTR(enable_3d, 0644,
			show_enable_3d, store_enable_3d),
	__ATTR(free_scale, 0664,
			show_free_scale, store_free_scale),
	__ATTR(scale_axis, 0644,
			show_scale_axis, store_scale_axis),
	__ATTR(scale_width, 0444,
			show_scale_width, NULL),
	__ATTR(scale_height, 0444,
			show_scale_height, NULL),
	__ATTR(color_key, 0644,
			show_color_key, store_color_key),
	__ATTR(enable_key, 0664,
			show_enable_key, store_enable_key),
	__ATTR(enable_key_onhold, 0664,
			show_enable_key_onhold, store_enable_key_onhold),
	__ATTR(block_windows, 0644,
			show_block_windows, store_block_windows),
	__ATTR(block_mode, 0664,
			show_block_mode, store_block_mode),
	__ATTR(free_scale_axis, 0664,
			show_free_scale_axis, store_free_scale_axis),
	__ATTR(debug, 0644,
			show_debug, store_debug),
	__ATTR(log_level, 0644,
			show_log_level, store_log_level),
	__ATTR(log_module, 0644,
			show_log_module, store_log_module),
	__ATTR(window_axis, 0664,
			show_window_axis, store_window_axis),
	__ATTR(freescale_mode, 0664,
			show_freescale_mode, store_freescale_mode),
	__ATTR(flush_rate, 0444,
			show_flush_rate, NULL),
	__ATTR(osd_reverse, 0644,
			show_osd_reverse, store_osd_reverse),
	__ATTR(osd_antiflicker, 0644,
			show_antiflicker, store_antiflicker),
	__ATTR(ver_clone, 0644,
			show_ver_clone, store_ver_clone),
	__ATTR(ver_update_pan, 0220,
			NULL, store_ver_update_pan),
	__ATTR(osd_afbcd, 0664,
			show_afbcd, store_afbcd),
	__ATTR(osd_clear, 0220,
			NULL, osd_clear),
	__ATTR(reset_status, 0444,
			show_reset_status, NULL),
	__ATTR(free_scale_switch, 0220,
			NULL, free_scale_switch),
	__ATTR(osd_deband, 0644,
			show_osd_deband, store_osd_deband),
	__ATTR(osd_fps, 0644,
			show_osd_fps, store_osd_fps),
	__ATTR(osd_reg, 0220,
			NULL, store_osd_reg),
	__ATTR(osd_display_debug, 0644,
			show_osd_display_debug, store_osd_display_debug),
	__ATTR(osd_background_size, 0644,
			show_osd_background_size, store_osd_background_size),
	__ATTR(osd_hdr_mode, 0644,
			show_osd_hdr_mode, store_osd_hdr_mode),
	__ATTR(osd_afbc_format, 0644,
			show_osd_afbc_format, store_osd_afbc_format),
	__ATTR(osd_hwc_enable, 0644,
			show_osd_hwc_enalbe, store_osd_hwc_enalbe),
	__ATTR(osd_do_hwc, 0220,
			NULL, store_do_hwc),
	__ATTR(osd_urgent, 0644,
			show_osd_urgent, store_osd_urgent),
	__ATTR(osd_single_step_mode, 0220,
			NULL, store_osd_single_step_mode),
	__ATTR(osd_single_step, 0220,
			NULL, store_osd_single_step),
	__ATTR(afbc_err_cnt, 0444,
			show_afbc_err_cnt, NULL),
	__ATTR(osd_dimm, 0644,
			show_osd_dimm, store_osd_dimm),
	__ATTR(osd_plane_alpha, 0644,
			show_osd_plane_alpha, store_osd_plane_alpha),
	__ATTR(osd_status, 0444,
			show_osd_status, NULL),
	__ATTR(osd_line_n_rdma, 0644,
			show_osd_line_n_rdma, store_osd_line_n_rdma),
	__ATTR(osd_hold_line, 0644,
			show_osd_hold_line, store_osd_hold_line),
	__ATTR(osd_blend_bypass, 0644,
			show_osd_blend_bypass, store_osd_blend_bypass),
	__ATTR(trace_enable, 0644,
			show_rdma_trace_enable, store_rdma_trace_enable),
	__ATTR(trace_reg, 0644,
			show_rdma_trace_reg, store_rdma_trace_reg),
};

static struct device_attribute osd_attrs_viu2[] = {
	__ATTR(color_key, 0644,
			show_color_key, store_color_key),
	__ATTR(enable_key, 0664,
			show_enable_key, store_enable_key),
	__ATTR(enable_key_onhold, 0664,
			show_enable_key_onhold, store_enable_key_onhold),
	__ATTR(block_windows, 0644,
			show_block_windows, store_block_windows),
	__ATTR(block_mode, 0664,
			show_block_mode, store_block_mode),
	__ATTR(debug, 0644,
			show_debug, store_debug),
	__ATTR(log_level, 0644,
			show_log_level, store_log_level),
	__ATTR(log_module, 0644,
			show_log_module, store_log_module),
	__ATTR(flush_rate, 0444,
			show_flush_rate, NULL),
	__ATTR(osd_reverse, 0644,
			show_osd_reverse, store_osd_reverse),
	__ATTR(osd_antiflicker, 0644,
			show_antiflicker, store_antiflicker),
	__ATTR(ver_clone, 0644,
			show_ver_clone, store_ver_clone),
	__ATTR(ver_update_pan, 0220,
			NULL, store_ver_update_pan),
	__ATTR(osd_afbcd, 0664,
			show_afbcd, store_afbcd),
	__ATTR(osd_clear, 0220,
			NULL, osd_clear),
	__ATTR(reset_status, 0444,
			show_reset_status, NULL),
	__ATTR(osd_fps, 0644,
			show_osd_fps, store_osd_fps),
	__ATTR(osd_reg, 0220,
			NULL, store_osd_reg),
	__ATTR(osd_display_debug, 0644,
			show_osd_display_debug, store_osd_display_debug),
	__ATTR(osd_background_size, 0644,
			show_osd_background_size, store_osd_background_size),
	__ATTR(osd_afbc_format, 0644,
			show_osd_afbc_format, store_osd_afbc_format),
	__ATTR(osd_rotate, 0644,
			show_osd_rotate, store_osd_rotate),
	__ATTR(osd_status, 0444,
			show_osd_status, NULL),
	__ATTR(osd_hwc_enable, 0644,
			show_osd_hwc_enalbe, store_osd_hwc_enalbe),
	__ATTR(osd_hold_line, 0644,
			show_osd_hold_line, store_osd_hold_line),
	__ATTR(osd_do_hwc, 0220,
			NULL, store_do_hwc),
};

#ifdef CONFIG_PM
static int osd_suspend(struct platform_device *dev, pm_message_t state)
{
#ifdef CONFIG_AMLOGIC_LEGACY_EARLY_SUSPEND
	if (early_suspend_flag)
		return 0;
#endif
	osd_suspend_hw();
	return 0;
}

static int osd_resume(struct platform_device *dev)
{
#ifdef CONFIG_SCREEN_ON_EARLY
	if (early_resume_flag) {
		early_resume_flag = 0;
		return 0;
	}
#endif
#ifdef CONFIG_AMLOGIC_LEGACY_EARLY_SUSPEND
	if (early_suspend_flag)
		return 0;
#endif
	osd_resume_hw();
	return 0;
}
#endif

#ifdef CONFIG_AMLOGIC_LEGACY_EARLY_SUSPEND
static void osd_early_suspend(struct early_suspend *h)
{
	if (early_suspend_flag)
		return;
	osd_suspend_hw();
	early_suspend_flag = 1;
}

static void osd_late_resume(struct early_suspend *h)
{
	if (!early_suspend_flag)
		return;
	early_suspend_flag = 0;
	osd_resume_hw();
}
#endif

#ifdef CONFIG_SCREEN_ON_EARLY
void osd_resume_early(void)
{
#ifdef CONFIG_AMLOGIC_LEGACY_EARLY_SUSPEND
	osd_resume_hw();
	early_suspend_flag = 0;
#endif
	early_resume_flag = 1;
}
EXPORT_SYMBOL(osd_resume_early);
#endif

#ifdef CONFIG_HIBERNATION
static int osd_realdata_save(void)
{
	osd_realdata_save_hw();
	return 0;
}

static void osd_realdata_restore(void)
{
	osd_realdata_restore_hw();
}

static struct instaboot_realdata_ops osd_realdata_ops = {
	.save		= osd_realdata_save,
	.restore	= osd_realdata_restore,
};

static int osd_freeze(struct device *dev)
{
	osd_freeze_hw();
	return 0;
}

static int osd_thaw(struct device *dev)
{
	osd_thaw_hw();
	return 0;
}

static int osd_restore(struct device *dev)
{
	osd_restore_hw();
	return 0;
}
static int osd_pm_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);

	return osd_suspend(pdev, PMSG_SUSPEND);
}
static int osd_pm_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);

	return osd_resume(pdev);
}
#endif

static void mem_free_work(struct work_struct *work)
{
	if (fb_memsize[0] > 0) {
#ifdef CONFIG_CMA
		osd_log_info("%s, free memory: addr:%x\n",
			__func__, fb_memsize[0]);

		dma_release_from_contiguous(&(gp_fbdev_list[0]->dev->dev),
			osd_page[0],
			fb_memsize[0] >> PAGE_SHIFT);
#else
#ifdef CONFIG_ARM64
		long r = -EINVAL;
#elif defined(CONFIG_ARM) && defined(CONFIG_HIGHMEM)
		unsigned long r;
#endif
		unsigned long start_addr;
		unsigned long end_addr;

#ifdef CONFIG_ARM64
		if (fb_rmem.base && fb_map_flag) {
#elif defined(CONFIG_ARM) && defined(CONFIG_HIGHMEM)
		if (fb_rmem.base) {
#endif
			if (fb_rmem.size >= (fb_memsize[0] + fb_memsize[1]
				+ fb_memsize[2])) {
				/* logo memory before fb0/fb1 memory, free it*/
				start_addr = fb_rmem.base;
				end_addr = fb_rmem.base + fb_memsize[0];
			} else {
				/* logo reserved only, free it*/
				start_addr = fb_rmem.base;
				end_addr = fb_rmem.base + fb_rmem.size;
			}
			osd_log_info("%s, free memory: addr:%lx\n",
				__func__, start_addr);
#ifdef CONFIG_ARM64
			r = free_reserved_area(__va(start_addr),
				__va(end_addr), 0, "fb-memory");
#elif defined(CONFIG_ARM) && defined(CONFIG_HIGHMEM)
			for (r = start_addr; r < end_addr; ) {
				free_highmem_page(phys_to_page(r));
				r += PAGE_SIZE;
			}
#endif

		}
#endif
	}
}

static struct osd_device_data_s osd_gxbb = {
	.cpu_id = __MESON_CPU_MAJOR_ID_GXBB,
	.osd_ver = OSD_NORMAL,
	.afbc_type = NO_AFBC,
	.osd_count = 2,
	.has_deband = 0,
	.has_lut = 0,
	.has_rdma = 1,
	.has_dolby_vision = 0,
	.osd_fifo_len = 32,
	.vpp_fifo_len = 0x77f,
	.dummy_data = 0x00808000,
	.has_viu2 = 0,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_gxtvbb = {
	.cpu_id = __MESON_CPU_MAJOR_ID_GXTVBB,
	.osd_ver = OSD_NORMAL,
	.afbc_type = MESON_AFBC,
	.osd_count = 2,
	.has_deband = 0,
	.has_lut = 0,
	.has_rdma = 1,
	.has_dolby_vision = 0,
	.osd_fifo_len = 32,
	.vpp_fifo_len = 0xfff,
	.dummy_data = 0x0,
	.has_viu2 = 0,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_gxl = {
	.cpu_id = __MESON_CPU_MAJOR_ID_GXL,
	.osd_ver = OSD_NORMAL,
	.afbc_type = NO_AFBC,
	.osd_count = 2,
	.has_deband = 0,
	.has_lut = 0,
	.has_rdma = 1,
	.has_dolby_vision = 0,
	.osd_fifo_len = 32,
	.vpp_fifo_len = 0x77f,
	.dummy_data = 0x00808000,
	.has_viu2 = 0,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_gxm = {
	.cpu_id = __MESON_CPU_MAJOR_ID_GXM,
	.osd_ver = OSD_NORMAL,
	.afbc_type = MESON_AFBC,
	.osd_count = 2,
	.has_deband = 0,
	.has_lut = 0,
	.has_rdma = 1,
	.has_dolby_vision = 0,
	.osd_fifo_len = 32,
	.vpp_fifo_len = 0xfff,
	.dummy_data = 0x00202000,/* dummy data is different */
	.has_viu2 = 0,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_txl = {
	.cpu_id = __MESON_CPU_MAJOR_ID_TXL,
	.osd_ver = OSD_NORMAL,
	.afbc_type = NO_AFBC,
	.osd_count = 2,
	.has_deband = 0,
	.has_lut = 0,
	.has_rdma = 1,
	.has_dolby_vision = 0,
	.osd_fifo_len = 64,
	.vpp_fifo_len = 0x77f,
	.dummy_data = 0x00808000,
	.has_viu2 = 0,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_txlx = {
	.cpu_id = __MESON_CPU_MAJOR_ID_TXLX,
	.osd_ver = OSD_NORMAL,
	.afbc_type = NO_AFBC,
	.osd_count = 2,
	.has_deband = 1,
	.has_lut = 1,
	.has_rdma = 1,
	.has_dolby_vision = 1,
	.osd_fifo_len = 64, /* fifo len 64*8 = 512 */
	.vpp_fifo_len = 0x77f,
	.dummy_data = 0x00808000,
	.has_viu2 = 0,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_axg = {
	.cpu_id = __MESON_CPU_MAJOR_ID_AXG,
	.osd_ver = OSD_SIMPLE,
	.afbc_type = NO_AFBC,
	.osd_count = 1,
	.has_deband = 1,
	.has_lut = 1,
	.has_rdma = 0,
	.has_dolby_vision = 0,
	 /* use iomap its self, no rdma, no canvas, no freescale */
	.osd_fifo_len = 64, /* fifo len 64*8 = 512 */
	.vpp_fifo_len = 0x400,
	.dummy_data = 0x00808000,
	.has_viu2 = 0,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_g12a = {
	.cpu_id = __MESON_CPU_MAJOR_ID_G12A,
	.osd_ver = OSD_HIGH_ONE,
	.afbc_type = MALI_AFBC,
	.osd_count = 4,
	.has_deband = 1,
	.has_lut = 1,
	.has_rdma = 1,
	.has_dolby_vision = 1,
	.osd_fifo_len = 64, /* fifo len 64*8 = 512 */
	.vpp_fifo_len = 0xfff,/* 2048 */
	.dummy_data = 0x00808000,
	.has_viu2 = 1,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_g12b = {
	.cpu_id = __MESON_CPU_MAJOR_ID_G12B,
	.osd_ver = OSD_HIGH_ONE,
	.afbc_type = MALI_AFBC,
	.osd_count = 4,
	.has_deband = 1,
	.has_lut = 1,
	.has_rdma = 1,
	.has_dolby_vision = 1,
	.osd_fifo_len = 64, /* fifo len 64*8 = 512 */
	.vpp_fifo_len = 0xfff,/* 2048 */
	.dummy_data = 0x00808000,
	.has_viu2 = 1,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_tl1 = {
	.cpu_id = __MESON_CPU_MAJOR_ID_TL1,
	.osd_ver = OSD_HIGH_ONE,
	.afbc_type = MALI_AFBC,
	.osd_count = 3,
	.has_deband = 1,
	.has_lut = 1,
	.has_rdma = 1,
	.has_dolby_vision = 0,
	.osd_fifo_len = 64, /* fifo len 64*8 = 512 */
	.vpp_fifo_len = 0xfff,/* 2048 */
	.dummy_data = 0x00808000,
	.has_viu2 = 1,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_sm1 = {
	.cpu_id = __MESON_CPU_MAJOR_ID_SM1,
	.osd_ver = OSD_HIGH_ONE,
	.afbc_type = MALI_AFBC,
	.osd_count = 4,
	.has_deband = 1,
	.has_lut = 1,
	.has_rdma = 1,
	.has_dolby_vision = 1,
	.osd_fifo_len = 64, /* fifo len 64*8 = 512 */
	.vpp_fifo_len = 0xfff,/* 2048 */
	.dummy_data = 0x00808000,
	.has_viu2 = 1,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_tm2 = {
	.cpu_id = __MESON_CPU_MAJOR_ID_TM2,
	.osd_ver = OSD_HIGH_ONE,
	.afbc_type = MALI_AFBC,
	.osd_count = 4,
	.has_deband = 1,
	.has_lut = 1,
	.has_rdma = 1,
	.has_dolby_vision = 1,
	.osd_fifo_len = 64, /* fifo len 64*8 = 512 */
	.vpp_fifo_len = 0xfff,/* 2048 */
	.dummy_data = 0x00808000,
	.has_viu2 = 1,
	.osd0_sc_independ = 0,
};

static struct osd_device_data_s osd_a1 = {
	.cpu_id = __MESON_CPU_MAJOR_ID_A1,
	.osd_ver = OSD_NONE,
	.afbc_type = NO_AFBC,
	.osd_count = 1,
	.has_deband = 0,
	.has_lut = 0,
	.has_rdma = 0,
	.has_dolby_vision = 0,
	.osd_fifo_len = 64, /* fifo len 64*8 = 512 */
	.vpp_fifo_len = 0xfff,/* 2048 */
	.dummy_data = 0x00808000,
	.has_viu2 = 0,
	.osd0_sc_independ = 0,
};

static const struct of_device_id meson_fb_dt_match[] = {
	{
		.compatible = "amlogic, meson-gxbb",
		.data = &osd_gxbb,

	},
	{
		.compatible = "amlogic, meson-gxtvbb",
		.data = &osd_gxtvbb,

	},
	{
		.compatible = "amlogic, meson-gxl",
		.data = &osd_gxl,
	},
	{
		.compatible = "amlogic, meson-gxm",
		.data = &osd_gxm,

	},
	{
		.compatible = "amlogic, meson-txl",
		.data = &osd_txl,
	},
	{
		.compatible = "amlogic, meson-txlx",
		.data = &osd_txlx,

	},
	{
		.compatible = "amlogic, meson-axg",
		.data = &osd_axg,

	},
	{
		.compatible = "amlogic, meson-g12a",
		.data = &osd_g12a,
	},
	{
		.compatible = "amlogic, meson-g12b",
		.data = &osd_g12b,
	},
	{
		.compatible = "amlogic, meson-tl1",
		.data = &osd_tl1,
	},
	{
		.compatible = "amlogic, meson-sm1",
		.data = &osd_sm1,
	},
	{
		.compatible = "amlogic, meson-tm2",
		.data = &osd_tm2,
	},
	{
		.compatible = "amlogic, meson-a1",
		.data = &osd_a1,
	},
	{},
};

static int osd_probe(struct platform_device *pdev)
{
	struct fb_info *fbi = NULL;
	const struct vinfo_s *vinfo;
	struct fb_var_screeninfo *var;
	struct fb_fix_screeninfo *fix;
	int  index, bpp;
	struct osd_fb_dev_s *fbdev = NULL;
	const void *prop;
	int prop_idx = 0;
	const char *str;
	#ifdef CONFIG_CMA
	struct cma *cma;
	phys_addr_t base_addr;
	#endif
	int i;
	int ret = 0;

	if (pdev->dev.of_node) {
		const struct of_device_id *match;
		struct osd_device_data_s *osd_meson;
		struct device_node	*of_node = pdev->dev.of_node;

		match = of_match_node(meson_fb_dt_match, of_node);
		if (match) {
			osd_meson = (struct osd_device_data_s *)match->data;
			if (osd_meson)
				memcpy(&osd_meson_dev, osd_meson,
					sizeof(struct osd_device_data_s));
			else {
				pr_err("%s data NOT match\n", __func__);
				return -ENODEV;
			}
		} else {
			pr_err("%s NOT match\n", __func__);
			return -ENODEV;
		}
	}
	if (osd_meson_dev.osd_ver == OSD_NONE) {
		amlfb_virtual_probe(pdev);
		return 0;
	}
	/* get interrupt resource */
	int_viu_vsync = platform_get_irq_byname(pdev, "viu-vsync");
	if (int_viu_vsync  == -ENXIO) {
		osd_log_err("cannot get viu irq resource\n");
		goto failed1;
	} else
		osd_log_info("viu vsync irq: %d\n", int_viu_vsync);
	if (osd_meson_dev.has_viu2) {
		int_viu2_vsync = platform_get_irq_byname(pdev, "viu2-vsync");
		if (int_viu2_vsync  == -ENXIO) {
			osd_log_err("cannot get viu2 irq resource\n");
			goto failed1;
		} else
			osd_log_info("viu2 vsync irq: %d\n", int_viu2_vsync);
	}
	if (osd_meson_dev.has_rdma) {
		int_rdma = platform_get_irq_byname(pdev, "rdma");
		if (int_viu_vsync  == -ENXIO) {
			osd_log_err("cannot get osd rdma irq resource\n");
			goto failed1;
		}
	}
	if (osd_meson_dev.has_viu2) {
		osd_meson_dev.vpu_clkc = devm_clk_get(&pdev->dev, "vpu_clkc");
		if (IS_ERR(osd_meson_dev.vpu_clkc)) {
			osd_log_err("cannot get vpu_clkc\n");
			osd_meson_dev.vpu_clkc = NULL;
			ret = -ENOENT;
			goto failed1;
		}
	}
	osd_meson_dev.viu1_osd_count = osd_meson_dev.osd_count;
	if (osd_meson_dev.has_viu2) {
		/* set viu1 osd count */
		osd_meson_dev.viu1_osd_count--;
		osd_meson_dev.viu2_index = osd_meson_dev.viu1_osd_count;
	}

	ret = osd_io_remap(osd_meson_dev.osd_ver == OSD_SIMPLE);
	if (!ret) {
		osd_log_err("osd_io_remap failed\n");
		goto failed1;
	}
	/* init osd logo */
	ret = logo_work_init();
	if (ret == 0)
		osd_init_hw(1, 1, &osd_meson_dev);
	else
		osd_init_hw(0, 1, &osd_meson_dev);

	/* get buffer size from dt */
	ret = of_property_read_u32_array(pdev->dev.of_node,
			"mem_size", fb_memsize, osd_meson_dev.osd_count + 1);
	if (ret) {
		osd_log_err("not found mem_size from dtd\n");
		goto failed1;
	}
	for (i = 0; i < (HW_OSD_COUNT + 1); i++)
		osd_log_info("mem_size: 0x%x\n", fb_memsize[i]);

	/* init reserved memory */
	ret = of_reserved_mem_device_init(&pdev->dev);
	if (ret != 0)
		osd_log_err("failed to init reserved memory\n");
	else {
		b_reserved_mem = true;
#ifdef CONFIG_CMA
		cma = dev_get_cma_area(&pdev->dev);
		if (cma) {
			base_addr = cma_get_base(cma);
			pr_info("reserved memory base:%pa, size:%lx\n",
				&base_addr, cma_get_size(cma));
			if (fb_memsize[0] > 0) {
				osd_page[0] = dma_alloc_from_contiguous(
					&pdev->dev,
					fb_memsize[0] >> PAGE_SHIFT,
					0);
				if (!osd_page[0]) {
					pr_err("allocate buffer failed:%d\n",
						fb_memsize[0]);
				}
				osd_log_dbg(MODULE_BASE, "logo dma_alloc=%d\n",
					fb_memsize[0]);
			}
		} else
			pr_info("------ NO CMA\n");

	}
#endif

	/* get meson-fb resource from dt */
	prop = of_get_property(pdev->dev.of_node, "scale_mode", NULL);
	if (prop)
		prop_idx = of_read_ulong(prop, 1);
	/* Todo: only osd0 */
	osd_set_free_scale_mode_hw(DEV_OSD0, prop_idx);
	prop_idx = 0;
	prop = of_get_property(pdev->dev.of_node, "4k2k_fb", NULL);
	if (prop)
		prop_idx = of_read_ulong(prop, 1);
	osd_set_4k2k_fb_mode_hw(prop_idx);
	/* get default display mode from dt */
	ret = of_property_read_string(pdev->dev.of_node,
		"display_mode_default", &str);
	prop_idx = 0;
	prop = of_get_property(pdev->dev.of_node, "pxp_mode", NULL);
	if (prop)
		prop_idx = of_read_ulong(prop, 1);
	osd_set_pxp_mode(prop_idx);

	prop = of_get_property(pdev->dev.of_node, "ddr_urgent", NULL);
	if (prop) {
		prop_idx = of_read_ulong(prop, 1);
		osd_set_urgent(0, (prop_idx != 0) ? 1 : 0);
		osd_set_urgent(1, (prop_idx != 0) ? 1 : 0);
	}
	prop = of_get_property(pdev->dev.of_node, "mem_alloc", NULL);
	if (prop)
		b_alloc_mem = of_read_ulong(prop, 1);

	vinfo = get_current_vinfo();
	for (index = 0; index < osd_meson_dev.osd_count; index++) {
		/* register frame buffer memory */
		if (index > OSD_COUNT - 1)
			break;
		if (!fb_memsize[index + 1])
			continue;
		fbi = framebuffer_alloc(sizeof(struct osd_fb_dev_s),
				&pdev->dev);
		if (!fbi) {
			ret = -ENOMEM;
			goto failed1;
		}
		fbdev = (struct osd_fb_dev_s *)fbi->par;
		fbdev->fb_index = index;
		fbdev->fb_info = fbi;
		fbdev->dev = pdev;
		mutex_init(&fbdev->lock);
		var = &fbi->var;
		fix = &fbi->fix;
		gp_fbdev_list[index] = fbdev;
		fbdev->fb_len = 0;
		fbdev->fb_mem_paddr = 0;
		fbdev->fb_mem_vaddr = 0;
		if (vinfo) {
			fb_def_var[index].width = vinfo->screen_real_width;
			fb_def_var[index].height = vinfo->screen_real_height;
		}

		/* setup fb0 display size */
		if (index == DEV_OSD0) {
			ret = of_property_read_u32_array(pdev->dev.of_node,
					"display_size_default",
					&var_screeninfo[0], 5);
			if (ret)
				osd_log_info("not found display_size_default\n");
			else {
				if (osd_set_fb_var(index, vinfo)) {
					/* no available vinfo, set default */
					fb_def_var[index].xres =
						var_screeninfo[0];
					fb_def_var[index].yres =
						var_screeninfo[1];
					fb_def_var[index].xres_virtual =
						var_screeninfo[2];
					fb_def_var[index].yres_virtual =
						var_screeninfo[3];
					fb_def_var[index].bits_per_pixel =
						var_screeninfo[4];
				}
				pr_info("fb def : %d %d %d %d %d\n",
					fb_def_var[index].xres,
					fb_def_var[index].yres,
					fb_def_var[index].xres_virtual,
					fb_def_var[index].yres_virtual,
					fb_def_var[index].bits_per_pixel);
				pr_info("init fbdev bpp is:%d\n",
					fb_def_var[index].bits_per_pixel);

				if (fb_def_var[index].bits_per_pixel > 32)
					fb_def_var[index].bits_per_pixel = 32;
			}
		}

		_fbdev_set_default(fbdev, index);
		if (fbdev->color == NULL) {
			osd_log_err("fbdev->color NULL\n");
			ret = -ENOENT;
			goto failed1;
		}
		bpp = (fbdev->color->color_index > 8 ?
				(fbdev->color->color_index > 16 ?
				(fbdev->color->color_index > 24 ?
				 4 : 3) : 2) : 1);
		fix->line_length = CANVAS_ALIGNED(var->xres_virtual * bpp);
		fix->smem_start = fbdev->fb_mem_paddr;
		fix->smem_len = fbdev->fb_len;
		if (fb_alloc_cmap(&fbi->cmap, 16, 0) != 0) {
			osd_log_err("unable to allocate color map memory\n");
			ret = -ENOMEM;
			goto failed2;
		}
		fbi->pseudo_palette = kmalloc(sizeof(u32) * 16, GFP_KERNEL);
		if (!(fbi->pseudo_palette)) {
			osd_log_err("unable to allocate pseudo palette mem\n");
			ret = -ENOMEM;
			goto failed2;
		}
		memset(fbi->pseudo_palette, 0, sizeof(u32) * 16);
		fbi->fbops = &osd_ops;
		fbi->screen_base = (char __iomem *)fbdev->fb_mem_vaddr;
		fbi->screen_size = fix->smem_len;
		if (vinfo)
			set_default_display_axis(&fbdev->fb_info->var,
					&fbdev->osd_ctl, vinfo);
		osd_check_var(var, fbi);
		/* register frame buffer */
		register_framebuffer(fbi);
		/* create device attribute files */
		if (index <= (osd_meson_dev.viu1_osd_count - 1)) {
			for (i = 0; i < ARRAY_SIZE(osd_attrs); i++)
				ret = device_create_file(
				fbi->dev, &osd_attrs[i]);
		} else if ((osd_meson_dev.has_viu2) &&
			(index == osd_meson_dev.viu2_index)) {
			for (i = 0; i < ARRAY_SIZE(osd_attrs_viu2); i++)
			ret = device_create_file(fbi->dev, &osd_attrs_viu2[i]);
		}

		if (index == DEV_OSD0)
			osd_set_fb_parameters(DEV_OSD0, vinfo);
	}
#ifdef CONFIG_AMLOGIC_LEGACY_EARLY_SUSPEND
	early_suspend.level = EARLY_SUSPEND_LEVEL_STOP_DRAWING;
	early_suspend.suspend = osd_early_suspend;
	early_suspend.resume = osd_late_resume;
	register_early_suspend(&early_suspend);
#endif

	/* init osd reverse */
	if (osd_info.index == DEV_ALL) {
		for (i = 0; i < osd_meson_dev.viu1_osd_count; i++)
			osd_set_reverse_hw(i, osd_info.osd_reverse, 1);
		osd_set_reverse_hw(i, osd_info.osd_reverse, 0);
	} else if (osd_info.index <= osd_meson_dev.viu1_osd_count - 1)
		osd_set_reverse_hw(osd_info.index, osd_info.osd_reverse, 1);
	else if (osd_info.index == osd_meson_dev.viu2_index)
		osd_set_reverse_hw(osd_info.index, osd_info.osd_reverse, 0);
	/* register vout client */
	vout_register_client(&osd_notifier_nb);
#ifdef CONFIG_AMLOGIC_VOUT2_SERVE
	if (osd_meson_dev.has_viu2)
		vout2_register_client(&osd_notifier_nb2);
#endif
	INIT_DELAYED_WORK(&osd_dwork, mem_free_work);
	schedule_delayed_work(&osd_dwork, msecs_to_jiffies(60 * 1000));

	osd_log_info("osd probe OK\n");
	return 0;
failed2:
	fb_dealloc_cmap(&fbi->cmap);
failed1:
	osd_log_err("osd probe failed.\n");
	return ret;
}

static int osd_remove(struct platform_device *pdev)
{
	struct fb_info *fbi;
	int i;

	osd_log_info("osd_remove.\n");
	if (!pdev)
		return -ENODEV;
	if (osd_meson_dev.osd_ver == OSD_NONE) {
		amlfb_virtual_remove(pdev);
		return 0;
	}
#ifdef CONFIG_AMLOGIC_LEGACY_EARLY_SUSPEND
	unregister_early_suspend(&early_suspend);
#endif
	vout_unregister_client(&osd_notifier_nb);
#ifdef CONFIG_AMLOGIC_VOUT2_SERVE
	if (osd_meson_dev.has_viu2)
		vout2_unregister_client(&osd_notifier_nb2);
#endif
	for (i = 0; i < osd_meson_dev.osd_count; i++) {
		int j;

		if (i > OSD_COUNT - 1)
			break;
		if (gp_fbdev_list[i]) {
			struct osd_fb_dev_s *fbdev = gp_fbdev_list[i];

			fbi = fbdev->fb_info;
			if (i <= osd_meson_dev.viu1_osd_count - 1) {
				for (j = 0; j < ARRAY_SIZE(osd_attrs); j++)
					device_remove_file(
					fbi->dev, &osd_attrs[j]);
			} else if ((osd_meson_dev.has_viu2) &&
				(i == osd_meson_dev.viu2_index)) {
				for (j = 0; j < ARRAY_SIZE(osd_attrs_viu2); j++)
					device_remove_file(
					fbi->dev, &osd_attrs_viu2[j]);
			}
			iounmap(fbdev->fb_mem_vaddr);
			if (osd_get_afbc(i)) {
				for (j = 0; j < OSD_MAX_BUF_NUM; j++)
					iounmap(fbdev->fb_mem_afbc_vaddr[j]);
			}
			kfree(fbi->pseudo_palette);
			fb_dealloc_cmap(&fbi->cmap);
			unregister_framebuffer(fbi);
			framebuffer_release(fbi);
		}
	}
	return 0;
}

static void osd_shutdown(struct platform_device *pdev)
{
	if (osd_meson_dev.osd_ver == OSD_NONE)
		return;
	if (!osd_shutdown_flag) {
		osd_shutdown_flag = 1;
		osd_shutdown_hw();
	}
}

/* Process kernel command line parameters */
static int __init osd_setup_attribute(char *options)
{
	char *this_opt = NULL;
	int r = 0;
	unsigned long res;

	if (!options || !*options)
		goto exit;
	while (!r && (this_opt = strsep(&options, ",")) != NULL) {
		if (!strncmp(this_opt, "xres:", 5)) {
			r = kstrtol(this_opt + 5, 0, &res);
			fb_def_var[0].xres = fb_def_var[0].xres_virtual = res;
		} else if (!strncmp(this_opt, "yres:", 5)) {
			r = kstrtol(this_opt + 5, 0, &res);
			fb_def_var[0].yres = fb_def_var[0].yres_virtual = res;
		} else {
			osd_log_info("invalid option\n");
			r = -1;
		}
	}
exit:
	return r;
}


static int __init
rmem_fb_device_init(struct reserved_mem *rmem, struct device *dev)
{
	if (!of_get_flat_dt_prop(rmem->fdt_node, "no-map", NULL)) {
		fb_map_flag = true;
		osd_log_info("momery mapped[can free]\n");
	} else
		fb_map_flag = false;
	return 0;
}

static const struct reserved_mem_ops rmem_fb_ops = {
	.device_init = rmem_fb_device_init,
};

static int __init rmem_fb_setup(struct reserved_mem *rmem)
{
	phys_addr_t align = PAGE_SIZE;
	phys_addr_t mask = align - 1;

	if ((rmem->base & mask) || (rmem->size & mask)) {
		pr_err("Reserved memory: incorrect alignment of region\n");
		return -EINVAL;
	}
	fb_rmem.base = rmem->base;
	fb_rmem.size = rmem->size;
	rmem->ops = &rmem_fb_ops;
	osd_log_info("Reserved memory: created fb at 0x%p, size %ld MiB\n",
		     (void *)rmem->base, (unsigned long)rmem->size / SZ_1M);
	return 0;
}
RESERVEDMEM_OF_DECLARE(fb, "amlogic, fb-memory", rmem_fb_setup);


#ifdef CONFIG_HIBERNATION
const struct dev_pm_ops osd_pm = {
	.freeze		= osd_freeze,
	.thaw		= osd_thaw,
	.restore	= osd_restore,
	.suspend	= osd_pm_suspend,
	.resume		= osd_pm_resume,
};
#endif

static struct platform_driver osd_driver = {
	.probe      = osd_probe,
	.remove     = osd_remove,
	.shutdown = osd_shutdown,
#ifdef CONFIG_PM
	.suspend  = osd_suspend,
	.resume    = osd_resume,
#endif
	.driver     = {
		.name   = "meson-fb",
		.of_match_table = meson_fb_dt_match,
#ifdef CONFIG_HIBERNATION
		.pm     = &osd_pm,
#endif
	}
};

static int __init osd_init_module(void)
{
	char *option;

	osd_log_info("%s\n", __func__);
	if (fb_get_options("meson-fb", &option)) {
		osd_log_err("failed to get meson-fb options!\n");
		return -ENODEV;
	}
	osd_setup_attribute(option);
	if (platform_driver_register(&osd_driver)) {
		osd_log_err("failed to register OSD driver!\n");
		return -ENODEV;
	}
#ifdef CONFIG_INSTABOOT
	INIT_LIST_HEAD(&osd_realdata_ops.node);
	register_instaboot_realdata_ops(&osd_realdata_ops);
#endif
	return 0;
}

static void __exit osd_exit_module(void)
{
	osd_log_info("%s\n", __func__);
	platform_driver_unregister(&osd_driver);
}

module_init(osd_init_module);
module_exit(osd_exit_module);

MODULE_AUTHOR("Platform-BJ <platform.bj@amlogic.com>");
MODULE_DESCRIPTION("OSD Module");
MODULE_LICENSE("GPL");

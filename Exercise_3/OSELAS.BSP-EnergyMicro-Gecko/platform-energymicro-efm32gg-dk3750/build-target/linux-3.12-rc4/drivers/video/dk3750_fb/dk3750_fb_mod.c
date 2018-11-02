/*
 *  SSD2119 display driver for EFM32GG DK3750
 *
 *  Based on simplefb and EFM32 TFT example code
 *
 *  Asbjørn Djupdal 2013
 *
 */

#include <linux/errno.h>
#include <linux/fb.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/platform_data/simplefb.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include "bsp_dk_bcreg_3201.h"
#include "io.h"
#include "dmd_ssd2119.h"
#include "dmdif_ssd2119_ebi.h"
#include "dmd.h"

// Custom IOCTLs 
#define IOCTL_FB_DK3750_DIRTY 0x4680

// TODO: move to platform data
static uint8_t *dk3750_fb; // framebuffer
struct resource *mem_resource;
void *iomem;               // io registers

static struct fb_fix_screeninfo dk3750_fb_fix = {
	.id		= "dk3750_fb",
	.type		= FB_TYPE_PACKED_PIXELS,
	.visual		= FB_VISUAL_TRUECOLOR,
	.accel		= FB_ACCEL_NONE,
};

static struct fb_var_screeninfo dk3750_fb_var = {
	.height		= -1,
	.width		= -1,
	.activate	= FB_ACTIVATE_NOW,
	.vmode		= FB_VMODE_NONINTERLACED,
};

static ssize_t dk3750_fb_read(struct fb_info *info, char __user *buf,
         size_t count, loff_t *ppos)
{
  // do not allow reading, dmd driver code comment claims it does not work
  return -EINVAL;
}

static ssize_t dk3750_fb_write(struct fb_info *info, const char __user *buf,
          size_t count, loff_t *ppos)
{
  // FIXME: copy_from_user

  int pixel = *ppos / 2;
  int x = pixel % DMD_HORIZONTAL_SIZE;
  int y = pixel / DMD_HORIZONTAL_SIZE;
  int numPixels = count / 2;

  if((*ppos + count) > (DMD_HORIZONTAL_SIZE * DMD_VERTICAL_SIZE * 2))
    return -EINVAL;
  if(*ppos % 2) return -EINVAL;
  if(count % 2) return -EINVAL;

  DMD_setClippingArea(0, 0, DMD_HORIZONTAL_SIZE, DMD_VERTICAL_SIZE);
  DMD_writeData(x, y, buf, numPixels);

  return count;
}

int dk3750_fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
{
  // FIXME: copy_from_user

  // Custom ioctl to mark a region of the framebuffer as "dirty" (need
  // pushing to display controller)
  // Takes a rectangle as argument
  if(cmd == IOCTL_FB_DK3750_DIRTY) {
    struct fb_copyarea *rect = (struct fb_copyarea*)arg;
    DMD_setClippingArea(rect->dx, rect->dy, rect->width, rect->height);
    DMD_blitFB((uint16_t*)dk3750_fb);

  } else {
    return -EINVAL;
  }
  return 0;
}

static void dk3750_fb_fillrect(struct fb_info *info, const struct fb_fillrect *rect)
{
  cfb_fillrect(info, rect);
  DMD_setClippingArea(rect->dx, rect->dy, rect->width, rect->height);
  DMD_blitFB((uint16_t*)dk3750_fb);
}

static void dk3750_fb_copyarea(struct fb_info *info,
                               const struct fb_copyarea *region)
{
  cfb_copyarea(info, region);
  DMD_setClippingArea(region->dx, region->dy, region->width, region->height);
  DMD_blitFB((uint16_t*)dk3750_fb);
}

static void dk3750_fb_imageblit(struct fb_info *info, const struct fb_image *image)
{
  cfb_imageblit(info, image);
  DMD_setClippingArea(image->dx, image->dy, image->width, image->height);
  DMD_blitFB((uint16_t*)dk3750_fb);
}

static int dk3750_fb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
			      u_int transp, struct fb_info *info)
{
	u32 *pal = info->pseudo_palette;
	u32 cr = red >> (16 - info->var.red.length);
	u32 cg = green >> (16 - info->var.green.length);
	u32 cb = blue >> (16 - info->var.blue.length);
	u32 value;

	if (regno >= 16)
		return -EINVAL;

	value = (cr << info->var.red.offset) |
		(cg << info->var.green.offset) |
		(cb << info->var.blue.offset);
	if (info->var.transp.length > 0) {
		u32 mask = (1 << info->var.transp.length) - 1;
		mask <<= info->var.transp.offset;
		value |= mask;
	}
	pal[regno] = value;

	return 0;
}

static struct fb_ops dk3750_fb_ops = {
	.owner		= THIS_MODULE,
  .fb_read = dk3750_fb_read,
  .fb_write = dk3750_fb_write,
  .fb_ioctl = dk3750_fb_ioctl,
  .fb_setcolreg	= dk3750_fb_setcolreg,
  .fb_fillrect = dk3750_fb_fillrect,
  .fb_copyarea = dk3750_fb_copyarea,
  .fb_imageblit = dk3750_fb_imageblit,
};

static struct simplefb_format dk3750_fb_formats[] = SIMPLEFB_FORMATS;

struct dk3750_fb_params {
	u32 width;
	u32 height;
	u32 stride;
	struct simplefb_format *format;
};

static int dk3750_fb_parse_dt(struct platform_device *pdev,
			   struct dk3750_fb_params *params)
{
	struct device_node *np = pdev->dev.of_node;
	int ret;
	const char *format;
	int i;

	ret = of_property_read_u32(np, "width", &params->width);
	if (ret) {
		dev_err(&pdev->dev, "Can't parse width property\n");
		return ret;
	}

	ret = of_property_read_u32(np, "height", &params->height);
	if (ret) {
		dev_err(&pdev->dev, "Can't parse height property\n");
		return ret;
	}

	ret = of_property_read_u32(np, "stride", &params->stride);
	if (ret) {
		dev_err(&pdev->dev, "Can't parse stride property\n");
		return ret;
	}

	ret = of_property_read_string(np, "format", &format);
	if (ret) {
		dev_err(&pdev->dev, "Can't parse format property\n");
		return ret;
	}
	params->format = NULL;
	for (i = 0; i < ARRAY_SIZE(dk3750_fb_formats); i++) {
		if (strcmp(format, dk3750_fb_formats[i].name))
			continue;
		params->format = &dk3750_fb_formats[i];
		break;
	}
	if (!params->format) {
		dev_err(&pdev->dev, "Invalid format value\n");
		return -EINVAL;
	}

	return 0;
}

static int dk3750_fb_parse_pd(struct platform_device *pdev,
			     struct dk3750_fb_params *params)
{
	struct simplefb_platform_data *pd = pdev->dev.platform_data;
	int i;

	params->width = pd->width;
	params->height = pd->height;
	params->stride = pd->stride;

	params->format = NULL;
	for (i = 0; i < ARRAY_SIZE(dk3750_fb_formats); i++) {
		if (strcmp(pd->format, dk3750_fb_formats[i].name))
			continue;

		params->format = &dk3750_fb_formats[i];
		break;
	}

	if (!params->format) {
		dev_err(&pdev->dev, "Invalid format value\n");
		return -EINVAL;
	}

	return 0;
}

static bool initBoard(void) {
  uint16_t tmp;

  if(*UIF_AEM != BC_UIF_AEM_EFM) {
    printk(KERN_ALERT
           "DK3750_FB: Not in EFM mode, press AEM button to continue\n");
    // TODO: not sure how to handle this situation properly
    while(*UIF_AEM != BC_UIF_AEM_EFM) msleep(100);
  }

  // TODO: is it OK to write registers here?

  // display ebi
  *ARB_CTRL = BC_ARB_CTRL_EBI;
  
  // assert reset
  tmp  = *DISPLAY_CTRL;
  tmp |= BC_DISPLAY_CTRL_RESET;
  *DISPLAY_CTRL = tmp;

  // power disable
  tmp  = *DISPLAY_CTRL;
  tmp &= ~(BC_DISPLAY_CTRL_POWER_ENABLE);
  *DISPLAY_CTRL = tmp;

  /* Short reset delay */
  msleep(10);

  // mode 8080
  tmp &= ~(BC_DISPLAY_CTRL_MODE_GENERIC);
  *DISPLAY_CTRL = tmp;

  // power enable
  tmp |= (BC_DISPLAY_CTRL_POWER_ENABLE);
  *DISPLAY_CTRL = tmp;

  // reset release
  tmp &= ~(BC_DISPLAY_CTRL_RESET);
  *DISPLAY_CTRL = tmp;

  return true;
}

static int dk3750_fb_probe(struct platform_device *pdev)
{
	int ret;
	struct dk3750_fb_params params;
	struct fb_info *info;
  struct resource *res;

	if (fb_get_options("dk3750_fb", NULL))
		return -ENODEV;

	ret = -ENODEV;
	if (pdev->dev.platform_data)
		ret = dk3750_fb_parse_pd(pdev, &params);
	else if (pdev->dev.of_node)
		ret = dk3750_fb_parse_dt(pdev, &params);

	if (ret)
		return ret;

	mem_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem_resource) {
		dev_err(&pdev->dev, "No memory resource\n");
		return -EINVAL;
	}

  res = request_mem_region(mem_resource->start, resource_size(mem_resource), "dk3750_fb");
  if(!res) {
		dev_err(&pdev->dev, "Can't request mem\n");
    return -ENOMEM;
  }

  iomem = ioremap_nocache(mem_resource->start, resource_size(mem_resource));
  if(iomem == NULL) {
    printk("Can't remap\n");
    release_mem_region(mem_resource->start, resource_size(mem_resource));
    return -EBUSY;
  }

  dk3750_fb = (uint8_t*)kmalloc(params.width * params.height * 2, GFP_KERNEL);
  if(!dk3750_fb) {
    printk(KERN_ERR "DK3750_FB: Can't allocate framebuffer\n");
    iounmap(iomem);
    release_mem_region(mem_resource->start, resource_size(mem_resource));
    return -ENOMEM;
  }

  if(!initBoard()) {
    printk(KERN_ERR "DK3750_FB: Can't init board\n");
    kfree(dk3750_fb);
    iounmap(iomem);
    release_mem_region(mem_resource->start, resource_size(mem_resource));
    return -EBUSY;
  }

  if(DMDIF_init((uint32_t)iomem, (uint32_t)(iomem + 2)) != DMD_OK) {
    printk(KERN_ERR "DK3750_FB: Can't init DMDIF\n");
    kfree(dk3750_fb);
    iounmap(iomem);
    release_mem_region(mem_resource->start, resource_size(mem_resource));
    return -EBUSY;
  }

  if(DMD_init(0) != DMD_OK) {
    printk(KERN_ERR "DK3750_FB: Can't init DMD\n");
    kfree(dk3750_fb);
    iounmap(iomem);
    release_mem_region(mem_resource->start, resource_size(mem_resource));
    return -EBUSY;
  }

  DMD_flipDisplay(1, 1);

	info = framebuffer_alloc(sizeof(u32) * 16, &pdev->dev);
	if (!info) {
    kfree(dk3750_fb);
    iounmap(iomem);
    release_mem_region(mem_resource->start, resource_size(mem_resource));
		return -ENOMEM;
  }
	platform_set_drvdata(pdev, info);

	info->fix = dk3750_fb_fix;
	info->fix.smem_start = (unsigned long)dk3750_fb;
	info->fix.smem_len = params.width * params.height * 2;
	info->fix.line_length = params.stride;

	info->var = dk3750_fb_var;
	info->var.xres = params.width;
	info->var.yres = params.height;
	info->var.xres_virtual = params.width;
	info->var.yres_virtual = params.height;
	info->var.bits_per_pixel = params.format->bits_per_pixel;
	info->var.red = params.format->red;
	info->var.green = params.format->green;
	info->var.blue = params.format->blue;
	info->var.transp = params.format->transp;

	info->apertures = alloc_apertures(1);
	if (!info->apertures) {
		framebuffer_release(info);
    kfree(dk3750_fb);
    iounmap(iomem);
    release_mem_region(mem_resource->start, resource_size(mem_resource));
		return -ENOMEM;
	}
	info->apertures->ranges[0].base = info->fix.smem_start;
	info->apertures->ranges[0].size = info->fix.smem_len;

	info->fbops = &dk3750_fb_ops;
	info->flags = FBINFO_DEFAULT | FBINFO_MISC_FIRMWARE;
	info->screen_base = devm_ioremap(&pdev->dev, info->fix.smem_start,
					 info->fix.smem_len);
	if (!info->screen_base) {
		framebuffer_release(info);
    kfree(dk3750_fb);
    iounmap(iomem);
    release_mem_region(mem_resource->start, resource_size(mem_resource));
		return -ENODEV;
	}
	info->pseudo_palette = (void *)(info + 1);

	ret = register_framebuffer(info);
	if (ret < 0) {
		dev_err(&pdev->dev, "Unable to register dk3750_fb: %d\n", ret);
		framebuffer_release(info);
    kfree(dk3750_fb);
    iounmap(iomem);
    release_mem_region(mem_resource->start, resource_size(mem_resource));
		return ret;
	}

	dev_info(&pdev->dev, "fb%d: dk3750_fb registered!\n", info->node);

	return 0;
}

static int dk3750_fb_remove(struct platform_device *pdev)
{
	struct fb_info *info = platform_get_drvdata(pdev);

  kfree(dk3750_fb);

  iounmap(iomem);
  release_mem_region(mem_resource->start, resource_size(mem_resource));

	unregister_framebuffer(info);
	framebuffer_release(info);

	return 0;
}

static const struct of_device_id dk3750_fb_of_match[] = {
	{ .compatible = "dk3750_fb", },
	{ },
};
MODULE_DEVICE_TABLE(of, dk3750_fb_of_match);

static struct platform_driver dk3750_fb_driver = {
	.driver = {
		.name = "dk3750_fb",
		.owner = THIS_MODULE,
		.of_match_table = dk3750_fb_of_match,
	},
	.probe = dk3750_fb_probe,
	.remove = dk3750_fb_remove,
};
module_platform_driver(dk3750_fb_driver);

MODULE_DESCRIPTION("DK3750 frame buffer driver");
MODULE_LICENSE("GPL v2");

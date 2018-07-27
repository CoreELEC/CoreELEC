/*

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef CONFIG_ARM64
#include <mach/am_regs.h>
#else
#include <linux/reset.h>
#endif
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/firmware.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/version.h>
#include <linux/amlogic/aml_gpio_consumer.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#include <linux/of_fdt.h>
#include <linux/pinctrl/consumer.h>
#include <linux/amlogic/cpu_version.h>
#include <linux/of_reserved_mem.h>
#include <linux/amlogic/aml_thermal_hw.h>
#include <linux/gpio.h>
#include <linux/amlogic/aml_gpio_consumer.h>
#include <linux/of_gpio.h>
#include <linux/amlogic/iomap.h>

#define DEV_NAME           "kvimfan"

#undef pr_err
#define pr_err(fmt, args...) printk("[error] KVIM-FAN: " fmt, ## args)

#define pr_inf(fmt, args...)   printk("KVIM-FAN: " fmt, ## args)

#define pr_dbg(fmt, args...) \
	do {\
		if (debug_fan)\
			printk("KVIM-FAN: " fmt, ##args);\
	} while (0)

MODULE_PARM_DESC(debug_fan, "\n\t\t Enable debug information");
static int debug_fan = 0;
module_param(debug_fan, int, 0644);

MODULE_PARM_DESC(trig_temp_level0, "\n\t\t Trigger temp level0");
static int trig_temp_level0 = 50;
module_param(trig_temp_level0, int, 0644);

MODULE_PARM_DESC(trig_temp_level1, "\n\t\t Trigger temp level1");
static int trig_temp_level1 = 60;
module_param(trig_temp_level1, int, 0644);

MODULE_PARM_DESC(trig_temp_level2, "\n\t\t Trigger temp level2");
static int trig_temp_level2 = 70;
module_param(trig_temp_level2, int, 0644);

static 	int	fan_ctl0, fan_ctl1, last_level = 0, last_temp = 0;

int kvim_fan_gpio(int level)
{
	int l0, l1;
	pr_dbg("%s: level:%d last_level:%d\n", __FUNCTION__, level, last_level);

	switch (last_level) {
	case 1:
	  l0 = 1;
	  l1 = 1;
	  break;
	case 2:
	  l0 = 0;
	  l1 = 1;
	  break;
	case 3:
	  l0 = 1;
	  l1 = 0;
	  break;
	default:
	  l0 = 0;
	  l1 = 0;
	  break;
	}
	gpio_request(fan_ctl0,DEV_NAME);
	gpio_direction_output(fan_ctl0, l0);
	gpio_request(fan_ctl1,DEV_NAME);
	gpio_direction_output(fan_ctl1, l1);
	if (last_level > 3 && level > 3)
		last_level--;
        else
		last_level = level;

	return 0;
}
static int kvim_fan_open(struct inode *inode, struct file *file)
{
	pr_dbg("%s\n", __func__);
	return 0;
}

static int kvim_fan_release(struct inode *inode, struct file *file)
{
	pr_dbg("%s\n", __func__);
	return 0;
}

static ssize_t kvim_fan_read(struct file *filp, char __user * buf,
				  size_t count, loff_t * f_pos)
{
	int temp, level =1;

	temp =  get_cpu_temp();
	pr_dbg("CPU temp: %d C\n", temp);
	if (temp < 0)
		return 1;

	last_temp = (last_temp + temp) / 2;
	if (last_temp < trig_temp_level0)
		level = 0;
	else if (last_temp < trig_temp_level0 + 2)
		level = 6;
	else if (last_temp < trig_temp_level0 + 4)
		level = 5;
	else if (last_temp < trig_temp_level0 + 6)
		level = 4;
	else if (last_temp < trig_temp_level1)
		level = 3;
	else if (last_temp < trig_temp_level2)
		level = 2;

	kvim_fan_gpio(level);
	if (level > 3)
		return 1;

	return 0;
}

static struct file_operations kvim_fan_fops = {
	.owner = THIS_MODULE,
	.open = kvim_fan_open,
	.release = kvim_fan_release,
	.read = kvim_fan_read,
	.write = NULL,
	.unlocked_ioctl = NULL,
	.compat_ioctl = NULL,
	.poll = NULL
};

static struct miscdevice kvim_fan_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEV_NAME,
	.fops = &kvim_fan_fops
};

static int kvim_fan_probe(struct platform_device *pdev)
{
        struct gpio_desc *desc;
        int ret;

	pr_inf("Init kvimfan\n");
                                
 	desc = of_get_named_gpiod_flags(pdev->dev.of_node, "fan_ctl0", 0, NULL);
	fan_ctl0 = desc_to_gpio(desc);
        pr_inf("fan_ctl0 = 0x%02x\n", fan_ctl0);

 	desc = of_get_named_gpiod_flags(pdev->dev.of_node, "fan_ctl1", 0, NULL);
	fan_ctl1 = desc_to_gpio(desc);
        pr_inf("fan_ctl1 = 0x%02x\n", fan_ctl1);

        if (of_property_read_u32(pdev->dev.of_node, "trig_temp_level0", &trig_temp_level0))
	{
		ret = -ENOMEM;
		goto err_resource;
	}
        pr_inf("trig_temp_level0 = %3d\n", trig_temp_level0);

        if (of_property_read_u32(pdev->dev.of_node, "trig_temp_level1", &trig_temp_level1))
	{
		ret = -ENOMEM;
		goto err_resource;
	}
        pr_inf("trig_temp_level1 = %3d\n", trig_temp_level1);

        if (of_property_read_u32(pdev->dev.of_node, "trig_temp_level2", &trig_temp_level2))
	{
		ret = -ENOMEM;
		goto err_resource;
	}
        pr_inf("trig_temp_level2 = %3d\n", trig_temp_level2);

	ret = misc_register(&kvim_fan_device);
	if (!ret)
	{
		pr_inf("%s: Succeeded to register %s device\n", __func__, DEV_NAME);	
		return 0;
	}
err_resource:
	pr_err("%s: Failed to register %s device\n", __func__, DEV_NAME);
	return ret;
}

static int kvim_fan_remove(struct platform_device *pdev)
{
	return 0;
}

static int kvim_fan_resume(struct platform_device *pdev)
{
	pr_inf("%s\n", __func__);
	return 0;
}

static int kvim_fan_suspend(struct platform_device *pdev, pm_message_t state)
{
	pr_inf("%s\n", __func__);
	kvim_fan_gpio(0);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id kvim_fan_dt_match[]={
        {
                .compatible = "fanctl",
        },
        {},
};
#endif /*CONFIG_OF*/
static struct platform_driver kvim_fan_driver = {
        .probe = kvim_fan_probe,
        .remove = kvim_fan_remove,
        .resume = kvim_fan_resume,
        .suspend = kvim_fan_suspend,
        .driver = {
    	.name = "kvimfan",
        .owner = THIS_MODULE,
#ifdef CONFIG_OF
        .of_match_table = kvim_fan_dt_match,
#endif
        }
};

static int __init kvim_fan_init(void) {
	 return platform_driver_register(&kvim_fan_driver);
}

static void __exit kvim_fan_exit(void) {
        platform_driver_unregister(&kvim_fan_driver);
}

module_init(kvim_fan_init);
module_exit(kvim_fan_exit);
MODULE_AUTHOR("afl1");
MODULE_DESCRIPTION("Khadas VIM fan driver");
MODULE_LICENSE("GPL");

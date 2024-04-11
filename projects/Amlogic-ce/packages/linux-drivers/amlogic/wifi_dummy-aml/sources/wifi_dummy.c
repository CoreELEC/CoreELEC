#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kszaq");
MODULE_DESCRIPTION("Amlogic WiFi power on and SDIO rescan module");

extern void extern_wifi_set_enable(int);
extern void sdio_reinit(void);
#ifdef CONFIG_PCI
extern void set_usb_wifi_power(int);
extern void pci_remove(void);
extern void pci_reinit(void);
#endif

static int __init wifi_dummy_init(void)
{
	printk(KERN_INFO "Triggered SDIO/PCI WiFi power on and bus rescan.\n");
#ifdef CONFIG_PCI
	pci_remove();
	set_usb_wifi_power(0);
#endif
	extern_wifi_set_enable(0);
	msleep(300);
	extern_wifi_set_enable(1);
#ifdef CONFIG_PCI
	set_usb_wifi_power(1);
#endif
	msleep(300);
	sdio_reinit();
#ifdef CONFIG_PCI
	pci_reinit();
#endif
	return 0;
}

static void __exit wifi_dummy_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(wifi_dummy_init);
module_exit(wifi_dummy_cleanup);

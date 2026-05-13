#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/reboot.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kszaq");
MODULE_DESCRIPTION("Amlogic WiFi power on and SDIO/PCIe rescan module");

extern void extern_wifi_set_enable(int);
extern void sdio_reinit(void);

#define FIREFCUBE_MT7668_PMU_RESET_MS 800
#define FIREFCUBE_MT7668_BOOT_SETTLE_MS 500
#define FIREFCUBE_MT7668_SHUTDOWN_RESET_MS 500

#undef CONFIG_PCI

#ifdef CONFIG_PCI
extern void set_usb_wifi_power(int);
extern void pci_remove(void);
extern void pci_reinit(void);
#endif

static bool firecube_mt7668_resident;

/*
      name sdio                name sdio                 name sd1          
 full_name sdio@fe088000  full_name /sdio@ffe03000  full_name /sd1@ffe05000

      name pcie                name pcieA
 full_name pcie@e0000000  full_name /pcieA@fc000000
*/

static bool device_enabled(const char *path, const char *prefix)
{
struct device_node *parent_node;
struct device_node *child;
bool ret = false;
int len;

	/*pr_info("wifi_dummy: path=%s prefix=%s\n", path, prefix);*/

	parent_node = of_find_node_by_path(path);
	if (parent_node) {
		for_each_child_of_node(parent_node, child) {
			/*pr_info("wifi_dummy: full_name=%s, name=%s\n", child->full_name, child->name);*/
			if (!strncmp(child->name, prefix, strlen(prefix))) {
				len = strlen(child->name);

				if (child->full_name[0] == '/')
					len++;  /* include '/' */

				if (strlen(child->full_name) > len && child->full_name[len] == '@') {
					if (of_device_is_available(child)) {
						pr_info("wifi_dummy: found enabled %s\n", child->full_name);
						ret = true;
						break;
					} else {
						pr_info("wifi_dummy: found disabled %s\n", child->full_name);
					}
				}
			}
		}

		of_node_put(parent_node);
	}

	return ret;
}

static bool firecube_is_2nd_gen(void)
{
	struct device_node *root;
	const char *dt_id = NULL;
	bool ret = false;

	root = of_find_node_by_path("/");
	if (!root)
		return false;

	if (!of_property_read_string(root, "coreelec-dt-id", &dt_id) &&
	    !strcmp(dt_id, "g12b_s922z_amazon_2nd_gen_cube"))
		ret = true;

	of_node_put(root);
	return ret;
}

static void firecube_reset_sdio_wifi(void)
{
	pr_info("wifi_dummy: resetting MT7668 SDIO power rail before bus rescan\n");
	extern_wifi_set_enable(0);
	msleep(FIREFCUBE_MT7668_PMU_RESET_MS);
	extern_wifi_set_enable(1);
	msleep(FIREFCUBE_MT7668_BOOT_SETTLE_MS);
}

static int firecube_mt7668_reboot_notify(struct notifier_block *nb,
					 unsigned long action, void *data)
{
	pr_info("wifi_dummy: powering down MT7668 SDIO rail for warm reboot\n");
	extern_wifi_set_enable(0);
	msleep(FIREFCUBE_MT7668_SHUTDOWN_RESET_MS);

	return NOTIFY_DONE;
}

static struct notifier_block firecube_mt7668_reboot_nb = {
	.notifier_call = firecube_mt7668_reboot_notify,
};

static int __init wifi_dummy_init(void)
{
bool sdio_en = false;
bool pcie_en = false;
bool is_firecube = false;

	pr_info("wifi_dummy: Triggered SDIO/PCIe WiFi power on and bus rescan\n");

	sdio_en  = device_enabled("/soc", "sdio");
	pcie_en  = device_enabled("/soc", "pcie");
	sdio_en |= device_enabled("/", "sdio");
	pcie_en |= device_enabled("/", "pcie");
	sdio_en |= device_enabled("/", "sd2");

	if (!sdio_en && !pcie_en) {
		pr_info("wifi_dummy: SDIO/PCIe not enabled\n");
		return -ENODEV;
	}

	pr_info("wifi_dummy: SDIO %s, PCIe %s\n",
		sdio_en ? "enabled" : "disabled",
		pcie_en ? "enabled" : "disabled");

	is_firecube = firecube_is_2nd_gen();
	if (is_firecube)
		pr_info("wifi_dummy: Fire TV Cube 2nd Gen MT7668 reset path enabled\n");

#ifdef CONFIG_PCI
	if (pcie_en) {
		pci_remove();
		set_usb_wifi_power(0);
	}
#endif

	if (sdio_en) {
		if (is_firecube) {
			firecube_reset_sdio_wifi();
		} else {
			extern_wifi_set_enable(0);
			msleep(300);
			extern_wifi_set_enable(1);
		}
	} else {
		msleep(300);
	}

#ifdef CONFIG_PCI
	if (pcie_en)
		set_usb_wifi_power(1);
#endif

	msleep(300);

	if (sdio_en)
		sdio_reinit();

#ifdef CONFIG_PCI
	if (pcie_en)
		pci_reinit();
#endif

	/*
	 * Keep this helper resident on SDIO systems so a software reboot leaves
	 * the MT7668 Wi-Fi PMU rail powered down before the next kernel probes
	 * the combo chip.  Bluetooth stays under the normal bt-dev GPIO path; the
	 * FireCube/CE21 device tree relies on the Wi-Fi and BT functions sharing
	 * the vendor combo-chip initialization sequence.
	 */
	if (is_firecube && sdio_en) {
		if (!register_reboot_notifier(&firecube_mt7668_reboot_nb)) {
			firecube_mt7668_resident = true;
			pr_info("wifi_dummy: registered MT7668 warm reboot reset notifier\n");
			return 0;
		}

		pr_warn("wifi_dummy: failed to register MT7668 warm reboot reset notifier\n");
	}

	return -ENODEV;
}

static void __exit wifi_dummy_cleanup(void)
{
	if (firecube_mt7668_resident)
		unregister_reboot_notifier(&firecube_mt7668_reboot_nb);
}

module_init(wifi_dummy_init);
module_exit(wifi_dummy_cleanup);

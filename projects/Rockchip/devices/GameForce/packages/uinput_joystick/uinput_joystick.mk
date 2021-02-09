################################################################################
#
# uinput_joystick
#
################################################################################

UINPUT_JOYSTICK_LICENSE_FILES = NOTICE
UINPUT_JOYSTICK_LICENSE = Apache V2.0

define UINPUT_JOYSTICK_BUILD_CMDS
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) \
		package/rockchip/uinput_joystick/ts_uart.c -o $(@D)/uinput_joystick \
		package/rockchip/uinput_joystick/uinput-ctl.c -o $(@D)/uinput_joystick \
		package/rockchip/uinput_joystick/common.c -o $(@D)/uinput_joystick
endef

define UINPUT_JOYSTICK_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 755 $(@D)/uinput_joystick $(TARGET_DIR)/usr/bin/uinput_joystick
	$(INSTALL) -D -m 755 package/rockchip/uinput_joystick/S60load_uinput_joystick $(TARGET_DIR)/etc/init.d/
endef

$(eval $(generic-package))

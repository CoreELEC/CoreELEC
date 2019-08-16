#!/bin/sh

# Configure ADVMAME P1 gamepad based on js0
CONFIG_DIR="/storage/.advance"
CONFIG=${CONFIG_DIR}/advmame.rc
FOUND=0
EE_DEV="js0"

clean_pad() {
	sed -i '/input_map\[coin1\].*/d' ${CONFIG}
	sed -i '/input_map\[start1\].*/d' ${CONFIG}
	sed -i '/input_map\[ui_cancel\].*/d' ${CONFIG}
	sed -i '/input_map\[ui_configure\].*/d' ${CONFIG}
	 }

set_pad(){
	clean_pad
	echo "input_map[coin1] joystick_button[${VENDOR}_${PRODUCT},select]" >> ${CONFIG}
	echo "input_map[start1] joystick_button[${VENDOR}_${PRODUCT},start]" >> ${CONFIG}
	echo "input_map[ui_cancel] joystick_button[${VENDOR}_${PRODUCT},thumbl] or joystick_button[${VENDOR}_${PRODUCT},mode]" >> ${CONFIG}
	echo "input_map[ui_configure] joystick_button[${VENDOR}_${PRODUCT},thumbr]" >> ${CONFIG}
	}

for file in /tmp/joypads/*.cfg; do
	file2=$(basename "${file}")
	EE_GAMEPAD="${file2%.*}"

if cat /proc/bus/input/devices | grep -E -A 4 -B 1 "${EE_GAMEPAD}" | grep ${EE_DEV} > /dev/null; then
	FOUND=1
	VENDOR=$(cat /proc/bus/input/devices | grep -E -A 4 -B 1 "${EE_GAMEPAD}" | grep Vendor | cut -d = -f 3 | cut -d " " -f 1)
	PRODUCT=$(cat /proc/bus/input/devices | grep -E -A 4 -B 1 "${EE_GAMEPAD}" | grep Vendor | cut -d = -f 4 | cut -d " " -f 1)
	break
fi
done

set_pad

# none of this is needed, it gave weird results, but left just in case
##if [ ${FOUND} = 1 ]; then
#	# Update advmame.rc with the corresponding keys
#	EE_GAMEPAD="/tmp/joypads/${file2}"
#	KEY1=$(cat "${EE_GAMEPAD}" | grep -E 'input_select_btn' | cut -d '"' -f2)
#	KEY2=$(cat "${EE_GAMEPAD}" | grep -E 'input_start_btn' | cut -d '"' -f2)
#	KEY3=$(cat "${EE_GAMEPAD}" | grep -E 'input_r3_btn' | cut -d '"' -f2)
#	KEY4=$(cat "${EE_GAMEPAD}" | grep -E 'input_enable_hotkey_btn' | cut -d '"' -f2)
#	set_pad ${KEY1} ${KEY2} ${KEY3} ${KEY4}
	
#	DEVICE=$(cat "${EE_GAMEPAD}" | grep -E 'input_device' | cut -d '"' -f2)
#	echo "Advmame configured to use ${EE_GAMEPAD} Dev: $EE_DEV"
#else
#	echo "Dev: $EE_DEV not connected or it hasn't been configured in ES"
#fi

#!/bin/bash

OGA=$(cat /proc/device-tree/compatible)

case $1 in
   pre)
    # unload esp8090 WiFi module
[[ "${OGA}" == *"gameforce"* ]] && modprobe -r rtl8723ds 
    # Store sound state. Try to avoid having max volume after resume
    alsactl store -f /tmp/asound.state
	# workaround until dwc2 is fixed
	modprobe -r dwc2
    # stop hotkey service
    systemctl stop odroidgoa-headphones.service
    ;;
   post)
    # Restore pre-sleep sound state
    alsactl restore -f /tmp/asound.state
    # workaround until dwc2 is fixed
	modprobe -r dwc2
	modprobe -i dwc2
	# re-load WiFi module
[[ "${OGA}" == *"gameforce"* ]] && modprobe rtl8723ds 
    # re-detect and reapply sound, brightness and hp state
    systemctl start odroidgoa-headphones.service
	;;
esac

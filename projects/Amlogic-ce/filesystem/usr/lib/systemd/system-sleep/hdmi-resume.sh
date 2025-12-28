#!/bin/sh
# Force HDMI HPD on resume to fix black screen on S905X4
case "$1" in
  post)
    if [ -f /sys/class/amhdmitx/amhdmitx0/hpd_state ]; then
        echo "Force HDMI HPD on resume..."
        echo 1 > /sys/class/amhdmitx/amhdmitx0/hpd_state
    fi
    ;;
esac

#!/bin/sh
# Force HDMI HPD on resume to fix black screen on S905X4
case "$1" in
  post)
    if [ -f /sys/class/amhdmitx/amhdmitx0/hpd_state ]; then
        echo "Force HDMI HPD on resume..."
        echo 1 > /sys/class/amhdmitx/amhdmitx0/hpd_state
        
        # Wait a moment for handshake to potentially settle (optional but recommended for caps reading, though we are setting policy regardless)
        sleep 1

        # HDR10+ Optimization
        # Ensure HDR10+ processing is enabled
        if [ -f /sys/class/amvecm/enable_hdr10plus ]; then
            echo "Enabling HDR10+ support..."
            echo 1 > /sys/class/amvecm/enable_hdr10plus
        fi

        # Set Dolby Vision HDR10 Policy to 1 (Passthrough/Dynamic Metadata)
        if [ -f /sys/module/aml_media/parameters/dolby_vision_hdr10_policy ]; then
            echo "Setting HDR10+ Policy to 1..."
            echo 1 > /sys/module/aml_media/parameters/dolby_vision_hdr10_policy
        fi
    fi
    ;;
esac

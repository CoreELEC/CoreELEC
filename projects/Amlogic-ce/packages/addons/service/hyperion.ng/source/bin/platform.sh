#!/bin/sh


if [[ -d /sys/module/amvdec_h265 && -d /sys/module/amvdec_vp9 ]];then
    case "$1" in
    start)
        echo 3 > /sys/module/amvdec_h265/parameters/double_write_mode
        echo 3 > /sys/module/amvdec_vp9/parameters/double_write_mode
    ;;
    stop)
        echo 0 > /sys/module/amvdec_h265/parameters/double_write_mode
        echo 0 > /sys/module/amvdec_vp9/parameters/double_write_mode
    ;;
    esac
fi

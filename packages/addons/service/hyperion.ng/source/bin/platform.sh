#!/bin/sh

if [ -f /sys/module/amvdec_h265/parameters/double_write_mode ];then
    case "$1" in
    start)
        echo 3 > /sys/module/amvdec_h265/parameters/double_write_mode
    ;;
    stop)
        echo 0 > /sys/module/amvdec_h265/parameters/double_write_mode
    ;;
    esac
fi

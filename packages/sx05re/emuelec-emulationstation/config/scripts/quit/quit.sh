#!/bin/bash

. /etc/profile

case ${1} in
 "reboot")
    small-cores enable
    systemctl reboot 
 ;;
 "shutdown")
    small-cores enable
    systemctl poweroff
 ;;
esac

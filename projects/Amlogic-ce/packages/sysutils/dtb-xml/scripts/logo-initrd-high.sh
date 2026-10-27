#!/bin/sh
#
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2026-present Team CoreELEC (https://coreelec.org)
#
#  Set the logo reserved memory to the u-boot initrd_high address
#
#####################################################

BOOT_ROOT='/flash'
xml_file="$BOOT_ROOT/dtb.xml"
default_xml_file="/usr/share/bootloader/dtb.xml"
dtb_file="$BOOT_ROOT/dtb.img"
amlogic_dt_id=''
[ -f /proc/device-tree/amlogic-dt-id ] && amlogic_dt_id="-a $(tr -d '\0' < /proc/device-tree/amlogic-dt-id)"

# u-boot sets initrd_high to the logo address it draws at, cfgload or boot.ini pass it as logo_addr
initrd_high=''
for arg in $(cat /proc/cmdline); do
  case "$arg" in
    logo_addr=*) initrd_high="${arg#logo_addr=}" ;;
  esac
done
if [ -z "$initrd_high" ]; then
  echo "no logo_addr in kernel cmdline"
  exit 0
fi

# 0xffffffff is the u-boot default for 'no limit', not a logo address
logo_addr="$initrd_high"
initrd_high="${initrd_high#0[xX]}"
case "$initrd_high" in
  ''|*[!0-9a-fA-F]*) initrd_high='' ;;
esac
[ "${#initrd_high}" -gt 8 ] && initrd_high=''
# same format as 'fdtget -t x': lower case hex, no 0x, no leading zeros
[ -n "$initrd_high" ] && initrd_high=$(printf '%x' "0x$initrd_high")
if [ -z "$initrd_high" -o "$initrd_high" = "0" -o "$initrd_high" = "ffffffff" ] || \
   [ $((0x$initrd_high & 0xfff)) -ne 0 ]; then
  echo "invalid logo_addr: '$logo_addr'"
  exit 1
fi

act_value=$(fdtget $amlogic_dt_id -t x $dtb_file /reserved-memory/linux,meson-fb reg 2>/dev/null)
if [ -z "$act_value" ]; then
  echo "no /reserved-memory/linux,meson-fb reg in $dtb_file"
  exit 0
fi

set -- $act_value
status=$(xmlstarlet sel -t -v "//logo_addr/@status" $xml_file 2>/dev/null)
if [ "$2" = "$initrd_high" -a "$status" != "initrd_high" ]; then
  echo "logo reg already at initrd_high: $initrd_high"
  exit 0
fi

if ! xmlstarlet sel -t -c "//logo_addr/initrd_high/cmd" $default_xml_file >/dev/null; then
  echo "no logo_addr node in $default_xml_file"
  exit 1
fi

# dtb-xml -e runs first and creates a missing dtb.xml, update.sh adds new nodes
if ! xmlstarlet sel -t -c "//logo_addr" $xml_file >/dev/null 2>&1; then
  echo "no logo_addr node in $xml_file"
  exit 1
fi

new_status="initrd_high"
new_value="$initrd_high"
# reg is in place, release the node so another u-boot address is detected again
if [ "$2" = "$initrd_high" ]; then
  new_status="migrated"
  new_value='${initrd_high}'
fi

grep -q " $BOOT_ROOT vfat ro," /proc/mounts
rw="$?"
[ "$rw" = "0" ] && mount -o rw,remount "$BOOT_ROOT"

# take the cmd from the default dtb.xml, so a changed initrd_high replaces an older value
cmd_path=$(xmlstarlet sel -t -v "//logo_addr/initrd_high/cmd/@path" $default_xml_file)
xmlstarlet ed -L -u "//logo_addr/initrd_high/cmd/@path" -v "$(echo "$cmd_path" | sed "s/\${initrd_high}/$new_value/")" $xml_file
cnt=1
for value in $(xmlstarlet sel -t -m "//logo_addr/initrd_high/cmd/value" -v "." -n $default_xml_file); do
  xmlstarlet ed -L -u "//logo_addr/initrd_high/cmd/value[$cnt]" -v "$(echo "$value" | sed "s/\${initrd_high}/$new_value/")" $xml_file
  cnt=$((cnt + 1))
done
xmlstarlet ed -L -u "//logo_addr/@status" -v "$new_status" $xml_file
xmlstarlet ed -L -d "//logo_addr/logo_addr_migrated" $xml_file

[ "$rw" = "0" ] && mount -o ro,remount "$BOOT_ROOT"

echo "logo reg $2 -> $initrd_high, dtb.xml status set to '$new_status'"

if [ "$new_status" = "initrd_high" ]; then
  /usr/lib/coreelec/dtb-xml
fi

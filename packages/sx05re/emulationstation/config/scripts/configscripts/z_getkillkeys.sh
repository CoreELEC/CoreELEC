#!/bin/sh


# IMPORTANT: js0 is always used as default, but you can change this in ee_kill.cfg

EE_CFG="/emuelec/configs/ee_kill.cfg"
EE_DEV="$(cat $EE_CFG |grep EE_KILLDEV |awk -F= '{print $2}' | tr -d \")"

for file in /tmp/joypads/*.cfg; do
file2=$(basename "$file")
EE_GAMEPAD="${file2//[ ()@$,]/_}"
EE_GAMEPAD="${EE_GAMEPAD%.*}"

if ls -la /dev/input/by-id/ | grep $EE_GAMEPAD | grep $EE_DEV > /dev/null; then
sed -i "s/EE_GAMEPAD=".*"/EE_GAMEPAD=\"${file2}\"/g" /emuelec/configs/ee_kill.cfg
	break
fi

done

# Update ee_kill.cfg with the corresponding keys
EE_GAMEPAD="/tmp/joypads/$file2"
KEY1=$(cat "$EE_GAMEPAD" | grep -E 'hotkey_btn' | cut -d '"' -f2)
KEY2=$(cat "$EE_GAMEPAD" | grep -E 'input_exit_emulator_btn' | cut -d '"' -f2)
KEY1=$(echo $((16#130 + 10#$KEY1)))
KEY2=$(echo $((16#130 + 10#$KEY2)))
EE_KILLKEYS="$KEY1+$KEY2"
sed -i "s/EE_KILLKEYS=".*"/EE_KILLKEYS=\"${EE_KILLKEYS}\"/g" $EE_CFG

DEVICE=$(cat "$EE_GAMEPAD" | grep -E 'input_device' | cut -d '"' -f2)

echo "Kill combo set to $EE_KILLKEYS from $EE_GAMEPAD Dev: $EE_DEV"

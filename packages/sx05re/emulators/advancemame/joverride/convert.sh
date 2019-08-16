#!/bin/bash

array_contains () { 
    local array="$1[@]"
    local seeking=$2
    local in=1
    for element in "${!array}"; do
        if [[ $element == $seeking ]]; then
            in=0
            break
        fi
    done
    return $in
}


if [ -f joverride.dat ]; then 
	rm joverride.dat
fi 

# these give an error on advmame
declare -a BLACKLIST=('Mocute_Bluetooth_Remote' 'ACRUX_QuanBa_Arcade_JoyStick_1008' 'Qnix_SNES_Replica' 'raphnet.net_GC_N64_to_USB_Adapter_2_3')

for filename in ${1}/*.cfg; do
fbname=$(basename "$filename" .cfg)
array_contains BLACKLIST "$fbname" && echo "skipping $filename" || python joverride.py "$filename" >> joverride.dat
done

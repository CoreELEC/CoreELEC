#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_console enable
python /usr/bin/scripts/scriptmodules/supplementary/bluetoothcontroller.py ES 2>/dev/null
ee_console disable

#!/bin/bash

# Source predefined functions and variables
. /etc/profile

ee_console enable
python /emuelec/scriptmodules/supplementary/bluetoothcontroller.py ES 2>/dev/null
ee_console disable

#!/bin/bash

# Original here: https://github.com/hooby3dfx/lakka-addons/blob/master/btautopair.sh
# Slightly modified by Shanti Gilbert for EmuELEC 2019

#script to run on system boot that enables bluetooth, and pairs any available game controllers
#that are available during the ~1-2 mins that the script runs for.

#requires empty (http://empty.sourceforge.net/)
EMPTYBIN='/usr/bin/empty'
#log events to a file (helpful when script is run in background; use tail -f to see whats going on)
LOGPTH='/storage/.config/emuelec/logs/bt_helper.log'

#string utils that work on busybox...
stringContain() { [ -z "${2##*$1*}" ]; }
strindex() { 
  x="${1%%$2*}"
  [ "$x" = "$1" ] && echo -1 || echo ${#x}
}

echo "script started" > $LOGPTH

sleep 2

#systemctl status bluetooth

#make sure bluetooth is up and running.
touch /storage/.cache/services/bluez.conf
systemctl enable bluetooth
systemctl start bluetooth

echo "bluetooth up and running" >> $LOGPTH

#start the bluetooth interactive console
$EMPTYBIN -f bluetoothctl
sleep 2

echo "bluetooth setting up" >> $LOGPTH

$EMPTYBIN -s "agent on\n"
$EMPTYBIN -w "Agent registered"
sleep 1

$EMPTYBIN -s "default-agent\n"
$EMPTYBIN -w "Default agent request successful"
sleep 1

$EMPTYBIN -s "power on\n"
$EMPTYBIN -w "Changing power on succeeded"
sleep 1

$EMPTYBIN -s "discoverable on\n"
$EMPTYBIN -w "Changing discoverable on succeeded"
sleep 1

$EMPTYBIN -s "pairable on\n"
$EMPTYBIN -w "Changing pairable on succeeded"
sleep 1

$EMPTYBIN -s "scan on\n"
$EMPTYBIN -w "Discovery started"

echo "bluetooth scanning" >> $LOGPTH

#look for a bunch of messages... might want to tweak this
for i in `seq 1 40`
do
	echo "waiting for event $i..." >> $LOGPTH
	#look for a NEW, waiting up to 10s
	ONELINE="$($EMPTYBIN -r -t 10)"
	echo "saw $ONELINE" >> $LOGPTH
	if [ -z "$ONELINE" ]; then
		#echo "empty string"
		#hack
		ONELINE="cheese"
	fi

	if stringContain "NEW" "$ONELINE" && stringContain "Device " "$ONELINE"; then
		echo "processing device" >> $LOGPTH
		#capture MAC
		MAC_INDEX=$(strindex "$ONELINE" "Device")
		#the MAC address is offset 7 chars from the word Device...
		MAC_INDEX=$((MAC_INDEX + 7))
		#mac is 17 chars long
  		MACADDR=${ONELINE:$MAC_INDEX:17}

		#for any new MAC, request info
		echo "requesting info for $MACADDR" >> $LOGPTH
		$EMPTYBIN -s "info $MACADDR\n"

		#listen for "Icon: input-gaming"
		$EMPTYBIN -w "Icon: input-" -t 5
		ERRORCODE=$?
		echo "ERRORCODE $ERRORCODE" >> $LOGPTH

		if [ "$ERRORCODE" -eq 1 ]; then
			
			#dont ask - workaround/hack for first command sent
			$EMPTYBIN -s "poopies\n"
			$EMPTYBIN -w "Invalid command"

			#pair
			echo "going to pair" >> $LOGPTH
			$EMPTYBIN -s "pair $MACADDR\n"
			#eat some lines (workaround/hack)
			ONELINE="$($EMPTYBIN -r -t 10)"
			echo "saw $ONELINE" >> $LOGPTH
			ONELINE="$($EMPTYBIN -r -t 10)"
			echo "saw $ONELINE" >> $LOGPTH
			ONELINE="$($EMPTYBIN -r -t 10)"
			echo "saw $ONELINE" >> $LOGPTH
			ONELINE="$($EMPTYBIN -r -t 10)"
			echo "saw $ONELINE" >> $LOGPTH
			#actually wait for success
			$EMPTYBIN -w "Pairing successful"

			#connect
			echo "going to connect" >> $LOGPTH
			$EMPTYBIN -s "connect $MACADDR\n"
			$EMPTYBIN -w "Connection successful"

			#trust
			echo "going to trust" >> $LOGPTH
			$EMPTYBIN -s "trust $MACADDR\n"
			sleep 2

		else
			echo "not a gamepad" >> $LOGPTH
		fi

	else
		echo "not a new device event" >> $LOGPTH
	fi
done

#exit! 
echo "exiting" >> $LOGPTH
$EMPTYBIN -s "exit\n"
sleep 5
killall bluetoothctl

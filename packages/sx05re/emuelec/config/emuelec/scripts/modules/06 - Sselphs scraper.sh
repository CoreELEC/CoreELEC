#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

ee_console enable

function scrape_confirm() {
    echo -ne "This will Kill Emulationstation and will start Sselph's Scraper, do you want to continue?\n\nYou will need a keyboard to be able to use the scraping menu" > /tmp/display
	text_viewer -y -t "Sselph's Scraper" -f 24 /tmp/display
    [[ $? == 21 ]] && start_scraper || exit 0;
 }

function start_scraper() {
ee_console enable
systemd-run bash /emuelec/scripts/modules/scraper.start
systemctl stop emustation
}

scrape_confirm
ee_console disable

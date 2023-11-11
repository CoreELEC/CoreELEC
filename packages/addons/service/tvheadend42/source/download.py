# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-present Team LibreELEC (https://libreelec.tv)

import os
import shutil
import urllib.request
import subprocess
import re
import xbmc, xbmcvfs, xbmcgui, xbmcaddon

ADDON_NAME = xbmcaddon.Addon().getAddonInfo("name")
LS = xbmcaddon.Addon().getLocalizedString


def clear_directory(directory):
    try:
        for file_name in os.listdir(directory):
            file_path = os.path.join(directory, file_name)
            if os.path.isfile(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                shutil.rmtree(file_path)
    except Exception as e:
        xbmcgui.Dialog().notification(ADDON_NAME, LS(30041), xbmcgui.NOTIFICATION_INFO)
        exit(1)


def download_and_extract(url, destination, extract_path):
    try:
        # Download the file
        urllib.request.urlretrieve(url, destination)

        # Extract the file to the specified directory, ignoring the root path
        subprocess.run(["tar", "xf", destination, "--strip-components=3", "-C", extract_path])

    except Exception as e:
        xbmcgui.Dialog().notification(ADDON_NAME, LS(30040), xbmcgui.NOTIFICATION_INFO)
        exit(1)


if __name__ == "__main__":
    scan_tables_path = os.path.join(xbmcvfs.translatePath(xbmcaddon.Addon().getAddonInfo("path")), "dvb-scan")
    download_url = "https://linuxtv.org/downloads/dtv-scan-tables/dtv-scan-tables-LATEST.tar.bz2"
    downloaded_file_path = "/tmp/dtv-scan-tables-LATEST.tar.bz2"

    # Clear the contents of the dvb_scan directory
    clear_directory(scan_tables_path)

    # Download and extract the file using subprocess
    download_and_extract(download_url, downloaded_file_path, scan_tables_path)

    # Clean up the downloaded file
    os.remove(downloaded_file_path)

    # Notify download complete
    xbmcgui.Dialog().notification(ADDON_NAME, LS(30039), xbmcgui.NOTIFICATION_INFO)

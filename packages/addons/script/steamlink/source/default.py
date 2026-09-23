# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2019-present Team LibreELEC (https://libreelec.tv)

import os
import shutil
import sys
import tarfile
import subprocess
from contextlib import contextmanager
from hashlib import sha256
from pathlib import Path
from tempfile import TemporaryDirectory
from time import sleep
from urllib.request import urlopen

import xbmcaddon
import xbmcgui


STEAMLINK_VERSION = "@STEAMLINK_VERSION@"
STEAMLINK_HASH = "@STEAMLINK_HASH@"
STEAMLINK_TARBALL_NAME = f"steamlink-rpi-trixie-arm64-{STEAMLINK_VERSION}.tar.gz"
STEAMLINK_URL = f"http://media.steampowered.com/steamlink/rpi/trixie/arm64/{STEAMLINK_TARBALL_NAME}"
ADDON_DIR = xbmcaddon.Addon().getAddonInfo("path").rstrip("/")


# Utility functions

def execute(cmd):
    """Run a command and return stdout. Raise on failure."""
    result = subprocess.run(
            cmd,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            shell=False
        )
    return result.stdout.decode()


def sha256_file(file_path):
    """Compute SHA256 hash of a file in 32KB segments."""
    hasher = sha256()
    buf = bytearray(32768)
    view = memoryview(buf)
    with open(file_path, "rb") as f:
        readinto = f.readinto
        while True:
            segment = readinto(buf)
            if not segment:
                break
            hasher.update(view[:segment])
    return hasher.hexdigest()


def read_file(file):
    """ Read everything in file """
    with open(file) as data:
        return data.read()


# UI functions

def notify(title, message, level=xbmcgui.NOTIFICATION_INFO, duration=5000):
    """Display one screen message."""
    xbmcgui.Dialog().notification(title, message, level, duration)


@contextmanager
def progress(title, message):
    bar = xbmcgui.DialogProgress()
    bar.create(title, message)
    try:
        yield bar
    finally:
        bar.close()


# Download functions

def download_file(url, dest, expected_hash):
    """Download a file with progress and verify its hash."""
    if len(expected_hash) != 64:
        notify("Steam Link", "Invalid SHA256 hash format", xbmcgui.NOTIFICATION_ERROR)
        return False

    file_name = url.rsplit("/", 1)[1]
    with progress("File Download", f"Downloading {file_name}...") as bar:
        buf = bytearray(32768)
        view = memoryview(buf)
        last_pct = -1

        def progress_hook(downloaded, total_size):
            nonlocal last_pct
            if total_size <= 0:
                bar.update(0, "Filesize unknown")
                return
            pct = int(downloaded * 100 / total_size)
            # Update progress bar every 5%
            band = pct // 5
            if band != last_pct:
                last_pct = band
                bar.update(pct)
        try:
            with urlopen(url) as response, open(dest,"wb") as out:
                total_header = response.getheader("Content-Length")
                total = int(total_header) if total_header is not None else -1
                downloaded = 0
                readinto = response.readinto
                write = out.write

                while True:
                    segment = readinto(buf)
                    if not segment:
                        break
                    write(view[:segment])
                    downloaded += segment
                    progress_hook(downloaded, total)
        except Exception:
            notify("Steam Link", "Download failed", xbmcgui.NOTIFICATION_ERROR)
            return False

        # Verify hash
        actual_hash = sha256_file(dest)
        if actual_hash != expected_hash:
            if os.path.isfile(dest):
                os.remove(dest)
            notify("Steam Link", "Downloaded file failed integrity check", xbmcgui.NOTIFICATION_ERROR)
            return False

        return True


# Steam Link preparation functions

def get_install_state():
    # prepare_steamlink() has not run
    if not os.path.isfile(f"{ADDON_DIR}/prep.ok"):
        return "missing"
    # Steam Link has not been unpacked
    if not os.path.isfile(f"{ADDON_DIR}/steamlink/version.txt"):
        return "missing"

    # Get installed package's version
    try:
        installed = read_file(f"{ADDON_DIR}/steamlink/version.txt").rstrip()
    except Exception:
        return "corrupt"

    # Check if installed version matches addon expectations
    if installed != STEAMLINK_VERSION:
        return "outdated"

    return "current"


def prepare_steamlink():
    """ System preparation before launching Steam Link """
    # Add system libraries to bundled
    for file in os.listdir(f"{ADDON_DIR}/system-libs/"):
        os.symlink(f"{ADDON_DIR}/system-libs/{file}", f"{ADDON_DIR}/steamlink/lib/{file}")

    # Disable arch test
    Path(f"{ADDON_DIR}/steamlink/.ignore_arch").touch()

    # Finalize
    Path(f"{ADDON_DIR}/prep.ok").touch()


# Main

def start_steamlink():
    state = get_install_state()

    if state in ("missing", "corrupt", "outdated"):
        shutil.rmtree(f"{ADDON_DIR}/steamlink/", ignore_errors=True)
        if os.path.exists(f"{ADDON_DIR}/prep.ok"):
            os.remove(f"{ADDON_DIR}/prep.ok")

        # Download Steam Link package
        with TemporaryDirectory() as temp_dir:
            steamlink_tarball_path = os.path.join(temp_dir, STEAMLINK_TARBALL_NAME)
            # Steam Link
            download_file(STEAMLINK_URL, steamlink_tarball_path, STEAMLINK_HASH)
            if os.path.isfile(steamlink_tarball_path):
                with tarfile.open(steamlink_tarball_path) as steamlink_tarball:
                    steamlink_tarball.extractall(path=f"{ADDON_DIR}/", filter="data")

        prepare_steamlink()

    # Start Steam Link
    notify("Steam Link", "Starting Steam Link", duration=3000)
    # XXX: Assumes audio will use ALSA
    try:
        execute([
            "systemd-run",
            "--setenv=PULSE_SERVER=none",
            "--setenv=SDL_AUDIODRIVER=alsa",
            f"{ADDON_DIR}/bin/steamlink-start.sh"
        ])
    except subprocess.CalledProcessError as e:
        notify("Steam Link", "Failed to start Steam Link")


start_steamlink()

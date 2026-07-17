#!/usr/bin/python3
# -*- coding: utf-8 -*-

# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2023-present Team LibreELEC (https://libreelec.tv)

# Rewritten to use Microsoft's official release-metadata JSON API instead of
# scraping the "thank-you" marketing download page. The old approach broke
# because that page is not a stable interface: it can 404 or omit entries for
# a release with no warning. The releases.json feed below is the same source
# used by dotnet-install.sh and is versioned per major.minor channel, so its
# structure doesn't shift under us.
#
# Additional python3 modules required (ubuntu 26.04)
#sudo apt update; sudo apt install -y python3-requests
import sys
import requests
import hashlib

HEADERS = {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) LibreELEC-update-pkg'}

# rid suffix -> LibreELEC ARCH name
RID_TO_ARCH = {
    'linux-arm64': 'aarch64',
    'linux-arm':   'arm',
    'linux-x64':   'x86_64',
}

def pkgheader(pkgname, version):
    print('# SPDX-License-Identifier: GPL-2.0-only')
    print('# Copyright (C) 2022-present Team LibreELEC (https://libreelec.tv)')
    print('')
    print('PKG_NAME="'+pkgname+'"')
    print('PKG_VERSION="'+version+'"')
    print('PKG_LICENSE="MIT"')
    print('PKG_SITE="https://dotnet.microsoft.com/"')
    print('PKG_DEPENDS_TARGET="toolchain"')
    print('PKG_LONGDESC="ASP.NET Core Runtime enables you to run existing web/server applications."')
    print('PKG_TOOLCHAIN="manual"')
    print('')
    print('case "${ARCH}" in')

def pkgfooter():
    print('esac')
    print('PKG_SOURCE_NAME="aspnetcore-runtime_${PKG_VERSION}_${ARCH}.tar.gz"')

def fetch_release_entry(channel, version):
    """Fetch releases.json for the given channel (e.g. '8.0') and return the
    release object whose release-version matches, or None."""
    url = f'https://builds.dotnet.microsoft.com/dotnet/release-metadata/{channel}/releases.json'
    resp = requests.get(url, headers=HEADERS, timeout=30)
    resp.raise_for_status()
    data = resp.json()
    for release in data.get('releases', []):
        if release.get('release-version') == version:
            return release
    return None

def printEntries(release, version):
    """Emit PKG_SHA256/PKG_URL blocks for each linux RID found in the
    aspnetcore-runtime files list. Returns True if all three expected
    RIDs (arm, arm64, x64) were found and emitted."""
    component = release.get('aspnetcore-runtime')
    if not component:
        print(f"ERROR: release {version} has no aspnetcore-runtime component in releases.json", file=sys.stderr)
        return False

    files_by_rid = {}
    for f in component.get('files', []):
        rid = f.get('rid')
        name = f.get('name', '')
        if rid not in RID_TO_ARCH or not name.endswith('.tar.gz'):
            continue
        # releases.json lists both the plain framework-dependent tarball
        # (aspnetcore-runtime-{version}-linux-{rid}.tar.gz) and a separate
        # "composite" self-contained build (aspnetcore-runtime-composite-...)
        # per rid. We want the plain one only - skip composite variants.
        if 'composite' in name:
            continue
        if rid in files_by_rid:
            print(f"WARNING: multiple non-composite tar.gz matches for rid {rid}, "
                  f"keeping first ({files_by_rid[rid]['name']}), ignoring {name}", file=sys.stderr)
            continue
        files_by_rid[rid] = f

    ok = True
    for rid, le_arch in RID_TO_ARCH.items():
        f = files_by_rid.get(rid)
        if not f:
            print(f"ERROR: no {rid} tar.gz entry found for aspnetcore-runtime {version}", file=sys.stderr)
            ok = False
            continue
        download_url = f['url']
        try:
            bin_resp = requests.get(download_url, headers=HEADERS, timeout=60)
            bin_resp.raise_for_status()
        except Exception as err:
            print(f"[{version} {rid}] download failed: {err}", file=sys.stderr)
            ok = False
            continue
        hashed_string = hashlib.sha256(bin_resp.content).hexdigest()
        print ('  "'+le_arch+'")')
        print ('    PKG_SHA256="'+hashed_string+'"')
        print ('    PKG_URL="'+download_url+'"')
        print ('    ;;')
    return ok

if __name__== "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} PKG_VERSION", file=sys.stderr)
        sys.exit(1)

    version = sys.argv[1]
    release_parts = version.split(".")
    channel = f"{release_parts[0]}.{release_parts[1]}"
    pkgname = 'aspnet'+release_parts[0]+'-runtime'

    try:
        release = fetch_release_entry(channel, version)
    except Exception as err:
        print(f"ERROR: failed to fetch/parse releases.json for channel {channel}: {err}", file=sys.stderr)
        sys.exit(1)

    if release is None:
        print(f"ERROR: version {version} not found in channel {channel} releases.json "
              f"(it may not be published yet, or the version string is wrong)", file=sys.stderr)
        sys.exit(1)

    pkgheader(pkgname, version)
    ok = printEntries(release, version)
    pkgfooter()

    if not ok:
        sys.exit(1)

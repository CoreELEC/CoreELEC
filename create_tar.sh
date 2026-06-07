#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2011 Stephan Raue (stephan@openelec.tv)
# Copyright (C) 2017-present Team LibreELEC (https://libreelec.tv)

git archive --format=tar --prefix=LibreELEC-source-$1/ tags/$1 | bzip2 > LibreELEC-source-$1.tar.bz2

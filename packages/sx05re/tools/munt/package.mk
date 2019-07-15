# SPDX-License-Identifier: GPL-2.0
# Copyright (C) 0riginally created by Escalade (https://github.com/escalade)
# Copyright (C) 2018-present 5schatten (https://github.com/5schatten)

PKG_NAME="munt"
PKG_VERSION="939cc98"
PKG_SHA256="fb6577e09bd373875464c54ea64f572c97b642da00c84a9d47af9db2c00d1dba"
PKG_LICENSE="GPL"
PKG_SITE="https://github.com/munt/munt"
PKG_URL="https://github.com/munt/munt/archive/$PKG_VERSION.tar.gz"
PKG_DEPENDS_TARGET="toolchain"
PKG_LONGDESC="A multi-platform software synthesiser emulating (currently inaccurately) pre-GM MIDI devices such as the Roland MT-32, CM-32L, CM-64 and LAPC-I. In no way endorsed by or affiliated with Roland Corp."

PKG_CMAKE_OPTS_TARGET="-Dmunt_WITH_MT32EMU_QT=0 \
                       -Dmunt_WITH_MT32EMU_SMF2WAV=0 \
                       -Dlibmt32emu_SHARED=1"

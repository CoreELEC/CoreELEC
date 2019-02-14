# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2018-present Team CoreELEC (https://coreelec.tv)

case $1 in
   @PROJECT@|S905*|S912*)
       exit 0
    ;;
   *)
       exit 1
    ;;
esac

# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.tv)

case $1 in
   @PROJECT@|Amlogic*)
       exit 0
    ;;
   *)
       exit 1
    ;;
esac

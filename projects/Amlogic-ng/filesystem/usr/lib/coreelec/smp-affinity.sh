#!/bin/sh

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2019-present Team CoreELEC (https://coreelec.org)

echo 2 > /proc/irq/70/smp_affinity
echo 4 > /proc/irq/22/smp_affinity
echo 8 > /proc/irq/41/smp_affinity
echo 10 > /proc/irq/21/smp_affinity

exit 0

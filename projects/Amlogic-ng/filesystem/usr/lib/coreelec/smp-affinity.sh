#!/bin/sh
#
# DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
#                    Version 2, December 2004
#
# Copyright (C) 2004 Sam Hocevar
#  14 rue de Plaisance, 75014 Paris, France
# Everyone is permitted to copy and distribute verbatim or modified
# copies of this license document, and changing it is allowed as long
# as the name is changed.
#
#            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
#   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
#
#  0. You just DO WHAT THE FUCK YOU WANT TO.

echo 2 > /proc/irq/70/smp_affinity
echo 4 > /proc/irq/22/smp_affinity
echo 8 > /proc/irq/41/smp_affinity
echo 10 > /proc/irq/21/smp_affinity

exit 0

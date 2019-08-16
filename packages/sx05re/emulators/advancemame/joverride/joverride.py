#
# This file is part of the Advance project.
#
# Copyright (C) 2018 Andrea Mazzoleni
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# In addition, as a special exception, Andrea Mazzoleni
# gives permission to link the code of this program with
# the MAME library (or with modified versions of MAME that use the
# same license as MAME), and distribute linked combinations including
# the two.  You must obey the GNU General Public License in all
# respects for all of the code used other than MAME.  If you modify
# this file, you may extend this exception to your version of the
# file, but you are not obligated to do so.  If you do not wish to
# do so, delete this exception statement from your version.
#

#
# Convert files from libretro udev mapping:
#
# https://github.com/libretro/retroarch-joypad-autoconfig/tree/master/udev
#

import sys

def entry(amap, desc, vendor, product, key, v0, v1):
	if not amap.has_key(key):
		return
	value = amap[key]
	print "\t\t{ 0x" + format(vendor, '04x') + ", 0x" + format(product, '04x') + ", \"" + desc + "\", " + str(value) + ", \"" + v0 + "\", " + v1 + " },"

def process(file):
	with open(file) as f:
		lines = f.readlines()

	amap = dict()

	for s in lines:
		if len(s) == 0 or s[0] == '#':
			continue
		t = s.split("=")
		if len(t) != 2:
			continue
		t0 = t[0].strip()
		t1 = t[1].strip().strip("\"")
		amap[t0] = t1

	if not amap.has_key("input_vendor_id"):
		return
	if not amap.has_key("input_product_id"):
		return
	if not amap.has_key("input_device"):
		return

	vendor = amap["input_vendor_id"]
	product = amap["input_product_id"]
	desc = amap["input_device"]

	print "\t\t/* " + desc + " */"

	if vendor.isdigit() and product.isdigit():
		ivendor = int(vendor)
		iproduct = int(product)
	else:
		ivendor = int(vendor, 16)
		iproduct = int(product, 16)

	entry(amap, desc, ivendor, iproduct, "input_a_btn", "a", "JOYB_A")
	entry(amap, desc, ivendor, iproduct, "input_b_btn", "b", "JOYB_B")
	entry(amap, desc, ivendor, iproduct, "input_c_btn", "c", "JOYB_C")
	entry(amap, desc, ivendor, iproduct, "input_x_btn", "x", "JOYB_X")
	entry(amap, desc, ivendor, iproduct, "input_y_btn", "y", "JOYB_Y")
	entry(amap, desc, ivendor, iproduct, "input_z_btn", "z", "JOYB_Z")
	entry(amap, desc, ivendor, iproduct, "input_l_btn", "tl", "JOYB_TL")
	entry(amap, desc, ivendor, iproduct, "input_r_btn", "tr", "JOYB_TR")
	entry(amap, desc, ivendor, iproduct, "input_l2_btn", "tl2", "JOYB_TL2")
	entry(amap, desc, ivendor, iproduct, "input_r2_btn", "tr2", "JOYB_TR2")
	entry(amap, desc, ivendor, iproduct, "input_select_btn", "select", "JOYB_SELECT")
	entry(amap, desc, ivendor, iproduct, "input_start_btn", "start", "JOYB_START")
	entry(amap, desc, ivendor, iproduct, "input_menu_toggle_btn", "mode", "JOYB_MODE")
	entry(amap, desc, ivendor, iproduct, "input_l3_btn","thumbl","JOYB_THUMBL")
	entry(amap, desc, ivendor, iproduct, "input_r3_btn","thumbr","JOYB_THUMBR") 

for file in sys.argv[1:]:
	process(file)



# Allwinner Linux patches

Flat patch directory, one kernel tree for all Allwinner devices. Patches are
grouped into topic ranges with gaps, so new topics (or a future SoC family)
can be inserted without renumbering everything else.

| Range | Topic |
|---|---|
| 0001-0099 | Generic/LE-local: wakeup (axp20x-pek, rtc, SCPI, protected clocks), Bluetooth, mmc, USB3 board enables, GPU OPP includes |
| 0100-0199 | DRM sun4i/dw-hdmi fixes and refactors |
| 0200-0399 | Display engine stack, driver code only (DE33 core, scaler/mixer, register shadows/RCQ, writeback, AFBC, deep colour/YUV420) |
| 0400-0499 | Media: cedrus/VE/IOMMU/VP9/P010/AFBC, VC-1, H6 deinterlace, DI300, hantro g2 |
| 0500-0599 | Audio: HDMI card + IEC958/HBR, R40 topology, AC200 codec + simple-card, AHUB |
| 0600-0699 | AC200 mfd/EPHY/codec, dwmac-sun8i PHY regulators, OPi3 / OPi3 LTS DTS |
| 0700-0799 | H616 SoC + board DTS (display, VE, DI, writeback, EMAC1, AHUB, HDMI audio) |
| 0800-0899 | reserved: Wi-Fi/BT drivers (separate series) |
| 0900-0999 | HACK/WIP that must apply last |
| 1000-1999 | reserved: A523/A527/T527 |
| 2000-2999 | reserved: A733 |

Only functional patches are carried here. The dt-bindings and Documentation
changes that come with the upstream commits are dropped, since they have no
effect on the built kernel; `include/dt-bindings/` headers are kept, because
the DTS files include them.

## Source-of-truth tree

The patches in the 0001-0799 ranges (excluding locally-authored/re-derived
ones noted in individual commit messages) are generated from an integration
branch `le-7.2` in a separate clone of the kernel tree, based on
`v7.2.3` with material cherry-picked from `jernejsk/linux-1`
(`https://github.com/jernejsk/linux-1.git`) topic branches. `git format-patch`
of a contiguous range of that branch reproduces the corresponding patch
group here; `git am` of a patch group reproduces that range.

Regenerate a range with:

```
git format-patch --start-number <N> <old>..<new> -o projects/Allwinner/patches/linux/
```

After regenerating, refresh both architectures:

```
PROJECT=Allwinner DEVICE=H6 ARCH=aarch64 tools/refresh-patches linux
PROJECT=Allwinner DEVICE=H3 ARCH=arm tools/refresh-patches linux
```

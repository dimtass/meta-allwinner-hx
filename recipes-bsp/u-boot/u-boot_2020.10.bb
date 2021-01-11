DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-allwinner.inc

UBOOT_VERSION = "2020.10"

SRCREV = "050acee119b3757fee3bd128f55d720fdd9bb890"
PV = "v${UBOOT_VERSION}+git${SRCPV}"
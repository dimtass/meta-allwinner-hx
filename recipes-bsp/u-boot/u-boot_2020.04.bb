DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-allwinner.inc

UBOOT_VERSION = "2020.04"

SRCREV = "36fec02b1f90b92cf51ec531564f9284eae27ab4"
PV = "v${UBOOT_VERSION}+git${SRCPV}"
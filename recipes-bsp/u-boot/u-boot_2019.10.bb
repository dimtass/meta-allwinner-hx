DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-allwinner.inc

UBOOT_VERSION = "2019.10"

SRCREV = "61ba1244b548463dbfb3c5285b6b22e7c772c5bd"
PV = "v${UBOOT_VERSION}+git${SRCPV}"
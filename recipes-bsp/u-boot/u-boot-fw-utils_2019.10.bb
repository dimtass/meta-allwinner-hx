DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-fw-utils.inc

UBOOT_VERSION = "2019.10"

LIC_FILES_CHKSUM = "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

SRCREV = "61ba1244b548463dbfb3c5285b6b22e7c772c5bd"
PV = "v${UBOOT_VERSION}+git${SRCPV}"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_${LINUX_VERSION}:${THISDIR}/../../scripts:"
DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-fw-utils.inc

UBOOT_VERSION = "2019.04"

LIC_FILES_CHKSUM = "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

SRCREV = "3c99166441bf3ea325af2da83cfe65430b49c066"
PV = "v${UBOOT_VERSION}+git${SRCPV}"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_${LINUX_VERSION}:${THISDIR}/../../scripts:"
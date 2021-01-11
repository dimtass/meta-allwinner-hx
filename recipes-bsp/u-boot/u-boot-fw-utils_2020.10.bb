DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-fw-utils.inc

UBOOT_VERSION = "2020.10"

LIC_FILES_CHKSUM = "file://Licenses/README;md5=5a7450c57ffe5ae63fd732446b988025"

EXTRA_OEMAKE_class-target = 'CROSS_COMPILE=${TARGET_PREFIX} CC="${CC} ${CFLAGS} ${LDFLAGS}" HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}" V=1'
EXTRA_OEMAKE_class-cross = 'HOSTCC="${CC} ${CFLAGS} ${LDFLAGS}" V=1'

SRCREV = "050acee119b3757fee3bd128f55d720fdd9bb890"
PV = "v${UBOOT_VERSION}+git${SRCPV}"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:${THISDIR}/../../scripts:"
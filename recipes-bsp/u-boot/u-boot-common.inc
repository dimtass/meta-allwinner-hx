DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
LICENSE = "GPLv2+"

require recipes-bsp/u-boot/u-boot.inc

FILESEXTRAPATHS_prepend := "${THISDIR}/files:${THISDIR}/../../scripts:"

DEPENDS += "bc-native dtc-native swig-native python3-native bison-native flex-native"
DEPENDS_append_sun50iw2 = " atf-arm "
DEPENDS_append_sun50iw6 = " atf-arm "

do_compile_sun50iw2[depends] += "atf-arm:do_deploy"
do_compile_sun50iw6[depends] += "atf-arm:do_deploy"

COMPATIBLE_MACHINE = "(sun8i|sun50iw2|sun50iw6)"

DEFAULT_PREFERENCE = "1"

EXTRA_OEMAKE += ' HOSTLDSHARED="${BUILD_CC} -shared ${BUILD_LDFLAGS} ${BUILD_CFLAGS}" '
EXTRA_OEMAKE_append_sun50iw2 = " BL31=${DEPLOY_DIR_IMAGE}/bl31.bin "
EXTRA_OEMAKE_append_sun50iw6 = " BL31=${DEPLOY_DIR_IMAGE}/bl31.bin "

SRC_URI = " \
        git://gitlab.denx.de/u-boot.git;branch=master \
        file://armbian-patcher.sh \
        file://patches-${SOC_FAMILY}-${UBOOT_VERSION} \
    "

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"
do_configure[cleandirs] = "${B}"
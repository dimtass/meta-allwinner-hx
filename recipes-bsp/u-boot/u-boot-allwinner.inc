DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require recipes-bsp/u-boot/u-boot.inc
DEPENDS += " bc-native dtc-native swig-native python3-native "
DEPENDS_append_sun50i = " atf-arm "

LICENSE = "GPLv2+"

COMPATIBLE_MACHINE = "(sun8i|sun50i)"

DEFAULT_PREFERENCE_sun8i="1"
DEFAULT_PREFERENCE_sun50i="1"

SRC_URI = "git://git.denx.de/u-boot.git;branch=master \
            file://${SOC_FAMILY}-boot/boot.cmd \
            file://${SOC_FAMILY}-boot/fixup.cmd \
            file://do_patch.sh \
            file://allwinnerEnv.txt \
"

UBOOT_ENV_SUFFIX = "scr"
UBOOT_ENV = "boot"
UBOOT_FIXUP_BINARY = "fixup.scr"

EXTRA_OEMAKE += ' HOSTLDSHARED="${BUILD_CC} -shared ${BUILD_LDFLAGS} ${BUILD_CFLAGS}" '
EXTRA_OEMAKE_append_sun50i = " BL31=${DEPLOY_DIR_IMAGE}/bl31.bin "

do_compile_sun50i[depends] += "atf-arm:do_deploy"

S = "${WORKDIR}/git"

DESCRIPTION="Upstream's U-boot configured for sunxi devices"

require recipes-bsp/u-boot/u-boot.inc

DEPENDS += " bc-native dtc-native swig-native python3-native "
DEPENDS_append_sun50i = " atf-sunxi "

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

COMPATIBLE_MACHINE = "(sun8i|sun50i)"

DEFAULT_PREFERENCE_sun8i="1"
DEFAULT_PREFERENCE_sun50i="1"

SRC_URI = "git://git.denx.de/u-boot.git;branch=master \
            file://${SOC_FAMILY}-boot/boot.cmd \
            file://${SOC_FAMILY}-boot/fixup.cmd \
            file://do_patch.sh \
            file://patches-2018.11 \
            file://allwinnerEnv.txt \
"

SRCREV = "0157013f4a4945bbdb70bb4d98d680e0845fd784"

PV = "v2018.11+git${SRCPV}"
PE = "2"

S = "${WORKDIR}/git"

UBOOT_ENV_SUFFIX = "scr"
UBOOT_ENV = "boot"
UBOOT_FIXUP_BINARY = "fixup.scr"

EXTRA_OEMAKE += ' HOSTLDSHARED="${BUILD_CC} -shared ${BUILD_LDFLAGS} ${BUILD_CFLAGS}" '
EXTRA_OEMAKE_append_sun50i = " BL31=${DEPLOY_DIR_IMAGE}/bl31.bin "

do_compile_sun50i[depends] += "atf-sunxi:do_deploy"

do_compile_append() {
    cp ${WORKDIR}/${SOC_FAMILY}-boot/boot.cmd ${WORKDIR}/boot.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/boot.cmd ${WORKDIR}/${UBOOT_ENV_BINARY}

    cp ${WORKDIR}/${SOC_FAMILY}-boot/fixup.cmd ${WORKDIR}/fixup.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/fixup.cmd ${WORKDIR}/${UBOOT_FIXUP_BINARY}
}

do_configure_prepend() {
    cd ${S}
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-2018.11
}

do_deploy_append() {
    # Copy also the fixup script to the deploy dir
    install -m 644 ${WORKDIR}/${UBOOT_FIXUP_BINARY} ${DEPLOYDIR}/${UBOOT_FIXUP_BINARY}

    # Add the soc specific parameters in the environment
    echo "overlay_prefix=${OVERLAY_PREFIX}" >> ${WORKDIR}/allwinnerEnv.txt
    echo "overlays=${DEFAULT_OVERLAYS}" >> ${WORKDIR}/allwinnerEnv.txt
    install -m 644 ${WORKDIR}/allwinnerEnv.txt ${DEPLOYDIR}/allwinnerEnv.txt
}
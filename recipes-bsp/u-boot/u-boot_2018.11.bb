DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require recipes-bsp/u-boot/u-boot.inc
require u-boot-allwinner.inc

<<<<<<< HEAD
DEPENDS += " bc-native dtc-native swig-native python3-native "
DEPENDS_append_sun50i = " atf-arm "

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
            file://fw_env.config \
"
=======
LIC_FILES_CHKSUM = "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"
>>>>>>> c6d877f244c9bee1f34d24c50b9e535995c51f0c

SRCREV = "0157013f4a4945bbdb70bb4d98d680e0845fd784"
PV = "v2018.11+git${SRCPV}"

<<<<<<< HEAD
INSANE_SKIP_${PN} = "already-stripped"
EXTRA_OEMAKE += '${LDFLAGS} HOSTLDSHARED="${BUILD_CC} -shared ${BUILD_LDFLAGS} ${BUILD_CFLAGS}" '
EXTRA_OEMAKE_class-target = 'CROSS_COMPILE=${TARGET_PREFIX} CC="${CC} ${CFLAGS} ${LDFLAGS}" HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}" V=1'
EXTRA_OEMAKE_append_sun50i = " BL31=${DEPLOY_DIR_IMAGE}/bl31.bin "
=======
SRC_URI += " \
            file://patches-2018.11 \
"
>>>>>>> c6d877f244c9bee1f34d24c50b9e535995c51f0c

do_configure_prepend() {
    cd ${S}
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-2018.11
}

do_compile_append() {
	oe_runmake envtools

    cp ${WORKDIR}/${SOC_FAMILY}-boot/boot.cmd ${WORKDIR}/boot.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/boot.cmd ${WORKDIR}/${UBOOT_ENV_BINARY}

    cp ${WORKDIR}/${SOC_FAMILY}-boot/fixup.cmd ${WORKDIR}/fixup.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/fixup.cmd ${WORKDIR}/${UBOOT_FIXUP_BINARY}
}

<<<<<<< HEAD
do_install_append () {
	install -d ${D}${base_sbindir}
	install -d ${D}${sysconfdir}
	install -m 755 ${B}/tools/env/fw_printenv ${D}${base_sbindir}/fw_printenv
	install -m 755 ${B}/tools/env/fw_printenv ${D}${base_sbindir}/fw_setenv
	install -m 0644 ${WORKDIR}/fw_env.config ${D}${sysconfdir}/fw_env.config
}

do_configure_prepend() {
    cd ${S}
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-2018.11
}

=======
>>>>>>> c6d877f244c9bee1f34d24c50b9e535995c51f0c
do_deploy_append() {
    # Copy also the fixup script to the deploy dir
    install -m 644 ${WORKDIR}/${UBOOT_FIXUP_BINARY} ${DEPLOYDIR}/${UBOOT_FIXUP_BINARY}

    # Add the soc specific parameters in the environment
    echo "overlay_prefix=${OVERLAY_PREFIX}" >> ${WORKDIR}/allwinnerEnv.txt
    echo "overlays=${DEFAULT_OVERLAYS}" >> ${WORKDIR}/allwinnerEnv.txt
    install -m 644 ${WORKDIR}/allwinnerEnv.txt ${DEPLOYDIR}/allwinnerEnv.txt
}

FILES_${PN} += " \
    ${base_sbindir}/fw_printenv \
    ${base_sbindir}/fw_setenv \
    ${sysconfdir}/fw_env.config \
"
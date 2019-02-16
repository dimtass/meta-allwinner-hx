DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-allwinner.inc

LIC_FILES_CHKSUM = "file://Licenses/README;md5=a2c678cfd4a4d97135585cad908541c6"

SRC_URI += "file://patches-2018.05 \
            file://fw_env.config \
"

SRCREV = "890e79f2b1c26c5ba1a86d179706348aec7feef7"
PV = "v2018.05+git${SRCPV}"

INSANE_SKIP_${PN} = "already-stripped"
EXTRA_OEMAKE_class-target = 'CROSS_COMPILE=${TARGET_PREFIX} CC="${CC} ${CFLAGS} ${LDFLAGS}" HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}" V=1'

do_configure_prepend() {
    cd ${S}
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-2018.05
}

do_compile_append() {
	oe_runmake envtools

    cp ${WORKDIR}/${SOC_FAMILY}-boot/boot.cmd ${WORKDIR}/boot.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/boot.cmd ${WORKDIR}/${UBOOT_ENV_BINARY}

    cp ${WORKDIR}/${SOC_FAMILY}-boot/fixup.cmd ${WORKDIR}/fixup.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/fixup.cmd ${WORKDIR}/${UBOOT_FIXUP_BINARY}
}

do_install_append () {
	install -d ${D}${base_sbindir}
	install -d ${D}${sysconfdir}
	install -m 755 ${B}/tools/env/fw_printenv ${D}${base_sbindir}/fw_printenv
	install -m 755 ${B}/tools/env/fw_printenv ${D}${base_sbindir}/fw_setenv
	install -m 0644 ${WORKDIR}/fw_env.config ${D}${sysconfdir}/fw_env.config
}

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
DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require recipes-bsp/u-boot/u-boot.inc
require u-boot-allwinner.inc

SRCREV = "890e79f2b1c26c5ba1a86d179706348aec7feef7"
PV = "v2018.05+git${SRCPV}"

SRC_URI += " \
            file://patches-2018.05 \
"

do_configure_prepend() {
    cd ${S}
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-2018.05
}

do_compile_append() {
    cp ${WORKDIR}/${SOC_FAMILY}-boot/boot.cmd ${WORKDIR}/boot.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/boot.cmd ${WORKDIR}/${UBOOT_ENV_BINARY}

    cp ${WORKDIR}/${SOC_FAMILY}-boot/fixup.cmd ${WORKDIR}/fixup.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/fixup.cmd ${WORKDIR}/${UBOOT_FIXUP_BINARY}
}

do_deploy_append() {
    # Copy also the fixup script to the deploy dir
    install -m 644 ${WORKDIR}/${UBOOT_FIXUP_BINARY} ${DEPLOYDIR}/${UBOOT_FIXUP_BINARY}

    # Add the soc specific parameters in the environment
    echo "overlay_prefix=${OVERLAY_PREFIX}" >> ${WORKDIR}/allwinnerEnv.txt
    echo "overlays=${DEFAULT_OVERLAYS}" >> ${WORKDIR}/allwinnerEnv.txt
    install -m 644 ${WORKDIR}/allwinnerEnv.txt ${DEPLOYDIR}/allwinnerEnv.txt
}
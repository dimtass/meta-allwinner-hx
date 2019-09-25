DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-allwinner.inc

LIC_FILES_CHKSUM = "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

SRC_URI += "file://patches-2019.04 \
"

SRCREV = "3c99166441bf3ea325af2da83cfe65430b49c066"
PV = "v2019.04+git${SRCPV}"

do_configure() {
    cd ${S}
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-2019.04
    oe_runmake -C ${S} O=${B} ${UBOOT_MACHINE}
}

do_compile_append() {

    if [ ! -f "${DEPLOY_DIR_IMAGE}/bl31.bin" ]; then
        bbwarn "Could not find ${DEPLOY_DIR_IMAGE}/bl31.bin. You need to build the atf-arm package first"
    fi

    cp ${WORKDIR}/${SOC_FAMILY}-boot/boot.cmd ${WORKDIR}/boot.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/boot.cmd ${WORKDIR}/${UBOOT_ENV_BINARY}

    cp ${WORKDIR}/${SOC_FAMILY}-boot/fixup.cmd ${WORKDIR}/fixup.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/fixup.cmd ${WORKDIR}/${UBOOT_FIXUP_BINARY}

    # Add the soc specific parameters in the environment
    echo "overlay_prefix=${OVERLAY_PREFIX}" >> ${WORKDIR}/allwinnerEnv.txt
    echo "overlays=${DEFAULT_OVERLAYS}" >> ${WORKDIR}/allwinnerEnv.txt
}

do_install_append() {
    # Install files to rootfs/boot/
    install -m 644 ${WORKDIR}/${UBOOT_FIXUP_BINARY} ${D}/boot/${UBOOT_FIXUP_BINARY}
    install -m 644 ${WORKDIR}/allwinnerEnv.txt ${D}/boot/allwinnerEnv.txt

    # Fix broken device tree reference build into u-boot
    for dtb in ${KERNEL_DEVICETREE}; do
        dtb_base_name=`basename $dtb`
        dtb_dir_name=`dirname $dtb`
        install -d ${D}/boot/$dtb_dir_name
        ln -rsf ${D}/boot/$dtb_base_name ${D}/boot/$dtb
    done

    # Cleanup u-boot rootfs files
    rm -rf ${D}/boot/${SPL_BINARYNAME} ${D}/boot/${SPL_IMAGE} ${D}/boot/${UBOOT_BINARY} ${D}/boot/${UBOOT_IMAGE}
}

do_deploy_append() {
    # Copy also the fixup script to the deploy dir
    install -m 644 ${WORKDIR}/${UBOOT_FIXUP_BINARY} ${DEPLOYDIR}/${UBOOT_FIXUP_BINARY}

    install -m 644 ${WORKDIR}/allwinnerEnv.txt ${DEPLOYDIR}/allwinnerEnv.txt
}

DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-common.inc

LIC_FILES_CHKSUM ?= "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

SRC_URI += " \
            file://${SOC_FAMILY}-boot/boot.cmd \
            file://allwinnerEnv.txt \
"

do_configure() {
    set -x
    cd ${S}
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/patches-${SOC_FAMILY}-${UBOOT_VERSION}
    oe_runmake -C ${S} ${UBOOT_CONFIG} O=${B}
}

do_compile() {
    oe_runmake -C ${S} ${PARALLEL_MAKE} O=${B}

    if [ "${SOC_FAMILY}" == "sun50iw2" -o "${SOC_FAMILY}" == "sun50iw6" ] && [ ! -f "${DEPLOY_DIR_IMAGE}/bl31.bin" ]; then
        bberror "Could not find ${DEPLOY_DIR_IMAGE}/bl31.bin. You need to build the atf-arm package first"
    fi

    cp ${WORKDIR}/${SOC_FAMILY}-boot/boot.cmd ${WORKDIR}/boot.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/boot.cmd ${WORKDIR}/boot.scr

    # Add the soc specific parameters in the environment
    sed -e "s,overlay_prefix=,overlay_prefix=${OVERLAY_PREFIX},g" \
        -i ${WORKDIR}/allwinnerEnv.txt
    sed -e "s,overlays=,overlays=${DEFAULT_OVERLAYS} ,g" \
        -i ${WORKDIR}/allwinnerEnv.txt
}

do_install() {
    # Install files to rootfs/boot/
    install -D -m 644 ${WORKDIR}/allwinnerEnv.txt ${D}/boot/allwinnerEnv.txt

    # Fix broken device tree reference build into u-boot
    for dtb in ${KERNEL_DEVICETREE}; do
        dtb_base_name=`basename $dtb`
        dtb_dir_name=`dirname $dtb`
        if [ "${dtb_dir_name}" != "." ]; then
            install -d ${D}/boot/$dtb_dir_name
            ln -rsf ${D}/boot/$dtb_base_name ${D}/boot/$dtb
        fi
    done

    # Cleanup u-boot rootfs files
    rm -rf ${D}/boot/${SPL_BINARYNAME} ${D}/boot/${SPL_IMAGE} ${D}/boot/${UBOOT_BINARY} ${D}/boot/${UBOOT_IMAGE}
}

do_deploy() {
    if [ -n "${UBOOT_BINARY}" ]; then
        install -D -m 644 ${B}/${UBOOT_BINARY} ${DEPLOYDIR}/${UBOOT_IMAGE}
        rm -f ${DEPLOYDIR}/${UBOOT_BINARY} ${DEPLOYDIR}/${UBOOT_SYMLINK}
        ln -sf ${UBOOT_IMAGE} ${DEPLOYDIR}/${UBOOT_SYMLINK}
        ln -sf ${UBOOT_IMAGE} ${DEPLOYDIR}/${UBOOT_BINARY}
    fi

    if [ -n "${SPL_BINARY}" ]; then
        install -D -m 644 ${B}/${config}/${SPL_BINARY} ${DEPLOYDIR}/${SPL_IMAGE}-${type}-${PV}-${PR}
        rm -f ${DEPLOYDIR}/${SPL_BINARYNAME} ${DEPLOYDIR}/${SPL_SYMLINK}-${type}
        ln -sf ${SPL_IMAGE}-${type}-${PV}-${PR} ${DEPLOYDIR}/${SPL_BINARYNAME}-${type}
        ln -sf ${SPL_IMAGE}-${type}-${PV}-${PR} ${DEPLOYDIR}/${SPL_BINARYNAME}
        ln -sf ${SPL_IMAGE}-${type}-${PV}-${PR} ${DEPLOYDIR}/${SPL_SYMLINK}-${type}
        ln -sf ${SPL_IMAGE}-${type}-${PV}-${PR} ${DEPLOYDIR}/${SPL_SYMLINK}
    fi
    install -D -m 644 ${WORKDIR}/allwinnerEnv.txt ${DEPLOYDIR}/
    install -D -m 644 ${WORKDIR}/boot.scr ${DEPLOYDIR}/
}

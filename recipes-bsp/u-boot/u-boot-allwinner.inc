DESCRIPTION="Upstream's U-boot configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require recipes-bsp/u-boot/u-boot.inc

DEPENDS += "bc-native dtc-native swig-native python3-native bison-native flex-native"
DEPENDS_append_sun50i = " atf-arm "

LICENSE = "GPLv2+"

LIC_FILES_CHKSUM ?= "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

COMPATIBLE_MACHINE = "(sun8i|sun50i)"

DEFAULT_PREFERENCE_sun8i="1"
DEFAULT_PREFERENCE_sun50i="1"
SRC_URI = "git://gitlab.denx.de/u-boot.git;branch=master \
            file://${SOC_FAMILY}-boot/boot.cmd \
            file://${SOC_FAMILY}-boot/fixup.cmd \
            file://patches-${UBOOT_VERSION} \
            file://armbian-patcher.sh \
            file://allwinnerEnv.txt \
"

UBOOT_ENV_SUFFIX = "scr"
UBOOT_ENV = "boot"
UBOOT_FIXUP_BINARY = "fixup.scr"

EXTRA_OEMAKE += ' HOSTLDSHARED="${BUILD_CC} -shared ${BUILD_LDFLAGS} ${BUILD_CFLAGS}" '
EXTRA_OEMAKE_append_sun50i = " BL31=${DEPLOY_DIR_IMAGE}/bl31.bin "

do_compile_sun50i[depends] += "atf-arm:do_deploy"

S = "${WORKDIR}/git"

do_configure() {
    cd ${S}
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/patches-${UBOOT_VERSION}
    oe_runmake -C ${S} O=${B} ${UBOOT_MACHINE}
}

do_compile_append() {

    if [ ! -f "${DEPLOY_DIR_IMAGE}/bl31.bin" && "${SOC_FAMILY}" == "sun50i"]; then
        bberror "Could not find ${DEPLOY_DIR_IMAGE}/bl31.bin. You need to build the atf-arm package first"
    fi

    cp ${WORKDIR}/${SOC_FAMILY}-boot/boot.cmd ${WORKDIR}/boot.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/boot.cmd ${WORKDIR}/${UBOOT_ENV_BINARY}

    cp ${WORKDIR}/${SOC_FAMILY}-boot/fixup.cmd ${WORKDIR}/fixup.cmd
    ${B}/tools/mkimage -C none -A arm -T script -d ${WORKDIR}/fixup.cmd ${WORKDIR}/${UBOOT_FIXUP_BINARY}

    # Add the soc specific parameters in the environment
    sed -e "s,overlay_prefix=,overlay_prefix=${OVERLAY_PREFIX},g" \
        -i ${WORKDIR}/allwinnerEnv.txt
    sed -e "s,overlays=,overlays=${DEFAULT_OVERLAYS} ,g" \
        -i ${WORKDIR}/allwinnerEnv.txt
    # echo "overlay_prefix=${OVERLAY_PREFIX}" >> ${WORKDIR}/allwinnerEnv.txt
    # echo "overlays=${DEFAULT_OVERLAYS}" >> ${WORKDIR}/allwinnerEnv.txt
}

do_install_append() {
    # Install files to rootfs/boot/
    install -m 644 ${WORKDIR}/${UBOOT_FIXUP_BINARY} ${D}/boot/${UBOOT_FIXUP_BINARY}
    install -m 644 ${WORKDIR}/allwinnerEnv.txt ${D}/boot/allwinnerEnv.txt

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

do_deploy_append() {
    # Copy also the fixup script to the deploy dir
    install -m 644 ${WORKDIR}/${UBOOT_FIXUP_BINARY} ${DEPLOYDIR}/${UBOOT_FIXUP_BINARY}

    install -m 644 ${WORKDIR}/allwinnerEnv.txt ${DEPLOYDIR}/allwinnerEnv.txt
}

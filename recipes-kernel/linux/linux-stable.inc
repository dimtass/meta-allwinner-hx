AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
SECTION = "kernel"
LICENSE = "GPLv2"
COMPATIBLE_MACHINE = "(sun8i|sun50i)"

LIC_FILES_CHKSUM ?= "file://COPYING;md5=bbea815ee2795b2f4230826c0c6b8814"

# Pull in the devicetree files and u-boot config files into the rootfs
RDEPENDS_${KERNEL_PACKAGE_NAME}-base += "kernel-devicetree u-boot"

# We need mkimage for the overlays
DEPENDS += "u-boot-tools-native"
do_compile[depends] += "u-boot-tools-native:do_populate_sysroot"

inherit deploy
require recipes-kernel/linux/linux-yocto.inc

LINUX_VERSION ?= ""
PREEMPT_RT_VERSION ?= ""

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

S = "${WORKDIR}/git"

# Choose the correct path for the defconfig
DEFCONFIG_PATH =  "${@'${SOC_FAMILY}-defconfig' if d.getVar('PREEMPT_RT_VERSION') == '' else '${SOC_FAMILY}-rt-defconfig'}"

SRC_URI = " \
        file://armbian-patcher.sh \
        file://patches-${LINUX_VERSION} \
        file://custom-patches-${LINUX_VERSION} \
        file://${DEFCONFIG_PATH}/defconfig \
"

# Apply the armbian patches and defconfig
do_patch_append() {
    cp ${WORKDIR}/${DEFCONFIG_PATH}/defconfig ${WORKDIR}/defconfig
    cd ${WORKDIR}/git
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/patches-${LINUX_VERSION}
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/custom-patches-${LINUX_VERSION}
}

# Compile overlays. This is for compatibility seince from version 4.20
# and later individual dtbo build is not supported
do_compile_append() {
    set -x
    bbnote "Compiling kernel overlays"
    oe_runmake -C ${B} CC="${KERNEL_CC}" ${PARALLEL_MAKE} dtbs
}

# deploy the dtbo overlays in the DEPLOYDIR
do_deploy_append() {
    set -x
    dtbos=$(find . -name "*.dtbo" | grep ${OVERLAY_PREFIX})
    for dtbo in $dtbos; do
        install -m 644 $dtbo ${DEPLOYDIR}/
    done
}

python() {
    if not d.getVar('SOC_FAMILY'):
        bb.fatal("SOC_FAMILY is not set! Please set it in your machine configuration.")
    else:
        bb.note("%s/defconfig will be used for the kernel." % (d.getVar('DEFCONFIG_PATH')))
}
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
SECTION = "kernel"
LICENSE = "GPLv2"
COMPATIBLE_MACHINE = "(sun8i|sun50i)"

require recipes-kernel/linux/linux-yocto.inc

LIC_FILES_CHKSUM = "file://COPYING;md5=bbea815ee2795b2f4230826c0c6b8814"

# Pull in the devicetree files into the rootfs
RDEPENDS_${KERNEL_PACKAGE_NAME}-base += "kernel-devicetree"

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

LINUX_VERSION = "4.19"
LINUX_VERSION_EXTENSION = "-allwinner"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_4.19:"

S = "${WORKDIR}/git"

PV = "4.19.12"
SRCREV = "79bf89b88a87f2ebf147f76d8c40183283b49b51"

SRC_URI = " \
        git://github.com/megous/linux.git;branch=orange-pi-${LINUX_VERSION} \
        file://do_patch.sh \
        file://patches-4.19-rt \
        file://patch-4.19.15-rt12 \
        file://enable_uart0_on_linux_boot.cfg \
        file://${SOC_FAMILY}-rt-defconfig/defconfig \
"

do_patch_append() {
    bbinfo "Will use ${SOC_FAMILY}-rt-defconfig for the kernel"
    cp ${WORKDIR}/${SOC_FAMILY}-rt-defconfig/defconfig ${WORKDIR}/defconfig
    cd ${WORKDIR}/git
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-4.19-rt
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patch-4.19.15-rt12
}

python() {
    if not d.getVar('SOC_FAMILY'):
        bb.fatal("You need to set 'SOC_FAMILY' in your local.conf file to 'sunxi' or 'sunxi64' depending your board.")
    else:
        bb.note("%s-defconfig/defconfig will be used for the kernel." % (d.getVar('SOC_FAMILY')))
}
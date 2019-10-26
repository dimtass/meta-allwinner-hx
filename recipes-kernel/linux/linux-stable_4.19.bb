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

PV = "4.19.80"
SRCREV = "c3038e718a19fc596f7b1baba0f83d5146dc7784"

SRC_URI = " \
        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
        file://do_patch.sh \
        file://patches-4.19 \
        file://enable_uart0_on_linux_boot.cfg \
        file://${SOC_FAMILY}-defconfig/defconfig \
"

do_patch_append() {
    cp ${WORKDIR}/${SOC_FAMILY}-defconfig/defconfig ${WORKDIR}/defconfig
    cd ${WORKDIR}/git
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-4.19
}

python() {
    if not d.getVar('SOC_FAMILY'):
        bb.fatal("You need to set 'SOC_FAMILY' in your local.conf file to 'sunxi' or 'sunxi64' depending your board.")
    else:
        bb.note("%s-defconfig/defconfig will be used for the kernel." % (d.getVar('SOC_FAMILY')))
}
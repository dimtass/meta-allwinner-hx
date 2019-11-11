AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
SECTION = "kernel"
LICENSE = "GPLv2"
COMPATIBLE_MACHINE = "(sun8i|sun50i)"

inherit kernel
require recipes-kernel/linux/linux-yocto.inc

LIC_FILES_CHKSUM = "file://COPYING;md5=bbea815ee2795b2f4230826c0c6b8814"

# Pull in the devicetree files and u-boot config files into the rootfs
RDEPENDS_${KERNEL_PACKAGE_NAME}-base += "kernel-devicetree u-boot"

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

LINUX_VERSION = "4.19"
LINUX_VERSION_EXTENSION = "-allwinner"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_4.19:"

S = "${WORKDIR}/git"

PV = "4.19.81"
SRCREV = "ef244c3088856cf048c77231653b4c92a7b2213c"

SRC_URI = " \
        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
        file://do_patch.sh \
        file://patches-${LINUX_VERSION} \
        file://custom-patches-${LINUX_VERSION} \
        file://${SOC_FAMILY}-defconfig/defconfig \
"

# Apply the armbian patches and defconfig
do_patch_prepend() {
    cp ${WORKDIR}/${SOC_FAMILY}-defconfig/defconfig ${WORKDIR}/defconfig
    cd ${WORKDIR}/git
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-${LINUX_VERSION}
}

python() {
    if not d.getVar('SOC_FAMILY'):
        bb.fatal("You need to set 'SOC_FAMILY' in your local.conf file to 'sunxi' or 'sunxi64' depending your board.")
    else:
        bb.note("%s-defconfig/defconfig will be used for the kernel." % (d.getVar('SOC_FAMILY')))
}

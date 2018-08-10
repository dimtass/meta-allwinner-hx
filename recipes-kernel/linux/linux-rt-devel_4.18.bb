AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>
SECTION = "kernel"
DESCRIPTION = "Standard LTS kernel 4.14 for RK3399"
LICENSE = "GPLv2"

require recipes-kernel/linux/linux-yocto.inc 

COMPATIBLE_MACHINE = "(sun8i|sun50i)""

RDEPENDS_kernel-base += "kernel-devicetree"

KERNEL_IMAGETYPE = "Image"

LINUX_VERSION = "4.18"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-rt-devel_${LINUX_VERSION}:"

S = "${WORKDIR}/git"

PV = "4.18.rc8-rt1"
SRCREV = "ce25bc4adeb52cb55d996bfef8820ac07ef91ac6"
SRC_URI = " \
    git://git.kernel.org/pub/scm/linux/kernel/git/rt/linux-rt-devel.git;branch=linux-${LINUX_VERSION}.y-rt \
        file://do_patch.sh \
        file://patches \
        file://${ARMBIAN_DEFCONFIG}-defconfig/defconfig \
"

do_patch_append() {
    cp ${WORKDIR}/${ARMBIAN_DEFCONFIG}-defconfig/defconfig ${WORKDIR}/defconfig
    cd ${WORKDIR}/git
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches
}

python() {
    if not d.getVar('ARMBIAN_DEFCONFIG'):
        bb.fatal("You need to set 'ARMBIAN_DEFCONFIG' in your local.conf file to 'sunxi' or 'sunxi64' depending your board.")
    else:
        bb.note("%s-defconfig/defconfig will be used for the kernel." % (d.getVar('ARMBIAN_DEFCONFIG')))
}

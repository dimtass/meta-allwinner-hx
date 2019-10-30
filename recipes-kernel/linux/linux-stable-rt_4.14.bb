AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
SECTION = "kernel"
LICENSE = "GPLv2"
COMPATIBLE_MACHINE = "(sun8i|sun50i)"

require recipes-kernel/linux/linux-yocto.inc

LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

# Pull in the devicetree files and u-boot config files into the rootfs
RDEPENDS_${KERNEL_PACKAGE_NAME}-base += "kernel-devicetree u-boot"

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

LINUX_VERSION = "4.14"
LINUX_VERSION_EXTENSION = "-allwinner"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-stable-rt_4.14:"

S = "${WORKDIR}/git"

PV = "4.14.78"
SRCREV = "e7405910ca5553eae8744af4e5c03e64ee048cb1"

SRC_URI = " \
        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
        file://do_patch.sh \
        file://patches \
        file://patch-4.14.78-rt47 \
        file://${SOC_FAMILY}-rt-defconfig/defconfig \
"

do_patch_append() {
    bbinfo "Will use ${SOC_FAMILY}-rt-defconfig for the kernel"
    cp ${WORKDIR}/${SOC_FAMILY}-rt-defconfig/defconfig ${WORKDIR}/defconfig
    cd ${WORKDIR}/git
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patch-4.14.78-rt47
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches
}

python() {
    if not d.getVar('SOC_FAMILY'):
        bb.fatal("You need to set 'SOC_FAMILY' in your local.conf file to 'sunxi' or 'sunxi64' depending your board.")
    else:
        bb.note("%s-defconfig/defconfig will be used for the kernel." % (d.getVar('SOC_FAMILY')))
}

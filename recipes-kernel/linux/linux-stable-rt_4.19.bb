AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
SECTION = "kernel"
LICENSE = "GPLv2"
COMPATIBLE_MACHINE = "(sun8i|sun50i)"

require recipes-kernel/linux/linux-yocto.inc

LIC_FILES_CHKSUM = "file://COPYING;md5=bbea815ee2795b2f4230826c0c6b8814"

# Pull in the devicetree files and u-boot config files into the rootfs
RDEPENDS_${KERNEL_PACKAGE_NAME}-base += "kernel-devicetree u-boot"

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

LINUX_VERSION = "4.19"
LINUX_VERSION_EXTENSION = "-allwinner"
PREEMPT_RT_VERSION = "4.19.72-rt26"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_4.19:"

S = "${WORKDIR}/git"

PV = "4.19.72"
SRCREV = "ee809c7e08956d737cb66454f5b6ca32cc0d9f26"

SRC_URI = " \
        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
        file://do_patch.sh \
        file://patches-${LINUX_VERSION} \
        file://patch-${PREEMPT_RT_VERSION} \
        file://${SOC_FAMILY}-rt-defconfig/defconfig \
"

# For preempt-rt kernels, we need first to use the do_patch.sh tool
# to apply the armbian patches and then apply the preempt-rt patches
# for the proper kernel version
do_patch_append() {
    bbinfo "Will use ${SOC_FAMILY}-rt-defconfig for the kernel"
    cp ${WORKDIR}/${SOC_FAMILY}-rt-defconfig/defconfig ${WORKDIR}/defconfig
    cd ${WORKDIR}/git
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-${LINUX_VERSION}      # apply kernel patches
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patch-${PREEMPT_RT_VERSION}   # apply preempt-rt patches
}

python() {
    if not d.getVar('SOC_FAMILY'):
        bb.fatal("You need to set 'SOC_FAMILY' in your local.conf file to 'sunxi' or 'sunxi64' depending your board.")
    else:
        bb.note("%s-defconfig/defconfig will be used for the kernel." % (d.getVar('SOC_FAMILY')))
}

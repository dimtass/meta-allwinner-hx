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

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_4.19:"

S = "${WORKDIR}/git"

PV = "4.19.80"
SRCREV = "c3038e718a19fc596f7b1baba0f83d5146dc7784"

SRC_URI = " \
        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
        file://do_patch.sh \
        file://patches-${LINUX_VERSION} \
        file://${SOC_FAMILY}-defconfig/defconfig \
"

# Apply the armbian patches and defconfig
do_patch_append() {
    cp ${WORKDIR}/${SOC_FAMILY}-defconfig/defconfig ${WORKDIR}/defconfig
    cd ${WORKDIR}/git
    ${WORKDIR}/do_patch.sh ${WORKDIR}/patches-${LINUX_VERSION}
}

do_install_append() {
    # Install kernel-modules
	install -d ${D}${nonarch_base_libdir}/modules
    # oe_runmake INSTALL_MOD_PATH=${D} modules_install
	oe_runmake DEPMOD=echo MODLIB=${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION} INSTALL_FW_PATH=${D}${nonarch_base_libdir}/firmware modules_install
    rm "${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/build"
    rm "${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/source"
    # If the kernel/ directory is empty remove it to prevent QA issues
    rmdir --ignore-fail-on-non-empty "${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel"
}

python() {
    if not d.getVar('SOC_FAMILY'):
        bb.fatal("You need to set 'SOC_FAMILY' in your local.conf file to 'sunxi' or 'sunxi64' depending your board.")
    else:
        bb.note("%s-defconfig/defconfig will be used for the kernel." % (d.getVar('SOC_FAMILY')))
}

FILES_${KERNEL_PACKAGE_NAME}-base += "${nonarch_base_libdir}/*"
# pkg_postinst_ontarget_${PN} () {
# 		depmod -a ${KERNEL_VERSION}
# }

# FILES_${KERNEL_PACKAGE_NAME}-base = "${nonarch_base_libdir}/modules/${KERNEL_VERSION}/modules.order ${nonarch_base_libdir}/modules/${KERNEL_VERSION}/modules.builtin"

pkg_postinst_${PN}() {
    depmod -a ${KERNEL_VERSION}
}
DESCRIPTION="Upstream's U-boot fw utils configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
LICENSE = "GPLv2+"

require recipes-bsp/u-boot/u-boot.inc

COMPATIBLE_MACHINE = "(sun8i|sun50i)"

DEPENDS += "bc-native dtc-native swig-native python3-native bison-native flex-native"
DEPENDS_append_sun50i = " atf-arm "

PROVIDES = "u-boot-fw-utils"
RPROVIDES_${PN} = "u-boot-fw-utils"

DEFAULT_PREFERENCE_sun8i="1"
DEFAULT_PREFERENCE_sun50i="1"

SRC_URI = " \
        git://gitlab.denx.de/u-boot.git;branch=master \
        file://armbian-patcher.sh \
        file://patches-${UBOOT_VERSION} \
        file://fw_env.config \
    "

S = "${WORKDIR}/git"

INSANE_SKIP_${PN} = "already-stripped"
EXTRA_OEMAKE_class-target = 'CROSS_COMPILE=${TARGET_PREFIX} CC="${CC} ${CFLAGS} ${LDFLAGS}" HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}" V=1'
EXTRA_OEMAKE_class-cross = 'HOSTCC="${CC} ${CFLAGS} ${LDFLAGS}" V=1'

do_configure() {
    cd ${S}
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/patches-${UBOOT_VERSION}
}

do_compile() {
	cd ${S}
	oe_runmake -C ${S} O=${B} ${UBOOT_MACHINE}
	oe_runmake -C ${S} O=${B} envtools
}

do_install() {
	install -d ${D}${base_sbindir}
	install -d ${D}${sysconfdir}
	install -m 755 ${B}/tools/env/fw_printenv ${D}${base_sbindir}/fw_printenv
	install -m 755 ${B}/tools/env/fw_printenv ${D}${base_sbindir}/fw_setenv
	install -m 0644 ${WORKDIR}/fw_env.config ${D}${sysconfdir}/fw_env.config
}

do_deploy() {
	:
}

FILES_${PN} += " \
    ${base_sbindir}/fw_printenv \
    ${base_sbindir}/fw_setenv \
    ${sysconfdir}/fw_env.config \
"

addtask do_configure before do_compile
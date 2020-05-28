DESCRIPTION="Upstream's U-boot fw utils configured for allwinner devices"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

require u-boot-common.inc

LIC_FILES_CHKSUM ?= "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

PROVIDES = "u-boot-fw-utils"
RPROVIDES_${PN} = "u-boot-fw-utils"

SRC_URI += " \
        file://fw_env.config \
    "

INSANE_SKIP_${PN} = "already-stripped"

do_configure() {
    cd ${S}
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/patches-${SOC_FAMILY}-${UBOOT_VERSION}
}

do_compile() {
	cd ${S}
	oe_runmake -C ${S} O=${B} ${UBOOT_CONFIG}
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
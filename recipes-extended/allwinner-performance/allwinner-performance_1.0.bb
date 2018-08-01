DESCRIPTION = "Setup CPU for maximum performance"
AUTHOR = "Dimitris Tassopoulos <dimitris.tassopoulos@native-instruments.de>"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit systemd

RDEPENDS_${PN} += "bash"

FILESEXTRAPATHS_append = "${THISDIR}/files:"

SRC_URI = "file://allwinner_performance;md5=10279eb0a47c560c709f6b7ce84d5ade \
	file://allwinner_performance.service;md5=ba08d621de902cbb46c87a135f721176"
S = "${WORKDIR}"

do_install () {
	install -d ${D}${systemd_unitdir}/system/ ${D}${sysconfdir}/systemd/system/
	install -m 0644 ${WORKDIR}/allwinner_performance.service ${D}${systemd_unitdir}/system

	install -d ${D}/usr/bin/
	install -m 0755 ${WORKDIR}/allwinner_performance ${D}/usr/bin/

}

NATIVE_SYSTEMD_SUPPORT = "1"
SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE_${PN} = "allwinner_performance.service"

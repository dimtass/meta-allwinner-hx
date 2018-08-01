DESCRIPTION = "Program for reseting USB devices"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

RDEPENDS_${PN} += "bash"
TARGET_CC_ARCH += "${LDFLAGS}"

FILESEXTRAPATHS_append = "${THISDIR}/files:"

SRCREV = "${AUTOREV}"
SRC_URI = "file://usbreset.c"

S = "${WORKDIR}"

do_compile () {
    ${CC} usbreset.c -o usbreset
}

do_install () {
	install -d ${D}${bindir}/
	install -m 0755 ${S}/usbreset ${D}${bindir}
}

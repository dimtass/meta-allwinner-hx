SUMMARY = "udev rule for Python and gpio permissions in sysfs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
       file://60-python-gpio-permissions.rules \
"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${sysconfdir}/udev/rules.d
    install -m 0644 ${WORKDIR}/60-python-gpio-permissions.rules     ${D}${sysconfdir}/udev/rules.d/
}

FILES_${PN} = "${sysconfdir}/udev"
RDEPENDS_${PN} = "udev"

RPROVIDES_${PN} = "udev-python-gpio"

# Add a gpio group and then add root user to that
pkg_postinst_ontarget_${PN}() {
    /usr/sbin/addgroup gpio
    /usr/sbin/usermod -aG gpio root
}

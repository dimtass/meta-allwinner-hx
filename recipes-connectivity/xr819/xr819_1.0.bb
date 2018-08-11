DESCRIPTION = "Install firmware files for xradio XR819"
AUTHOR = "Dimitris Tassopoulos <dimitris.tassopoulos@native-instruments.de>"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

RDEPENDS_${PN} += "bash"

FILESEXTRAPATHS_append = "${THISDIR}/files:"

SRC_URI = " \
        file://boot_xr819.bin \
	    file://device-xradio.mk \
        file://fw_xr819.bin \
        file://sdd_xr819.bin \
"

S = "${WORKDIR}"

do_install () {
    install -d ${D}${base_libdir}/firmware
    install -d ${D}${base_libdir}/firmware/xr819
	install -m 0644 ${WORKDIR}/boot_xr819.bin ${D}${base_libdir}/firmware/xr819
	install -m 0644 ${WORKDIR}/device-xradio.mk ${D}${base_libdir}/firmware/xr819
	install -m 0644 ${WORKDIR}/fw_xr819.bin ${D}${base_libdir}/firmware/xr819
	install -m 0644 ${WORKDIR}/sdd_xr819.bin ${D}${base_libdir}/firmware/xr819
}

FILES_${PN} += " \
        ${base_libdir}/firmware/xr819/boot_xr819.bin \
        ${base_libdir}/firmware/xr819/device-xradio.mk \
        ${base_libdir}/firmware/xr819/fw_xr819.bin \
        ${base_libdir}/firmware/xr819/sdd_xr819.bin \
"

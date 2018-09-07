FILESEXTRAPATHS_append := "${THISDIR}/${PN}:"
SRC_URI += " \
    file://wlan0.network \
    file://systemd-udevd.service \
"

PACKAGECONFIG_append = " networkd resolved"

do_install_append() {
	install -m 0644 ${WORKDIR}/wlan0.network ${D}${sysconfdir}/systemd/network
	install -m 0644 ${WORKDIR}/systemd-udevd.service ${D}${sysconfdir}/systemd/system/
}

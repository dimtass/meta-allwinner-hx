FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

# set these from your local.conf
SSID ?= ""
PSK ?= ""

SYSTEMD_SERVICE_${PN} += " wpa_supplicant@wlan0.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_install_append() {
    install -d ${D}${sysconfdir}/wpa_supplicant
	install -m 600 ${WORKDIR}/wpa_supplicant.conf ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant-wlan0.conf

	sed -i 's/%SSID/${SSID}/g' ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant-wlan0.conf
	sed -i 's/%PSK/${PSK}/g' ${D}${sysconfdir}/wpa_supplicant/wpa_supplicant-wlan0.conf

    if [ -z "${SSID}" ]; then
        bbwarn "The SSID for wpa_supplicant is not set in local.conf"
    fi

    if [ -z "${PSK}" ]; then
        bbwarn "The PSK for wpa_supplicant is not set in local.conf"
    fi
}
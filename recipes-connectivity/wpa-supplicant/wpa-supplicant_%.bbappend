FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

# set these from your local.conf
SSID ?= ""
PSK ?= ""

do_install_append() {
	install -m 600 ${WORKDIR}/wpa_supplicant.conf ${D}${sysconfdir}/wpa_supplicant.conf

	sed -i 's/%SSID/${SSID}/g' ${D}${sysconfdir}/wpa_supplicant.conf
	sed -i 's/%PSK/${PSK}/g' ${D}${sysconfdir}/wpa_supplicant.conf

    if [ -z "${SSID}" ]; then
        bbwarn "The SSID for wpa_supplicant is not set in local.conf"
    fi

    if [ -z "${PSK}" ]; then
        bbwarn "The PSK for wpa_supplicant is not set in local.conf"
    fi
}
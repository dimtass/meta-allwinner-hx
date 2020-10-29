do_install_append() {
    install -m 644 ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43430-sdio.AP6212.txt ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43430-sdio.xunlong,orangepi-zero-plus2-h3.txt
    install -m 644 ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43430-sdio.AP6212.txt ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43430-sdio.xunlong,orangepi-zero-plus2-h5.txt
}

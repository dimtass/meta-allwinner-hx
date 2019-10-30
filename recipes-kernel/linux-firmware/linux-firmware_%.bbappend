do_install_append_orange-pi-zero-plus2() {
    # Set brcmfmac config file used with this machine
    ln -sf brcmfmac43430-sdio.AP6212.txt ${D}${nonarch_base_libdir}/firmware/brcm/brcmfmac43430-sdio.txt
}

FILES_${PN}-bcm43430_append_orange-pi-zero-plus2 += " \
    ${nonarch_base_libdir}/firmware/brcm/brcmfmac43430-sdio.txt \
    ${nonarch_base_libdir}/firmware/brcm/brcmfmac43430-sdio.AP6212.txt \
"

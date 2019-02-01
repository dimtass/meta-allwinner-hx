FILESEXTRAPATHS_append := "${THISDIR}/files:"
SRC_URI += " \
        file://fw_env.config \
        "

# override the default fw_env.config
do_install_append () {
	install -m 0644 ${WORKDIR}/fw_env.config ${D}${sysconfdir}/fw_env.config
}
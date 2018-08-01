DESCRIPTION = "Merge prebuilt/extra files into rootfs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit allarch

SRC_URI = "file://merge"
S = "${WORKDIR}"

MERGED_DST = "/"

do_install () {
    install -d ${D}/${MERGED_DST}
    find ${WORKDIR}/merge/ -maxdepth 1 -mindepth 1 -not -name README \
    -exec cp -fr '{}' ${D}/${MERGED_DST}/ \;
    find ${WORKDIR}/merge/ -maxdepth 1 -mindepth 1 -exec rm -fr '{}' \;
}
do_unpack[nostamp] = "1"
do_install[nostamp] = "1"
do_configure[noexec] = "1"
do_compile[noexec] = "1"

FILES_${PN} = "/*"
ALLOW_EMPTY_${PN} = "1"
INSANE_SKIP_${PN} = "debug-files dev-so"

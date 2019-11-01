DESCRIPTION = "This recipe adds the default modules for each machine that are suggested \
from the Armbian distro. Each MACHINE in conf/machine/ folder has the MODULES and \
MODULES_BLACKLIST param, which is used from this recipe. These modules are added in the \
/etc/modules file and /etc/modprobe.d/blacklist-${MACHINE} \
"
AUTHOR = "Dimitris Tassopoulos <dimitris.tassopoulos@gmail.com>"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

do_compile () {
    :
}

do_install () {
    set -x
    # Create modules file
    if [ ! -z "${MODULES}" ]; then
        # Clean previous
        [ -f ${WORKDIR}/modules ] && rm ${WORKDIR}/modules
        # Create new modules files
        touch ${WORKDIR}/modules
        # Add load modules
        echo "${MODULES}" | tr " " "\n" > ${WORKDIR}/modules

        install -d ${D}${sysconfdir}
        install -m 644 ${WORKDIR}/modules ${D}${sysconfdir}
    fi

    # # Create blacklist file
    if [ ! -z "${MODULES_BLACKLIST}" ]; then
        # Clean previous
        [ -f ${WORKDIR}/blacklist-${MACHINE}.conf ] && rm ${WORKDIR}/blacklist-${MACHINE}.conf
        # Create new modules files
        touch ${WORKDIR}/blacklist-${MACHINE}.conf
        # Add blacklisted modules
        echo "${MODULES_BLACKLIST}" | tr " " "\n" | sed -e 's/^/blacklist /' > ${WORKDIR}/blacklist-${MACHINE}.conf

        install -d ${D}${sysconfdir}/modprobe.d/
        install -m 644 ${WORKDIR}/blacklist-${MACHINE}.conf ${D}${sysconfdir}/modprobe.d
    fi
}

FILES_${PN} += " \
        ${sysconfdir}/* \
        "
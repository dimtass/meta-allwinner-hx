require linux-stable.inc

LINUX_VERSION = "5.10"

PV = "5.10.18"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-megous_${LINUX_VERSION}:${THISDIR}/../../scripts:"

SRC_URI += " \
        git://github.com/megous/linux.git;branch=orange-pi-${LINUX_VERSION} \
        ${ARMBIAN_URI} \
"
SRCREV = "99d39196f263e0c12f5184ec079c9267ac926352"

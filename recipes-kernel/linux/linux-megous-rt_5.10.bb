require linux-stable-rt.inc

LINUX_VERSION = "5.10"
PREEMPT_RT_VERSION = "5.10.17-rt32"
PV = "5.10.18"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-megous-rt_${LINUX_VERSION}:${THISDIR}/../../scripts:"

SRC_URI += " \
        git://github.com/megous/linux.git;branch=orange-pi-${LINUX_VERSION} \
        ${ARMBIAN_URI} \
        file://patch-${PREEMPT_RT_VERSION} \
"
SRCREV = "99d39196f263e0c12f5184ec079c9267ac926352"

require linux-stable-rt.inc

LINUX_VERSION = "5.4"
PREEMPT_RT_VERSION = "5.4.40-rt24"
PV = "5.4.18"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-megous-rt_${LINUX_VERSION}:${THISDIR}/../../scripts:"

SRC_URI += " \
        git://github.com/megous/linux.git;branch=orange-pi-${LINUX_VERSION} \
        ${ARMBIAN_URI} \
        file://patch-${PREEMPT_RT_VERSION} \
"
SRCREV = "db5ec55d5a9448c472568a39a895dc89fb7ade20"

# If I don't do this then do_compile_kernelmodules fails with an
# error that <drv_types.h> is missing, while building the net/wireless
# drivers. I couldn't find the reason, but after a lot of testing
# I found this solution
B = "${S}"
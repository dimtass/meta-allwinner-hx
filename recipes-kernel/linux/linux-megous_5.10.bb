require linux-stable.inc

LINUX_VERSION = "5.10"

PV = "5.10.4"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-megous_${LINUX_VERSION}:${THISDIR}/../../scripts:"

SRC_URI += " \
        git://github.com/megous/linux.git;branch=orange-pi-${LINUX_VERSION} \
        ${ARMBIAN_URI} \
"
SRCREV = "6cbe584b73a5dcb00476b4c46dcf6ab480fc7e29"

# If I don't do this then do_compile_kernelmodules fails with an
# error that <drv_types.h> is missing, while building the net/wireless
# drivers. I couldn't find the reason, but after a lot of testing
# I found this solution
B = "${S}"
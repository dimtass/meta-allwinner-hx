require linux-stable.inc

LINUX_VERSION = "5.3"

PV = "5.3.13"
SRCREV ?= "c353336cc5c5204eb46563a83b2aad2c9348adf3"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_${LINUX_VERSION}:${THISDIR}/../../scripts:"

SRC_URI += " \
        git://github.com/megous/linux.git;branch=orange-pi-${LINUX_VERSION} \
"
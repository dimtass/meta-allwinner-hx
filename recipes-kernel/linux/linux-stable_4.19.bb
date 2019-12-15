require linux-stable.inc

LINUX_VERSION = "4.19"

PV = "4.19.89"
SRCREV ?= "312017a460d5ea31d646e7148e400e13db799ddc"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_${LINUX_VERSION}:${THISDIR}/../../scripts:"

SRC_URI += " \
        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
"
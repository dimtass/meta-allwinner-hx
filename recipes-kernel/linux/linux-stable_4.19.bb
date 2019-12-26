require linux-stable.inc
# last armbian 4.19 commit: 85080ff373c00b9c7b25c93232dfbfd3ff1719c7

LINUX_VERSION = "4.19"

PV = "4.19.91"
SRCREV ?= "672481c2deffb371d8a7dfdc009e44c09864a869"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_${LINUX_VERSION}:${THISDIR}/../../scripts:"

SRC_URI += " \
        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
"
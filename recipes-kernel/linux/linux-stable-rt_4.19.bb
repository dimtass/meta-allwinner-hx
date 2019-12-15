require linux-stable.inc

LINUX_VERSION = "4.19"
PREEMPT_RT_VERSION = "4.19.82-rt30"

PV = "4.19.82"
SRCREV ?= "5ee93551c703f8fa1a6c414a7d08f956de311df3"

FILESEXTRAPATHS_prepend := "${THISDIR}/linux_${LINUX_VERSION}:${THISDIR}/../../scripts:"

SRC_URI += " \
        git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git;branch=linux-${LINUX_VERSION}.y \
        file://patch-${PREEMPT_RT_VERSION} \
"
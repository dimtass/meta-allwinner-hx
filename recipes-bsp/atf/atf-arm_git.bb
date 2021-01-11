inherit deploy

DESCRIPTION = "ARM Trusted Firmware"
LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://license.rst;md5=1dd070c98a281d18d9eefd938729b031"

FILESEXTRAPATHS_append := "${THISDIR}/${PN}:"

SRC_URI = " \
        git://github.com/ARM-software/arm-trusted-firmware.git;nobranch=1 \
        file://0001-Fix-reset-issue-on-H6-by-using-R_WDOG.patch \
        "
SRCREV_sun50iw2 = "c390ecd6db5fadb054466a8d4168d9bbbff2fa95"
SRCREV_sun50iw6 = "c390ecd6db5fadb054466a8d4168d9bbbff2fa95"

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

COMPATIBLE_MACHINE = "(sun50iw2|sun50iw6)"

PLATFORM_sun50iw2 = "sun50i_a64"
PLATFORM_sun50iw6 = "sun50i_h6"

LDFLAGS[unexport] = "1"

do_compile() {
    oe_runmake -C ${S} BUILD_BASE=${B} \
      CROSS_COMPILE=${TARGET_PREFIX} \
      PLAT=${PLATFORM} \
      bl31 \
      all
}

do_deploy() {
    install -D -p -m 0644 ${B}/${PLATFORM}/release/bl31.bin ${DEPLOYDIR}/bl31.bin
}

addtask deploy after do_compile

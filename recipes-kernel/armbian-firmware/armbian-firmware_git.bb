DESCRIPTION = "This recipe will Armbian firmware files in the image. \
This is a two step procedure. First the linux-firmware.git files are \
fetched and then the Armbian files are installed on top. \
 \
WARNING 1: \
Be aware that this recipe is not compatible with the linux-firmware.bb \
and also bitbake will not be able to extract license information about \
each firmware file. Therefore you need to do this by yourself! \
 \
WARNING 2: \
This recipe will add to the image around 500MB more!!! \
If you need only specific firmware then create a custom recipe \
or bbappend this one \
"
AUTHOR = "Dimitris Tassopoulos <dimitris.tassopoulos@gmail.com>"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRCREV_linux-firmware = "9e194c730712addbdf5b50cf71f9a4ec42623a29"
SRCREV_armbian-firmware = "78a566d50b7f82bfe77b32c172ce0dfee8642dea"

TMP_FIRMWARE_FOLDER = "${WORKDIR}/lib-firmware"

SRC_URI = " \
        git://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git;name=linux-firmware;destsuffix=git/linux-firmware \
        git://github.com/armbian/firmware.git;name=armbian-firmware;destsuffix=git/armbian-firmware \
        "

S = "${WORKDIR}/git"

inherit allarch

do_compile() {
    :
}

do_install () {
    install -d ${D}${nonarch_base_libdir}/firmware/
    cp -r ${WORKDIR}/git/linux-firmware/* ${D}${nonarch_base_libdir}/firmware/
    cp -rf ${WORKDIR}/git/armbian-firmware/*  ${D}${nonarch_base_libdir}/firmware/

    # Remove firmware that doesn't make sense
    if [ -d "${D}${nonarch_base_libdir}/firmware/amdgpu" ]; then
        rm -rf ${D}${nonarch_base_libdir}/firmware/amdgpu
    fi
    if [ -d "${D}${nonarch_base_libdir}/firmware/amd-ucode" ]; then
        rm -rf ${D}${nonarch_base_libdir}/firmware/amd-ucode
    fi
    if [ -d "${D}${nonarch_base_libdir}/firmware/radeon" ]; then
        rm -rf ${D}${nonarch_base_libdir}/firmware/radeon
    fi
    if [ -d "${D}${nonarch_base_libdir}/firmware/nvidia" ]; then
        rm -rf ${D}${nonarch_base_libdir}/firmware/nvidia
    fi
    if [ -d "${D}${nonarch_base_libdir}/firmware/matrox" ]; then
        rm -rf ${D}${nonarch_base_libdir}/firmware/matrox
    fi
    # Permissions
    find ${D}${nonarch_base_libdir}/firmware -type f -exec chmod 644 '{}' ';'
    find ${D}${nonarch_base_libdir}/firmware -type f -exec chown root:root '{}' ';'
}

FILES_${PN} += "${nonarch_base_libdir}/firmware"
# Firmware files are generally not ran on the CPU, so they can be
# allarch despite being architecture specific
INSANE_SKIP = "arch file-rdeps"

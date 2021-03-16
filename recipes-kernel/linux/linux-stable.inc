AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
SECTION = "kernel"
LICENSE = "GPLv2"
COMPATIBLE_MACHINE = "(sun8i|sun50iw2|sun50iw6)"

LIC_FILES_CHKSUM ?= "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

# Install fixup.scr to /boot/
FILES_${KERNEL_PACKAGE_NAME}-fixup = "${KERNEL_IMAGEDEST}/fixup.scr"
# Install overlays to /boot/overlay
FILES_${KERNEL_PACKAGE_NAME}-dtbo = "${KERNEL_IMAGEDEST}/overlay"
# Install devicetree files
FILES_${KERNEL_PACKAGE_NAME}-dt = "${KERNEL_IMAGEDEST}/allwinner"
PACKAGES_append += "${KERNEL_PACKAGE_NAME}-fixup ${KERNEL_PACKAGE_NAME}-dtbo ${KERNEL_PACKAGE_NAME}-dt"

# Pull in the devicetree files and u-boot config files into the rootfs
RDEPENDS_${KERNEL_PACKAGE_NAME}-base += "kernel-devicetree kernel-image kernel-dt kernel-dtbo kernel-fixup u-boot"

# We need mkimage for the overlays
DEPENDS += "u-boot-tools-native"

do_compile[depends] += "u-boot-tools-native:do_populate_sysroot"

inherit deploy
require recipes-kernel/linux/linux-yocto.inc

# Fix missing gmp.h issues on kernel >= 5.8
DEPENDS += "gmp-native"
EXTRA_OEMAKE += "HOSTCXX="${BUILD_CXX} ${BUILD_CXXFLAGS} ${BUILD_LDFLAGS}""

LINUX_VERSION ?= ""
PREEMPT_RT_VERSION ?= ""

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

S = "${WORKDIR}/git"

# Choose the correct path for the defconfig
DEFCONFIG_PATH =  "${@'${SOC_FAMILY}-defconfig' if d.getVar('PREEMPT_RT_VERSION') == '' else '${SOC_FAMILY}-rt-defconfig'}"

ARMBIAN_URI = " \
        file://process_patch_file.sh \
        file://process_patch_folder.sh \
        file://patches-${LINUX_VERSION} \
        file://custom-patches \
        file://${DEFCONFIG_PATH}/defconfig \
        git://github.com/sfjro/aufs5-standalone.git;name=aufs5;branch=aufs5.10;destsuffix=extras/aufs5 \
        git://github.com/igorpecovnik/realtek-r8152-linux.git;name=rtl8152;destsuffix=extras/rtl8152 \
        git://github.com/jwrdegoede/rtl8189ES_linux.git;name=rtl8189es;destsuffix=extras/rtl8189es \
        git://github.com/jwrdegoede/rtl8189ES_linux.git;name=rtl8189fs;branch=rtl8189fs;destsuffix=extras/rtl8189fs \
        git://github.com/Mange/rtl8192eu-linux-driver.git;branch=realtek-4.4.x;name=rtl8192eu;destsuffix=extras/rtl8192eu \
        git://github.com/aircrack-ng/rtl8812au.git;branch=v5.6.4.2;name=rtl8812au;destsuffix=extras/rtl8812au \
        git://github.com/karabek/xradio.git;name=xradio;destsuffix=extras/xradio \
        git://github.com/brektrou/rtl8821CU.git;name=rtl8811cu;destsuffix=extras/rtl8811cu \
        git://github.com/aircrack-ng/rtl8188eus.git;branch=v5.7.6.1;name=rtl8188eu;destsuffix=extras/rtl8188eu \
        git://github.com/cilynx/rtl88x2bu.git;branch=5.6.1_30362.20181109_COEX20180928-6a6a;name=rtl88x2bu;destsuffix=extras/rtl88x2bu \
        git://github.com/lwfinger/rtl8723ds.git;name=rtl8723ds;destsuffix=extras/rtl8723ds \
        git://github.com/lwfinger/rtl8723du.git;name=rtl8723du;destsuffix=extras/rtl8723du \
"
SRCREV_aufs5 =          "05111a0a66afcfce8eb4aafb5cfc887e052ac594"
SRCREV_xradio =         "354e8c32e7948d46a63796d0ca266b1f702999b0"
SRCREV_rtl8152 =        "006c40bcdf56c1701c9eadc34e29662bab5948ca"
SRCREV_rtl8189es =      "a57f6985018f601b22ab689f36d1205273ae9e90"
SRCREV_rtl8189fs =      "1ebc898cc5b671fe7145141fb9f3da14dab8c8ad"
SRCREV_rtl8192eu =      "7e8858e0dfc10c025c8d114330f6c7d0353b4819"
SRCREV_rtl8812au =      "b65dcf4105641716d16f3a6c96507fdd9c1862b4"
SRCREV_rtl8811cu =      "2bebdb9a35c1d9b6e6a928e371fa39d5fcec8a62"
SRCREV_rtl8188eu =      "0683c3382f7ad4bb90d72b9c903a90a7bd7b200d"
SRCREV_rtl88x2bu =      "48e7c19c92a77554403e1347447f8e2cfd780228"
SRCREV_rtl8723ds =      "80af87ac7687eb66e9359be118ef654160d0db53"
SRCREV_rtl8723du =      "9690f34fc2603d04b53432df4e3d6c282f3d1ca6"

# Apply the armbian patches and defconfig
do_patch_append() {
    set -x
    set -e
    # Add extra armbian modules in the kernel?
    if [ "${AUFS}" = "yes" ]; then
        patch_aufs5
    fi
    # Add extra armbian modules in the kernel?
    if [ "${EXTRAWIFI}" = "yes" ]; then
        patch_rtl8152
        patch_rtl8189fs
        patch_rtl8189es
        patch_rtl8812au
        patch_xradio
        patch_rtl8811cu
        patch_rtl8188eu
        patch_rtl88x2bu
        patch_rtl8723ds
        patch_rtl8723du
        patch_rtl8192eu
    fi
    # Copy defconfig
    cp ${WORKDIR}/${DEFCONFIG_PATH}/defconfig ${WORKDIR}/defconfig
    cp ${WORKDIR}/${DEFCONFIG_PATH}/defconfig ${B}/.config
    cd ${S}
    # Apply patches
    if [ "${BOOTSPLASH}" = "yes" ]; then
        bbnote "Apply bootsplash patches..."
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/bootsplash-5.8.10-0001-Revert-vgacon-remove-software-scrollback-support.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/bootsplash-5.8.10-0002-Revert-fbcon-remove-now-unusued-softback_lines-curso.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/bootsplash-5.10.y-0003-Revert-fbcon-remove-soft-scrollback-code.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0001-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0002-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0003-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0004-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0005-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0006-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0007-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0008-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0009-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0010-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0011-bootsplash.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/0012-bootsplash.patch"
    fi
    if [ "${WIFI_INJECTION}" = "yes" ]; then
        bbnote "Adding" "Wireless package injections for mac80211 compatible chipsets"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/kali-wifi-injection-1-v5.9-post.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/kali-wifi-injection-2.patch"
        ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/kali-wifi-injection-3.patch"
    fi

    if [ -d "${WORKDIR}/patch-${PREEMPT_RT_VERSION}" ]; then
        bbnote "Applying preempt-rt patches"
        ${WORKDIR}/process_patch_folder.sh ${WORKDIR}/patches-${LINUX_VERSION}
        ${WORKDIR}/process_patch_folder.sh ${WORKDIR}/patch-${PREEMPT_RT_VERSION}
    else
        ${WORKDIR}/process_patch_folder.sh ${WORKDIR}/patches-${LINUX_VERSION}
    fi
    ${WORKDIR}/process_patch_file.sh "${WORKDIR}/custom-patches/fix-rtl8723cs-makefile.patch"
}

# Compile overlays. This is for compatibility since from version 4.20
# and later individual dtbo build is not supported
do_compile_append() {
    set -x
    bbnote "Compiling kernel overlays"
    oe_runmake -C ${B} CC="${KERNEL_CC}" ${PARALLEL_MAKE} modules dtbs
}

do_install_append() {
    set -x
    dtbos=$(find . -name "*.dtbo" | grep ${OVERLAY_PREFIX})
    install -d ${D}/${KERNEL_IMAGEDEST}/overlay
    for dtbo in $dtbos; do
        install -m 644 $dtbo ${D}/${KERNEL_IMAGEDEST}/overlay
    done

    install -d ${D}/${KERNEL_IMAGEDEST}/allwinner
    dtbs=$(find . -name "*.dtb" | grep ${OVERLAY_PREFIX})
    for dtb in $dtbs; do
        install -m 644 $dtb ${D}/${KERNEL_IMAGEDEST}/allwinner
    done

    bbnote "Extract ${OVERLAY_PREFIX} fixup"
    fixup=$(find ${STAGING_KERNEL_DIR} -name "*.scr-cmd" | grep ${OVERLAY_PREFIX})
    mkimage -C none -A arm -T script -d $fixup ${WORKDIR}/fixup.scr
    install -m 644 ${WORKDIR}/fixup.scr ${D}/${KERNEL_IMAGEDEST}/
}

# deploy the dtbo overlays in the DEPLOYDIR
do_deploy_append() {
    set -x
    dtbos=$(find . -name "*.dtbo" | grep ${OVERLAY_PREFIX})
    for dtbo in $dtbos; do
        install -m 644 $dtbo ${DEPLOYDIR}/
    done

    install -m 644 ${WORKDIR}/fixup.scr ${DEPLOYDIR}/
}

python() {
    if not d.getVar('SOC_FAMILY'):
        bb.fatal("SOC_FAMILY is not set! Please set it in your machine configuration.")
    else:
        bb.note("%s/defconfig will be used for the kernel." % (d.getVar('DEFCONFIG_PATH')))
}

# Extra patches
patch_aufs5() {
    bbnote "Adding AUFS"
    cd ${S}
    ${WORKDIR}/process_patch_file.sh ${WORKDIR}/extras/aufs5/aufs5-kbuild.patch
    ${WORKDIR}/process_patch_file.sh ${WORKDIR}/extras/aufs5/aufs5-base.patch
    ${WORKDIR}/process_patch_file.sh ${WORKDIR}/extras/aufs5/aufs5-mmap.patch
    ${WORKDIR}/process_patch_file.sh ${WORKDIR}/extras/aufs5/aufs5-standalone.patch
    cp -R ${WORKDIR}/extras/aufs5/Documentation .
    cp -R ${WORKDIR}/extras/aufs5/fs .
    cp ${WORKDIR}/extras/aufs5/include/uapi/linux/aufs_type.h include/uapi/linux/
}

patch_rtl8152() {
    bbnote "Adding Wireless drivers for Realtek 8152 chipsets"
    cp -R ${WORKDIR}/extras/rtl8152/r8152.c \
        "${S}/drivers/net/usb/"
    cp -R ${WORKDIR}/extras/rtl8152/compatibility.h \
        "${S}/drivers/net/usb/"
}

patch_rtl8189es() {
    bbnote "Adding Wireless drivers for Realtek 8189ES chipsets"
    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl8189es
    mkdir -p ${S}/drivers/net/wireless/rtl8189es/
    cp -R ${WORKDIR}/extras/rtl8189es/core \
        ${S}/drivers/net/wireless/rtl8189es
    cp -R ${WORKDIR}/extras/rtl8189es/hal \
        ${S}/drivers/net/wireless/rtl8189es
    cp -R ${WORKDIR}/extras/rtl8189es/include \
        ${S}/drivers/net/wireless/rtl8189es
    cp -R ${WORKDIR}/extras/rtl8189es/os_dep \
        ${S}/drivers/net/wireless/rtl8189es
    cp -R ${WORKDIR}/extras/rtl8189es/platform \
        ${S}/drivers/net/wireless/rtl8189es

    # Makefile
    cp ${WORKDIR}/extras/rtl8189fs/Makefile \
        ${S}/drivers/net/wireless/rtl8189es/Makefile

    # Kconfig
    sed -i 's/---help---/help/g' ${WORKDIR}/extras/rtl8189fs/Kconfig
    cp ${WORKDIR}/extras/rtl8189fs/Kconfig \
            ${S}/drivers/net/wireless/rtl8189es/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8189ES) += rtl8189es/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8189es\/Kconfig"' \
        ${S}/drivers/net/wireless/Kconfig
}


patch_rtl8189fs() {
    bbnote "Adding Wireless drivers for Realtek 8189FS chipsets"
    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl8189fs
    mkdir -p ${S}/drivers/net/wireless/rtl8189fs/
    cp -R ${WORKDIR}/extras/rtl8189fs/core \
        ${S}/drivers/net/wireless/rtl8189fs
    cp -R ${WORKDIR}/extras/rtl8189fs/hal \
        ${S}/drivers/net/wireless/rtl8189fs
    cp -R ${WORKDIR}/extras/rtl8189fs/include \
        ${S}/drivers/net/wireless/rtl8189fs
    cp -R ${WORKDIR}/extras/rtl8189fs/os_dep \
        ${S}/drivers/net/wireless/rtl8189fs
    cp -R ${WORKDIR}/extras/rtl8189fs/platform \
        ${S}/drivers/net/wireless/rtl8189fs

    # Makefile
    cp ${WORKDIR}/extras/rtl8189fs/Makefile \
        ${S}/drivers/net/wireless/rtl8189fs/Makefile

    # Kconfig
    sed -i 's/---help---/help/g' ${WORKDIR}/extras/rtl8189fs/Kconfig
    cp ${WORKDIR}/extras/rtl8189fs/Kconfig \
        ${S}/drivers/net/wireless/rtl8189fs/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8189FS) += rtl8189fs/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8189fs\/Kconfig"' \
        ${S}/drivers/net/wireless/Kconfig
}

patch_rtl8192eu() {
    bbnote "Adding Wireless drivers for Realtek 8192EU chipsets"

    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl8192eu
    mkdir -p ${S}/drivers/net/wireless/rtl8192eu/
    cp -R ${WORKDIR}/extras/rtl8192eu/core \
            ${S}/drivers/net/wireless/rtl8192eu
    cp -R ${WORKDIR}/extras/rtl8192eu/hal \
            ${S}/drivers/net/wireless/rtl8192eu
    cp -R ${WORKDIR}/extras/rtl8192eu/include \
            ${S}/drivers/net/wireless/rtl8192eu
    cp -R ${WORKDIR}/extras/rtl8192eu/os_dep \
            ${S}/drivers/net/wireless/rtl8192eu
    cp -R ${WORKDIR}/extras/rtl8192eu/platform \
            ${S}/drivers/net/wireless/rtl8192eu

    # Makefile
    cp ${WORKDIR}/extras/rtl8192eu/Makefile \
            ${S}/drivers/net/wireless/rtl8192eu/Makefile

    # Kconfig
    sed -i 's/---help---/help/g' ${WORKDIR}/extras/rtl8192eu/Kconfig
    cp ${WORKDIR}/extras/rtl8192eu/Kconfig \
            ${S}/drivers/net/wireless/rtl8192eu/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8192EU) += rtl8192eu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8192eu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
}

patch_rtl8812au() {
    bbnote "Adding Wireless drivers for Realtek 8811, 8812, 8814 and 8821 chipsets"

    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl8812au
    mkdir -p ${S}/drivers/net/wireless/rtl8812au/
    cp -R ${WORKDIR}/extras/rtl8812au/core \
            ${S}/drivers/net/wireless/rtl8812au
    cp -R ${WORKDIR}/extras/rtl8812au/hal \
            ${S}/drivers/net/wireless/rtl8812au
    cp -R ${WORKDIR}/extras/rtl8812au/include \
            ${S}/drivers/net/wireless/rtl8812au
    cp -R ${WORKDIR}/extras/rtl8812au/os_dep \
            ${S}/drivers/net/wireless/rtl8812au
    cp -R ${WORKDIR}/extras/rtl8812au/platform \
            ${S}/drivers/net/wireless/rtl8812au

    # Makefile
    cp ${WORKDIR}/extras/rtl8812au/Makefile \
            ${S}/drivers/net/wireless/rtl8812au/Makefile

    # Kconfig
    cp ${WORKDIR}/extras/rtl8812au/Kconfig \
            ${S}/drivers/net/wireless/rtl8812au/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8812AU) += rtl8812au/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8812au\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
}

patch_xradio() {
    bbnote "Adding Wireless drivers for Xradio XR819 chipsets"

    cd ${S}
    rm -rf ${S}/drivers/net/wireless/xradio
    mkdir -p ${S}/drivers/net/wireless/xradio/
    cp ${WORKDIR}/extras/xradio/*.h \
            ${S}/drivers/net/wireless/xradio/
    cp ${WORKDIR}/extras/xradio/*.c \
            ${S}/drivers/net/wireless/xradio/

    # Makefile
    cp ${WORKDIR}/extras/xradio/Makefile \
            ${S}/drivers/net/wireless/xradio/Makefile

    # Kconfig
    sed -i 's/---help---/help/g' ${WORKDIR}/extras/xradio/Kconfig
    cp ${WORKDIR}/extras/xradio/Kconfig \
            ${S}/drivers/net/wireless/xradio/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_WLAN_VENDOR_XRADIO) += xradio/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/xradio\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
}

patch_rtl8811cu() {
    bbnote "Adding Wireless drivers for Realtek RTL8811CU and RTL8821C chipsets"

    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl8811cu
    mkdir -p ${S}/drivers/net/wireless/rtl8811cu/
    cp -R ${WORKDIR}/extras/rtl8811cu/core \
            ${S}/drivers/net/wireless/rtl8811cu
    cp -R ${WORKDIR}/extras/rtl8811cu/hal \
            ${S}/drivers/net/wireless/rtl8811cu
    cp -R ${WORKDIR}/extras/rtl8811cu/include \
            ${S}/drivers/net/wireless/rtl8811cu
    cp -R ${WORKDIR}/extras/rtl8811cu/os_dep \
            ${S}/drivers/net/wireless/rtl8811cu
    cp -R ${WORKDIR}/extras/rtl8811cu/platform \
            ${S}/drivers/net/wireless/rtl8811cu
    cp -R ${WORKDIR}/extras/rtl8811cu/rtl8821c.mk \
            ${S}/drivers/net/wireless/rtl8811cu

    # Makefile
    cp ${WORKDIR}/extras/rtl8811cu/Makefile \
            ${S}/drivers/net/wireless/rtl8811cu/Makefile

    # Kconfig
    sed -i 's/---help---/help/g' ${WORKDIR}/extras/rtl8811cu/Kconfig
    cp ${WORKDIR}/extras/rtl8811cu/Kconfig \
            ${S}/drivers/net/wireless/rtl8811cu/Kconfig

    # Disable debug
    sed -i "s/^CONFIG_RTW_DEBUG.*/CONFIG_RTW_DEBUG = n/" \
            ${S}/drivers/net/wireless/rtl8811cu/Makefile

    # Address ARM related bug https://github.com/aircrack-ng/rtl8812au/issues/233
    sed -i "s/^CONFIG_MP_VHT_HW_TX_MODE.*/CONFIG_MP_VHT_HW_TX_MODE = n/" \
            ${S}/drivers/net/wireless/rtl8811cu/Makefile

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8821CU) += rtl8811cu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8811cu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig

    ${WORKDIR}/process_patch_file.sh ${WORKDIR}/custom-patches/wireless-rtl8811cu.patch
}

patch_rtl8188eu() {
    bbnote "Adding Wireless drivers for Realtek 8188EU 8188EUS and 8188ETV chipsets"

    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl8188eu
    mkdir -p ${S}/drivers/net/wireless/rtl8188eu/
    cp -R ${WORKDIR}/extras/rtl8188eu/core \
            ${S}/drivers/net/wireless/rtl8188eu
    cp -R ${WORKDIR}/extras/rtl8188eu/hal \
            ${S}/drivers/net/wireless/rtl8188eu
    cp -R ${WORKDIR}/extras/rtl8188eu/include \
            ${S}/drivers/net/wireless/rtl8188eu
    cp -R ${WORKDIR}/extras/rtl8188eu/os_dep \
            ${S}/drivers/net/wireless/rtl8188eu
    cp -R ${WORKDIR}/extras/rtl8188eu/platform \
            ${S}/drivers/net/wireless/rtl8188eu

    # Makefile
    cp ${WORKDIR}/extras/rtl8188eu/Makefile \
            ${S}/drivers/net/wireless/rtl8188eu/Makefile

    # Kconfig
    sed -i 's/---help---/help/g' ${WORKDIR}/extras/rtl8188eu/Kconfig
    cp ${WORKDIR}/extras/rtl8188eu/Kconfig \
            ${S}/drivers/net/wireless/rtl8188eu/Kconfig

    # Disable debug
    sed -i "s/^CONFIG_RTW_DEBUG.*/CONFIG_RTW_DEBUG = n/" \
            ${S}/drivers/net/wireless/rtl8188eu/Makefile

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8188EU) += rtl8188eu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8188eu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig

    ${WORKDIR}/process_patch_file.sh ${WORKDIR}/custom-patches/wireless-rtl8188eu.patch
}

patch_rtl88x2bu() {
    bbnote "Adding Wireless drivers for Realtek 88x2bu chipsets"

    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl88x2bu
    mkdir -p ${S}/drivers/net/wireless/rtl88x2bu/
    cp -R ${WORKDIR}/extras/rtl88x2bu/core \
            ${S}/drivers/net/wireless/rtl88x2bu
    cp -R ${WORKDIR}/extras/rtl88x2bu/hal \
            ${S}/drivers/net/wireless/rtl88x2bu
    cp -R ${WORKDIR}/extras/rtl88x2bu/include \
            ${S}/drivers/net/wireless/rtl88x2bu
    cp -R ${WORKDIR}/extras/rtl88x2bu/os_dep \
            ${S}/drivers/net/wireless/rtl88x2bu
    cp -R ${WORKDIR}/extras/rtl88x2bu/platform \
            ${S}/drivers/net/wireless/rtl88x2bu
    cp -R ${WORKDIR}/extras/rtl88x2bu/halmac.mk \
            ${S}/drivers/net/wireless/rtl88x2bu
    cp -R ${WORKDIR}/extras/rtl88x2bu/rtl8822b.mk \
            ${S}/drivers/net/wireless/rtl88x2bu

    # Makefile
    cp ${WORKDIR}/extras/rtl88x2bu/Makefile \
            ${S}/drivers/net/wireless/rtl88x2bu/Makefile
    # Kconfig
    sed -i 's/---help---/help/g' ${WORKDIR}/extras/rtl88x2bu/Kconfig
    cp ${WORKDIR}/extras/rtl88x2bu/Kconfig \
            ${S}/drivers/net/wireless/rtl88x2bu/Kconfig

    # Adjust path
    sed -i 's/include $(src)\/rtl8822b.mk /include $(TopDIR)\/drivers\/net\/wireless\/rtl88x2bu\/rtl8822b.mk/' \
            ${S}/drivers/net/wireless/rtl88x2bu/Makefile

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8822BU) += rtl88x2bu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl88x2bu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
}

patch_rtl8723ds() {
    bbnote "Adding Wireless drivers for Realtek 8723DS chipsets"

    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl8723ds
    mkdir -p ${S}/drivers/net/wireless/rtl8723ds/
    cp -R ${WORKDIR}/extras/rtl8723ds/core \
            ${S}/drivers/net/wireless/rtl8723ds
    cp -R ${WORKDIR}/extras/rtl8723ds/hal \
            ${S}/drivers/net/wireless/rtl8723ds
    cp -R ${WORKDIR}/extras/rtl8723ds/include \
            ${S}/drivers/net/wireless/rtl8723ds
    cp -R ${WORKDIR}/extras/rtl8723ds/os_dep \
            ${S}/drivers/net/wireless/rtl8723ds
    cp -R ${WORKDIR}/extras/rtl8723ds/platform \
            ${S}/drivers/net/wireless/rtl8723ds

    # Makefile
    cp ${WORKDIR}/extras/rtl8723ds/Makefile \
            ${S}/drivers/net/wireless/rtl8723ds/Makefile

    # Kconfig
    sed -i 's/---help---/help/g' ${WORKDIR}/extras/rtl8723ds/Kconfig
    cp ${WORKDIR}/extras/rtl8723ds/Kconfig \
            ${S}/drivers/net/wireless/rtl8723ds/Kconfig

    # Disable debug
    sed -i "s/^CONFIG_RTW_DEBUG.*/CONFIG_RTW_DEBUG = n/" \
            ${S}/drivers/net/wireless/rtl8723ds/Makefile

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8723DS) += rtl8723ds/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8723ds\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
    
    ${WORKDIR}/process_patch_file.sh ${WORKDIR}/custom-patches/wireless-rtl8723ds.patch
}

patch_rtl8723du() {
    bbnote "Adding Wireless drivers for Realtek 8723DU chipsets"

    cd ${S}
    rm -rf ${S}/drivers/net/wireless/rtl8723du
    mkdir -p ${S}/drivers/net/wireless/rtl8723du/
    cp -R ${WORKDIR}/extras/rtl8723du/core \
            ${S}/drivers/net/wireless/rtl8723du
    cp -R ${WORKDIR}/extras/rtl8723du/hal \
            ${S}/drivers/net/wireless/rtl8723du
    cp -R ${WORKDIR}/extras/rtl8723du/include \
            ${S}/drivers/net/wireless/rtl8723du
    cp -R ${WORKDIR}/extras/rtl8723du/os_dep \
            ${S}/drivers/net/wireless/rtl8723du
    cp -R ${WORKDIR}/extras/rtl8723du/platform \
            ${S}/drivers/net/wireless/rtl8723du

    # Makefile
    cp ${WORKDIR}/extras/rtl8723du/Makefile \
            ${S}/drivers/net/wireless/rtl8723du/Makefile

    # Disable debug
    sed -i "s/^CONFIG_RTW_DEBUG.*/CONFIG_RTW_DEBUG = n/" \
            ${S}/drivers/net/wireless/rtl8723du/Makefile

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8723DU) += rtl8723du/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8723du\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8822BU) += rtl88x2bu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl88x2bu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
    
    ${WORKDIR}/process_patch_file.sh ${WORKDIR}/custom-patches/wireless-rtl8723du.patch
}

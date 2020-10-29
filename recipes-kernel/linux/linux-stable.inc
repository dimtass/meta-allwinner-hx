AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
SECTION = "kernel"
LICENSE = "GPLv2"
COMPATIBLE_MACHINE = "(sun8i|sun50iw2|sun50iw6)"

LIC_FILES_CHKSUM ?= "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

# Install fixup.scr to /boot/
FILES_${KERNEL_PACKAGE_NAME}-fixup = "${KERNEL_IMAGEDEST}/fixup.scr"
PACKAGES_append += "${KERNEL_PACKAGE_NAME}-fixup"

# Pull in the devicetree files and u-boot config files into the rootfs
RDEPENDS_${KERNEL_PACKAGE_NAME}-base += "kernel-devicetree kernel-image kernel-fixup u-boot"

# We need mkimage for the overlays
DEPENDS += "u-boot-tools-native"
do_compile[depends] += "u-boot-tools-native:do_populate_sysroot"

inherit deploy
require recipes-kernel/linux/linux-yocto.inc

LINUX_VERSION ?= ""
PREEMPT_RT_VERSION ?= ""

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"

S = "${WORKDIR}/git"

# Choose the correct path for the defconfig
DEFCONFIG_PATH =  "${@'${SOC_FAMILY}-defconfig' if d.getVar('PREEMPT_RT_VERSION') == '' else '${SOC_FAMILY}-rt-defconfig'}"

ARMBIAN_URI = " \
        file://armbian-patcher.sh \
        file://patches-${LINUX_VERSION} \
        file://custom-patches \
        file://${DEFCONFIG_PATH}/defconfig \
        git://github.com/sfjro/aufs5-standalone.git;name=aufs5;branch=aufs5.7;destsuffix=extras/aufs5 \
        git://git.zx2c4.com/wireguard-linux-compat.git;name=wireguard;destsuffix=extras/wireguard \
        git://github.com/igorpecovnik/realtek-r8152-linux.git;name=rtl8152;destsuffix=extras/rtl8152 \
        git://github.com/jwrdegoede/rtl8189ES_linux.git;name=rtl8189es;destsuffix=extras/rtl8189es \
        git://github.com/jwrdegoede/rtl8189ES_linux.git;name=rtl8189fs;branch=rtl8189fs;destsuffix=extras/rtl8189fs \
        git://github.com/aircrack-ng/rtl8812au.git;branch=v5.6.4.2;name=rtl8812au;destsuffix=extras/rtl8812au \
        git://github.com/karabek/xradio.git;name=xradio;destsuffix=extras/xradio \
        git://github.com/brektrou/rtl8821CU.git;name=rtl8811cu;destsuffix=extras/rtl8811cu \
        git://github.com/aircrack-ng/rtl8188eus.git;branch=v5.7.6.1;name=rtl8188eu;destsuffix=extras/rtl8188eu \
        git://github.com/cilynx/rtl88x2bu.git;branch=5.6.1_30362.20181109_COEX20180928-6a6a;name=rtl88x2bu;destsuffix=extras/rtl88x2bu \
        git://github.com/lwfinger/rtl8723ds.git;name=rtl8723ds;destsuffix=extras/rtl8723ds \
        git://github.com/lwfinger/rtl8723du.git;name=rtl8723du;destsuffix=extras/rtl8723du \
        git://github.com/Mange/rtl8192eu-linux-driver.git;branch=realtek-4.4.x;name=rtl8192eu;destsuffix=extras/rtl8192eu \
"
SRCREV_aufs5 =          "ac34d21c4044d0232f5ff74e5543e3793071c671"
SRCREV_wireguard =      "55385041a8cb74188be566e65d80bf9baecdd2e7"
SRCREV_xradio =         "354e8c32e7948d46a63796d0ca266b1f702999b0"
SRCREV_rtl8152 =        "1ae92418ba8002c6eb77eda34cc0f998fc9ef094"
SRCREV_rtl8189es =      "42f293406700988c10655aaa5ee865b411389aeb"
SRCREV_rtl8189fs =      "86dba26ae963c3c704d0e623d4ea7a6fa020d3f1"
SRCREV_rtl8812au =      "006c821ae82f0675d98db5c569a30591f5fc2a70"
SRCREV_rtl8811cu =      "2bebdb9a35c1d9b6e6a928e371fa39d5fcec8a62"
SRCREV_rtl8188eu =      "f7806db66f015dfe6aab3d4b1b730205d2ef96be"
SRCREV_rtl88x2bu =      "a1c53f43fb9995fbe3ad26567079d6384626d350"
SRCREV_rtl8723ds =      "149dcf11405bd5ab7b2f5dc638b63e060a63d72c"
SRCREV_rtl8723du =      "ac34f36dba33ba5996db1c1e4ade170b2e0ab43b"
SRCREV_rtl8192eu =      "6d2705d8023c25b43f845298e707252fd813e68e"

# Apply the armbian patches and defconfig
do_patch_append() {
    set -x
    set -e
    # Add extra armbian modules in the kernel?
    if [ "${AUFS}" = "yes" ]; then
        patch_aufs5
    fi
    if [ "${WIREGUARD}" = "yes" ]; then
        patch_wireguard
    fi
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
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/custom-patches
    if [ -d "${WORKDIR}/patch-${PREEMPT_RT_VERSION}" ]; then
        echo "Applying preempt-rt patches"
        ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/patches-${LINUX_VERSION}
        ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/patch-${PREEMPT_RT_VERSION}
    else
        ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/patches-${LINUX_VERSION}
    fi
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
    for dtbo in $dtbos; do
        install -m 644 $dtbo ${D}/${KERNEL_IMAGEDEST}/
    done

    bbnote "Extract ${OVERLAY_PREFIX} fixup"
    fixup=$(find . -name "*.scr-cmd" | grep ${OVERLAY_PREFIX})
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
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/extras/aufs5/aufs5-kbuild.patch
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/extras/aufs5/aufs5-base.patch
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/extras/aufs5/aufs5-mmap.patch
    ${WORKDIR}/armbian-patcher.sh ${WORKDIR}/extras/aufs5/aufs5-standalone.patch
    cp -R ${WORKDIR}/extras/aufs5/Documentation .
    cp -R ${WORKDIR}/extras/aufs5/fs .
    cp ${WORKDIR}/extras/aufs5/include/uapi/linux/aufs_type.h include/uapi/linux/
}

patch_wireguard() {
    bbnote "Adding WireGuard"

    cd ${S}
    rm -rf ${S}/net/wireguard
    cp -R ${WORKDIR}/extras/wireguard/src/ ${S}/net/wireguard
    sed -i "/^obj-\\\$(CONFIG_NETFILTER).*+=/a obj-\$(CONFIG_WIREGUARD) += wireguard/" \
            ${S}/net/Makefile
    sed -i "/^if INET\$/a source \"net/wireguard/Kconfig\"" \
            ${S}/net/Kconfig
    # remove duplicates
    [[ $(cat ${S}/net/Makefile | grep wireguard | wc -l) -gt 1 ]] && \
            sed -i '0,/wireguard/{/wireguard/d;}' ${S}/net/Makefile
    [[ $(cat ${S}/net/Kconfig | grep wireguard | wc -l) -gt 1 ]] && \
            sed -i '0,/wireguard/{/wireguard/d;}' ${S}/net/Kconfig
    # headers workaround
    bbnote "Patching WireGuard. Applying workaround for headers compilation"
    sed -i '/mkdir -p "$destdir"/a mkdir -p "$destdir"/net/wireguard; \
    touch "$destdir"/net/wireguard/{Kconfig,Makefile} # workaround for Wireguard' \
            ${S}/scripts/package/builddeb
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
    cp ${WORKDIR}/extras/rtl8189fs/Kconfig \
        ${S}/drivers/net/wireless/rtl8189fs/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8189FS) += rtl8189fs/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8189fs\/Kconfig"' \
        ${S}/drivers/net/wireless/Kconfig

    # Remove all printk
    sed -i 's/printk/\/\//g' ${S}/drivers/net/wireless/rtl8189fs/include/rtw_debug.h
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
    cp ${WORKDIR}/extras/rtl8812au/Kconfig \
            ${S}/drivers/net/wireless/rtl8812au/Kconfig

    # Adjust path
    sed -i 's/include $(src)\/hal\/phydm\/phydm.mk/include $(TopDIR)\/drivers\/net\/wireless\/rtl8812au\/hal\/phydm\/phydm.mk/' \
            ${S}/drivers/net/wireless/rtl8812au/Makefile
    sed -i 's/include $(TopDIR)\/hal\/phydm\/phydm.mk/include $(TopDIR)\/drivers\/net\/wireless\/rtl8812au\/hal\/phydm\/phydm.mk/' \
            ${S}/drivers/net/wireless/rtl8812au/Makefile

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
    cp ${WORKDIR}/extras/rtl8811cu/Kconfig \
            ${S}/drivers/net/wireless/rtl8811cu/Kconfig

    # Address ARM related bug https://github.com/aircrack-ng/rtl8812au/issues/233
    sed -i "s/^CONFIG_MP_VHT_HW_TX_MODE.*/CONFIG_MP_VHT_HW_TX_MODE = n/" \
            ${S}/drivers/net/wireless/rtl8811cu/Makefile

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8821CU) += rtl8811cu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8811cu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
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
    cp ${WORKDIR}/extras/rtl8188eu/Kconfig \
            ${S}/drivers/net/wireless/rtl8188eu/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8188EU) += rtl8188eu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8188eu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
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
    cp ${WORKDIR}/extras/rtl8723ds/Kconfig \
            ${S}/drivers/net/wireless/rtl8723ds/Kconfig

	# Disable debug
	sed -i "s/^CONFIG_RTW_DEBUG.*/CONFIG_RTW_DEBUG = n/" \
		${S}/drivers/net/wireless/rtl8723ds/Makefile

	# Add to section Makefile
	echo "obj-\$(CONFIG_RTL8723DS) += rtl8723ds/" >> ${S}/drivers/net/wireless/Makefile
	sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8723ds\/Kconfig"' \
		${S}/drivers/net/wireless/Kconfig
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
    echo "obj-\$(CONFIG_RTL8822BU) += rtl88x2bu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl88x2bu\/Kconfig"' \
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
    sed -i 's/---help---/help/g' "${WORKDIR}/extras/rtl8192eu/Kconfig"
    cp ${WORKDIR}/extras/rtl8192eu/Kconfig \
            ${S}/drivers/net/wireless/rtl8192eu/Kconfig

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8192EU) += rtl8192eu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl8192eu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
}

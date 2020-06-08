AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
SECTION = "kernel"
LICENSE = "GPLv2"
COMPATIBLE_MACHINE = "(sun8i|sun50iw2|sun50iw6)"

LIC_FILES_CHKSUM ?= "file://COPYING;md5=bbea815ee2795b2f4230826c0c6b8814"

# Pull in the devicetree files and u-boot config files into the rootfs
RDEPENDS_${KERNEL_PACKAGE_NAME}-base += "kernel-devicetree"

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
        git://github.com/sfjro/aufs5-standalone.git;name=aufs5;branch=aufs5.4.3;destsuffix=extras/aufs5 \
        git://git.zx2c4.com/wireguard-linux-compat.git;name=wireguard;destsuffix=extras/wireguard \
        git://github.com/jwrdegoede/rtl8189ES_linux.git;name=rtl8189es;destsuffix=extras/rtl8189es \
        git://github.com/jwrdegoede/rtl8189ES_linux.git;name=rtl8189fs;destsuffix=extras/rtl8189fs \
        git://github.com/aircrack-ng/rtl8812au.git;branch=v5.6.4.2;name=rtl8812au;destsuffix=extras/rtl8812au \
        git://github.com/karabek/xradio.git;name=xradio;destsuffix=extras/xradio \
        git://github.com/brektrou/rtl8821CU.git;name=rtl8811cu;destsuffix=extras/rtl8811cu \
        git://github.com/aircrack-ng/rtl8188eus.git;branch=v5.7.6.1;name=rtl8188eu;destsuffix=extras/rtl8188eu \
        git://github.com/cilynx/rtl88x2bu.git;branch=5.6.1_30362.20181109_COEX20180928-6a6a;name=rtl88x2bu;destsuffix=extras/rtl88x2bu \
"
SRCREV_aufs5 =          "11887377b825f6d534bbd496dbba8e47d1b22d6c"
SRCREV_wireguard =      "55385041a8cb74188be566e65d80bf9baecdd2e7"
SRCREV_xradio =         "354e8c32e7948d46a63796d0ca266b1f702999b0"
SRCREV_rtl8189es =      "2c8d44ae26485052f39d933a3a132b3ff395803a"
SRCREV_rtl8189fs =      "2c8d44ae26485052f39d933a3a132b3ff395803a"
SRCREV_rtl8812au =      "df2b8dfd8cb7d9f6cfeb55abaeab8a5372011fc9"
SRCREV_rtl8811cu =      "ca0bcba405f8865e6acd6a6d3cacf93f7e1b8a2c"
SRCREV_rtl8188eu =      "9315815f5a446ba87e5965fde71094b10a00fac7"
SRCREV_rtl88x2bu =      "64aa087c5ead11960425125c955dcfe98233202e"

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
        patch_rtl8189es
        patch_rtl8812au
        patch_xradio
        patch_rtl8811cu
        patch_rtl8188eu
        patch_rtl88x2bu
    fi
    # Copy defconfig
    cp ${WORKDIR}/${DEFCONFIG_PATH}/defconfig ${WORKDIR}/defconfig
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
    oe_runmake -C ${B} CC="${KERNEL_CC}" ${PARALLEL_MAKE} dtbs
}

# deploy the dtbo overlays in the DEPLOYDIR
do_deploy_append() {
    set -x
    dtbos=$(find . -name "*.dtbo" | grep ${OVERLAY_PREFIX})
    for dtbo in $dtbos; do
        install -m 644 $dtbo ${DEPLOYDIR}/
    done

    bbnote "Extract ${OVERLAY_PREFIX} fixup"
    fixup=$(find . -name "*.scr-cmd" | grep ${OVERLAY_PREFIX})
    mkimage -C none -A arm -T script -d $fixup ${WORKDIR}/fixup.scr
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
    cp ${WORKDIR}/extras/rtl8811cu//Makefile \
            ${S}/drivers/net/wireless/rtl8811cu/Makefile
    cp ${WORKDIR}/extras/rtl8811cu//Kconfig \
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
    cp ${WORKDIR}/extras/rtl88x2bu//Makefile \
            ${S}/drivers/net/wireless/rtl88x2bu/Makefile
    cp ${WORKDIR}/extras/rtl88x2bu//Kconfig \
            ${S}/drivers/net/wireless/rtl88x2bu/Kconfig

    # Adjust path
    sed -i 's/include $(src)\/rtl8822b.mk /include $(TopDIR)\/drivers\/net\/wireless\/rtl88x2bu\/rtl8822b.mk/' \
            ${S}/drivers/net/wireless/rtl88x2bu/Makefile

    # Add to section Makefile
    echo "obj-\$(CONFIG_RTL8822BU) += rtl88x2bu/" >> ${S}/drivers/net/wireless/Makefile
    sed -i '/source "drivers\/net\/wireless\/ti\/Kconfig"/a source "drivers\/net\/wireless\/rtl88x2bu\/Kconfig"' \
            ${S}/drivers/net/wireless/Kconfig
}
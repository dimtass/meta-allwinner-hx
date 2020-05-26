#!/bin/bash
# This script is used to detect changes in the armbian repo and then
# it tries to automate the update procedure by copying the necessary
# files.
# ./compile.sh  BOARD=nanopik1plus BRANCH=current KERNEL_ONLY=yes KERNEL_CONFIGURE=no

: ${ARMBIAN:=/rnd2/armbian/build}
: ${ALLWINNER:=/rnd/yocto/allwinner/sources/meta-allwinner-hx}

LINUX_INC_PATH=${ALLWINNER}/recipes-kernel/linux/linux-stable.inc
UBOOT_RECIPE_PATH=${ALLWINNER}/recipes-bsp/u-boot/u-boot_2019.10.bb
LINUX_RECIPE_PATH=${ALLWINNER}/recipes-kernel/linux/linux-megous_5.4.bb

SOURCES=${ARMBIAN}/cache/sources

UBOOT_PATH=${SOURCES}/u-boot/v2019.10
KERNEL_PATH=${SOURCES}/linux-mainline/orange-pi-5.4

AUF5_PATH=${SOURCES}/aufs5/aufs5.4.3
WIREGUARD_PATH=${SOURCES}/wireguard/master
XRADIO_PATH=${SOURCES}/xradio/master
RTL8189ES_PATH=${SOURCES}/rtl8189es/master
RTL8812AU_PATH=${SOURCES}/rtl8812au/v5.6.4.2
RTL8811CU_PATH=${SOURCES}/rtl8811cu/master
RTL8188EU_PATH=${SOURCES}/rtl8188eu/v5.7.6.1
RTL88X2BU_PATH=${SOURCES}/rtl88x2bu/5.6.1_30362.20181109_COEX20180928-6a6a/

ARMBIAN_UBOOT_PATCHES_PATH=${ARMBIAN}/patch/u-boot/u-boot-sunxi/
ALLWINNER_UBOOT_PATCHES_PATH=${ALLWINNER}/recipes-bsp/u-boot/files/patches-sun8i-2019.10/

ARMBIAN_UBOOT_SUN50IW6_PATCHES_PATH=${ARMBIAN}/patch/u-boot/u-boot-sunxi/
ALLWINNER_UBOOT_SUN50IW6_PATCHES_PATH=${ALLWINNER}/recipes-bsp/u-boot/files/patches-sun50iw6-2019.10/

ARMBIAN_KERNEL_PATCHES_PATH=${ARMBIAN}/patch/kernel/sunxi-current/
ALLWINNER_KERNEL_PATCHES_PATH=${ALLWINNER}/recipes-kernel/linux/linux-megous_5.4/patches-5.4/

ARMBIAN_KERNEL_SUNXI_CONFIG_PATH=${ARMBIAN}/config/kernel/linux-sunxi-current.config
ARMBIAN_KERNEL_SUNXI64_CONFIG_PATH=${ARMBIAN}/config/kernel/linux-sunxi64-current.config
ALLWINNER_KERNEL_SUNXI_CONFIG_PATH=${ALLWINNER}/recipes-kernel/linux/linux-megous_5.4/sun8i-defconfig/defconfig
ALLWINNER_KERNEL_SUNXI64_CONFIG_PATH=${ALLWINNER}/recipes-kernel/linux/linux-megous_5.4/sun50iw2-defconfig/defconfig

function check_hash() {
    cd $1
    local HASH=$(git rev-parse HEAD)
    local HASH_EXISTS=$(grep -i "$HASH" $3)
    if [ -z "${HASH_EXISTS}" ]; then
        echo -e "$2: \t ${HASH}"
    fi
}

# Search for new hashes
echo "Checking for changes in hashes..."
check_hash "${AUF5_PATH}"       "aufs5.4"   "${LINUX_INC_PATH}"
check_hash "${WIREGUARD_PATH}"  "wireguard" "${LINUX_INC_PATH}"
check_hash "${XRADIO_PATH}"     "xradio"    "${LINUX_INC_PATH}"
check_hash "${RTL8189ES_PATH}"  "rtl8189es" "${LINUX_INC_PATH}"
check_hash "${RTL8812AU_PATH}"  "rtl8812au" "${LINUX_INC_PATH}"
check_hash "${RTL8811CU_PATH}"  "rtl8811cu" "${LINUX_INC_PATH}"
check_hash "${RTL8188EU_PATH}"  "rtl8188eu" "${LINUX_INC_PATH}"
check_hash "${RTL88X2BU_PATH}"  "rtl88x2bu" "${LINUX_INC_PATH}"
check_hash "${UBOOT_PATH}"      "u-boot"    "${UBOOT_RECIPE_PATH}"
check_hash "${KERNEL_PATH}"     "kernel"    "${LINUX_RECIPE_PATH}"

function check_patches() {
    diff -q -N $1 $2 > /dev/null
    if [ $? -ne 0 ]; then
        echo "Patches are different..."
        while true; do
            read -p "Copy patches?" yn
            case $yn in
                [Yy]* ) echo "Deleting patches..."
                        rm -rf $2/*
                        echo "Copying patches..."
                        cp $1/* $2/
                        break;;
                [Nn]* ) echo "Skipping kernel patches"; break;;
                * ) echo "Please answer yes or no.";;
            esac
        done
    fi
}

# Copy u-boot patches?
echo "Checking u-boot patches..."
echo -e "\tArmbian u-boot patches: ${ARMBIAN_UBOOT_PATCHES_PATH}"
echo -e "\tAllwinner u-boot patches: ${ALLWINNER_UBOOT_PATCHES_PATH}"
check_patches "${ARMBIAN_UBOOT_PATCHES_PATH}" "${ALLWINNER_UBOOT_PATCHES_PATH}"

echo "Checking u-boot sun50iw6 patches..."
echo -e "\tArmbian u-boot patches: ${ARMBIAN_UBOOT_SUN50IW6_PATCHES_PATH}"
echo -e "\tAllwinner u-boot patches: ${ALLWINNER_UBOOT_SUN50IW6_PATCHES_PATH}"
check_patches "${ARMBIAN_UBOOT_SUN50IW6_PATCHES_PATH}" "${ALLWINNER_UBOOT_SUN50IW6_PATCHES_PATH}"

# Copy kernel patches?
echo "Checking kernel patches..."
echo -e "\tArmbian kernel patches: ${ARMBIAN_KERNEL_PATCHES_PATH}"
echo -e "\tAllwinner kernel patches: ${ALLWINNER_KERNEL_PATCHES_PATH}"
check_patches "${ARMBIAN_KERNEL_PATCHES_PATH}" "${ALLWINNER_KERNEL_PATCHES_PATH}"

# Copy config files?
echo "Checking kernel config files..."
cp "${ARMBIAN_KERNEL_SUNXI_CONFIG_PATH}" "${ALLWINNER_KERNEL_SUNXI_CONFIG_PATH}"
cp "${ARMBIAN_KERNEL_SUNXI64_CONFIG_PATH}" "${ALLWINNER_KERNEL_SUNXI64_CONFIG_PATH}"

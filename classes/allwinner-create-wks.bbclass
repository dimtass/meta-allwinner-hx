AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"

include allwinner-wks-defs.inc

wks_build() {

    set -ex
    wks="${IMGDEPLOYDIR}/${IMAGE_BASENAME}.wks"

#### Common for all images
    cat >> "$wks" <<EOF
###
# This file is created by the allwinner-create-wks.bbclass script
# These are the partitions of the allwinner image
# Author: Dimitris Tassopoulos <dimtass@gmail.com>

EOF

#### Partitions specific to sunxi images
    if [ "${SOC_FAMILY}" = "sun8i" ]; then
        bbnote "Creating a wks file for sunxi..."
        cat >> "$wks" <<EOF
part SPL --source rawcopy --sourceparams="file=${SPL_NAME}" --ondisk ${SUNXI_STORAGE_DEVICE} --no-table --align 8
EOF

#### Partitions specific to sunxi64 images
    elif [ "${SOC_FAMILY}" = "sun50iw2" -o "${SOC_FAMILY}" = "sun50iw6" ]; then
        bbnote "Creating a wks file for sunxi64..."
        cat >> "$wks" <<EOF
part SPL --source rawcopy --sourceparams="file=${SPL_NAME}" --ondisk ${SUNXI_STORAGE_DEVICE} --no-table --align 8
part u-boot --source rawcopy --sourceparams="file=${UBOOT_IMAGE}" --ondisk ${SUNXI_STORAGE_DEVICE} --no-table --align 40
EOF
    fi

#### Add boot partition and separate rootfs partition if SUNXI_BOOT_IMAGE is set in allwinner-wks-defs.inc
if [ ! -z "${SUNXI_BOOT_IMAGE}" ]; then
    cat >> "$wks" <<EOF
part /boot --source rawcopy --sourceparams="file=${SUNXI_BOOT_IMAGE}" --ondisk ${SUNXI_STORAGE_DEVICE} --fstype=vfat --label boot --align 2048 --active
part / --source rootfs --ondisk ${SUNXI_STORAGE_DEVICE} --fstype=ext4 --label root --align 1024 --extra-space ${ROOT_EXTRA_SPACE}
EOF
else
    cat >> "$wks" <<EOF
part / --source rootfs --ondisk ${SUNXI_STORAGE_DEVICE} --fstype=ext4 --label root --align 1024 --extra-space ${ROOT_EXTRA_SPACE} --active
EOF
fi
#### Add swap file if SUNXI_SWAP_SIZE is set in allwinner-wks-defs.inc
    if [ ! -z "${SUNXI_SWAP_SIZE}" ]; then
    cat >> "$wks" <<EOF
part swap --size ${SUNXI_SWAP_SIZE} --ondisk ${SUNXI_STORAGE_DEVICE} --label swap1 --fstype=swap --align 1024
EOF
    fi
}

IMAGE_CMD_wksbuild() {
    wks_build
}

addtask do_image_wksbuild before do_rootfs_wicenv after do_image

# Definitions for creating wks images for the allwinner
#
# Author: Dimitris Tassopoulos <dimtass@gmail.com>

# name of boot partition image, empty value combines the boot and rootfs partitions
SUNXI_BOOT_IMAGE ?= "boot.img"

# device will be used in u-boot and /etc/fstab
SUNXI_STORAGE_DEVICE ?= "mmcblk2"

# This is the extra space for the rootfs
ROOT_EXTRA_SPACE ?= "4096"

# if you need swap file then add here the size of it
SUNXI_SWAP_SIZE ?= "512"

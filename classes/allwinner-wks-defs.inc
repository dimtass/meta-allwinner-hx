# Definitions for creating wks images for the allwinner
#
# Author: Dimitris Tassopoulos <dimtass@gmail.com>

# sunxi8 builds a composite image for SPL and u-boot
SUNXI_SPL_NAME = "u-boot-sunxi-with-spl.bin"

# sunxi64 builds an SPL and a u-boot image
SUNXI64_SPL_NAME = "sunxi-spl.bin"
SUNXI64_UBOOT_IMAGE = "u-boot.itb"

SUNXI_BOOT_IMAGE = "boot.img"

# device will be used in u-boot and /etc/fstab
SUNXI_STORAGE_DEVICE = "mmcblk0"

# This is the extra space for the rootfs
ROOT_EXTRA_SPACE = "10240"

# if you need swap file then add here the size of it
SUNXI_SWAP_SIZE = "512"
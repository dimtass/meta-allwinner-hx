require conf/machine/include/arm/arch-armv8a.inc
require conf/machine/include/soc-family.inc
require sun-common.inc

SOC_FAMILY = "sun50iw6"

KERNEL_IMAGETYPE ?= "Image"

UBOOT_BINARY ?= "u-boot.itb"
SPL_BINARY ?= "spl/sunxi-spl.bin"

KERNEL_DEVICETREE ?= "${SUNXI_KERNEL_DEVICETREE}"

OVERLAY_PREFIX ?= "sun50i-h6"

UBOOT_PATCHES ?= "sun50iw6"
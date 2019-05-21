require conf/machine/include/arm/arch-armv8a.inc
require conf/machine/include/soc-family.inc
require allwinner-overlays.inc
require sun-common.inc

SOC_FAMILY = "sun50i"

KERNEL_IMAGETYPE ?= "Image"

UBOOT_BINARY ?= "u-boot.itb"
SPL_BINARY ?= "spl/sunxi-spl.bin"

KERNEL_DEVICETREE ?= "${SUN50I_OVERLAYS} ${SUNXI_KERNEL_DEVICETREE}"

OVERLAY_PREFIX ?= "sun50i-h5"
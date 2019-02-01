require conf/machine/include/tune-cortexa7.inc
require conf/machine/include/soc-family.inc
require allwinner-overlays.inc
require sun-common.inc

SOC_FAMILY = "sun8i"

# Sub-architecture support
MACHINE_SOCARCH_SUFFIX ?= ""
MACHINE_SOCARCH_SUFFIX_sun4i = "-sun4i"

KERNEL_IMAGETYPE ?= "zImage"

SPL_BINARY ?= "u-boot-sunxi-with-spl.bin"

KERNEL_DEVICETREE ?= "${SUN8I_OVERLAYS} ${SUNXI_KERNEL_DEVICETREE}"

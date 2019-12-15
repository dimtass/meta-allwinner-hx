require conf/machine/include/soc-family.inc
require sun-common.inc
require conf/machine/include/tune-cortexa7.inc

SOC_FAMILY = "sun8i"

# Sub-architecture support
MACHINE_SOCARCH_SUFFIX ?= ""
MACHINE_SOCARCH_SUFFIX_sun4i = "-sun4i"

KERNEL_IMAGETYPE ?= "zImage"

SPL_BINARY ?= "u-boot-sunxi-with-spl.bin"

KERNEL_DEVICETREE ?= "${SUNXI_KERNEL_DEVICETREE}"

OVERLAY_PREFIX ?= "sun8i-h3"
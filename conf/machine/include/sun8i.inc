require conf/machine/include/sunxi.inc
require conf/machine/include/sunxi-mali.inc
require conf/machine/include/tune-cortexa7.inc

SOC_FAMILY = "sun8i"
ARMBIAN_DEFCONFIG = "sunxi"

PREFERRED_VERSION_u-boot = "v2018.05%"
PREFERRED_PROVIDER_virtual/kernel = "linux-stable"
PREFERRED_VERSION_linux-stable = "4.17%"
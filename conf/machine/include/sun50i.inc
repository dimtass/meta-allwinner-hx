require conf/machine/include/sunxi64.inc
require conf/machine/include/arm/arch-armv8.inc

SOC_FAMILY = "sun50i"
ARMBIAN_DEFCONFIG = "sunxi64"

PREFERRED_VERSION_u-boot = "v2018.05%"
PREFERRED_PROVIDER_virtual/kernel = "linux-stable"
PREFERRED_VERSION_linux-stable = "4.17%"
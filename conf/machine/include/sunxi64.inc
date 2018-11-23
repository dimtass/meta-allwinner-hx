SOC_FAMILY ??= ""
include conf/machine/include/soc-family.inc
require allwinner-overlays.inc

PREFERRED_PROVIDER_virtual/xserver = "xserver-xorg"
XSERVER = "xserver-xorg \
           xf86-video-fbturbo \
           xf86-input-evdev \
           xf86-input-mouse \
           xf86-input-keyboard"

PREFERRED_PROVIDER_virtual/kernel ?= "linux-stable"
PREFERRED_VERSION_linux-stable ?= "4.14%"
PREFERRED_PROVIDER_u-boot ?= "u-boot"
PREFERRED_PROVIDER_virtual/bootloader ?= "u-boot"
PREFERRED_VERSION_u-boot ?= "v2018.05%"

KERNEL_IMAGETYPE ?= "Image"

IMAGE_CLASSES += "sdcard_image-sunxi"
IMAGE_FSTYPES += "ext4 tar.gz sunxi-sdimg wic.bz2 wic.bmap"
do_image_wic[depends] += " gptfdisk-native:do_populate_sysroot mtools-native:do_populate_sysroot dosfstools-native:do_populate_sysroot"
# Build wic image after sunxi_sdimg, because we need the 'boot/img' file
do_image_wic[recrdeptask] += " do_image_sunxi_sdimg"

MACHINE_EXTRA_RRECOMMENDS = "kernel-modules"

UBOOT_LOCALVERSION = "-g${@d.getVar('SRCPV', True).partition('+')[2][0:7]}"

UBOOT_ENTRYPOINT ?= "0x40008000"
UBOOT_LOADADDRESS ?= "0x400080OB00"

UBOOT_BINARY ?= "u-boot.itb"
SPL_BINARY ?= "spl/sunxi-spl.bin"

SERIAL_CONSOLE ?= "115200 ttyS0"
MACHINE_FEATURES ?= "alsa apm keyboard rtc serial screen usbgadget usbhost vfat"

KERNEL_DEVICETREE ?= "${SUNXI64_OVERLAYS} ${SUNXI_KERNEL_DEVICETREE}"


PREFERRED_PROVIDER_virtual/xserver = "xserver-xorg"
XSERVER = "xserver-xorg \
           xf86-input-evdev \
           xf86-input-mouse \
           xf86-input-keyboard"

IMAGE_CLASSES += "sdcard_image-sunxi"
IMAGE_FSTYPES += "ext4 tar.gz sunxi-sdimg wic.bz2 wic.bmap"

do_image_wic[depends] += " gptfdisk-native:do_populate_sysroot mtools-native:do_populate_sysroot dosfstools-native:do_populate_sysroot"
# Build wic image after sunxi_sdimg, because we need the 'boot/img' file
do_image_wic[recrdeptask] += " do_image_sunxi_sdimg"

UBOOT_LOCALVERSION = "-g${@d.getVar('SRCPV', True).partition('+')[2][0:7]}"

UBOOT_ENTRYPOINT ?= "0x40008000"

LINUX_VERSION_EXTENSION ?= "-allwinner"

SERIAL_CONSOLES ?= "115200;ttyS0 115200;ttyGS0"
MACHINE_FEATURES ?= "alsa apm keyboard rtc serial screen usbgadget usbhost vfat"

# By default allwinner supports video output, unless specific machine conf disables it
BUILD_DESKTOP ?= "yes"
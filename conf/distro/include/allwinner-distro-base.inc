require conf/distro/poky.conf

DISTRO = "allwinner-distro-base"
DISTRO_NAME = "Allwinner Distro Base"
SDK_VENDOR = "-allwinnersdk"

MAINTAINER = "Dimitris Tassopoulos <dimtass@gmail.com>"

TARGET_VENDOR = "-allwinner"

# Allwinner BSP default settings

XSERVER ?= "xserver-xorg \
		   xf86-input-libinput \
		   xf86-video-modesetting"

DISTRO_FEATURES += " \
		alsa \
		usbgadget \
		usbhost \
		bluez5 \
		x11 \
		wayland \
		"

DISTRO_FEATURES_append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"
DISTRO_FEATURES_BACKFILL_CONSIDERED = "sysvinit"
VIRTUAL-RUNTIME_initscripts = ""
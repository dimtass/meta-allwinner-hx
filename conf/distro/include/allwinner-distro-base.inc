require conf/distro/poky.conf

DISTRO = "allwinner-distro-base"
DISTRO_NAME = "Allwinner Distro Base"
SDK_VENDOR = "-allwinnersdk"

MAINTAINER = "Dimitris Tassopoulos <dimtass@gmail.com>"

TARGET_VENDOR = "-allwinner"

# Allwinner BSP default settings

VIRTUAL-RUNTIME_init_manager = "systemd"

XSERVER ?= "xserver-xorg \
		   xf86-input-libinput \
		   xf86-video-modesetting"

DISTRO_FEATURES += " \
		alsa \
		usbgadget \
		usbhost \
		bluez5 \
		systemd \
		x11 \
		wayland \
		"
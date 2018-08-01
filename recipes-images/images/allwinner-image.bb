SUMMARY = "Allwinner console image"
LICENSE = "MIT"
AUTHOR = "Dimitris Tassopoulos"

inherit core-image

EXTRA_IMAGE_FEATURES = ""
CORE_IMAGE_EXTRA_INSTALL_append = " packagegroup-core-ssh-dropbear"
IMAGE_FEATURES = " debug-tweaks "
IMAGE_LINGUAS = "en-us"

IMAGE_INSTALL += " \
    tar \
    bash \
    merge-files \
    wget \
    allwinner-performance \
"

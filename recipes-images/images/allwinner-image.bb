SUMMARY = "Allwinner console image"
LICENSE = "MIT"
AUTHOR = "Dimitris Tassopoulos"

inherit core-image
inherit allwinner-create-wks
include package-groups.inc

# Add the wks creation class. This will enable the custom
# wks file creation per image
IMAGE_FSTYPES += " wksbuild"

EXTRA_IMAGE_FEATURES = ""
CORE_IMAGE_EXTRA_INSTALL_append = " packagegroup-core-ssh-dropbear"
IMAGE_FEATURES = " debug-tweaks "
IMAGE_LINGUAS = "en-us"

EXTRA_PKGS = " \
    ${WIFI_SUPPORT} \
    xr819 \
"

IMAGE_INSTALL += " \
    ${TEST_TOOLS} \
    ${EXTRA_PKGS} \
    tar \
    bash \
    merge-files \
    wget \
    allwinner-performance \
    procps \
    udev-automount \
    usbreset \
"

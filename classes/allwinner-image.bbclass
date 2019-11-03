SUMMARY = "Allwinner console image"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
LICENSE = "MIT"

inherit core-image
inherit allwinner-create-wks
require package-groups.inc

# Add the wks creation class. This will enable the custom
# wks file creation per image
IMAGE_CLASSES += "sdcard_image-sunxi"
IMAGE_FSTYPES += "ext4 tar.gz sunxi-sdimg wic.bz2 wic.bmap wksbuild"

CORE_IMAGE_EXTRA_INSTALL_append = " packagegroup-core-ssh-openssh"
IMAGE_FEATURES += "package-management \
            debug-tweaks \
            hwcodecs \
            ssh-server-openssh \
            "
IMAGE_LINGUAS = "en-us"


# Packages specific to allwinner
SUNXI_PKGS = " \
    allwinner-performance \
    default-modules \
"

# Most of the package groups are located in the classes/package-groups.inc
IMAGE_INSTALL += " \
    ${STANDARD_PKGS} \
    ${TEST_TOOLS} \
    ${WIFI_SUPPORT} \
    ${SUNXI_PKGS} \
"

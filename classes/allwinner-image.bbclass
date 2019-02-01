SUMMARY = "Allwinner console image"
AUTHOR = "Dimitris Tassopoulos <dimtass@gmail.com>"
LICENSE = "MIT"

inherit core-image
inherit allwinner-create-wks
require package-groups.inc

# Add the wks creation class. This will enable the custom
# wks file creation per image
IMAGE_FSTYPES += " wksbuild"

EXTRA_IMAGE_FEATURES = ""
CORE_IMAGE_EXTRA_INSTALL_append = " packagegroup-core-ssh-openssh"
IMAGE_FEATURES += "package-management debug-tweaks"
IMAGE_LINGUAS = "en-us"

EXTRA_PKGS = " \
    ${WIFI_SUPPORT} \
    xr819 \
"

IMAGE_INSTALL += " \
    ${STANDARD_PKGS} \
    ${TEST_TOOLS} \
    ${EXTRA_PKGS} \
"

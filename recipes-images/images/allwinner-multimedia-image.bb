SUMMARY = "Allwinner multimedia image"
LICENSE = "MIT"
AUTHOR = "Dimitris Tassopoulos"

inherit allwinner-base-image

IMAGE_INSTALL += " \
    ${MULTIMEDIA_PKGS} \
    ${GRAPHICS_TEST_TOOLS} \
"
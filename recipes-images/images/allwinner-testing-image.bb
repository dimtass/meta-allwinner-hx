SUMMARY = "Allwinner testing image"
LICENSE = "MIT"
AUTHOR = "Dimitris Tassopoulos"

inherit allwinner-base-image

IMAGE_INSTALL += " \
    ${MULTIMEDIA_PKGS} \
    ${GRAPHICS_TEST_TOOLS} \
    ${TEST_TOOLS} \
    ${EXTRA_DEBUG_PKGS} \
    ${KERNEL_EXTRA_INSTALL} \
    ${DEV_SDK_INSTALL} \
    ${GRAPHICS_TEST_TOOLS} \
	packagegroup-core-tools-testapps \
	packagegroup-core-tools-profile \
	packagegroup-core-tools-debug \
	packagegroup-core-buildessential \
    packagegroup-extra-base \
    packagegroup-extra-buildessential \
    packagegroup-extra-debug \
    packagegroup-extra-testing \
"
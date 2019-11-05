SUMMARY = "Allwinner multimedia image"
LICENSE = "MIT"
AUTHOR = "Dimitris Tassopoulos"

inherit allwinner-base-image

IMAGE_INSTALL += " \
    ${MULTIMEDIA_PKGS} \
    ${GRAPHICS_TEST_TOOLS} \
"

# python() {
#     bb.warn('DISTRO_FEATURES: %s' % d.getVar('DISTRO_FEATURES'))
#     bb.warn('%s' % bb.utils.contains("DISTRO_FEATURES", "x11", True, False, d))
#     bb.warn('%s' % bb.utils.contains("DISTRO_FEATURES", "wayland", True, False, d))
#     is_desktop_distro = bb.utils.contains("DISTRO_FEATURES", "x11", True, False, d) or bb.utils.contains("DISTRO_FEATURES", "wayland", True, False, d)
#     bb.fatal("is_desktop_distro: %s" % is_desktop_distro)
#     if d.getVar('BUILD_DESKTOP') == 'no' and is_desktop_distro:
#         bb.fatal("%s doesn't support video output. Please try to build the allwinner-distro-console." % d.getVar('MACHINE'))
# }
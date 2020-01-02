## These are some package groups I'm using
# but are not enabled by default. If you
# want to enable them then add them in the
# IMAGE_INSTALL in allwinner-image.bb

BUSYBOX_SPLIT_SUID = "0"

# Sensors
SENSORS_PKGS = " \
    lmsensors-sensors \
    lmsensors-libsensors \
"

SDK_CONFLICT_PACKAGES = " \
    busybox \
    bash \
    pkgconfig \
"

# Standard packages
STANDARD_PKGS = " \
    systemd \
    tar \
    merge-files \
    wget \
    procps \
    usbreset \
    udev \
    udev-automount \
    udev-python-gpio \
    dfu-util \
    u-boot-fw-utils \
    kernel-base \
    kernel-modules \
"

MULTIMEDIA_PKGS = " \
    gstreamer1.0 \
	gstreamer1.0-plugins-base \
	gstreamer1.0-plugins-good \
	gstreamer1.0-plugins-bad \
	v4l-utils \
"

# Extra debug packages
EXTRA_DEBUG_PKGS = " \
    bc \
    ethtool \
    htop \
    nano \
    pciutils \
    binutils \
    zip \
    unzip \
    util-linux \
    tmux \
    lsof \
    strace \
"

APT_PKGS = " \
    apt \
    apt-repo \
"

# Python2.7 related packages
PYTHON2_PKGS = " \
    python \
    python-pip \
    python-modules \
    python-dbus \
"

# Python3 packages
PYTHON3_PKGS = " \
    python3 \
    python3-pip \
    python3-modules \
"

# Bmap tool used to flash images
BMAPTOOL = " \
    gnupg \
    python3 \
    python3-modules \
    python3-six \
    bzip2 \
    bmap-tools-git \
"

WIFI_SUPPORT = " \
    crda \
    iw \
    wpa-supplicant \
    dnsmasq \
    hostapd \
"

# Packages for ALSA
ALSA_PKGS = " \
    libasound \
    alsa-tools \
    alsa-utils \
    alsa-server \
"

# Kernel packages
KERNEL_EXTRA_INSTALL = " \
    kernel-dev \
    kernel-devsrc \
"

# Dev and SDK packages
DEV_SDK_INSTALL = " \
    packagegroup-core-buildessential \
    gdb \
    gdbserver \
    cmake \
    coreutils \
    diffutils \
    findutils \
    file \
    git \
    git-perltools \
    ldd \
    ninja \
    valgrind \
"

# Testing tools
TEST_TOOLS = " \
    rt-tests \
    stress \
    sysstat \
    perf \
    memtester \
    iperf3 \
	cpufrequtils \
	usbutils \
"

GRAPHICS_TEST_TOOLS = " \
    kmscube \
    glmark2 \
	${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'mesa-demos', '', d)} \
"
## These are some package groups I'm using
# but are not enabled by default. If you
# want to enable them then add them in the
# IMAGE_INSTALL in allwinner-image.bb

# Sensors
SENSORS_PKGS = " \
    lmsensors-sensors \
    lmsensors-libsensors \
"

# Standard packages
STANDARD_PKGS = " \
    udev \
    dfu-util \
    pkgconfig \
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
    wireless-tools \
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
    kernel-modules \
    kernel-dev \
    kernel-devsrc \
"

# Dev and SDK packages
DEV_SDK_INSTALL = " \
    cmake \
    binutils \
    binutils-symlinks \
    coreutils \
    gdb \
    gdbserver \
    diffutils \
    file \
    gettext \
    git \
    ldd \
    libtool \
    make \
    pkgconfig \
    ninja \
    valgrind \
"

# Testing tools
TEST_TOOLS = " \
    rt-tests \
    stress \
    python-pandas \
    crashme \
    sysstat \
    perf \
    lttng-modules \
    memtester \
    iperf3 \
"
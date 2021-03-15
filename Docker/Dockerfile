FROM ubuntu:18.04

LABEL description="Dockerfile for meta-allwinner Yocto builds"

# Usage:
# 1. Install docker
# 2. Build image:
#       docker build --build-arg userid=$(id -u) --build-arg groupid=$(id -g) -t allwinner-yocto-image .
#
# 3. Create a container and run it
#       docker run -it --name allwinner-builder -v $(pwd):/docker -w /docker allwinner-yocto-image bash
#
# 4. When exit the container, then to re-run and attach:
#       docker run allwinner-builder
#       docker attach allwinner-builder

# Enables us to overwrite the user and group ID for the yoctouser. See below
ARG userid=1000
ARG groupid=1000

USER root

# Install dependencies in one command to avoid potential use of previous cache
# like explained here: https://stackoverflow.com/a/37727984
RUN apt-get update && \
    apt-get install -y \
        bc \
        bison \
        build-essential \
        chrpath \
        coreutils \
        cpio \
        curl \
        cvs \
        debianutils \
        diffstat \
        flex \
        g++-multilib \
        gawk \
        gcc-multilib \
        libmpc-dev \
        libgmp-dev \
        git-core \
        graphviz \
        help2man \
        iptables \
        iputils-ping \
        libegl1-mesa \
        libfdt1 \
        libsdl1.2-dev \
        libxml2-utils \
        locales \
        m4 \
        openssh-server \
        patchutils \
        python \
        python-dev \
        python-pip \
        python-pysqlite2 \
        python3 \
        python3-dev \
        python3-git \
        python3-jinja2 \
        python3-pexpect \
        python3-pip \
        pbzip2 \
        qemu-user \
        repo \
        rsync \
        screen \
        socat \
        subversion \
        sudo \
        swig \
        sysstat \
        texinfo \
        tmux \
        unzip \
        wget \
        xterm \
        xz-utils

RUN apt-get clean

RUN pip install --upgrade pip
RUN pip3 install --upgrade pip

# For Yocto bitbake -c testimage XML reporting
RUN pip3 install unittest-xml-reporting

# For git-lfs
# The downloaded script is needed since git-lfs is not available per default for Ubuntu 16.04
RUN curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash && sudo apt-get install -y git-lfs

# en_US.utf8 is required by Yocto sanity check
RUN /usr/sbin/locale-gen en_US.UTF-8
RUN localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8
RUN echo 'export LC_ALL="en_US.UTF-8"' >> /etc/profile
ENV LANG en_US.utf8

RUN useradd -U -m yoctouser

# Make sure the user/groupID matches the UID/GID given to Docker. This is so that mounted
# dirs will get the correct permissions
RUN usermod --uid $userid yoctouser
RUN groupmod --gid $groupid yoctouser
RUN echo 'yoctouser:yoctouser' | chpasswd
RUN echo 'yoctouser ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/yoctouser

USER yoctouser
WORKDIR /home/yoctouser

# Script which allows to pass containers CMD as an argument to timeout command
# in case we need redefine entrypoint '--entrypoint' key can be used durring container start
RUN echo "#!/usr/bin/env bash" >> /home/yoctouser/docker-ep.sh && \
    echo 'exec  timeout --signal=SIGKILL 21600 "$@"' >> /home/yoctouser/docker-ep.sh && \
    chmod +x /home/yoctouser/docker-ep.sh
ENTRYPOINT ["/home/yoctouser/docker-ep.sh"]


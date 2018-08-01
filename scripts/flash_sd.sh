#!/bin/bash
: ${MACHINE:=}
: ${SD:=$1}
: ${IMAGE:=build/tmp/deploy/images/${MACHINE}/sunxi-image-${MACHINE}.wic.bz2}

if [ "$(whoami)" != "root" ]; then
    echo "You need to run the script as root..."
    exit 1
fi

if [ -z "${MACHINE}" ]; then
    cat <<EOF
You need to set which MACHINE image to flash to the SD. eg.:
$ sudo MACHINE=nanopi-k1-plus ./flash_sd.sh
EOF
    exit 1
fi

umount ${SD}*
bmaptool copy ${IMAGE} ${SD}

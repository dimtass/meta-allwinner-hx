inherit image_types

#
# Create an image that can by written onto a SD card using dd.
# Originally written for rasberrypi adapt for the needs of allwinner sunxi based boards
#
# The disk layout used is:
#
#    0                      -> 8*1024                           - reserverd
#    8*1024                 ->                                  - arm combined spl/u-boot or aarch64 spl
#    40*1024                ->                                  - aarch64 u-boot
#    2048*1024              -> BOOT_SPACE                       - bootloader and kernel
#
#

# This image depends on the rootfs image
IMAGE_TYPEDEP_sunxi-sdimg = "${SDIMG_ROOTFS_TYPE}"

# Boot partition volume id
BOOTDD_VOLUME_ID ?= "${MACHINE}"

# Boot partition size [in KiB]
BOOT_SPACE ?= "40960"

# First partition begin at sector 2048 : 2048*1024 = 2097152
IMAGE_ROOTFS_ALIGNMENT = "2048"

# Use an uncompressed ext4 by default as rootfs
SDIMG_ROOTFS_TYPE ?= "ext4"
SDIMG_ROOTFS = "${IMGDEPLOYDIR}/${IMAGE_NAME}.rootfs.${SDIMG_ROOTFS_TYPE}"

do_image_sunxi_sdimg[depends] += " \
			parted-native:do_populate_sysroot \
			mtools-native:do_populate_sysroot \
			dosfstools-native:do_populate_sysroot \
			virtual/kernel:do_deploy \
			virtual/bootloader:do_deploy \
			"

# SD card image name
SDIMG = "${IMGDEPLOYDIR}/${IMAGE_NAME}.rootfs.sunxi-sdimg"

IMAGE_CMD_sunxi-sdimg () {
	set -x

	# Align partitions
	BOOT_SPACE_ALIGNED=$(expr ${BOOT_SPACE} + ${IMAGE_ROOTFS_ALIGNMENT} - 1)
	BOOT_SPACE_ALIGNED=$(expr ${BOOT_SPACE_ALIGNED} - ${BOOT_SPACE_ALIGNED} % ${IMAGE_ROOTFS_ALIGNMENT})

	bbnote "BOOT_SPACE_ALIGNED: ${BOOT_SPACE_ALIGNED}"

	# Create a vfat image with boot files
	rm -f ${WORKDIR}/boot.img
	mkfs.vfat -n "${BOOTDD_VOLUME_ID}" -S 512 -C ${WORKDIR}/boot.img ${BOOT_SPACE}

	mcopy -i ${WORKDIR}/boot.img -s ${DEPLOY_DIR_IMAGE}/${KERNEL_IMAGETYPE}-${MACHINE}.bin ::${KERNEL_IMAGETYPE}
	# Create folder for overlays
	mmd -i ${WORKDIR}/boot.img ::/overlay
	
	# Copy device tree file
	if test -n "${KERNEL_DEVICETREE}"; then
		for DTS_FILE in ${KERNEL_DEVICETREE}; do
			DTS_BASE_NAME=`basename ${DTS_FILE} | awk -F "." '{print $1}'`
			DTS_DIR_NAME=`dirname ${DTS_FILE}`
			# Copy all the dtbo files
			if [ "${DTS_FILE##*.}" = "dtbo" ]; then
				#bbwarn "copy ${DTS_BASE_NAME}.dtbo to boot.img::/${DTS_FILE}"
				mcopy -i ${WORKDIR}/boot.img -s ${DEPLOY_DIR_IMAGE}/${DTS_BASE_NAME}.dtbo ::/overlay/${DTS_BASE_NAME}.dtbo
			fi
			if [ -e ${DEPLOY_DIR_IMAGE}/"${DTS_BASE_NAME}.dtb" ]; then

				if [ ${DTS_FILE} != ${DTS_BASE_NAME}.dtb ]; then
					mmd -i ${WORKDIR}/boot.img ::/${DTS_DIR_NAME}
				fi

				kernel_bin="`readlink ${DEPLOY_DIR_IMAGE}/${KERNEL_IMAGETYPE}-${MACHINE}.bin`"
				kernel_bin_for_dtb="`readlink ${DEPLOY_DIR_IMAGE}/${DTS_BASE_NAME}.dtb | sed "s,$DTS_BASE_NAME,${MACHINE},g;s,\.dtb$,.bin,g"`"
				if [ $kernel_bin = $kernel_bin_for_dtb ]; then
					mcopy -i ${WORKDIR}/boot.img -s ${DEPLOY_DIR_IMAGE}/${DTS_BASE_NAME}.dtb ::/${DTS_FILE}
				fi
			fi
		done
	fi

	if [ -e "${DEPLOY_DIR_IMAGE}/fex.bin" ]; then
		mcopy -i ${WORKDIR}/boot.img -s ${DEPLOY_DIR_IMAGE}/fex.bin ::script.bin
	fi
	if [ -e "${DEPLOY_DIR_IMAGE}/boot.scr" ]; then
		mcopy -i ${WORKDIR}/boot.img -s ${DEPLOY_DIR_IMAGE}/boot.scr ::boot.scr
	fi
	if [ -e "${DEPLOY_DIR_IMAGE}/fixup.scr" ]; then
		mcopy -i ${WORKDIR}/boot.img -s ${DEPLOY_DIR_IMAGE}/fixup.scr ::fixup.scr
	fi
	if [ -e "${DEPLOY_DIR_IMAGE}/allwinnerEnv.txt" ]; then
		mcopy -i ${WORKDIR}/boot.img -s ${DEPLOY_DIR_IMAGE}/allwinnerEnv.txt ::allwinnerEnv.txt
	fi


	# Add stamp file
	echo "${IMAGE_NAME}" > ${WORKDIR}/image-version-info
	mcopy -i ${WORKDIR}/boot.img -v ${WORKDIR}/image-version-info ::

	# Burn Partitions
	dd if=${WORKDIR}/boot.img of=${SDIMG} conv=notrunc seek=1 bs=$(expr ${IMAGE_ROOTFS_ALIGNMENT} \* 1024) && sync && sync
	# If SDIMG_ROOTFS_TYPE is a .xz file use xzcat
	if echo "${SDIMG_ROOTFS_TYPE}" | egrep -q "*\.xz"
	then
		xzcat ${SDIMG_ROOTFS} | dd of=${SDIMG} conv=notrunc seek=1 bs=$(expr 1024 \* ${BOOT_SPACE_ALIGNED} + ${IMAGE_ROOTFS_ALIGNMENT} \* 1024) && sync && sync
	else
		dd if=${SDIMG_ROOTFS} of=${SDIMG} conv=notrunc seek=1 bs=$(expr 1024 \* ${BOOT_SPACE_ALIGNED} + ${IMAGE_ROOTFS_ALIGNMENT} \* 1024) && sync && sync
	fi

	cp ${WORKDIR}/boot.img ${DEPLOY_DIR_IMAGE}/boot.img
}

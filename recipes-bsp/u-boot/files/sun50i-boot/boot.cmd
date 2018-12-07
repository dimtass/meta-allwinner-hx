# Default to (primary) SD
setenv load_addr "0x44000000"
setenv rootdev "mmcblk0p2"
setenv docker_optimizations "on"

rootdev=mmcblk0p2
if itest.b *0x28 == 0x02 ; then
	# U-Boot loaded from eMMC or secondary SD so use it for rootfs too
	echo "U-boot loaded from eMMC or secondary SD"
	rootdev=mmcblk1p2
fi

if test -e mmc 0:1 allwinnerEnv.txt; then
	load mmc 0:1 ${load_addr} allwinnerEnv.txt
	env import -t ${load_addr} ${filesize}
fi

load mmc 0:1 ${kernel_addr_r} Image
load mmc 0:1 ${fdt_addr_r} ${fdtfile}
fdt addr ${fdt_addr_r}
fdt resize 65536

# Load environment file
for overlay_file in ${overlays}; do
	if load mmc 0:1 ${load_addr} overlay/${overlay_file}.dtbo; then
		echo "Applying kernel provided DT overlay ${overlay_file}.dtbo"
		fdt apply ${load_addr} || setenv overlay_error "true"
	fi
done

if test "${overlay_error}" = "true"; then
	echo "Error applying DT overlays, restoring original DT"
	load mmc 0:1 ${fdt_addr_r} ${fdtfile}
else
	if test -e mmc 0:1 fixup.scr; then
		load mmc 0:1 ${load_addr} fixup.scr
		echo "Applying user provided fixup script (fixup.scr)"
		source ${load_addr}
	fi
fi

setenv bootargs "console=${console} root=/dev/${rootdev} rootwait rootfstype=${rootfstype} ${extra_bootargs} loglevel=${verbosity} panic=10"

booti ${kernel_addr_r} - ${fdt_addr_r}
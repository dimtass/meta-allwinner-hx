#!/bin/bash
# This patch code is striped from the armbian distro as
# also the patches for the allwinner H2,H3,H5

patch()
{
	local names=()
	local dirs=(
		$1
	)
	names=()
	#/usr/bin/shopt -s nullglob dotglob

	for dir in "${dirs[@]}"; do
		for patch in ${dir%%:*}/*.patch; do
			names+=($(basename $patch))
		done
	done

	# remove duplicates
	names_s=($(echo "${names[@]}" | tr ' ' '\n' | LC_ALL=C sort -u | tr '\n' ' '))
	# apply patches
	for name in "${names_s[@]}"; do
		for dir in "${dirs[@]}"; do
			if [[ -f ${dir%%:*}/$name ]]; then
				if [[ -s ${dir%%:*}/$name ]]; then
					process_patch_file "${dir%%:*}/$name"
				else
					echo "* ${dir##*:} $name" "skipped"
				fi
				break # next name
			fi
		done
	done
}

process_patch_file()
{
	local patch=$1

	# detect and remove files which patch will create
	/usr/bin/lsdiff -s --strip=1 $patch | /bin/grep '^+' | /usr/bin/awk '{print $2}' | /usr/bin/xargs -I % sh -c 'rm -f %'
	#lsdiff -s --strip=1 $patch | grep '^+' | awk '{print $2}' | xargs -I % sh -c 'echo %'

	echo "Processing file $patch"
	/usr/bin/patch --batch -p1 -N < $patch
	#/usr/bin/patch --batch --dry-run -p1 -N < $patch

	if [[ $? -ne 0 ]]; then
		echo "* $(basename $patch)" "failed" "wrn"
		[[ $EXIT_PATCHING_ERROR == yes ]] && exit_with_error "Aborting due to" "EXIT_PATCHING_ERROR"
	else
		echo "* $(basename $patch)" "" "info"
	fi
}

patch $1

#!/bin/bash
EXIT_PATCHING_ERROR=yes

patch()
{
	local names=()
	local dirs=(
		$1
	)
	names=()
	# /usr/bin/shopt -s nullglob dotglob

	for dir in "${dirs[@]}"; do
		for patch in ${dir%%:*}/*.patch; do
			names+=($(basename $patch))
		done
	done
	# merge static and linked
	names=("${names[@]}")

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

	echo "Processing file $patch"
	/usr/bin/patch --batch --silent -p1 -N < $patch

	if [[ $? -ne 0 ]]; then
		echo "* $(basename $patch)" "failed" "wrn"
		bbwarn "* $(basename $patch) failed wrn"
		[[ $EXIT_PATCHING_ERROR == yes ]] && echo "Aborting due to EXIT_PATCHING_ERROR=${EXIT_PATCHING_ERROR}"
		return 1
	else
		echo "$(basename $patch)" "" "info"
	fi
}

patch $1
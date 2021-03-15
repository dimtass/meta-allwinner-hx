#!/bin/bash
EXIT_PATCHING_ERROR=yes

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

process_patch_file $1

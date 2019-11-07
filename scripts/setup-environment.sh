#!/bin/bash

NCPU=$(grep -c processor /proc/cpuinfo)
CWD=$(pwd)
METALAYER=meta-allwinner-hx

if [ "$(whoami)" = "root" ]; then
    echo "ERROR: do not use the BSP as root. Exiting..."
fi

if [ -z "$MACHINE" ]; then
    echo "ERROR: you have to set the MACHINE. Exiting..."
fi

#Get available images
LIST_IMAGES=$(ls sources/*/recipes-images/*/*.bb | sed s/\.bb//g | sed -r 's/^.+\///' | xargs -I {} echo -e "\t"{})

# Check the machine type specified
LIST_MACHINES=$(ls -1 $CWD/sources/*/conf/machine $CWD/sources/poky/*/conf/machine | grep -F ".conf" | sed s/\.conf//g)
VALID_MACHINE=$(echo -e "$LIST_MACHINES" | grep ${MACHINE} | wc -l )
if [ "x$MACHINE" = "x" ] || [ "$VALID_MACHINE" = "0" ]; then
	echo -e "\nThe \$MACHINE you have specified ($MACHINE) is not supported by this build setup"
    echo -e "\n \$x$MACHINE=(x$MACHINE) and \$VALID_MACHINE=($VALID_MACHINE)"
    echo -e "\n\nLIST:\n$LIST_MACHINES\n\n"
    return 1
else
    if [ ! -e $1/conf/local.conf.sample ]; then
        echo "Configuring for ${MACHINE}"
    fi
fi

# Check the distro
LIST_DISTROS=$(ls -1 $CWD/sources/*/conf/distro | grep -F ".conf" | sed s/\.conf//g | xargs -I {} echo -e "\t"{})
VALID_DISTROS=$(echo -e "$LIST_DISTROS" | grep ${DISTRO} | wc -l )
if [ "x$DISTRO" = "x" ] || [ "$VALID_DISTROS" = "0" ]; then
	echo -e "\nThe \$DISTRO you have specified ($DISTRO) is not supported by this build setup"
    echo -e "\n \$x$DISTRO=(x$DISTRO) and \$VALID_DISTROS=($VALID_DISTROS)"
    echo -e "\n\nLIST:\n$LIST_DISTROS\n\n"
    return 1
else
    if [ ! -e $1/conf/local.conf.sample ]; then
        echo "Configuring for distro ${DISTRO}"
    fi
fi

OEROOT=$(pwd)/sources/poky
if [ -e sources/oe-core ]; then
    OEROOT=sources/oe-core
fi

. $OEROOT/oe-init-build-env $CWD/$1 > /dev/null

# if conf/local.conf not generated, no need to go further
if [ ! -e conf/local.conf ]; then
    echo "conf/local.conf failed to generate. Exiting..."
	return 1
fi

export PATH="$(echo $PATH | sed 's/\(:.\|:\)*:/:/g;s/^.\?://;s/:.\?$//')"

generated_config=
if [ ! -e conf/local.conf.sample ]; then

    # Replace conf/bblayers.conf and conf/local.conf with the platform's ones
    cp -f $CWD/sources/${METALAYER}/conf/bblayers.conf.sample $CWD/$1/conf/bblayers.conf
    cp -f $CWD/sources/${METALAYER}/conf/local.conf.sample $CWD/$1/conf/local.conf

    # Change settings according environment
    sed -e "s,MACHINE ?=.*,MACHINE = '$MACHINE',g" \
        -i conf/local.conf
    sed -e "s,DISTRO ?=.*,DISTRO = '$DISTRO',g" \
        -i conf/local.conf

    # Append in conf the number of cpus
    cat >> conf/local.conf <<EOF
BB_NUMBER_THREADS = "$NCPU"
PARALLEL_MAKE = "-j $NCPU"
EOF

    generated_config=1
fi

# Run append configurations from other compatible meta-layers.
# This helps with BSP layers that they need custom configuration and
# run their own setup scripts.
echo -e "Searching for append setup scripts...\n"
extra_setup_scripts=$(ls -1 $CWD/sources/*/setup-environment-append.sh)
for extra_script in $extra_setup_scripts; do
	if ! basename $extra_script | grep ${MACHINE}; then
		echo "Run append script: $extra_script"
		$extra_script $(dirname $extra_script)
	fi
done

echo -e "These are the default supported images:\n${LIST_IMAGES}"

echo -e "\nThese are the supported distros:\n${LIST_DISTROS}"
cat <<EOF

You can now build your image. To build the allwinner-console-image then run this:
$ bitbake allwinner-console-image

EOF

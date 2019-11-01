#!/bin/bash
CWD=$(pwd)

#Get available images
LIST_MACHINES=$(ls -1 $CWD/sources/meta-allwinner-hx/conf/machine | grep -F ".conf" | sed s/\.conf//g)

echo "Supported machines are: "
echo "----"
echo "${LIST_MACHINES}"
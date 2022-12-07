#!/bin/sh
#-------------------------------
# Setup environments for Ubuntu
# - Library path
#-------------------------------


#setting OS
FLAVOR=`grep '^ID=' /etc/os-release | awk -F= '{print $2}' | tr -d '"'`
if [ $FLAVOR == "ubuntu" ]; then
	export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
fi

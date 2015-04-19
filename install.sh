#!/bin/sh

OPTS=`getopt -o t:v:p:d: --long target:,version:,prefix:,dest-dir: -n 'install.sh' -- "$@"`
if [ $? != 0 ]
then
    exit 1
fi

eval set -- "$OPTS"

TARGET=bindump
VERSION=`./version.sh`
PREFIX=/usr
DESTDIR=/usr

while true ; do
    case "$1" in
	-t|--target) TARGET=$2; shift 2;;
	-v|--version) VERSION=$2; shift 2;;
	-p|--prefix) PREFIX=$2; shift 2;;
	-d|--dest-dir) DESTDIR=$2; shift 2;;
	--) shift; break;;
    esac
done

# Removing previous versions
rm -f ${DESTDIR}/bin/bindump
rm -f ${DESTDIR}/bin/bd

# Copying executable to /usr/bin
cp bindump ${DESTDIR}/bin
# Creating symlink for short name
ln -s ${DESTDIR}/bin/bindump ${DESTDIR}/bin/bd

# Set execution privileges for each user
chmod oga+x ${DESTDIR}/bin/bindump

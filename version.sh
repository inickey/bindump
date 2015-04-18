#!/bin/sh

if [ -d .git ] ; then
    VERSION=`git describe --long --tags | sed 's/v\([0-9]*\)\.\([0-9]*\)-\([0-9]*\).*/\1\.\2\.\3/g'`
else if [ -f .version ] ; then
        VERSION=`cat .version`
     else
        VERSION='0.0.0'
     fi
fi

echo $VERSION > .version
echo $VERSION

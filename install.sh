#!/bin/sh

# Removind previous versions
rm -f /usr/bin/bindump
rm -f /usr/bin/bd

# Copying executable to /usr/bin
cp bindump /usr/bin
# Creating symlink for short name
ln -s /usr/bin/bindump /usr/bin/bd

# Set execution privileges for each user
chmod oga+x /usr/bin/bindump

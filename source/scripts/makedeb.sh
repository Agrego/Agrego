#!/bin/bash

#
# make deb package
#

ENDPOINT=$1
TMPDIR="/tmp/agrego-$ENDPOINT"
PACKAGE_FILE="agrego-$ENDPOINT.deb"

usage()
{
	if [ "$1" ] # string is not empty
	then
		echo "error: $1"
	fi

	echo "Usage:"
	echo "$0 (local|distant) [source_dir] "
	exit 1
}

if [ $# -ne 2 ]
then
	if [ $# -ne 1 ]
	then
		usage "wrong number of argument"
	fi
fi

if [ "$1" != "local" ]
then
	if [ "$1" != "distant" ]
	then
		usage "$1 is not a valid endpoint name, must local or distant"
	fi
fi

SOURCE_DIR="."
if [ $# -eq 2 ]
then
	if [ ! -d "$2" ]
	then
		usage "$2 is not a valid directory"
	fi

	SOURCE_DIR="$2"
fi

# replace @@ENDPOINT@@ by $ENDPOINT in $1
set_endpoint()
{
	sed -i "s/@@ENDPOINT@@/$ENDPOINT/" $1
}

BIN_DIR=$SOURCE_DIR/bin
SCRIPT_DIR=$SOURCE_DIR/scripts #TODO move it to $SOURCE_DIR/deb
if [ ! -d "$BIN_DIR" -o ! -d "$SCRIPT_DIR" ]
then
	echo "Error: $SOURCE_DIR is not an Agrego source directory."
	echo "It should contains directory bin and script."
	exit 1
fi

if [ ! -x "$BIN_DIR/agrego-$ENDPOINT" ]
then
	echo "Error: $BIN_DIR/agrego-$ENDPOINT doesn't exists, did you compile it ?"
	exit 1
fi

if [ -d $TMPDIR ]
then
	rm -r $TMPDIR
fi
mkdir -p $TMPDIR

# put deb specific files into DEBIAN directory
mkdir -p $TMPDIR/DEBIAN/
cp $SCRIPT_DIR/control-$ENDPOINT $TMPDIR/DEBIAN/control

for FILE in "prerm" "postrm" "postinst"
do
	cp $SCRIPT_DIR/$FILE $TMPDIR/DEBIAN/
	chmod 755 $TMPDIR/DEBIAN/$FILE
	set_endpoint $TMPDIR/DEBIAN/$FILE
done

# create directory architecture
mkdir -p $TMPDIR/etc/agrego-$ENDPOINT
touch $TMPDIR/etc/agrego-$ENDPOINT/startup
mkdir -p $TMPDIR/etc/logrotate.d
mkdir -p $TMPDIR/etc/init.d
mkdir -p $TMPDIR/var/run/agrego-$ENDPOINT/
mkdir -p $TMPDIR/var/log/agrego-$ENDPOINT/
mkdir -p $TMPDIR/usr/sbin/
mkdir -p $TMPDIR/usr/share/doc/agrego-$ENDPOINT/
TPLDIR=$TMPDIR/usr/share/agrego-$ENDPOINT
mkdir -p $TPLDIR

# put template files in the template dir
cp $SCRIPT_DIR/agrego.ini $TPLDIR/
cp $SCRIPT_DIR/start.sh   $TPLDIR/
set_endpoint $TPLDIR/start.sh
cp $SCRIPT_DIR/stop.sh    $TPLDIR/
set_endpoint $TPLDIR/stop.sh

# copy init.d file
cp $SCRIPT_DIR/agrego.init  $TMPDIR/etc/init.d/agrego-$ENDPOINT
set_endpoint $TMPDIR/etc/init.d/agrego-$ENDPOINT
chmod +x $TMPDIR/etc/init.d/agrego-$ENDPOINT

# copy binaries
cp $BIN_DIR/agrego-$ENDPOINT $TMPDIR/usr/sbin/
cp $SCRIPT_DIR/agrego-manage-endpoint.sh $TMPDIR/usr/sbin/agrego-manage-$ENDPOINT.sh
set_endpoint $TMPDIR/usr/sbin/agrego-manage-$ENDPOINT.sh
chmod +x $TMPDIR/usr/sbin/*

cp $SCRIPT_DIR/logrotate_script $TMPDIR/etc/logrotate.d/agrego-$ENDPOINT

# add man pages
MAN_DIR=$TMPDIR/usr/share/man/man8
mkdir -p $MAN_DIR

cp $SCRIPT_DIR/agrego-endpoint.8 $MAN_DIR/agrego-$ENDPOINT.8
set_endpoint $MAN_DIR/agrego-$ENDPOINT.8
gzip $MAN_DIR/agrego-$ENDPOINT.8

cp $SCRIPT_DIR/agrego-manage-endpoint.8 $MAN_DIR/agrego-manage-$ENDPOINT.8
set_endpoint $MAN_DIR/agrego-manage-$ENDPOINT.8
gzip $MAN_DIR/agrego-manage-$ENDPOINT.8

# TODO add documentation files - HOWTO_RUN_AGREGO
#DOC_DIR=$TMP_DIR/usr/share/doc/agrego-$ENDPOINT
#mkdir -p $DOC_DIR

# create the .deb package
dpkg-deb --build $TMPDIR $SCRIPT_DIR

# remove temporary files
rm -r $TMPDIR

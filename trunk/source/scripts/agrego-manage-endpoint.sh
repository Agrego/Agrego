#!/bin/bash

#
# add or remove agrego instance
#

# initialize some var
ENDPOINT="@@ENDPOINT@@"
ETC_DIR="/etc/agrego-$ENDPOINT"
INIT_DIR="/etc/init.d"
TPL_DIR="/usr/share/agrego-$ENDPOINT"

# fill optionnal parameter
FORCE=0
QUIET=0

# check availability of commands needed
RM=`which "rm"`
if [ ! "$RM" ]; then echo "rm command not found"; ERROR="1"; fi

MKDIR=`which "mkdir"`
if [ ! "$MKDIR" ]; then echo "mkdir command not found"; ERROR="1"; fi

CP=`which "cp"`
if [ ! "$CP" ]; then echo "cp command not found"; ERROR="1"; fi

CHMOD=`which "chmod"`
if [ ! "$CHMOD" ]; then echo "chmod command not found"; ERROR="1"; fi

HOSTNAME=`which "hostname"`
if [ ! "$HOSTNAME" ]; then echo "hostname command not found"; ERROR="1"; fi

DATE=`which "date"`
if [ ! "$DATE" ]; then echo "date command not found"; ERROR="1"; fi

OPENSSL=`which "openssl"`
if [ ! "$OPENSSL" ]; then echo "openssl command not found"; ERROR="1"; fi

# if one is missing exit
if [ "$ERROR" ]; then exit 1; fi

usage()
{
	if [ "$1" ] # string is not empty
	then
		echo "error: $1"
		echo ""
	fi

	echo "Usage: $0 [-f|--force] [-q|--quiet] (list|delall)"
	echo "       $0 [-f|--force] [-q|--quiet] (add|del) agrego_name"
	exit 1
}

# parse arguments
TEMP=`getopt -o fq --longoptions force,quiet -n "$0" -- "$@"`
# if error on argument parsing
if [ $? != 0 ] ; then usage; fi

eval set -- "$TEMP"

while true ; do
	case "$1" in
		-f|--force) FORCE=1 ; shift ;;
		-q|--quiet) QUIET=1 ; shift ;;
		--) shift ; break ;;
		*) echo "Internal error!" ; exit 1 ;;
	esac
done

# retrieve last arguments
CPT=0
for ARG
do
      ARGS[$CPT]="$ARG"
      CPT=`expr $PCT + 1`
done

ACTION=${ARGS[0]}
# TODO check that AGREGO_NAME contains only a-zA-Z0-9_-
AGREGO_NAME=${ARGS[1]}

if [ -z "$ACTION" ]
then
	usage "bad parameter"
fi

# utils function
write_question()
{
	if [ $FORCE -eq 0 ]
	then
		echo "$1 [Y/n] ?"
	fi
}

do_echo()
{
	if [ $QUIET -eq 0 ]
	then
		echo $1
	fi
}

check_answer()
{
	# check if -f| --force has been gave in arguments
	if [ $FORCE -eq 1 ]
	then
		echo 1
		return
	fi

	read ANS

	# allow empty answer or y or Y
	if [ -z "$ANS" -o "$ANS" = "y" -o "$ANS" = "Y" ]
	then
		echo 1
		return
	fi
	echo 0
}

# declare function (each coreespond to one action)
agrego_list()
{
	for AGREGO_NAME in $(ls $ETC_DIR)
	do
		if [ -d "$ETC_DIR/$AGREGO_NAME" ]
		then
			echo -n "$AGREGO_NAME "
		fi
	done
	echo ""
}

agrego_add()
{
	if [ -d "$ETC_DIR/$AGREGO_NAME" ]
	then
		do_echo "This name already exists, you can remove it using the script agrego-del.sh"
		exit 1
	fi

	do_echo "Create agrego: $AGREGO_NAME"

	$MKDIR $ETC_DIR/$AGREGO_NAME
	$CP $TPL_DIR/start.sh $ETC_DIR/$AGREGO_NAME/
	sed -i "s/@@NAME@@/$AGREGO_NAME/" $ETC_DIR/$AGREGO_NAME/start.sh
	$CP $TPL_DIR/stop.sh $ETC_DIR/$AGREGO_NAME/
	sed -i "s/@@NAME@@/$AGREGO_NAME/" $ETC_DIR/$AGREGO_NAME/stop.sh
	$CP $TPL_DIR/agrego.ini $ETC_DIR/$AGREGO_NAME/
	sed -i "s/@@NAME@@/$AGREGO_NAME/" $ETC_DIR/$AGREGO_NAME/agrego.ini

	# add private key
	DT="`$DATE`"
	PVKEY=`$OPENSSL passwd "$DT" 2>/dev/null | tr / @`
	sed -i "s/@@PRIVATE_KEY@@/$PVKEY/" $ETC_DIR/$AGREGO_NAME/agrego.ini

	# launch this Agrego instance at boot
	write_question "start $AGREGO_NAME at startup"
	if [ `check_answer` -eq 1 ]
	then
		echo $AGREGO_NAME >> $ETC_DIR/startup
	else
		do_echo ""
		do_echo "To launch the Agrego $AGREGO_NAME daemon, launch:"
		do_echo "/etc/init.d/agrego-$ENDPOINT start"
		do_echo ""
		do_echo "To launch $AGREGO_NAME at startup, add the next line into the file $ETC_DIR/startup"
		do_echo $AGREGO_NAME
	fi

	do_echo ""
	do_echo "To remove $AGREGO_NAME launch:"
	do_echo "$0 del $AGREGO_NAME"
}

agrego_del()
{
	if [ ! -d "$ETC_DIR/$AGREGO_NAME" ]
	then
		echo "There is no agrego with this name, nothing to do."
		exit 0
	fi


	do_echo "Following directory will be removed:"
	do_echo "$ETC_DIR/$AGREGO_NAME"
	do_echo ""

	write_question "Are you sure"
	if [ `check_answer` -eq 0 ]
	then
		return 0
	fi

	$RM -r $ETC_DIR/$AGREGO_NAME/

	# remove log file if it exist
	FILE="/var/log/agrego-$ENDPOINT/$AGREGO_NAME.log"
	if [ -x $FILE ]
	then
		$RM $FILE
	fi
}

agrego_delall()
{
	for AGREGO_NAME in $(agrego_list)
	do
		agrego_del
	done
}

# if action is list number of argument must be 1
if [ "$ACTION" = "list" ]
then
	if [ -n "$AGREGO_NAME" ]
	then
		usage "action $ACTION don't take any argument"
	fi
	agrego_list
	exit 0
fi

if [ "$ACTION" = "delall" ]
then
	if [ -n "$AGREGO_NAME" ]
	then
		usage "action $ACTION don't take any argument"
	fi
	agrego_delall
	exit 0
fi

# action add and del
if [ -z "$AGREGO_NAME" ]
then
	usage "action $1 need the agrego_name argument"
fi


if [ "$ACTION" = "add" ]
then
	agrego_add
	exit 0
fi

if [ "$ACTION" = "del" ]
then
	agrego_del
	exit 0
fi

usage "$ACTION action not found"

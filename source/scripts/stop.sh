#!/bin/bash

# AgreGo stop script.
#
# This script is launched just after the binary AgreGo is stoped by the AgreGo
# init.d script.
#
# It's usefull to manage NAT, and route

INIFILE=@@sysconfdir_@@/agrego-@@ENDPOINT@@/@@NAME@@/agrego.ini

# simple function to parse agrego.ini file
# for example to get the value of the variable my_var
# my_var=`INI_GET my_var`
INI_GET()
{
	grep -Eo "^$1 *= *(.+)$" $INIFILE | sed "s/^$1 *= *//g"
}

exit 0

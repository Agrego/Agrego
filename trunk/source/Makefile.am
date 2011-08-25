## @start 1
## Makefile.am -- Process this file with automake to produce Makefile.in
## @end 1
## Copyright (C) 2004, 2006 Gary V. Vaughan
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
## MA 02110-1301, USA.

MAINTAINERCLEANFILES	= Makefile.in

## CFLAGS:

# -ansi -D_POSIX_SOURCE  // Commented for now, awaiting understanding of the problems it causes

# -lrt for timer functions

#-D_POSIX_SOURCE
# else I have :
# implicit declaration of function ‘kill’
# if I drop '-ansi' it works too

#--pedantic-errors
# We can't use --pedantic-errors because there is a bug a the file
# /usr/include/bits/resource.h:161: error: comma at end of enumerator list
# the error doesn't appears without --pedantic-errors

## @start 1

## to fill : man_MANS

include_HEADERS = ./iniparser/dictionary.h ./iniparser/iniparser.h ./cunit/xcunit.h ./cunit/agrego_cunit.h ./headers/log_save.h ./headers/connexiontab.h ./headers/ping.h ./headers/xlib.h ./headers/tuntap.h ./headers/agregate.h ./headers/agrego.h ./headers/xiniparser.h ./headers/handshake.h ./headers/connexion.h ./headers/endpoint.h

AM_CFLAGS = -O2 -W -Wall -Werror -pedantic

sbin_PROGRAMS = agrego-local agrego-distant

common_src = ./connexion.c ./agrego.c ./log_save.c ./tuntap.c ./ping.c ./xlib.c ./agregate.c ./connexiontab.c ./iniparser/dictionary.c ./iniparser/iniparser.c ./handshake.c ./xiniparser.c
local_src = ./local/endpoint.c ./local/handshake.c $(common_src)
distant_src = ./distant/endpoint.c ./distant/handshake.c $(common_src)

agrego_local_SOURCES = ./main.c $(local_src)
agrego_distant_SOURCES = ./main.c $(distant_src)

if HAVE_CUNIT
  ## cunit:
  check_PROGRAMS = cunit-local cunit-distant

  cunit_local_SOURCES = cunit/handshake.c cunit/main.c cunit/xcunit.c $(local_src)
  cunit_distant_SOURCES = cunit/handshake.c cunit/main.c cunit/xcunit.c $(distant_src)

  TESTS = $(check_SCRIPTS) $(check_PROGRAMS)
endif

## @end 1

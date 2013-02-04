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

AM_CFLAGS = -O2 -W -Wall -Werror -pedantic

builddir = ./build

sbin_PROGRAMS = agrego-local agrego-distant
sbin_SCRIPTS = agrego-manage-local.sh agrego-manage-distant.sh
man_MANS = agrego-local.8 agrego-distant.8 agrego-manage-local.8 agrego-manage-distant.8
sysconf_DATA = initd-agrego-local initd-agrego-distant
data_DATA = start-local.sh stop-local.sh start-distant.sh stop-distant.sh agrego-local.ini agrego-distant.ini

# file to add to the dist (archive)
dist_noinst_DATA = scripts/agrego-manage-endpoint.sh scripts/agrego.ini scripts/agrego.init \
	scripts/agrego-endpoint.8 scripts/agrego-manage-endpoint.8 scripts/start.sh scripts/stop.sh

common_src = ./connexion.c ./agrego.c ./log_save.c ./tuntap.c ./ping.c ./xlib.c \
						 ./agregate.c ./connexiontab.c ./iniparser/dictionary.c ./iniparser/iniparser.c \
						 ./handshake.c ./xiniparser.c ./iniparser/dictionary.h ./iniparser/iniparser.h \
						 ./cunit/xcunit.h ./cunit/agrego_cunit.h ./headers/log_save.h ./headers/connexiontab.h \
						 ./headers/ping.h ./headers/xlib.h ./headers/tuntap.h ./headers/agregate.h \
						 ./headers/agrego.h ./headers/xiniparser.h ./headers/handshake.h ./headers/connexion.h ./headers/endpoint.h

local_src = ./local/endpoint.c ./local/handshake.c $(common_src)
distant_src = ./distant/endpoint.c ./distant/handshake.c $(common_src)

agrego_local_SOURCES = ./main.c $(local_src)
agrego_distant_SOURCES = ./main.c $(distant_src)

if HAVE_CUNIT
  ## cunit:
  check_PROGRAMS = cunit-local cunit-distant
ck-ck
  cunit_local_SOURCES = cunit/handshake.c cunit/main.c cunit/xcunit.c $(local_src)
  cunit_distant_SOURCES = cunit/handshake.c cunit/main.c cunit/xcunit.c $(distant_src)

  TESTS = $(check_SCRIPTS) $(check_PROGRAMS)
endif

## @@sysconfdir@@ is always replaced by @${sysconfdir}@, it's the same for other autotools vars

agrego-manage-endpoint.sh:
	sed "s#@@sysconfdir_@@#${sysconfdir}#g" ${srcdir}/scripts/agrego-manage-endpoint.sh > $@
	sed -i "s#@@datadir_@@#${datadir}#g" $@

# sbin scripts
agrego-manage-local.sh: agrego-manage-endpoint.sh
	sed "s/@@ENDPOINT@@/local/g" agrego-manage-endpoint.sh > $@

agrego-manage-distant.sh: agrego-manage-endpoint.sh
	sed "s/@@ENDPOINT@@/distant/g" agrego-manage-endpoint.sh > $@

# manual pages
agrego-local.8:
	sed "s/@@ENDPOINT@@/local/g" ${srcdir}/scripts/agrego-endpoint.8 > $@

agrego-distant.8:
	sed "s/@@ENDPOINT@@/distant/g" ${srcdir}/scripts/agrego-endpoint.8 > $@

agrego-manage-local.8:
	sed "s/@@ENDPOINT@@/local/g" ${srcdir}/scripts/agrego-manage-endpoint.8 > $@

agrego-manage-distant.8:
	sed "s/@@ENDPOINT@@/distant/g" ${srcdir}/scripts/agrego-manage-endpoint.8 > $@

# configuration file
agrego-local.ini:
	sed "s/@@ENDPOINT@@/local/g" ${srcdir}/scripts/agrego.ini > $@

agrego-distant.ini:
	sed "s/@@ENDPOINT@@/distant/g" ${srcdir}/scripts/agrego.ini > $@

start.sh stop.sh:
	sed "s#@@sysconfdir_@@#$(sysconfdir)#g" ${srcdir}/scripts/$@ > $@

start-local.sh: start.sh
	sed "s/@@ENDPOINT@@/local/g" ${srcdir}/scripts/start.sh > $@

start-distant.sh: start.sh
	sed "s/@@ENDPOINT@@/distant/g" ${srcdir}/scripts/start.sh > $@

stop-local.sh: stop.sh
	sed "s/@@ENDPOINT@@/local/g" ${srcdir}/scripts/stop.sh > $@

stop-distant.sh: stop.sh
	sed "s/@@ENDPOINT@@/distant/g" ${srcdir}/scripts/stop.sh > $@

# startup files
agrego.init:
	sed "s#@@sbindir_@@#${sbindir}#g" ${srcdir}/scripts/agrego.init > $@
	sed -i "s#@@sysconfdir_@@#${sysconfdir}#g" $@
	sed -i "s#@@sharedstatedir_@@#${sharedstatedir}#g" $@

initd-agrego-local: agrego.init
	sed "s/@@ENDPOINT@@/local/g" agrego.init > $@

initd-agrego-distant: agrego.init
	sed "s/@@ENDPOINT@@/distant/g" agrego.init > $@

install-exec-hook:
	cd $(DESTDIR)$(sysconfdir) && \
		[ -d agrego-local ] || mkdir -p agrego-local && \
		[ -d agrego-distant ] || mkdir -p agrego-distant && \
		[ -d init.d ] || mkdir init.d && \
		mv initd-agrego-local init.d/agrego-local && \
		mv initd-agrego-distant init.d/agrego-distant && \
		chmod +x init.d/agrego-{local,distant}

install-data-hook:
	[ -d $(DESTDIR)$(sharedstatedir) ] || mkdir -p $(DESTDIR)$(sharedstatedir)/run/agrego-{local,distant}
	[ -d $(DESTDIR)$(datadir) ] || mkdir -p $(DESTDIR)$(datadir) && \
		cd $(DESTDIR)$(datadir) && \
		[ -d agrego-local ] || mkdir agrego-local && \
		[ -d agrego-distant ] || mkdir agrego-distant && \
		mv agrego-local.ini agrego-local/agrego.ini && \
		mv agrego-distant.ini agrego-distant/agrego.ini && \
		mv start-local.sh agrego-local/start.sh && \
		mv stop-local.sh agrego-local/stop.sh && \
		mv start-distant.sh agrego-distant/start.sh && \
		mv stop-distant.sh agrego-distant/stop.sh

uninstall-hook:
	cd $(DESTDIR)$(datadir) && \
		rm agrego-{local,distant}/st{art,op}.sh && \
		rm agrego-{local,distant}/agrego.ini && \
		rmdir agrego-{local,distant}
	cd $(DESTDIR)$(sysconfdir) && \
		rm init.d/agrego-{local,distant} && \
		rmdir agrego-{local,distant}

# cleaning

mostlyclean-local:
	rm $(sbin_SCRIPTS) $(man_MANS) $(sysconf_DATA) $(data_DATA) agrego.init agrego-manage-endpoint.sh

## @end 1

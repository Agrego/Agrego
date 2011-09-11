#http://www.freesoftwaremagazine.com/books/agaal/autotools_example
dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.67)
AC_INIT([agrego], [0.2], [agrego@googlegoups.com])

# give automake options
## subdir-objects to put each object files in the same (sub)dirs as the c source they correspond
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])

AC_CONFIG_SRCDIR([connexion.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
# need when use : AUTOMAKE_OPTIONS = subdir-objects in Makefile.am
AM_PROG_CC_C_O

# set the language
AC_LANG(C)

# Checks for libraries.
# TODO add  crypto for debian testing
AC_SEARCH_LIBS([SHA1], [ssl crypto], [], [
        echo "Error! libssl not found."
        exit -1
        ])
AC_SEARCH_LIBS([timer_create], [rt], [], [
        echo "Error! librt not found."
        exit -1
        ])

# add option to add or not cunit support
AC_MSG_CHECKING([whether to enable cunit])
AC_ARG_WITH(cunit,
    [  --with-cunit             enable cunit support],
    with_cunit=$withval,
    with_cunit=no)
AC_MSG_RESULT($with_cunit)

if test $with_cunit = "yes"; then
  # This add "-lcunit" to LIBS if it exists
  AC_SEARCH_LIBS([CU_initialize_registry], [cunit])
fi

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset select socket strchr strerror strtol])

# generatemacro HAVE_CUNIT
AM_CONDITIONAL([HAVE_CUNIT],[test "x$with_cunit" == "xyes"])

# TODO add doxygen

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "
  ($PACKAGE_NAME) version $PACKAGE_VERSION
  Prefix.........: $prefix
  C Compiler.....: $CC $CFLAGS $CPPFLAGS
  Linker.........: $LD $LDFLAGS $LIBS
  CUnit..........: ${CUNIT:-NONE} ${HAVE_CUNIT:-NONE}
"

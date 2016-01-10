AC_PREREQ([2.67])
AC_INIT([libatgpio],[0.1.0])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wno-portability 1.14 foreign no-dist-gzip dist-xz subdir-objects silent-rules])
AM_MAINTAINER_MODE([enable])
dnl AM_SILENT_RULES([yes])

AC_PROG_CC

LT_PREREQ([2.2])
LT_INIT([disable-static])

PKG_CHECK_MODULES(GLIB, [glib-2.0])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

lib_LTLIBRARIES = libatgpio.la

libatgpio_la_SOURCES = src/atgpio.c
libatgpio_la_CFLAGS = $(GLIB_CFLAGS)
libatgpio_la_LDFLAGS = $(GLIB_LIBS)
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = samples/a840-sw
samples_a840_sw_CFLAGS = $(GLIB_CFLAGS) -Isrc
samples_a840_sw_LDFLAGS = $(GLIB_LIBS)
samples_a840_sw_LDADD = libatgpio.la

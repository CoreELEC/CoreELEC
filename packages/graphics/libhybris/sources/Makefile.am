SUBDIRS = include common properties hardware gralloc

if HAS_ANDROID_4_2_0
SUBDIRS += libsync
endif
if HAS_ANDROID_5_0_0
SUBDIRS += libsync
endif
SUBDIRS += egl glesv1 glesv2

MAINTAINERCLEANFILES = \
	aclocal.m4 compile config.guess config.sub \
	configure depcomp install-sh ltmain.sh     \
	Makefile.in missing config.h.in            \
	mkinstalldirs *~ \
	$(NULL)

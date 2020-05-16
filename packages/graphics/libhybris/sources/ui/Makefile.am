lib_LTLIBRARIES = \
	libui.la

libui_la_SOURCES = ui.c
libui_la_CFLAGS = -I$(top_srcdir)/include $(ANDROID_HEADERS_CFLAGS)
if WANT_TRACE
libui_la_CFLAGS += -DDEBUG
endif
if WANT_DEBUG
libui_la_CFLAGS += -ggdb -O0
endif
libui_la_LDFLAGS = \
	$(top_builddir)/common/libhybris-common.la \
	-version-info "1":"0":"0"

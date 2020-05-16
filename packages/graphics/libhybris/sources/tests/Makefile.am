bin_PROGRAMS = \
	test_audio \
	test_egl \
	test_egl_configs \
	test_glesv2 \
	test_glesv3 \
	test_sf \
	test_sensors \
	test_input \
	test_lights \
	test_camera \
	test_vibrator \
	test_media \
	test_recorder \
	test_gps \
	test_opencl \
	test_wifi \
	test_hwcomposer \
	test_nfc

test_audio_SOURCES = test_audio.c
test_audio_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/include/android
test_audio_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/hardware/libhardware.la

test_egl_SOURCES = test_egl.cpp
test_egl_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-I$(top_srcdir)/egl/platforms/common
test_egl_CXXFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/egl/platforms/common \
	-I$(top_srcdir)/egl/platforms/hwcomposer \
	-I$(top_srcdir)/libsync \
	-std=gnu++11
if WANT_MESA
test_egl_CXXFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
if HAS_HWCOMPOSER2_HEADERS
test_egl_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=1
else
test_egl_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=0
endif

if WANT_MESA
test_egl_CFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
test_egl_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/libEGL.la

test_egl_configs_SOURCES = test_egl_configs.c
test_egl_configs_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)
test_egl_configs_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/libEGL.la

test_glesv2_SOURCES = test_glesv2.cpp test_common.cpp
test_glesv2_CXXFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/egl/platforms/common \
	-I$(top_srcdir)/egl/platforms/hwcomposer \
	-I$(top_srcdir)/libsync \
	-std=gnu++11 \
	-DUSE_HWCOMPOSER=1
if WANT_MESA
test_glesv2_CXXFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
test_glesv2_LDADD = \
	-lm \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/platforms/hwcomposer/libhybris-hwcomposerwindow.la \
	$(top_builddir)/egl/platforms/common/libhybris-eglplatformcommon.la \
	$(top_builddir)/egl/libEGL.la \
	$(top_builddir)/glesv2/libGLESv2.la \
	$(top_builddir)/libsync/libsync.la
if HAS_HWCOMPOSER2_HEADERS
test_glesv2_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=1
test_glesv2_LDADD += \
	$(top_builddir)/hwc2/libhwc2.la
else
test_glesv2_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=0
endif

test_glesv3_SOURCES = test_glesv3.cpp test_common.cpp
test_glesv3_CXXFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/egl/platforms/common \
	-I$(top_srcdir)/egl/platforms/hwcomposer \
	-I$(top_srcdir)/libsync \
	-std=gnu++11 \
	-DUSE_HWCOMPOSER=1
if WANT_MESA
test_glesv3_CXXFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
test_glesv3_LDADD = \
	-lm \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/platforms/hwcomposer/libhybris-hwcomposerwindow.la \
	$(top_builddir)/egl/platforms/common/libhybris-eglplatformcommon.la \
	$(top_builddir)/egl/libEGL.la \
	$(top_builddir)/glesv2/libGLESv2.la \
	$(top_builddir)/libsync/libsync.la
if HAS_HWCOMPOSER2_HEADERS
test_glesv3_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=1
test_glesv3_LDADD += \
	$(top_builddir)/hwc2/libhwc2.la
else
test_glesv3_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=0
endif

test_hwcomposer_SOURCES = test_hwcomposer.cpp test_common.cpp
test_hwcomposer_CXXFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/egl/platforms/common \
	-I$(top_srcdir)/egl/platforms/hwcomposer \
	-I$(top_srcdir)/libsync \
	-DUSE_HWCOMPOSER=1

if WANT_MESA
test_hwcomposer_CXXFLAGS += -DLIBHYBRIS_WANTS_MESA_X11_HEADERS
endif
test_hwcomposer_LDADD = \
	-lm \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/platforms/common/libhybris-eglplatformcommon.la \
	$(top_builddir)/egl/libEGL.la \
	$(top_builddir)/glesv2/libGLESv2.la \
	$(top_builddir)/hardware/libhardware.la

if HAS_HWCOMPOSER2_HEADERS
test_hwcomposer_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=1
test_hwcomposer_LDADD += \
	$(top_builddir)/hwc2/libhwc2.la
else
test_hwcomposer_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=0
endif


if HAS_ANDROID_4_2_0
test_hwcomposer_LDADD += \
	$(top_builddir)/libsync/libsync.la \
	$(top_builddir)/egl/platforms/hwcomposer/libhybris-hwcomposerwindow.la
else
if HAS_ANDROID_5_0_0
test_hwcomposer_LDADD += \
	$(top_builddir)/libsync/libsync.la \
	$(top_builddir)/egl/platforms/hwcomposer/libhybris-hwcomposerwindow.la
endif
endif

test_sensors_SOURCES = test_sensors.c
test_sensors_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)
test_sensors_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/hardware/libhardware.la

test_lights_SOURCES = test_lights.c
test_lights_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)

test_lights_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/hardware/libhardware.la

test_ui_SOURCES = test_ui.c
test_ui_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)
test_ui_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/ui/libui.la

test_sf_SOURCES = test_sf.cpp test_common.cpp
test_sf_CXXFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)
test_sf_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/libEGL.la \
	$(top_builddir)/glesv2/libGLESv2.la \
	$(top_builddir)/sf/libsf.la

test_input_SOURCES = test_input.c
test_input_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)
test_input_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/input/libis.la

test_camera_SOURCES = test_camera.cpp test_common.cpp
test_camera_CXXFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/egl/platforms/common \
	-I$(top_srcdir)/egl/platforms/hwcomposer \
	-I$(top_srcdir)/libsync \
	-std=gnu++11
if HAS_HWCOMPOSER2_HEADERS
test_camera_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=1
else
test_camera_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=0
endif
if WANT_WAYLAND_EGL
test_camera_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/libEGL.la \
	$(top_builddir)/glesv2/libGLESv2.la \
	$(top_builddir)/camera/libcamera.la \
	$(top_builddir)/input/libis.la \
	$(top_builddir)/egl/platforms/common/libwayland-egl.la \
	$(WAYLAND_CLIENT_LIBS)
else
test_camera_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/libEGL.la \
	$(top_builddir)/glesv2/libGLESv2.la \
	$(top_builddir)/camera/libcamera.la \
	$(top_builddir)/input/libis.la \
	$(WAYLAND_EGL_LIBS)
	$(WAYLAND_CLIENT_LIBS)
endif

test_media_SOURCES = test_media.cpp test_common.cpp
test_media_CFLAGS = \
	-I$(top_srcdir)/include
test_media_CXXFLAGS = \
	$(test_media_CFLAGS)
if HAS_HWCOMPOSER2_HEADERS
test_media_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=1
else
test_media_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=0
endif
test_media_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/libEGL.la \
	$(top_builddir)/glesv2/libGLESv2.la \
	$(top_builddir)/media/libmedia.la \
	$(top_builddir)/sf/libsf.la

test_recorder_SOURCES = test_recorder.cpp test_common.cpp
test_recorder_CFLAGS = \
	-I$(top_srcdir)/include
test_recorder_CXXFLAGS = \
	$(test_recorder_CFLAGS)
if HAS_HWCOMPOSER2_HEADERS
test_recorder_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=1
else
test_recorder_CXXFLAGS += \
	-DHAS_HWCOMPOSER2_HEADERS=0
endif
test_recorder_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/egl/libEGL.la \
	$(top_builddir)/glesv2/libGLESv2.la \
	$(top_builddir)/media/libmedia.la \
	$(top_builddir)/camera/libcamera.la \
	$(top_builddir)/input/libis.la \
	$(top_builddir)/sf/libsf.la

test_gps_SOURCES = test_gps.c
test_gps_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)
test_gps_LDFLAGS = -pthread
test_gps_LDADD =  \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/hardware/libhardware.la

test_nfc_SOURCES = test_nfc.c
test_nfc_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS) \
	-Wno-error=unused-function

if HAS_LIBNFC_NXP_HEADERS
test_nfc_CFLAGS += \
        -DHAS_LIBNFC_NXP_HEADERS=1
else
test_nfc_CFLAGS += \
        -DHAS_LIBNFC_NXP_HEADERS=0
endif

test_nfc_LDFLAGS = -pthread
test_nfc_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/libnfc_nxp/libnfc_nxp.la \
	$(top_builddir)/hardware/libhardware.la

test_vibrator_SOURCES = test_vibrator.c
test_vibrator_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)
if HAS_VIBRATOR_HEADER
test_vibrator_CFLAGS += -DHAS_VIBRATOR_HEADER=1
else
test_vibrator_CFLAGS += -DHAS_VIBRATOR_HEADER=0
endif
test_vibrator_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/hardware/libhardware.la \
	$(top_builddir)/vibrator/libvibrator.la

test_wifi_SOURCES = test_wifi.c
test_wifi_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)

if HAS_WIFI_LEGACY_HEADERS
test_wifi_CFLAGS += \
        -DHAS_WIFI_LEGACY_HEADERS=1
else
test_wifi_CFLAGS += \
        -DHAS_WIFI_LEGACY_HEADERS=0
endif

test_wifi_LDADD = \
	$(top_builddir)/wifi/libwifi.la

test_opencl_SOURCES = test_opencl.c
test_opencl_CFLAGS = \
	-I$(top_srcdir)/include \
	$(ANDROID_HEADERS_CFLAGS)
test_opencl_LDADD = \
	$(top_builddir)/common/libhybris-common.la \
	$(top_builddir)/opencl/libOpenCL.la

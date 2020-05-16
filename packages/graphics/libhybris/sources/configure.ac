AC_INIT([libhybris], [0.1.0])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# If library source has changed since last release, increment revision
# If public symbols have been added, removed or changed since last release,
#  increment current and set revision to 0
# If public symbols have been added since last release, increment age
# If public symbols have been removed since last release, set age to 0
m4_define([hybris_lt_current], [1])
m4_define([hybris_lt_revision], [0])
m4_define([hybris_lt_age], [0])

AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_GNU_SOURCE
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

PKG_PROG_PKG_CONFIG

LT_CURRENT=hybris_lt_current
LT_REVISION=hybris_lt_revision
LT_AGE=hybris_lt_age
AC_SUBST([LT_CURRENT])
AC_SUBST([LT_REVISION])
AC_SUBST([LT_AGE])

AC_CHECK_LIB(dl, dlopen, dummy=yes, AC_MSG_ERROR(dynamic linking loader is required))
AC_CHECK_LIB(rt, clock_gettime, dummy=yes, AC_MSG_ERROR(librt is required))

AC_ARG_ENABLE(debug,
  [  --enable-debug            Enable debug build (default=disabled)],
  [debug=$enableval],
  [debug="no"])
AM_CONDITIONAL( [WANT_DEBUG], [test x"$debug" = x"yes"])

AC_ARG_ENABLE(experimental,
  [  --enable-experimental     Enable experimental features (default=disabled)],
  [experimental=$enableval],
  [experimental="no"])
AM_CONDITIONAL( [WANT_EXPERIMENTAL], [test x"$experimental" = x"yes"])

AC_ARG_ENABLE(adreno_quirks,
  [  --enable-adreno-quirks     Enable adreno quirks (default=disabled)],
  [adreno_quirks=$enableval],
  [adreno_quirks="no"])
AM_CONDITIONAL( [WANT_ADRENO_QUIRKS], [test x"$adreno_quirks" = x"yes"])

AC_ARG_ENABLE(trace,
  [  --enable-trace            Enable TRACE statements (default=disabled)],
  [trace=$enableval],
  [trace="no"])
AM_CONDITIONAL( [WANT_TRACE], [test x"$trace" = x"yes"])

AC_ARG_ENABLE(arm_tracing,
  [  --enable-arm-tracing            Enable ARM (32-bit only) tracing, useful for understanding what the blob is doing, currently only the mm linker is supported. (default=disabled)],
  [arm_tracing=$enableval],
  [arm_tracing="no"])
AM_CONDITIONAL([WANT_ARM_TRACING], [test x"$arm_tracing" = x"yes"])

AC_ARG_ENABLE(mesa,
  [  --enable-mesa            Enable mesa headers (default=disabled)],
  [mesa=$enableval],
  [mesa="no"])
AM_CONDITIONAL( [WANT_MESA], [test x"$mesa" = x"yes"])

AC_ARG_ENABLE(ubuntu-camera-headers,
  [  --enable-ubuntu-camera-headers            Enable Ubuntu camera headers (default=disabled)],
  [ubuntu_camera_headers=$enableval],
  [ubuntu_camera_headers="no"])
AM_CONDITIONAL( [WANT_UBUNTU_CAMERA_HEADERS], [test x"$ubuntu_camera_headers" = x"yes"])

WANT_WAYLAND_EGL="no"

AC_ARG_ENABLE(wayland,
  [  --enable-wayland            Enable wayland support (default=disabled)],
  [wayland=$enableval
	PKG_CHECK_MODULES(WAYLAND_CLIENT, wayland-client,, exit)
	PKG_CHECK_MODULES(WAYLAND_SERVER, wayland-server,, exit)
	PKG_CHECK_MODULES(WAYLAND_EGL, [wayland-egl >= 1.15], , [WANT_WAYLAND_EGL="yes"])
	AC_DEFINE(WANT_WAYLAND_EGL, [], [We use internal Wayland EGL support])
	WAYLAND_PREFIX=`$PKG_CONFIG --variable=prefix wayland-client`

	AC_PATH_PROG([WAYLAND_SCANNER], [wayland-scanner],, [${WAYLAND_PREFIX}/bin$PATH_SEPARATOR$PATH])
	AC_DEFINE(WANT_WAYLAND, [], [We want Wayland support])
],
  [wayland="no"])
AM_CONDITIONAL( [WANT_WAYLAND], [test x"$wayland" = x"yes"])
AM_CONDITIONAL( [WANT_WAYLAND_EGL], [test x"$WANT_WAYLAND_EGL" = x"yes"])

AC_ARG_ENABLE(wayland_serverside_buffers,
  [  --enable-wayland_serverside_buffers            Enable serverside buffer allocation for wayland (default=enabled)],
  [wayland_serverside_buffers=$enableval],
  [wayland_serverside_buffers=yes])
AM_CONDITIONAL( [WANT_WL_SERVERSIDE_BUFFERS], [test x"$wayland_serverside_buffers" = x"yes"])

AC_ARG_ENABLE(property_cache,
  [  --enable-property-cache      Enable runtime android property cache (default=disabled)],
  [property_cache=$enableval],
  [property_cache=no])
AM_CONDITIONAL( [WANT_RUNTIME_PROPERTY_CACHE], [test x"$property_cache" = x"yes"])

AC_ARG_ENABLE(ubuntu-linker-overrides,
  [  --enable-ubuntu-linker-overrides  Enable Ubuntu linker overrides (default=disabled)],
  [ubuntu_linker_overrides=$enableval],
  [ubuntu_linker_overrides=no])
AM_CONDITIONAL([WANT_UBUNTU_LINKER_OVERRIDES], [test x"$ubuntu_linker_overrides" = x"yes"])

AC_ARG_ENABLE(arch,
  [  --enable-arch[=arch]     Compile specific CPU target(default=arm)
                              arm: compile for ARM
                              x86: Compile for x86],
  [ if   test "x$enableval" = "xarm" ; then
      arch="arm"
   elif test "x$enableval" = "xarm64" ; then
      arch="arm64"
   elif test "x$enableval" = "xx86" ; then
      arch="x86"
   elif test "x$enableval" = "xx86-64" ; then
      arch="x86-64"
   else
      echo
      echo "Error!"
      echo "Unknown architecture' type"
      exit 1
   fi
  ],
  [arch="arm"]
  )
AM_CONDITIONAL([WANT_ARCH_ARM], [test x$arch = xarm])
AM_CONDITIONAL([WANT_ARCH_ARM64], [test x$arch = xarm64])
AM_CONDITIONAL([WANT_ARCH_X86], [test x$arch = xx86])
AM_CONDITIONAL([WANT_ARCH_X86_64], [test x$arch = xx86-64])

DEFAULT_EGL_PLATFORM="null"
AC_ARG_WITH(default-egl-platform,
  [  --with-default-egl-platform=NAME     Use NAME as default EGL platform if not specified by environment ],
  [ DEFAULT_EGL_PLATFORM="$withval" ],
  [ ])
AC_SUBST(DEFAULT_EGL_PLATFORM)

if test "x$arch" = "xarm" -o "x$arch" = "xx86"; then
  DEFAULT_HYBRIS_LD_LIBRARY_PATH="/vendor/lib:/system/lib:/odm/lib"
else
  DEFAULT_HYBRIS_LD_LIBRARY_PATH="/vendor/lib64:/system/lib64:/odm/lib64"
fi
AC_ARG_WITH(default-hybris-ld-library-path,
  [  --with-default-hybris-ld-library-path=PATH1:PATH2:...     Use PATH1:PATH2 for default HYBRIS_LD_LIBRARY_PATH if not specified by environment ],
  [ DEFAULT_HYBRIS_LD_LIBRARY_PATH="$withval" ],
  [ ])
AC_SUBST(DEFAULT_HYBRIS_LD_LIBRARY_PATH)

AC_ARG_WITH(android-headers,
  [  --with-android-headers=DIR     Use android headers available in DIR. See utils/extract-headers.sh ],
  [
    AM_CONDITIONAL([HAS_LIBNFC_NXP_HEADERS], [test -f $withval/libnfc-nxp/phLibNfc.h])
    AM_CONDITIONAL([HAS_WIFI_LEGACY_HEADERS], [test -f $withval/hardware_legacy/wifi.h])
    AM_CONDITIONAL([HAS_HWCOMPOSER2_HEADERS], [test -f $withval/hardware/hwcomposer2.h])
    AM_CONDITIONAL([HAS_GRALLOC1_HEADER], [test -f $withval/hardware/gralloc1.h])
    AM_CONDITIONAL([HAS_VIBRATOR_HEADER], [test -f $withval/hardware_legacy/vibrator.h])
    AS_IF([test -f $withval/libnfc-nxp/phLibNfc.h], [ANDROID_HEADERS_CFLAGS="-I$withval -I$withval/libnfc-nxp"],[ANDROID_HEADERS_CFLAGS="-I$withval"])
    AC_SUBST([ANDROID_HEADERS_CFLAGS])
  ],
  [
    PKG_CHECK_MODULES(ANDROID_HEADERS, android-headers,, exit)
    AM_CONDITIONAL([HAS_LIBNFC_NXP_HEADERS], [false])
    AM_CONDITIONAL([HAS_WIFI_LEGACY_HEADERS], [false])
    AM_CONDITIONAL([HAS_HWCOMPOSER2_HEADERS], [false])
    AM_CONDITIONAL([HAS_GRALLOC1_HEADER], [false])
    AM_CONDITIONAL([HAS_VIBRATOR_HEADER], [false])
  ]
)
CPPFLAGS="$CPPFLAGS $ANDROID_HEADERS_CFLAGS"

AC_CHECK_HEADERS(android-config.h,,AC_MSG_ERROR(required header file is missing))
AC_CHECK_HEADERS(android-version.h,,AC_MSG_ERROR(required header file is missing))

# AC_AWK_CPP
AC_DEFUN([AC_AWK_CPP],
[AC_LANG_PREPROC_REQUIRE()dnl
AC_REQUIRE([AC_PROG_AWK])dnl
AC_LANG_CONFTEST([AC_LANG_SOURCE([[$2]])])

ac_awk_cpp_out="`$CPP $CPPFLAGS conftest.$ac_ext|$AWK '$1'`"
AS_IF([test "x$ac_awk_cpp_out" = x], [$4], [$3])
rm -f conftest*
])# AC_AWK_CPP


# According to http://en.wikipedia.org/wiki/Android_version_history#Version_history_by_API_level
###########################
AC_AWK_CPP([/has_ANDROID_VERSION_MAJOR/ {print \$2;}],
[
#include <android-version.h>
has_ANDROID_VERSION_MAJOR ANDROID_VERSION_MAJOR
],
[android_headers_major=$ac_awk_cpp_out],
[AC_MSG_ERROR(required 'major' version is missing)])


###########################
AC_AWK_CPP([/has_ANDROID_VERSION_MINOR/ {print \$2;}],
[
#include <android-version.h>
has_ANDROID_VERSION_MINOR ANDROID_VERSION_MINOR
],
[android_headers_minor=$ac_awk_cpp_out],
[AC_MSG_ERROR(required 'minor' version is missing)])

###########################
AC_AWK_CPP([/has_ANDROID_VERSION_PATCH/ {print \$2;}],
[
#include <android-version.h>
has_ANDROID_VERSION_PATCH ANDROID_VERSION_PATCH
],
[android_headers_patch=$ac_awk_cpp_out],
[AC_MSG_ERROR(require 'patch' version is missing)])

AC_MSG_NOTICE("Android headers version is $android_headers_major.$android_headers_minor.$android_headers_patch.$ac_awk_cpp_out")
###########################
AC_SUBST(ANDROID_VERSION_MAJOR, [$android_headers_major])
AC_SUBST(ANDROID_VERSION_MINOR, [$android_headers_minor])
AC_SUBST(ANDROID_VERSION_PATCH, [$android_headers_patch])

AC_MSG_NOTICE("Android headers version is $android_headers_major.$android_headers_minor.$android_headers_patch")

# Add automake tests for version/API needs here that you need in code, including test .am's
AM_CONDITIONAL([HAS_ANDROID_9_0_0], [test $android_headers_major -ge 9 -a $android_headers_minor -ge 0 ])
AM_CONDITIONAL([HAS_ANDROID_8_0_0], [test $android_headers_major -ge 8 -a $android_headers_minor -ge 0 ])
AM_CONDITIONAL([HAS_ANDROID_7_0_0], [test $android_headers_major -ge 7 -a $android_headers_minor -ge 0 ])
AM_CONDITIONAL([HAS_ANDROID_6_0_0], [test $android_headers_major -ge 6 -a $android_headers_minor -ge 0 ])
AM_CONDITIONAL([HAS_ANDROID_5_0_0], [test $android_headers_major -ge 5 -a $android_headers_minor -ge 0 ])
AM_CONDITIONAL([HAS_ANDROID_4_2_0], [test $android_headers_major -ge 4 -a $android_headers_minor -ge 2 ])
AM_CONDITIONAL([HAS_ANDROID_4_1_0], [test $android_headers_major -ge 4 -a $android_headers_minor -ge 1 ])
AM_CONDITIONAL([HAS_ANDROID_4_0_3], [test $android_headers_major -ge 4 -a $android_headers_patch -ge 3 ])
AM_CONDITIONAL([HAS_ANDROID_4_0_0], [test $android_headers_major -ge 4 -a $android_headers_minor -ge 0 ])
AM_CONDITIONAL([HAS_ANDROID_2_3_0], [test $android_headers_major -ge 2 -a $android_headers_minor -ge 3 ])

AC_ARG_ENABLE(mali-quirks,
  [  --enable-mali-quirks            Enable Mali GPU driver quirks (default=disabled)],
  [mali_quirks=$enableval],
  [mali_quirks="no"])
AM_CONDITIONAL([WANT_MALI_QUIRKS], [test x"$mali_quirks" = x"yes"])

AC_ARG_ENABLE(stub-linker,
  [  --enable-stub-linker            Enable a stub linker which does nothing (default=disabled)],
  [stub_linker=$enableval],
  [stub_linker="no"])
AM_CONDITIONAL([WANT_STUB_LINKER], [test x"$stub_linker" = x"yes"])

AC_CONFIG_FILES([
	Makefile
	properties/Makefile
	properties/libandroid-properties.pc
	camera/Makefile
	camera/libcamera.pc
	common/Makefile
	common/jb/Makefile
	common/stub/Makefile
	common/mm/Makefile
	common/n/Makefile
	common/o/Makefile
	egl/egl.pc
	egl/Makefile
	egl/platforms/Makefile
	egl/platforms/common/Makefile
	egl/platforms/common/hybris-egl-platform.pc
	egl/platforms/null/Makefile
	egl/platforms/fbdev/Makefile
	egl/platforms/wayland/Makefile
	egl/platforms/hwcomposer/Makefile
	egl/platforms/hwcomposer/hwcomposer-egl.pc
	glesv1/glesv1_cm.pc
	glesv1/Makefile
	glesv2/glesv2.pc
	glesv2/Makefile
	hardware/Makefile
	hardware/libhardware.pc
	gralloc/Makefile
	gralloc/libgralloc.pc
	libnfc_nxp/Makefile
	libnfc_nxp/libnfc_nxp.pc
	libnfc_ndef_nxp/Makefile
	libnfc_ndef_nxp/libnfc_ndef_nxp.pc
	libsync/Makefile
	libsync/libsync.pc
	vibrator/Makefile
	vibrator/libvibrator.pc
	media/Makefile
	media/libmedia.pc
	wifi/Makefile
	wifi/libwifi.pc
	include/Makefile
	input/Makefile
	input/libis.pc
	ui/Makefile
	utils/Makefile
	sf/Makefile
	sf/libsf.pc
	hwc2/Makefile
	hwc2/libhwc2.pc
	opencl/Makefile
	opencl/OpenCL.pc
	tests/Makefile
])

AM_COND_IF([WANT_WAYLAND_EGL], [AC_CONFIG_FILES([egl/platforms/common/wayland-egl.pc])])
AC_OUTPUT

echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE_NAME $PACKAGE_VERSION"
echo "------------------------------------------------------------------------"
echo
echo "Configuration Options:"
echo
echo "  debug build.............: $debug"
echo
echo "  trace...................: $trace"
echo
echo "  experimental features...: $experimental"
echo
echo "  prefix..................: $prefix"
echo
echo "  arch  ..................: $arch"
echo
echo "  headers path ...........: $android_headers_path"
echo
echo "  Android version.........: $android_headers_major.$android_headers_minor.$android_headers_patch"
echo
echo "  default egl platform ...: $DEFAULT_EGL_PLATFORM"
echo
echo "  default ld_library_path.: $DEFAULT_HYBRIS_LD_LIBRARY_PATH"
echo
echo "Features:"
echo
echo "  stub linker.............: $stub_linker"
echo
echo "  Ubuntu linker overrides.: $ubuntu_linker_overrides"
echo
echo "------------------------------------------------------------------------"
echo
echo "Now type 'make' to compile and 'make install' to install this package."

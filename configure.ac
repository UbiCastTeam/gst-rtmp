dnl required version of autoconf
AC_PREREQ([2.53])

AC_INIT([gst-plugin-rtmp],[0.0.1])

GST_MAJORMINOR=0.10
GST_REQUIRED=0.10.16
GSTPB_REQUIRED=0.10.16

dnl AM_MAINTAINER_MODE provides the option to enable maintainer mode
AM_MAINTAINER_MODE
dnl add this
AC_PROG_CXX

AC_SUBST(GST_MAJORMINOR)
AC_DEFINE_UNQUOTED(GST_MAJORMINOR, "$GST_MAJORMINOR",
  [GStreamer MAJORMINOR Version])

AC_CONFIG_AUX_DIR([src])
AC_CONFIG_HEADERS([config.h])

dnl required version of automake
AM_INIT_AUTOMAKE([1.10])

dnl make aclocal work in maintainer mode
AC_SUBST(ACLOCAL_AMFLAGS, "-I m4")

dnl enable mainainer mode by default
AM_MAINTAINER_MODE([enable])

dnl check for tools (compiler etc.)
AC_PROG_CC

dnl required version of libtool
LT_PREREQ([2.2.6])
LT_INIT

dnl give error and exit if we don't have pkgconfig
AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, [ ], [
  AC_MSG_ERROR([You need to have pkg-config installed!])
])

dnl checks for gstreamer
AG_GST_CHECK_GST_CHECK($GST_MAJORMINOR, [$GST_REQUIRED], no)
AG_GST_CHECK_GST_PLUGINS_BASE($GST_MAJORMINOR, [$GSTPB_REQUIRED], yes)

dnl Check for the required version of GStreamer core
PKG_CHECK_MODULES(GST, [
  gstreamer-0.10 >= $GST_REQUIRED
  gstreamer-base-0.10 >= $GST_REQUIRED
], [
  AC_SUBST(GST_CFLAGS)
  AC_SUBST(GST_LIBS)
], [
  AC_MSG_ERROR([
      You need to install or upgrade the GStreamer development
      packages on your system. On debian-based systems these are
      libgstreamer0.10-dev and libgstreamer-plugins-base0.10-dev.
      on RPM-based systems gstreamer0.10-devel, libgstreamer0.10-devel
      or similar. The minimum version required is $GST_REQUIRED.
  ])
])

PKG_CHECK_MODULES(SOUP, [
  libsoup-2.4
], [
  AC_SUBST(SOUP_CFLAGS)
  AC_SUBST(SOUP_LIBS)
], [
  AC_MSG_ERROR([
      libsoup-2.4 is required.
  ])
])


dnl check if compiler understands -Wall (if yes, add -Wall to GST_CFLAGS)
AC_MSG_CHECKING([to see if compiler understands -Wall])
save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Wall"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([ ], [ ])], [
  GST_CFLAGS="$GST_CFLAGS -Wall"
  AC_MSG_RESULT([yes])
], [
  AC_MSG_RESULT([no])
])

dnl *** rtmp ***
translit(dnm, m, l) AM_CONDITIONAL(USE_RTMP, true)
PKG_CHECK_MODULES(RTMP, [rtmp library], rtmp, 
  [
    PKG_CHECK_MODULES(RTMP, librtmp)
    HAVE_RTMP="yes"
  ],
  [ 
    HAVE_RTMP="no"
  ],
)
if test "x$HAVE_RTMP" == "xyes"; then
  saved_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $RTMP_CFLAGS"
  CPPFLAGS="$saved_CPPFLAGS"
  AC_DEFINE([RTMP_SOURCE], ["system install"], [Describes where the Librtmp libraries come from.])
  HAVE_RTMP_UNINSTALLED=0
  AC_MSG_NOTICE([Using system-installed RTMP code])
else
  echo $top_srcdir
  AC_MSG_NOTICE([Using local librtmp snapshot])
  dnl libgstrtmp.la: include dirs
  RTMP_CFLAGS="-I\$(top_srcdir)/gst-libs/ext/librtmp/ -I/usr/include/openssl/\
                 -Wno-deprecated-declarations "

  dnl libgstrtmp.la: libs to statically link to        
  RTMP_LIBS="\$(top_builddir)/gst-libs/ext/librtmp/librtmp.a -L/usr/lib/ssl/ -lssl -lcrypto -ldl"
  RTMP_SUBDIRS=gst-libs
  embrtmp_configure_args="$embrtmp_configure_args SHARED= CRYPTO=OPENSSL"
  AC_CONFIG_COMMANDS([configure-embedded-rtmp],
    [echo "Configuring included Librtmp instance with args $librtmp_configure_args"
     origdir=`pwd`
     if test -z "$srcdir" -o "$srcdir" = .; then
       confcmd=make
     else
       confcmd="$origdir"/"$ac_top_srcdir"/gst-libs/ext/librtmp/make
     fi
     AS_MKDIR_P(["$ac_top_build_prefix"gst-libs/ext/librtmp])
     cd "$ac_top_build_prefix"gst-libs/ext/librtmp &&
         eval "$confcmd $embrtmp_configure_args" ||
         AC_MSG_ERROR([Failed to configure embedded Librtmp tree])
     cd "$origdir"
    ],
    [embrtmp_configure_args="$embrtmp_configure_args"])
    AC_MSG_NOTICE([Using included Librtmp code])
fi

dnl set the plugindir where plugins should be installed (for src/Makefile.am)
if test "x${prefix}" = "x$HOME"; then
  plugindir="$HOME/.gstreamer-$GST_MAJORMINOR/plugins"
else
  plugindir="\$(libdir)/gstreamer-$GST_MAJORMINOR"
fi
AC_SUBST(plugindir)

dnl set proper LDFLAGS for plugins
GST_PLUGIN_LDFLAGS='-module -avoid-version -export-symbols-regex [_]*\(gst_\|Gst\|GST_\).*'
AC_SUBST(GST_PLUGIN_LDFLAGS)

AC_CONFIG_FILES(
Makefile
m4/Makefile
gst-libs/Makefile
gst-libs/ext/Makefile
src/Makefile
tests/Makefile
)
AC_OUTPUT

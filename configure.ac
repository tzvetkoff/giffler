# Run `./bootstrap` to generate the "configure" script.

# Define the package version numbers, bug reporting link, project homepage.
m4_define([GIFFLER_VERSION_MAJOR_], [0])
m4_define([GIFFLER_VERSION_MINOR_], [1])
m4_define([GIFFLER_VERSION_PATCH_], [0])
m4_define([GIFFLER_VERSION_STRING_], [GIFFLER_VERSION_MAJOR_.GIFFLER_VERSION_MINOR_.GIFFLER_VERSION_PATCH_])
m4_define([GIFFLER_BUGREPORT_URL_], [https://github.com/tzvetkoff/giffler/issues])
m4_define([GIFFLER_PROJECT_URL_], [https://github.com/tzvetkoff/giffler])

# Initialize autoconf & automake.
AC_PREREQ(2.59)
AC_INIT([giffler], [GIFFLER_VERSION_STRING_], [GIFFLER_BUGREPORT_URL_], [giffler], [GIFFLER_PROJECT_URL_])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config.hpp])
# AC_CONFIG_LIBOBJ_DIR([lib])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.9 foreign])

# Define macro variables for the package version numbers.
AC_DEFINE([GIFFLER_VERSION_MAJOR], [GIFFLER_VERSION_MAJOR_], [Define the major version number.])
AC_DEFINE([GIFFLER_VERSION_MINOR], [GIFFLER_VERSION_MINOR_], [Define the minor version number.])
AC_DEFINE([GIFFLER_VERSION_PATCH], [GIFFLER_VERSION_PATCH_], [Define the patch version number.])
AC_DEFINE([GIFFLER_VERSION_STRING], ["GIFFLER_VERSION_STRING_"], [Define the version string.])
AC_DEFINE([GIFFLER_BUGREPORT_URL], ["GIFFLER_BUGREPORT_URL_"], [Bug reporting url])
AC_DEFINE([GIFFLER_PROJECT_URL], ["GIFFLER_PROJECT_URL_"], [Project homepage])

# Checks for programs.
AC_PROG_CXX()
AC_PROG_INSTALL()
AC_PROG_LN_S()
AC_PROG_MAKE_SET()
AC_PROG_LIBTOOL()

# Check for ImageMagick C++ bindings
AC_CHECK_IMAGEMAGICK()

if test x"$ac_imagemagick" != "xyes"; then
  AC_MSG_ERROR([ImageMagick C++ bindings not found. Please install ImageMagick with C++ bindings or use --with-imagemagick-config=path.])
fi

AC_SUBST([IMAGEMAGICK_CXXFLAGS])
AC_SUBST([IMAGEMAGICK_LDFLAGS])

# Makefiles.
AC_CONFIG_FILES([Makefile src/Makefile])

# Le grand finale.
AC_OUTPUT()

echo
echo "Configuration complete."
echo
echo "Now, run make and pray."
echo

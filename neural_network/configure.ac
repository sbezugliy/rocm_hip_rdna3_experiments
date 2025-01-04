AC_INIT([neural_network], [1.0], [s.bezugliy@gmail.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Choose hipcc as the default C++ compiler
if test "x$CXX" = "xg++"; then
    CXX=/opt/rocm/bin/hipcc
fi
AC_SUBST([CXX])

# Set default CXXFLAGS if empty
if test "x$CXXFLAGS" = "x"; then
    CXXFLAGS="--offload-arch=gfx1036 --offload-arch=gfx1100 -I/opt/rocm/include -I/usr/include $CXXFLAGS"
fi
AC_SUBST([CXXFLAGS])

# Set default LDFLAGS if empty
if test "x$LDFLAGS" = "x"; then
    LDFLAGS="-L/opt/rocm/lib $LDFLAGS"
fi
AC_SUBST([LDFLAGS])

# Library checks
AC_CHECK_LIB([rocrand], [main], , AC_MSG_ERROR([rocRAND library not found]))
AC_CHECK_LIB([yaml-cpp], [main], , AC_MSG_ERROR([yaml-cpp library not found]))
AC_CHECK_LIB([imgui], [main], , AC_MSG_ERROR([ImGui library not found]))
AC_CHECK_LIB([SDL3], [main], , AC_MSG_ERROR([SDL3 library not found]))

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT
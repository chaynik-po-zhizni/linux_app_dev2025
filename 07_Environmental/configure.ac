#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([rhasher], [1.0], [blabla@mail.ru])
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_CONFIG_SRCDIR([src/rhasher.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC


AC_ARG_ENABLE([readline],
    [AS_HELP_STRING([--disable-readline], [Disable readline support])],
    [use_readline=$enableval],
    [use_readline=yes])

AC_MSG_NOTICE([use_readline is set to $use_readline])

# Checks for libraries.
AC_CHECK_LIB([rhash], [rhash_library_init], [], [AC_MSG_ERROR([librhash is required])])

# Checks for header files.
AC_CHECK_HEADER([rhash.h])

AS_IF([test "x$use_readline" != xno],
    [
    AC_CHECK_HEADER([readline/readline.h],
        [
        AC_CHECK_LIB([readline], [readline], [],
            [AC_MSG_WARN([readline library not found]); use_readline=no])
        ],
        [
        AC_MSG_WARN([readline headers not found]); use_readline=no
        ])
    ],
    [
    use_readline=no
    ])

AS_IF([test "x$use_readline" != xno], [AC_DEFINE([USE_READLINE], [1], [Use readline])] [])
# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([strcasecmp])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

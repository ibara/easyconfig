/*
 * Copyright (c) 2020 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/stat.h>

#include <dialog.h>
#include <limits.h>

#include "easyconfig.h"

static FILE *in, *out;

static char *line = NULL, *line2, *lhs, *rhs;
static char *inp_name = NULL, *inp_c_compiler = NULL;
static char *inp_cxx_compiler = NULL;
static char *inp_cflags = NULL, *inp_cxxflags = NULL;
static char *inp_cppflags = NULL, *inp_ldflags = NULL;
static char *inp_libs = NULL, *inp_objs = NULL;

static int objs_expand;

static void
write_c_compiler(void)
{
	fprintf(out, "cccheck() {\n");
	fprintf(out, "  if [ ! -z \"$CC\" ] ; then\n");
	fprintf(out, "cat << EOF > conftest.c\n");
	fprintf(out, "int main(void){return 0;}\n");
	fprintf(out, "EOF\n");
	fprintf(out, "    $CC $cflags -o conftest.o -c conftest.c > /dev/null 2>&1\n");
	fprintf(out, "    $CC $ldflags -o conftest conftest.o > /dev/null 2>&1\n");
	fprintf(out, "    if [ $? -eq 0 ] ; then\n");
	fprintf(out, "      ./conftest\n");
	fprintf(out, "      if [ $? -eq 0 ] ; then\n");
	fprintf(out, "        rm -f conftest conftest.o conftest.c\n");
	fprintf(out, "        cc=\"$CC\"\n");
	fprintf(out, "        return 0\n");
	fprintf(out, "      else\n");
	fprintf(out, "        echo \"could not build working executables\"\n");
	fprintf(out, "        echo \"Please ensure your C compiler is a native compiler\"\n");
	fprintf(out, "        exit 1\n");
	fprintf(out, "      fi\n");
	fprintf(out, "    else\n");
	fprintf(out, "      rm -f conftest conftest.o conftest.c\n");
	fprintf(out, "    fi\n");
	fprintf(out, "  fi\n\n");

	fprintf(out, "  for compiler in %s ; do\n", inp_c_compiler);
	fprintf(out, "cat << EOF > conftest.c\n");
	fprintf(out, "int main(void){return 0;}\n");
	fprintf(out, "EOF\n\n");
	fprintf(out, "    $compiler $cflags -o conftest.o -c conftest.c > /dev/null 2>&1\n");
	fprintf(out, "    $compiler $ldflags -o conftest conftest.o > /dev/null 2>&1\n\n");
	fprintf(out, "    if [ $? -eq 0 ] ; then\n");
	fprintf(out, "      ./conftest\n");
	fprintf(out, "      if [ $? -eq 0 ] ; then\n");
	fprintf(out, "        rm -f conftest conftest.o conftest.c\n");
	fprintf(out, "        cc=\"$compiler\"\n");
	fprintf(out, "        return 0\n");
	fprintf(out, "      else\n");
	fprintf(out, "        echo \"could not build working executables\"\n");
	fprintf(out, "        echo \"Please ensure your C compiler is a native compiler\"\n");
	fprintf(out, "        exit 1\n");
	fprintf(out, "      fi\n");
	fprintf(out, "    else\n");
	fprintf(out, "      rm -f conftest conftest.o conftest.c\n");
	fprintf(out, "    fi\n");
	fprintf(out, "  done\n\n");
	fprintf(out, "  return 1\n");
	fprintf(out, "}\n\n");

	fprintf(out, "printf \"checking for C compiler... \"\n");
	fprintf(out, "cccheck\n");
	fprintf(out, "if [ $? -ne 0 ] ; then\n");
	fprintf(out, "  echo \"not found\"\n");
	fprintf(out, "  echo \"Please install a C compiler and re-run configure.\"\n");
	fprintf(out, "  exit 1\n");
	fprintf(out, "else\n");
	fprintf(out, "  echo \"$cc\"\n");
	fprintf(out, "fi\n\n");
}

static void
write_cxx_compiler(void)
{
	fprintf(out, "cxxcheck() {\n");
	fprintf(out, "  if [ ! -z \"$CXX\" ] ; then\n");
	fprintf(out, "cat << EOF > conftest.c\n");
	fprintf(out, "int main(void){return 0;}\n");
	fprintf(out, "EOF\n");
	fprintf(out, "    $CXX $cxxflags -o conftest.o -c conftest.c > /dev/null 2>&1\n");
	fprintf(out, "    $CXX $ldflags -o conftest conftest.o > /dev/null 2>&1\n");
	fprintf(out, "    if [ $? -eq 0 ] ; then\n");
	fprintf(out, "      ./conftest\n");
	fprintf(out, "      if [ $? -eq 0 ] ; then\n");
	fprintf(out, "        rm -f conftest conftest.o conftest.c\n");
	fprintf(out, "        cxx=\"$CXX\"\n");
	fprintf(out, "        return 0\n");
	fprintf(out, "      else\n");
	fprintf(out, "        echo \"could not build working executables\"\n");
	fprintf(out, "        echo \"Please ensure your C++ compiler is a native compiler\"\n");
	fprintf(out, "        exit 1\n");
	fprintf(out, "      fi\n");
	fprintf(out, "    else\n");
	fprintf(out, "      rm -f conftest conftest.o conftest.c\n");
	fprintf(out, "    fi\n");
	fprintf(out, "  fi\n\n");

	fprintf(out, "  for compiler in %s ; do\n", inp_cxx_compiler);
	fprintf(out, "cat << EOF > conftest.c\n");
	fprintf(out, "int main(void){return 0;}\n");
	fprintf(out, "EOF\n\n");
	fprintf(out, "    $compiler $cflags -o conftest.o -c conftest.c > /dev/null 2>&1\n");
	fprintf(out, "    $compiler $ldflags -o conftest conftest.o > /dev/null 2>&1\n\n");
	fprintf(out, "    if [ $? -eq 0 ] ; then\n");
	fprintf(out, "      ./conftest\n");
	fprintf(out, "      if [ $? -eq 0 ] ; then\n");
	fprintf(out, "        rm -f conftest conftest.o conftest.c\n");
	fprintf(out, "        cxx=\"$compiler\"\n");
	fprintf(out, "        return 0\n");
	fprintf(out, "      else\n");
	fprintf(out, "        echo \"could not build working executables\"\n");
	fprintf(out, "        echo \"Please ensure your C++ compiler is a native compiler\"\n");
	fprintf(out, "        exit 1\n");
	fprintf(out, "      fi\n");
	fprintf(out, "    else\n");
	fprintf(out, "      rm -f conftest conftest.o conftest.c\n");
	fprintf(out, "    fi\n");
	fprintf(out, "  done\n\n");
	fprintf(out, "  return 1\n");
	fprintf(out, "}\n\n");

	fprintf(out, "printf \"checking for C++ compiler... \"\n");
	fprintf(out, "cxxcheck\n");
	fprintf(out, "if [ $? -ne 0 ] ; then\n");
	fprintf(out, "  echo \"not found\"\n");
	fprintf(out, "  echo \"Please install a C++ compiler and re-run configure.\"\n");
	fprintf(out, "  exit 1\n");
	fprintf(out, "else\n");
	fprintf(out, "  echo \"$cxx\"\n");
	fprintf(out, "fi\n\n");
}

static void
write_configure(void)
{
	size_t i;

	fprintf(out, "#!/bin/sh\n\n");
	fprintf(out, "# Auto-generated by Easy Config %s\n\n", version);

	fprintf(out, "Makefile() {\n");
	fprintf(out, "  if [ -z \"$cc\" ] ; then\n");
	fprintf(out, "    cc=$CC\n");
	fprintf(out, "  fi\n");
	fprintf(out, "  if [ -z \"$cxx\" ] ; then\n");
	fprintf(out, "    cxx=$CXX\n");
	fprintf(out, "  fi\n");

	fprintf(out, "cat << EOF > Makefile\n");
	fprintf(out, "# This Makefile automatically generated by configure.\n");
	fprintf(out, "\n");
	fprintf(out, "CC =\t\t$cc\n");
	fprintf(out, "CFLAGS =\t$cflags\n\n");
	fprintf(out, "CXX =\t\t$cxx\n");
	fprintf(out, "CXXFLAGS =\t$cxxflags\n\n");
	fprintf(out, "EOF\n\n");

	fprintf(out, "if [ ! -z \"$cppflags\" ] ; then\n");
	fprintf(out, "cat << EOF >> Makefile\n");
	fprintf(out, "CPPFLAGS =\t$cppflags\n");
	fprintf(out, "EOF\n");
	fprintf(out, "fi\n\n");

	fprintf(out, "if [ ! -z \"$ldflags\" ] ; then\n");
	fprintf(out, "cat << EOF >> Makefile\n");
	fprintf(out, "LDFLAGS =\t$ldflags\n");
	fprintf(out, "EOF\n");
	fprintf(out, "fi\n\n");

	fprintf(out, "cat << EOF >> Makefile\n\n");
	fprintf(out, "PREFIX =\t$prefix\n");
	fprintf(out, "BINDIR =\t$bindir\n");
	fprintf(out, "MANDIR =\t$mandir\n\n");

	fprintf(out, "PROG =\t$prog\n");
	if (objs_expand == 1)
		fprintf(out, "OBJS !=\t$objs\n\n");
	else
		fprintf(out, "OBJS =\t$objs\n\n");

	fprintf(out, "all: \\${PROG}\n\n");
	fprintf(out, "\\${PROG}: \\${OBJS}\n");

	if (project == PROJECT_C)
		fprintf(out, "\t\\${CC} \\${LDFLAGS} -o \\${PROG} \\${OBJS} $libs\n");
	else
		fprintf(out, "\t\\${CC} \\${LDFLAGS} -o \\${PROG} \\${OBJS} $libs\n");

	fprintf(out, "\n");
	fprintf(out, "install:\n");
	fprintf(out, "\tinstall -d \\${DESTDIR}\\${BINDIR}\n");
	fprintf(out, "\tinstall -c -s -m 755 \\${PROG} \\${DESTDIR}\\${BINDIR}\n");
	fprintf(out, "\n");
	fprintf(out, "test:\n");
	fprintf(out, "\techo \"No tests\"\n\n");
	fprintf(out, "clean:\n");
	fprintf(out, "\trm -f \\${PROG} \\${OBJS}\n\n");
	fprintf(out, "distclean: clean\n");
	fprintf(out, "\trm -f Makefile config.h\n");
	fprintf(out, "EOF\n");
	fprintf(out, "}\n\n");

	fprintf(out, "prog=\"%s\"\n", inp_name);
	if (objs_expand == 1) {
		fprintf(out, "objs=\"");
		for (i = 1; inp_objs[i] != '`'; i++)
			fputc(inp_objs[i], out);
		fprintf(out, "\"\n\n");
	} else {
		fprintf(out, "objs=\"%s\"\n\n", inp_objs);
	}

	write_c_compiler();
	write_cxx_compiler();

	fprintf(out, "cflags=\"%s\"\n", inp_cflags);
	fprintf(out, "cxxflags=\"%s\"\n", inp_cxxflags);
	fprintf(out, "cppflags=\"%s\"\n", inp_cppflags);
	fprintf(out, "ldflags=\"%s\"\n", inp_ldflags);
	fprintf(out, "libs=\"%s\"\n", inp_libs);

	fprintf(out, "printf \"creating Makefile... \"\n");
	fprintf(out, "Makefile\n");
	fprintf(out, "echo \"done\"\n");
}

void
create_configure(void)
{
	char buf[PATH_MAX];
	char *line = NULL, *line2, *lhs, *rhs;
	int valid;
	size_t linesize = 0;
	ssize_t linelen;

	save();

	if ((out = fopen("configure", "w+")) == NULL)
		return;

	if (chmod("configure", 0000755) == -1)
		return;

	snprintf(buf, sizeof(buf), "%s.conf", name);
	if ((in = fopen(buf, "r")) == NULL)
		name_menu();

	while ((linelen = getline(&line, &linesize, in)) != -1) {
		line2 = line;
		lhs = line;
		rhs = lhs;

		line[linelen - 1] = '\0';

		valid = 1;

		while (*rhs != '=') {
			if (*rhs++ == '\0') {
				valid = 0;
				break;
			}
		}
		*rhs++ = '\0';

		while (*rhs == ' ' || *rhs == '\t')
			++rhs;

		if (*rhs == '\0')
			rhs = strdup("");

		if (valid == 1) {
			while (*line2 != '\0' && *line2 != ' ' && *line2 != '\t'
)
				++line2;
			*line2 = '\0';

			if (!strcmp(lhs, "project"))
				inp_name = strdup(rhs);
			else if (!strcmp(lhs, "c_compiler"))
				inp_c_compiler = strdup(rhs);
			else if (!strcmp(lhs, "cxx_compiler"))
				inp_cxx_compiler = strdup(rhs);
			else if (!strcmp(lhs, "cflags"))
				inp_cflags = strdup(rhs);
			else if (!strcmp(lhs, "cxxflags"))
				inp_cxxflags = strdup(rhs);
			else if (!strcmp(lhs, "cppflags"))
				inp_cppflags = strdup(rhs);
			else if (!strcmp(lhs, "ldflags"))
				inp_ldflags = strdup(rhs);
			else if (!strcmp(lhs, "libs"))
				inp_libs = strdup(rhs);
			else if (!strcmp(lhs, "objs"))
				inp_objs = strdup(rhs);
		}
	}

	if (inp_objs[0] == '`')
		objs_expand = 1;

	write_configure();

	free(inp_name);
	inp_name = NULL;

	free(inp_c_compiler);
	inp_c_compiler = NULL;

	free(inp_cxx_compiler);
	inp_cxx_compiler = NULL;

	free(inp_cflags);
	inp_cflags = NULL;

	free(inp_cxxflags);
	inp_cxxflags = NULL;

	free(inp_cppflags);
	inp_cppflags = NULL;

	free(inp_ldflags);
	inp_ldflags = NULL;

	free(inp_libs);
	inp_libs = NULL;

	free(inp_objs);
	inp_objs = NULL;
}

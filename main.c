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

#include <dialog.h>

#include "easyconfig.h"

char *cc, *cpp;
char *cflags, *cppflags, *cxxflags;
char *ldflags, *libs;
char *name, *objs;

char version[] = "0.0.1";

int project;

static void
usage(void)
{

	fprintf(stderr, "usage: easyconfig [-x] [file]\n");

	exit(1);
}

static void
run(const char *s)
{

	if (s == NULL)
		usage();
}

static void
load(const char *s)
{
	FILE *in;
	char *line = NULL, *line2, *lhs, *rhs;
	int valid;
	size_t linesize = 0;
	ssize_t linelen;

	if ((in = fopen(s, "r")) == NULL)
		return;

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
			while (*line2 != '\0' && *line2 != ' ' && *line2 != '\t')
				++line2;
			*line2 = '\0';

			if (!strcmp(lhs, "project")) {
				free(name);
				name = strdup(rhs);
			} else if (!strcmp(lhs, "lang")) {
				if (!strcmp(rhs, "c"))
					project = PROJECT_C;
				else if (!strcmp(rhs, "cpp"))
					project = PROJECT_CPP;
			} else if (!strcmp(lhs, "c_compiler")) {
				free(cc);
				cc = strdup(rhs);
			} else if (!strcmp(lhs, "c_compiler")) {
				free(cpp);
				cpp = strdup(rhs);
			} else if (!strcmp(lhs, "cflags")) {
				free(cflags);
				cflags = strdup(rhs);
			} else if (!strcmp(lhs, "cxxflags")) {
				free(cxxflags);
				cxxflags = strdup(rhs);
			} else if (!strcmp(lhs, "cppflags")) {
				free(cppflags);
				cppflags = strdup(rhs);
			} else if (!strcmp(lhs, "ldflags")) {
				free(ldflags);
				ldflags = strdup(rhs);
			} else if (!strcmp(lhs, "libs")) {
				free(libs);
				libs = strdup(rhs);
			} else if (!strcmp(lhs, "objs")) {
				free(objs);
				objs = strdup(rhs);
			}
		}
	}

	free(line);
	fclose(in);
}

int
main(int argc, char *argv[])
{
	int status;

	while ((status = getopt(argc, argv, "x")) != -1) {
		switch (status) {
		case 'x':
			run(argv[2]);
			exit(0);
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	cc = strdup("cc clang gcc");
	cpp = strdup("c++ clang++ g++");
	cflags = strdup("-g -O2");
	cxxflags = strdup("-g -O2");
	cppflags = strdup("");
	ldflags = strdup("");
	libs = strdup("");
	name = strdup("");
	objs = strdup("`ls *.c | sed 's/\\.c/.o/'`");

	if (*argv != NULL)
		load(*argv);

	init_dialog(stdin, stdout);
	status = menu();
	end_dialog();

	return status;
}

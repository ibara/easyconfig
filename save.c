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
#include <limits.h>

#include "easyconfig.h"

void
save(void)
{
	FILE *out;
	char outfile[PATH_MAX];
	int ch;

	while (strlen(name) == 0)
		name_menu();

	snprintf(outfile, sizeof(outfile), "%s.conf", name);

	if ((out = fopen(outfile, "w+")) == NULL)
		return;

	fprintf(out, "# Easy Config %s file for %s\n\n", version, name);
	fprintf(out, "project = %s\n", name);

	switch (project) {
	case PROJECT_C:
		fprintf(out, "lang = c\n");
		break;
	case PROJECT_CPP:
		fprintf(out, "lang = c++\n");
	}

	fprintf(out, "c_compiler = %s\n", cc);
	fprintf(out, "cflags = %s\n", cflags);
	fprintf(out, "cxx_compiler = %s\n", cpp);
	fprintf(out, "cxxflags = %s\n", cxxflags);
	fprintf(out, "cppflags = %s\n", cppflags);
	fprintf(out, "ldflags = %s\n", ldflags);
	fprintf(out, "libs = %s\n", libs);
	fprintf(out, "objs = %s\n", objs);

	fclose(out);
}

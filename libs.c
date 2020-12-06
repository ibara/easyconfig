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

static char *items[] = {
	"1", "Default LDFLAGS",
	"2", "Required libraries",
	"3", "Go Back"
};

static int
required_libs(void)
{
	int status;

	dlg_clear();
	dlg_clr_result();

	status = dialog_inputbox("Required libraries",
			"List the required libraries (e.g., -lm -lncursesw -lz)",
			0, 0, libs, 0);

	free(libs);
	libs = strdup(dialog_vars.input_result);

	return status;
}

static int
default_ldflags(void)
{
	int status;

	dlg_clear();
	dlg_clr_result();

	status = dialog_inputbox("Default LDFLAGS",
			"Set the default LDFLAGS",
			0, 0, ldflags, 0);

	free(ldflags);
	ldflags = strdup(dialog_vars.input_result);

	return status;
}

int
libs_menu(void)
{
	char *choice;
	int status;

	while (1) {
		dlg_clear();
		dlg_clr_result();

		status = dialog_menu("Libraries",
			"Select a sub-menu", 0, 0, 0,
			(sizeof(items) / sizeof(items[0])) / 2, items);

		if (status == 1)
			break;

		choice = dialog_vars.input_result;

		if (!strcmp(choice, "1"))
			default_ldflags();
		else if (!strcmp(choice, "2"))
			required_libs();
		else if (!strcmp(choice, "3"))
			break;
	}

	return status;
}

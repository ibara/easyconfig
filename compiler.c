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
	"1", "C",
	"2", "C++",
	"3", "Go Back"
};

int
compiler(void)
{
	char *choice;
	int status;

	dlg_clear();
	dlg_clr_result();

	status = dialog_menu("Complier main menu",
			"What language is your project?",
			0, 0, 0,
			(sizeof(items) / sizeof(items[0])) / 2, items);

	if (status == 1)
		return status;

	choice = dialog_vars.input_result;

	if (!strcmp(choice, "1"))
		compiler_c();
	else if (!strcmp(choice, "2"))
		compiler_cpp();

	return status;
}

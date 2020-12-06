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

static char *items[] = {
	"1", "New function",
	"2", "Go Back"
};

static FILE *out;
static char buf[PATH_MAX];

static int
function_test(const char *f_name)
{
	char **list;
	int ch, i = 0, rows, status;

	dlg_clear();
	dlg_clr_result();

	list = malloc(3);
	list[0] = dlg_strclone("");
	list[1] = NULL;

	status = dlg_editbox("Write a test", &list, &rows, 0, 0);
	fprintf(out, "f_%s = \n", f_name);
	fputc('!', out);
	while (dialog_vars.input_result[i] != '\0') {
		fputc(dialog_vars.input_result[i], out);
		if (dialog_vars.input_result[i++] == '\n')
			fputc('!', out);
	}
	fputc('\n', out);
	fputc('\n', out);

	return status;
}

static int
new_function(void)
{
	char *f_name;
	int status;

	dlg_clear();
	dlg_clr_result();

	status = dialog_inputbox("New function test",
		"Enter the function name",
		0, 0, NULL, 0);

	f_name = strdup(dialog_vars.input_result);

	function_test(f_name);

	free(f_name);
	f_name = NULL;

	return status;
}

int
function_menu(void)
{
	char *choice;
	int status;

	while (strlen(name) == 0)
		name_menu();

	snprintf(buf, sizeof(buf), "%s_functions.conf", name);
	if ((out = fopen(buf, "a+")) == NULL)
		return 0;

	while (1) {
		dlg_clear();
		dlg_clr_result();

		status = dialog_menu("Functions",
			"Select a sub-menu", 0, 0, 0,
			(sizeof(items) / sizeof(items[0])) / 2, items);

		if (status == 1)
			break;

		choice = dialog_vars.input_result;

		if (!strcmp(choice, "1"))
			new_function();
		else if (!strcmp(choice, "2"))
			break;
	}

	return status;
}

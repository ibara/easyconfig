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
	"1", "Default C++ Compiler(s)",
	"2", "Default CXXFLAGS",
	"3", "Default CPPFLAGS",
	"4", "Go Back"
};

static int
compiler_cpp_name(void)
{
	int status;

	dlg_clear();
	dlg_clr_result();

	status = dialog_inputbox("C++ Compiler",
			"List the C++ compilers to test (first match wins)",
			0, 0, cpp, 0);

	free(cpp);
	cpp = strdup(dialog_vars.input_result);

	return status;
}

static int
compiler_cpp_flags(void)
{
	int status;

	dlg_clear();
	dlg_clr_result();

	status = dialog_inputbox("C++ Compiler Flags",
			"Set the default CXXFLAGS",
			0, 0, cxxflags, 0);

	free(cxxflags);
	cxxflags = strdup(dialog_vars.input_result);

	return status;
}

static int
compiler_cppflags(void)
{
	int status;

	dlg_clear();
	dlg_clr_result();

	status = dialog_inputbox("C++ Preprocessor Flags",
			"Set the default CPPFLAGS",
			0, 0, cppflags, 0);

	free(cppflags);
	cppflags = strdup(dialog_vars.input_result);

	return status;
}

int
compiler_cpp(void)
{
	char *choice;
	int status;

	project = PROJECT_CPP;

	while (1) {
		dlg_clear();
		dlg_clr_result();

		status = dialog_menu("C++ Project",
			"Select a sub-menu", 0, 0, 0,
			(sizeof(items) / sizeof(items[0])) / 2, items);

		if (status == 1)
			break;

		choice = dialog_vars.input_result;

		if (!strcmp(choice, "1"))
			compiler_cpp_name();
		else if (!strcmp(choice, "2"))
			compiler_cpp_flags();
		else if (!strcmp(choice, "3"))
			compiler_cppflags();
		else if (!strcmp(choice, "4"))
			break;
	}

	return status;
}

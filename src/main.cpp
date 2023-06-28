/*
	is a free open source engine for run classic Touhou games.
*/
#define THOE_FULLNAME "Touhou Open Engine"
#ifndef THOE_VERSION_FULL
#define THOE_VERSION_FULL "0.01"
#endif

#include "thtk/contrib/thtkpp.hh"

#include "sys/ospath.hpp"
#include "sys/oserror.hpp"

#include "daiopts.cpp"

#ifdef USE_GLFW
# include "ui/glfw.hpp"
#else
# include "ui/sdl.hpp"
#endif

const char help_text[] = "\n"
"    -d      Game Directory\n"
"    -c      Config Directory\n"
"    -v      Print Version\n"
"    -h      Print Help\n";

int main(int argc, const char *argv[]) {

	bool go_exit = false;
	int e_status = EXIT_SUCCESS;

	DaiOpts opts({'d', 'c', 'v', 'h'});
	/*...*/ opts.parse(argc, argv);

	if (opts.has_option('v')) {
		std::cout << THOE_FULLNAME << " " << THOE_VERSION_FULL << std::endl;
		go_exit = true;
	}
	if (opts.has_option('h')) {
		std::cout << help_text << std::endl;
		go_exit = true;
	}
#ifdef DEBUG
	std::cout << "(Debug) Passed Options:" << std::endl;
	for (const auto& [key, value] : opts) {
		std::cout << '[' << key << "] = " << value << ";" << std::endl;
	}
#endif
	if (go_exit)
		return e_status;

	UI_Init ui = UI_Init(0);
	UI_Window win = UI_Window("Touhou", -1,-1, 800, 600, false);
	UIEvent e;

	do {
		if (win.waitEvent(e)) {
			switch(e.type) {
			case UI_ERROR:
				std::cout << "error: " << e.vals[1] << "\n";
				e_status = EXIT_FAILURE;
			case UI_EXIT:
				go_exit = true;
				break;
			case UI_INPUT:
				std::cout << "key: " << e.vals[1] << "\n";
				break;
			case UI_PAUSE:
			case UI_RESIZE:
				break;
			}
		}
	} while (!go_exit);

	return e_status;
}

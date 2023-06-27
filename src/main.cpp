
#include "thtk/contrib/thtkpp.hh"

#include "sys/ospath.hpp"
#include "sys/oserror.hpp"

#ifdef USE_GLFW
# include "ui/glfw.hpp"
#else
# include "ui/sdl.hpp"
#endif

int main(int argc, const char *argv[]) {
	UI_Init ui = UI_Init(0);
	UI_Window win = UI_Window("Touhou", -1,-1, 800, 600, false);
	UIEvent e;

	bool running = true;
	int ok = EXIT_FAILURE;

	do {
		if (win.waitEvent(e)) {
			switch(e.type) {
			case UI_ERROR:
				std::cout << "error: " << e.vals[1] << "\n";
				break;
			case UI_EXIT:
				running = false;
				ok = EXIT_SUCCESS;
				break;
			case UI_INPUT:
				std::cout << "key: " << e.vals[1] << "\n";
				break;
			case UI_RESIZE:
				break;
			}
		}
	} while (running);

	return ok;
}

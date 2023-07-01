/*
	is a free open source engine for run classic Touhou games.
*/
#define THOE_FULLNAME "Touhou Open Engine"
#ifndef THOE_VERSION_FULL
#define THOE_VERSION_FULL "0.01"
#endif

#include "sys/ospath.hpp"

#ifndef WITH_THTK
#include "dat/thtk_bitstream.cpp"
#endif

#include "daiopts.cpp"

#ifdef USE_GLFW
# include "ui/glfw.cpp"
#else
# include "ui/sdl.cpp"
#endif

const char help_text[] = "\n"
"  -d=<path>  Game Directory\n"
"  -u         Unpacked Game Resources\n"
"  -c=<path>  Config Directory\n"
"  -v         Print Version\n"
"  -h         Print Help\n";

int main(int argc, const char *argv[]) {

	bool go_exit = false;
	int e_status = EXIT_SUCCESS;

	DaiOpts opts({'d', 'u', 'c', 'v', 'h'});
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

	str_t TH_dir, TH_title = "Touhou";

	if (opts.has_option('d'))
	{
		/**/TH_dir = opts.get_value('d');
		if (IS_PATH_NOT_ABSOLUTE(TH_dir)) {
			Sys_Err("Error parse option:\n\t-d=\"" << TH_dir << "\"\n"
			     << "\nShell path's is not supported."
			     << "\nPlease set full path of the game.\n");
			return EXIT_FAILURE;
		} else
		if (TH_dir.at(TH_dir.size()-1) != OS_PATH_S)
			TH_dir += std::to_string(OS_PATH_S);
	} else {
		Sys_Err("\nError: Unable to find game directory."
		     << "\nUse -d option for set full path of the game.");
		return EXIT_FAILURE;
	}

	str_t TL_path = TH_dir + str_t("TL.DAT"),
	      ST_path = TH_dir + str_t("ST.DAT"),
	      IN_path = TH_dir + str_t("IN.DAT");

	TH_Dat TL_data(TL_path.c_str());
	uint32_t TH_ver = TL_data.version();

	if (TH_ver)
		TH_title += " "+ std::to_string(TH_ver);

	TH_Entry TL_ent = TL_data.entry("title00.jpg");
	U8ClampVec title00 = TL_ent.extract();

	std::cout << "detect Touhou " << TH_ver  << std::endl;
	std::cout << "TL.DAT entries: " << TL_data.entry_count()<< std::endl;
	std::cout << "title00.jpg size: " << TL_ent.size() << std::endl;

	UI_Background bg(0);
	UI_Window win(TH_title.c_str(), -1,-1, 800, 600, false);
	UIEvent e;

	TH_Image sp = LOAD_IMG_RAW(title00);
	TH_Texture tx = win.createTexture(sp);
					win.render(tx);
					win.present();

	do {
		if (win.waitEvent(e)) {
			switch(e.type) {
			case UI_ERROR:
				e_status = EXIT_FAILURE;
			case UI_EXIT:
				go_exit = true;
				break;
			case UI_INPUT:
				std::cout << "key: " << e.vals[1] << "\n";
				break;
			case UI_PAUSE:
				break;
			case UI_RESIZE:
				win.render(tx);
				win.present();
			}
		}
	} while (!go_exit);

	return e_status;
}

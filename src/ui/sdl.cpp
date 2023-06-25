
#include "sdl.hpp"

/* UI Window constructor */
UI_Window::UI_Window(const char *title, int x, int y, int w, int h, bool fullsc)
{
	Uint32 wf = SDL_WINDOW_INPUT_FOCUS,
	       rf = SDL_RENDERER_PRESENTVSYNC;

	if ((m_fullscreen = fullsc))
		wf |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	else
		wf |= SDL_WINDOW_RESIZABLE;

	if (x < 0 || y < 0)
		x = y = SDL_WINDOWPOS_CENTERED;

	TH_Log("open SDL window ..");
	if ((m_win = SDL_CreateWindow(title, x, y, w, h, wf)))
		m_rendr = SDL_CreateRenderer(m_win, -1, rf);

	if (!m_win || !m_rendr) {
		/* */ TH_Log(".fail\n");
		throw TH_Except("ERR_SDL_WINDOW", SDL_GetError());
	}
	TH_Log(".ok \n");
}

/* UI Window destructor */
UI_Window::~UI_Window() {
#ifdef WITH_OPENGL
	SDL_GL_DeleteContext(m_glctx);
#endif
	SDL_DestroyRenderer(m_rendr);
	SDL_DestroyWindow(m_win);

	TH_Log("SDL window close \n");
}

void UI_Window::toggleFullscreen()
{
	Uint32 wf = m_fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
	
	if (SDL_SetWindowFullscreen(m_win, wf)) {
		TH_Err("can't set SDL window mode: " << SDL_GetError());
	} else {
		m_fullscreen = (wf != 0);
	}
}

void UI_Window::present() {
#ifdef WITH_OPENGL
	SDL_GL_SwapWindow(m_win);
#else
	SDL_RenderPresent(m_rendr);
#endif
}

bool UI_Window::waitEvent(UIEvent &o)
{
	SDL_Event e;

	bool hasNew = SDL_WaitEvent(&e);
	if ( hasNew ) {
		switch (e.type) {
		case SDL_QUIT: // Quit the game
			o.type = UI_EXIT;
			break;
		case SDL_KEYDOWN:
			o.type = UI_INPUT;
			o.vals[1] = (int)e.key.keysym.sym;
			break;
		case SDL_WINDOWEVENT:
			switch(e.window.event) {
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_MINIMIZED:
				TH_Err("win@" << e.window.windowID << " size: " << "x" << "\n");
				break;
			}
		}
	}
	return hasNew;
}

auto UI_Window::createTexture(Uint32 format, int access, int w, int h) -> TH_Texture
{
	TH_Texture tex = SDL_CreateTexture(m_rendr, format, access, w, h);
	if(NULL == tex) {
		throw TH_Except("ERR_SDL_TEXTURE", SDL_GetError());
	}
	return tex;
}

auto UI_Window::createTexture(TH_Image img) -> TH_Texture
{
	TH_Texture tex = SDL_CreateTextureFromSurface(m_rendr, img);
	if(NULL == tex) {
		throw TH_Except("ERR_SDL_TEXTURE", SDL_GetError());
	}
	return tex;
}

UI_Init::UI_Init(uint32_t audio_formats)
{
	TH_Log("init SDL ..");

	if (0 > SDL_Init(
	    SDL_INIT_VIDEO | SDL_INIT_TIMER
#ifndef NO_SOUND
	  | SDL_INIT_AUDIO
#endif
	) || TTF_Init() == -1 || IMG_Init(IMG_INIT_PNG) == 0) {
		/* */ TH_Log(".fail\n");
		throw TH_Except("ERR_SDL_INIT", SDL_GetError());
	}
#ifndef NO_SOUND
	m_formats = Mix_Init( audio_formats );
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
#endif
	TH_Log(".ok\n");
}

UI_Init::~UI_Init()
{
#ifndef NO_SOUND
	Mix_CloseAudio();
	Mix_Quit();
#endif
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	TH_Log("SDL quit \n");
}

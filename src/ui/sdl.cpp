
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

	Sys_Log("open SDL window ..");
	if ((m_win = SDL_CreateWindow(title, x, y, w, h, wf)))
		m_rendr = SDL_CreateRenderer(m_win, -1, rf);

	if (!m_win || !m_rendr) {
		Sys_Log(".fail\n");
		Sys_Err("ERR_SDL_WINDOW" << SDL_GetError());
		Sys_ExitFail();
	}
	if (SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY , "linear" ) &&
	    SDL_RenderSetLogicalSize( m_rendr, w, h ))
	    Sys_Err("can't set logical scale" << SDL_GetError());
	Sys_Log(".ok \n");
}

/* UI Window destructor */
UI_Window::~UI_Window() {
#ifdef WITH_OPENGL
	SDL_GL_DeleteContext(m_glctx);
#endif
	SDL_DestroyRenderer(m_rendr);
	SDL_DestroyWindow(m_win);
	Sys_Log("SDL window close \n");
}

void UI_Window::toggleFullscreen()
{
	Uint32 wf = m_fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
	
	if (SDL_SetWindowFullscreen(m_win, wf)) {
		Sys_Err("can't set SDL window mode: " << SDL_GetError());
	} else {
		m_fullscreen = (wf != 0);
	}
}

void UI_Window::render(TH_Texture tex) {
	if (SDL_RenderCopy( m_rendr, tex, NULL, NULL)) {
		Sys_Err("ERR_SDL_RENDER" << SDL_GetError());
		Sys_ExitFail();
	}
}
void UI_Window::render(TH_Image img) {
	if (SDL_RenderCopy( m_rendr, createTexture(img), NULL, NULL)) {
		Sys_Err("ERR_SDL_RENDER" << SDL_GetError());
		Sys_ExitFail();
	}
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
				o.type = UI_RESIZE;
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_MINIMIZED:
				Sys_Err("win@" << e.window.windowID << " size: " << "x" << "\n");
				break;
			}
		}
	}
	return hasNew;
}

void UI_Window::pushEvent(UIEvent e) {
	SDL_Event evt = {
		.type = SDL_USEREVENT
	};
	evt.user.type  = e.type;
	evt.user.data1 = e.vals;
	SDL_PushEvent(&evt);
};

auto UI_Window::createTexture(Uint32 format, int access, int w, int h) -> TH_Texture
{
	TH_Texture tex = SDL_CreateTexture(m_rendr, format, access, w, h);
	if(NULL == tex) {
		Sys_Err("ERR_SDL_TEXTURE" << SDL_GetError());
		Sys_ExitFail();
	}
	return tex;
}

auto UI_Window::createTexture(TH_Image img) -> TH_Texture
{
	TH_Texture tex = SDL_CreateTextureFromSurface(m_rendr, img);
	if(NULL == tex) {
		Sys_Err("ERR_SDL_TEXTURE" << SDL_GetError());
		Sys_ExitFail();
	}
	return tex;
}

UI_Background::UI_Background(uint32_t audio_formats)
{
	Sys_Log("init SDL ..");

	if (0 > SDL_Init(
	    SDL_INIT_VIDEO | SDL_INIT_TIMER
#ifndef NO_SOUND
	  | SDL_INIT_AUDIO
#endif
	) || TTF_Init() == -1 || IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
		Sys_Log(".fail\n");
		Sys_Err("ERR_SDL_INIT" << SDL_GetError());
		Sys_ExitFail();
	}
#ifndef NO_SOUND
	m_formats = Mix_Init( audio_formats );
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
#endif
	Sys_Log(".ok\n");
}

UI_Background::~UI_Background()
{
#ifndef NO_SOUND
	Mix_CloseAudio();
	Mix_Quit();
#endif
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Sys_Log("SDL quit \n");
}

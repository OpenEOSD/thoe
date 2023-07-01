
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#ifndef NO_SOUND
# include <SDL2/SDL_mixer.h>
#endif

/* files load intarface */
#define LOAD_IMG_RAW(buf) \
        IMG_Load_RW(SDL_RWFromConstMem(buf.data(), buf.size()),0)
#define LOAD_IMG_FILE(path) \
        IMG_Load(path.c_str())

#include "ui.hpp"

typedef SDL_Texture* TH_Texture;
typedef SDL_Surface* TH_Image;
typedef SDL_Rect     TH_Rect;

/* texture color transforms */
#define set_color_mod(tex, r, g, b) \
        SDL_SetTextureColorMod(tex, r, g, b)
#define set_alpha_mod(tex, a) \
        SDL_SetTextureAlphaMod(tex, a)
#define set_blend_mod(tex, bm) \
        SDL_SetTextureBlendMode(tex, bm)

class UI_Window {

	SDL_Window   *m_win;
	SDL_Renderer *m_rendr;
	bool m_fullscreen;

#ifdef WITH_OPENGL
	SDL_GLContext m_glctx;
#endif

public:
	 UI_Window(const char *title, int x, int y, int w, int h, bool fullsc);
	~UI_Window();

	unsigned int   id() const { return SDL_GetWindowID(m_win); };
	bool isFullscreen() const { return m_fullscreen; };

	void toggleFullscreen();
	bool waitEvent(UIEvent &);
	void pushEvent(UIEvent);

	auto createTexture(Uint32 format, int access, int w, int h) -> TH_Texture;
	auto createTexture(TH_Image img) -> TH_Texture;

	void render(TH_Texture);
	void render(TH_Image);

	void present() {
#ifdef WITH_OPENGL
		SDL_GL_SwapWindow(m_win);
#else
		SDL_RenderPresent(m_rendr);
#endif
	};
};

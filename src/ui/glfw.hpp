
#include <GLFW/glfw3.h>
#ifndef NO_SOUND

#endif

#include "ui.hpp"
#include <list>

typedef struct {
	int x,y,w,h;
} TH_Rect;

class UI_Window {

private:
	GLFWwindow  *m_win;
	GLFWmonitor *m_monic;
	std::list<UIEvent> m_events;

public:
	 UI_Window(const char *title, int x, int y, int w, int h, bool fullsc);
	~UI_Window();

	unsigned int   id() const { return 1; };
	bool isFullscreen() const { return m_monic != NULL; };

	void toggleFullscreen();
	bool waitEvent(UIEvent &);
};

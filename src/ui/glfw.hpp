
#include <GLFW/glfw3.h>
#ifndef NO_SOUND

#endif

#include "ui.hpp"
#include <forward_list>

typedef struct {
	int x,y,w,h;
} TH_Rect;

class UI_Window {

	GLFWwindow  *m_win;
	GLFWmonitor *m_monic;
	std::forward_list<UIEvent> m_events;

public:
	 UI_Window(const char *title, int x, int y, int w, int h, bool fullsc);
	~UI_Window();

	unsigned int   id() const { return (unsigned int)m_win; };
	bool isFullscreen() const { return m_monic != NULL; };

	void toggleFullscreen();

	bool waitEvent(UIEvent &);
	void pushEvent(UIEvent e) { m_events.push_front(e); };
};

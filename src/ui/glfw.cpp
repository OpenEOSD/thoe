
#include "glfw.hpp"

template<typename List>
static inline void handleEvent(GLFWwindow* win, List &evets)
{
	glfwSetWindowUserPointer(win, &evets);
	glfwSetFramebufferSizeCallback(win, [](GLFWwindow* win, int w, int h) {
		List *evets = (List *)glfwGetWindowUserPointer(win);
		evets->push_back(UIEvent{UI_RESIZE,-1,-1,w,h});
	});
	glfwSetWindowCloseCallback(win, [](GLFWwindow* win) {
		List *evets = (List *)glfwGetWindowUserPointer(win);
		evets->push_back(UIEvent{UI_EXIT,0,0,0,0});
	});
	glfwSetKeyCallback(win, [](GLFWwindow*win, int key, int scancode, int action, int mods) {
		List *evets = (List *)glfwGetWindowUserPointer(win);
		evets->push_back(UIEvent{UI_INPUT, key, scancode, action, mods});
	});
}

/* UI Window constructor */
UI_Window::UI_Window(const char *title, int x, int y, int w, int h, bool fullsc)
{
	bool cpos = (x < 0 || y < 0);

	Sys_Log("open GLFW window ..");
	m_monic = fullsc || cpos ? glfwGetPrimaryMonitor() : NULL;

	if (!fullsc && cpos) {
		const GLFWvidmode* vmode = glfwGetVideoMode(m_monic);
		x = vmode->width  / 2 - w / 2; m_monic = NULL;
		y = vmode->height / 2 - h / 2;

		// (1). Set the visibility window hint to false for subsequent window creation
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	}
	m_win = glfwCreateWindow(w, h, title, m_monic, NULL);

	if (!fullsc && cpos) {
		glfwSetWindowPos(m_win, x, y);
		glfwShowWindow(m_win);
	}
	handleEvent(m_win, m_events);
	Sys_Log(".ok\n");
};

/* UI Window destructor */
UI_Window::~UI_Window() {
	glfwDestroyWindow(m_win);
   Sys_Log("GLFW window closed\n");
};

void UI_Window::toggleFullscreen()
{
	if (m_monic == NULL) {
		m_monic = glfwGetPrimaryMonitor();
	} else {
		m_monic = NULL;
	}
	// TODO: save the previous size.
	glfwSetWindowMonitor(m_win, m_monic, 0, 0, 640, 480, 60);
}

bool UI_Window::waitEvent(UIEvent &o)
{
	if (m_events.empty())
		glfwWaitEvents();

	bool hasNew = !m_events.empty();
	if ( hasNew ) {
		UIEvent e = m_events.front();
		/*...*/ m_events.pop_front();
		o.type = e.type;
		o.vals[0] = e.vals[0]; o.vals[1] = e.vals[1];
		o.vals[2] = e.vals[2]; o.vals[3] = e.vals[3];
	}
	return hasNew;
}

UI_Init::UI_Init(uint32_t audio_formats)
{
	Sys_Log("init GLFW ..");
	if (glfwInit() == GLFW_FALSE) {
		Sys_Log(".fail\n");
		Sys_Err("ERR_GLFW_INIT");
		Sys_ExitFail();
	}
	Sys_Log(".ok\n");
}

UI_Init::~UI_Init()
{
	glfwTerminate();
	Sys_Log("GLFW terminated\n");
}

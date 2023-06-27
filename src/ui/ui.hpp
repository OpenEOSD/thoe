
#include "../sys/oserror.hpp"

#define KEY_STATE_SHOOT 0x01
#define KEY_STATE_BOMB  0x02
#define KEY_STATE_FOCUS 0x04
#define KEY_STATE_SKIP  0x08

#define KEY_STATE_UP    0x10
#define KEY_STATE_DOWN  0x20
#define KEY_STATE_LEFT  0x40
#define KEY_STATE_RIGHT 0x80

typedef enum {
	UI_ERROR = -1,
	UI_EXIT,
	UI_PAUSE,
	UI_FULLSCREEN,
	UI_SCREENSHOT,
	UI_RESIZE,
	UI_INPUT
} UIType;

typedef struct {
	UIType type;
	int vals[4];
} UIEvent;

class UI_Init {

private:
	uint32_t m_formats;

public:
	 UI_Init(uint32_t audio_formats);
	~UI_Init();

	bool hasFormatSupport(uint32_t format) const {
		return (m_formats & format) == format;
	};
};

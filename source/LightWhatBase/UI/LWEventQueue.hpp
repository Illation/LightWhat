#pragma once
#include <queue>

enum LWEvent{
	LW_EVENT_RENDER_START,
	LW_EVENT_RENDER_PAUSE,
	LW_EVENT_RENDER_STOP,
	LW_EVENT_SAVE,
	LW_EVENT_LOAD,
	LW_EVENT_LOAD_TEST,
	LW_EVENT_CLEAR,
	LW_EVENT_TOGGLE_BFC
};
class LWEventQueue
{
public:
	LWEventQueue();
	~LWEventQueue();

	void AddEvent(LWEvent evnt);
	void PumpEvents();
	LWEvent PopEvent();
	bool HasEvents();
private:
	std::queue<LWEvent> m_EventArr;
};


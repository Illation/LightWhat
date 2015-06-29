#include "LWEventQueue.hpp"


LWEventQueue::LWEventQueue()
{
}


LWEventQueue::~LWEventQueue()
{
}

void LWEventQueue::AddEvent(LWEvent evnt)
{
	m_EventArr.push(evnt);
}
void LWEventQueue::PumpEvents()
{
	std::queue<LWEvent> emptyQueue;
	std::swap(m_EventArr, emptyQueue);
}
LWEvent LWEventQueue::PopEvent()
{
	LWEvent ret = m_EventArr.front();
	m_EventArr.pop();
	return ret;
}
bool LWEventQueue::HasEvents()
{
	return !(m_EventArr.empty());
}
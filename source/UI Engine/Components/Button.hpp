#pragma once
#include "GuiObject.hpp"
class Button :
	public GuiObject
{
public:
	//Constructor destructor
	Button();
	Button(const std::string txt);
	virtual ~Button();
	//make non copyable
	Button(const Button&) = delete;
	Button& operator=(const Button&) = delete;
	//public member functions
	bool IsPressed() const;
	virtual void Paint();
private:
	virtual void ConsumeEvent();
	virtual void Tick();

	//Data members
	bool m_Armed;
};


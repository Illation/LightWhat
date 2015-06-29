#pragma once

//----------------------------
//Foreward declaration classes
//----------------------------
class Window;

//----------------------------
//Panel base class
//----------------------------
class Panel
{
public:
	Panel();
	virtual ~Panel();

private:
	Window *m_WindowPtr;
};


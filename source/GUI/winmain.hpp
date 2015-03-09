#pragma once

#include <string>
using namespace std;

#include <Windows.h>
#include <Commdlg.h>
#include <SDL/SDL.h>
#include <vector>
#include "../Rendering/common.hpp"

struct Button{
	Button(int lId);
	void setApperance(string, colRGB, colRGB);
	void setBounds(int, int, int, int);
	bool isMouseOver(int, int);
	int id;
	string text;
	colRGB col, border;
	int x1, y1, x2, y2;
};
class winmain
{
public:
	winmain(int lX, int lY);
	~winmain();

	string getFileName(HWND wnd);
	string getTTFName(HWND wnd);
	string saveFileName(HWND wnd);
	void injectMouse(int x, int y, bool isPressed);

	void drawString(string text, int x, int y);
	void drawRect(int x1, int y1, int x2, int y2, colRGB col);
	void addButton(Button but);

	bool isPressed(int ID);

private:
	void setPixel(int x, int y, colRGB col);

	Uint32 *m_Pixels;
	vector <Button> m_Buttons;
	int m_MouseX, m_MouseY;
	bool m_MouseClick;
};
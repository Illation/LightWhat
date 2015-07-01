#pragma once
#include "Rectangle.hpp"
#include "Font.hpp"
#include <string>
class GuiObject
{
	friend class GuiEngine;
public:
	//constructor and destructor
	GuiObject();
	GuiObject(std::string txt);
	virtual ~GuiObject();

	//Make uncopyable
	GuiObject(const GuiObject&) = delete;
	GuiObject& operator=(const GuiObject&) = delete;

	//Public user functions
	//virtual
	virtual void Paint()=0;
	//inherited
	void SetFont(std::string filename, int size);
	void SetText(std::string txt);
	std::string GetText();
	void SetBounds(Rect bRect);
	void SetEnabled(bool en);
	void SetFGColor(colRGB color);
	void SetBGColor(colRGB color);
	void SetDefaultFGColor();
	void SetDefaultBGColor();
protected:
	//inherited private member functions
	bool IsMouseOver();
	void TrimText();
	//inherited Data members
	Rect m_BBox;
	int m_WindowId;
	bool m_Triggered =false,
		m_Enabled = true;
	Font *m_FontPtr = nullptr;
	std::string m_Text, 
		m_TrimmedText;
	int m_TxtLength = 0;
	colRGB m_FGCol,
		m_BGCol,
		m_DefaultFGCol,
		m_DefaultBGCol;
private:
	//engine functions
	virtual void Tick()=0;
	virtual void ConsumeEvent() = 0;
	void Init();
};


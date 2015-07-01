#include "GuiObject.hpp"

#include "..\GuiEngine.hpp"
//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GUI_ENGINE (GuiEngine::GetSingleton())
//------------------------------
// Constructor and Destructor
//------------------------------
GuiObject::GuiObject()
{
	Init();
	m_TrimmedText = string("");
}
GuiObject::GuiObject(std::string txt)
	:m_Text(txt)
{
	Init();
	TrimText();
}
void GuiObject::Init()
{
	m_DefaultFGCol = colRGB(0, 0, 0);
	m_DefaultBGCol = colRGB(0.95f, 0.95f, 0.95f);
	m_FGCol = m_DefaultFGCol;
	m_BGCol = m_BGCol;
	m_BBox = Rect(vec2(), vec2(10, 10));
	m_FontPtr = new Font(GUI_ENGINE->GetRootDirectory() + 
		string("Resources/fonts/Inconsolata-Regular.ttf"), 14);
	GUI_ENGINE->RegisterGuiObject(this);
}
GuiObject::~GuiObject()
{
	GUI_ENGINE->UnRegisterGuiObject(this);
	delete m_FontPtr;
	m_FontPtr = nullptr;
}
//------------------------------
// Public user functions
//------------------------------
void GuiObject::SetFont(std::string filename, int size)
{
	delete m_FontPtr;
	m_FontPtr = new Font(filename, (float)size);
	TrimText();
}
void GuiObject::SetText(std::string txt)
{
	m_Text = txt;
	TrimText();
}
std::string GuiObject::GetText()
{
	return m_Text;
}
void GuiObject::SetBounds(Rect bRect)
{
	m_BBox = bRect;
	TrimText();
}
void GuiObject::SetEnabled(bool en)
{
	m_Enabled = en;
}
void GuiObject::SetFGColor(colRGB color)
{
	m_FGCol = color;
}
void GuiObject::SetBGColor(colRGB color)
{
	m_BGCol = color;
}
void GuiObject::SetDefaultFGColor()
{
	m_FGCol = m_DefaultFGCol;
}
void GuiObject::SetDefaultBGColor()
{
	m_FGCol = m_DefaultBGCol;
}
//------------------------------
// inherited private member functions
//------------------------------
bool GuiObject::IsMouseOver()
{
	vec2 mousePos = GUI_ENGINE->GetMousePosition();
	if ((mousePos.x >= m_BBox.pos.x)
		&& (mousePos.x <= (m_BBox.pos.x + m_BBox.dim.x)))
	{
		if ((mousePos.y >= m_BBox.pos.y)
			&& (mousePos.y <= (m_BBox.pos.y + m_BBox.dim.y)))
		{
			return true;
		}
	}
	return false;
}

void GuiObject::TrimText()
{
	float txtLength = m_Text.length() * m_FontPtr->size / 2;
	if (txtLength > m_BBox.dim.x)
	{
		int keepTxt = (int)(m_Text.length()*(m_BBox.dim.x/txtLength));
		m_TrimmedText = m_Text.substr(0, keepTxt);
	}
	else
	{
		m_TrimmedText = m_Text;
	}
	m_TxtLength = (int)(m_TrimmedText.length() * m_FontPtr->size / 2);
}
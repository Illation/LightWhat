#include "Button.hpp"

#include "..\GuiEngine.hpp"
//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GUI_ENGINE (GuiEngine::GetSingleton())
//-----------------------------------
//Construcor and Destructor
//-----------------------------------
Button::Button()
	:GuiObject()
	,m_Armed(false)
{
	m_DefaultBGCol = m_BGCol = colRGB(1.f, 1.f, 1.f);
}
Button::Button(std::string txt)
	: GuiObject(txt)
	, m_Armed(false)
{
	m_DefaultBGCol = m_BGCol = colRGB(1.f, 1.f, 1.f);
}
Button::~Button()
{
}
//-----------------------------------
//Public functions
//-----------------------------------
void Button::Paint()
{
	Font *originalFont = GUI_ENGINE->GetFont();

	Rect r = m_BBox;
	GUI_ENGINE->SetColor(colRGB(0.4f, 0.4f,  0.4f));
	GUI_ENGINE->FillRect(r);

	r.pos.x++; r.pos.y++; r.dim.x -= 2; r.dim.y -= 2;
	if (!m_Armed)GUI_ENGINE->SetColor(colRGB(0.99f, 0.99f, 0.99f));
	else GUI_ENGINE->SetColor(colRGB(0.4f, 0.4f, 0.4f));
	GUI_ENGINE->FillRect(r);

	r.pos.x++; r.pos.y++; r.dim.x -= 2; r.dim.y -= 2;
	GUI_ENGINE->SetColor(m_BGCol);
	GUI_ENGINE->FillRect(r);

	GUI_ENGINE->SetFont(m_FontPtr);

	if (!m_Armed)
	{
		GUI_ENGINE->SetColor(colRGB(0.4f, 0.4f, 0.4f));
		GUI_ENGINE->DrawLine(m_BBox.pos + vec2(m_BBox.dim.x-1.f, 1.f), m_BBox.pos + m_BBox.dim-vec2(1.f, 1.f));
		GUI_ENGINE->DrawLine(m_BBox.pos + vec2(1.f, m_BBox.dim.y-1.f), m_BBox.pos + m_BBox.dim - vec2(1.f, 1.f));

		GUI_ENGINE->SetColor(colRGB(0.6f, 0.6f, 0.6f));
		GUI_ENGINE->DrawLine(m_BBox.pos + vec2(m_BBox.dim.x - 2.f, 2.f), m_BBox.pos + m_BBox.dim - vec2(2.f, 2.f));
		GUI_ENGINE->DrawLine(m_BBox.pos + vec2(2.f, m_BBox.dim.y - 2.f), m_BBox.pos + m_BBox.dim - vec2(2.f, 2.f));

		if (m_Enabled)GUI_ENGINE->SetColor(m_FGCol);
		else GUI_ENGINE->SetColor(colRGB(0.4f, 0.4f, 0.4f));

		GUI_ENGINE->DrawString(m_TrimmedText, m_BBox.pos.x+m_BBox.dim.x/2-(m_TxtLength/2), 
			m_BBox.pos.y+m_BBox.dim.y/2-m_FontPtr->size/2);
	}
	else
	{
		GUI_ENGINE->SetColor(colRGB(0.4f, 0.4f, 0.4f));
		GUI_ENGINE->DrawLine(m_BBox.pos + vec2(2.f, 2.f), m_BBox.pos + vec2(m_BBox.dim.x - 2.f, 2.f));
		GUI_ENGINE->DrawLine(m_BBox.pos + vec2(2.f, 2.f), m_BBox.pos + vec2(2.f, m_BBox.dim.y - 2.f));

		GUI_ENGINE->SetColor(colRGB(0.6f, 0.6f, 0.6f));
		GUI_ENGINE->DrawLine(m_BBox.pos + vec2(3.f, 3.f), m_BBox.pos + vec2(m_BBox.dim.x - 3.f, 3.f));
		GUI_ENGINE->DrawLine(m_BBox.pos + vec2(3.f, 3.f), m_BBox.pos + vec2(3.f, m_BBox.dim.y - 3.f));

		GUI_ENGINE->SetColor(m_FGCol);

		GUI_ENGINE->DrawString(m_TrimmedText, m_BBox.pos.x + m_BBox.dim.x / 2 - (m_TxtLength / 2),
			m_BBox.pos.y + m_BBox.dim.y / 2 - m_FontPtr->size / 2);
	}

	GUI_ENGINE->SetFont(originalFont);
}
void Button::Tick()
{
	if (!m_Enabled)
	{
		m_Armed = false;
		return;
	}
	if (GUI_ENGINE->IsMouseButtonDown(SDL_BUTTON_LEFT) && IsMouseOver())
	{
		m_Armed = true;
	}
	if (m_Armed)
	{
		if (IsMouseOver())
		{
			if (GUI_ENGINE->IsMouseButtonReleased(SDL_BUTTON_LEFT))
			{
				m_Armed = false;
				m_Triggered = true;
			}
		}
		else
		{
			m_Armed = false;
		}
	}
}
bool Button::IsPressed() const
{
	return m_Triggered;
}

void Button::ConsumeEvent()
{
	m_Triggered = false;
}
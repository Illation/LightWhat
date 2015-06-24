#include "GuiEngine.hpp"
//----------------------------
//Static Variable
//----------------------------
GuiEngine* GuiEngine::m_GuiEnginePtr = nullptr;
//----------------------------
//Constructor and Destructor
//----------------------------
GuiEngine::GuiEngine()
{
	m_WindowPtr = new WindowManager();
}
GuiEngine::~GuiEngine()
{
	delete m_WindowPtr;
	m_WindowPtr = nullptr;
}
//----------------------------
//Static Engine Functions
//----------------------------
GuiEngine* GuiEngine::GetSingleton()
{
	if (m_GuiEnginePtr == nullptr)
	{
		m_GuiEnginePtr = new GuiEngine();
	}
	return m_GuiEnginePtr;
}
//----------------------------
//Application Methods
//----------------------------
void GuiEngine::Init()
{
	m_WindowPtr->InitWindow();
}
void GuiEngine::Paint()
{
	SDL_RenderClear(m_WindowPtr->GetRenderer());
	for (size_t i = 0; i < m_TexturePtrArr.size(); i++)
	{
		SDL_RenderCopy(m_WindowPtr->GetRenderer(), m_TexturePtrArr[i], NULL, &m_RectArr[i]);
		SDL_DestroyTexture(m_TexturePtrArr[i]);
	}
	m_WindowPtr->UpdateWindow();
	m_TexturePtrArr.clear();
	m_RectArr.clear();
}
//----------------------------
//General Getters
//----------------------------
WindowManager* GuiEngine::GetWindow()
{
	return m_WindowPtr;
}
int GuiEngine::GetWidth()
{
	return m_WindowPtr->GetWindowWidth();
}
int GuiEngine::GetHeight()
{
	return m_WindowPtr->GetWindowHeight();
}
//----------------------------
//Draw Methods
//----------------------------
void GuiEngine::DrawBitmap(int x, int y, Bitmap *bmpPtr)
{
	SDL_Rect texture_rect;
	texture_rect.x = x;  //the x coordinate
	texture_rect.y = y; // the y coordinate
	texture_rect.w = bmpPtr->GetWidth(); //the width of the texture
	texture_rect.h = bmpPtr->GetHeight();
	m_RectArr.push_back(texture_rect);

	SDL_Texture* texture = SDL_CreateTexture(m_WindowPtr->GetRenderer(),
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 
		bmpPtr->GetWidth(), bmpPtr->GetHeight());
	SDL_UpdateTexture(texture, NULL, bmpPtr->GetData(), bmpPtr->GetWidth() *sizeof(Uint32));
	m_TexturePtrArr.push_back(texture);
}
void GuiEngine::DrawString(const std::string &message, TTF_Font *daFont,
	SDL_Color color, int fontSize, int posX, int posY)
{
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(daFont, message.c_str(), color);
	if (surf == nullptr){
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(m_WindowPtr->GetRenderer(), surf);
	if (texture == nullptr){
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);

	SDL_Rect texture_rect;
	texture_rect.x = posX;  //the x coordinate
	texture_rect.y = posY; // the y coordinate
	texture_rect.w = message.length()*fontSize / 2; //the width of the texture
	texture_rect.h = fontSize;

	m_RectArr.push_back(texture_rect);
	m_TexturePtrArr.push_back(texture);
}
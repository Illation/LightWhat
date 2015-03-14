#include "ApplicationRoot.hpp"
#include <stdexcept>

ApplicationRoot::ApplicationRoot()
{
	_window = nullptr;
	_screenWidth = 1280;
	_screenHeight = 720;
	_state = RenderingState::SETUP;

	m_ResolutionX = 640;
	m_ResolutionY = 360;
	m_ImagePosX = _screenWidth - (m_ResolutionX + 50);
	m_ImagePosY = 100;

	m_TexResolutionX = 400;
	m_TexResolutionY = 400;
	m_TexPosX = 50;
	m_TexPosY = _screenHeight - (m_TexResolutionX + 50);
}


ApplicationRoot::~ApplicationRoot()
{
	SDL_DestroyTexture(renderTex);
	SDL_DestroyRenderer(sdlRenderer);
}


void ApplicationRoot::run()
{
	initSystems();
	functionLoop();
	end();
}

void ApplicationRoot::end()
{
	TTF_CloseFont(m_ConsoleFontRegularPtr);
	TTF_CloseFont(m_ConsoleFontBoldPtr);
	delete[] pixels;
	delete scPtr;
	scPtr = nullptr;
	delete renderer;
	renderer = nullptr;
	delete guiFunctions;
	guiFunctions = nullptr;
	delete ofWnd;
	ofWnd = nullptr;
	delete imgExp;
	imgExp = nullptr;
}

void ApplicationRoot::initSystems()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();
	atexit(TTF_Quit);

	ilInit();
	iluInit();
	ilutRenderer(ILUT_DIRECT3D9);

	_window = SDL_CreateWindow("LightWhat - Robert Lindner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
		, _screenWidth, _screenHeight, 
		0);

	sdlRenderer = SDL_CreateRenderer(_window, -1, 0);
	renderTex = SDL_CreateTexture(sdlRenderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, _screenWidth, _screenHeight);
	pixels = new Uint32[_screenWidth * _screenHeight];
	memset(pixels, 30, _screenWidth * _screenHeight * sizeof(Uint32));

	scPtr = new Scene();
	renderer = new Renderer();
	guiFunctions = new winmain(_screenWidth, _screenHeight); 
	imgExp = new ImageExporter();
	_mode = PerformanceMode::VIEW;


	cout << "loading fonts...";
	char ownPth[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));

		std::string t = ownPth;
		std::string s = "LightWhat.exe";

		std::string::size_type i = t.find(s);

		if (i != std::string::npos)
			t.erase(i, s.length());

		exeDirectory = t;

		m_ConsoleFontRegularPtr = TTF_OpenFont((exeDirectory + string("fonts/Inconsolata-Regular.ttf")).c_str(), 20);
		m_ConsoleFontBoldPtr = TTF_OpenFont((exeDirectory + string("fonts/Inconsolata-Bold.ttf")).c_str(), 20);

		cout << "    fonts loaded!" << endl;
	}
	else cout << "    font loading failed!" << endl;
	cout << "backface culling enabled" << endl;
}

void ApplicationRoot::functionLoop()
{
	while(_state != RenderingState::EXIT)
	{
		switch (_state)
		{
		case RenderingState::SETUP:
			processInput();
			drawImage();
			break;
		case RenderingState::RENDER:
			processInput();
			updateImage();
			drawImage();
			break;
		}
	}
}


void ApplicationRoot::processInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)){
		switch (evnt.type) {
		case SDL_QUIT:
			_state = RenderingState::EXIT;
			cout << "quitting.... " << endl;
			break; 
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_KP_ENTER:
				_state = RenderingState::RENDER;
				cout << endl << "starting render.... " << endl;
				start = clock();
				break;
			case SDLK_RETURN:
				_state = RenderingState::RENDER;
				cout << endl << "starting render.... " << endl;
				start = clock();
				break;
			case SDLK_l:
				fileName = guiFunctions->getFileName(ofWnd);
				if ((fileName==string("nofile")))
				{
					cout << "loading file canceled " << endl;
				}
				else
				{
					cout << "loading file: " << fileName.c_str() << endl;
					scPtr->loadFile(fileName);
					cout << "file loaded! " << endl;
					scPtr->updateSceneInfo();
					sceneInfo.clear();
					sceneInfo.push_back(string("Scene info:\n"));
					sceneInfo.push_back(string("Vertices: ") + to_string(scPtr->vertCount) + string("\n"));
					sceneInfo.push_back(string("Triangles: ") + to_string(scPtr->triCount) + string("\n"));
					sceneInfo.push_back(string("Objects: ") + to_string(scPtr->objCount) + string("\n"));
					sceneInfo.push_back(string("Lights: ") + to_string(scPtr->lightCount) + string("\n"));
				}
				break;
			case SDLK_t:
				cout << "loading test scene..." << endl;
				scPtr->loadTestScene();
				cout << "scene loaded! " << endl;
				scPtr->updateSceneInfo();
				sceneInfo.clear();
				sceneInfo.push_back(string("Scene info:\n"));
				sceneInfo.push_back(string("Vertices: ") + to_string(scPtr->vertCount)+string("\n"));
				sceneInfo.push_back(string("Triangles: ") + to_string(scPtr->triCount) + string("\n"));
				sceneInfo.push_back(string("Objects: ") + to_string(scPtr->objCount) + string("\n"));
				sceneInfo.push_back(string("Lights: ") + to_string(scPtr->lightCount) + string("\n"));
				break;
			case SDLK_c:
				cout << "clearing scene..." << endl;
				scPtr->clearScene();
				cout << "scene cleared! " << endl;
				break;
			case SDLK_s:
				fileName = guiFunctions->saveFileName(ofWnd); 
				if ((fileName == string("nofile")))
				{
					cout << "saving file canceled " << endl;
				}
				else
				{
					cout << "saving file: " << (fileName+string(".bmp")).c_str() << endl;
					imgExp->saveBMP((fileName + string(".bmp")).c_str(), m_ResolutionX, m_ResolutionY, 72, *daImage);
					cout << "file saved! " << endl;
				}
				break;
			case SDLK_p:
				if (_mode == PerformanceMode::VIEW){
					cout << "high performance mode"<< endl;
					_mode = PerformanceMode::HIGH;
				}
				else{
					cout << "progress viewing mode" << endl;
					_mode = PerformanceMode::VIEW;
				}
				break;
			case SDLK_b:
				if (renderer->m_BackfaceCulling==false){
					cout << "backface culling enabled" << endl;
					renderer->m_BackfaceCulling = true;
				}
				else{
					cout << "backface culling disabled" << endl;
					renderer->m_BackfaceCulling = false;
				}
				break;
			case SDLK_d:
				drawTexture();
				break;
			default:
				break;
			}
		}
	}
}

void ApplicationRoot::updateImage()
{
	//setup scene
	if (!isSceneLoaded)
	{
		scPtr->setupCamera(m_ResolutionX, m_ResolutionY);
		renderer->setScene(scPtr);
		renderer->init(m_ResolutionX, m_ResolutionY);
		isSceneLoaded = true;
		float duration = (std::clock() - start) / (float)CLOCKS_PER_SEC;
		cout << "time for setup: " << duration << " seconds" << endl;
		cout << endl << "rendering.... " << endl;
	}
	//render
	if (renderer->renderNextTile())
	{
		_state = RenderingState::SETUP;
		isSceneLoaded = false;
	}
	if (_mode == PerformanceMode::VIEW)
	{
		daImage = renderer->getImage();
		for (int i = 0; i < m_ResolutionX; i++)
		{
			for (int j = 0; j < m_ResolutionY; j++)
			{
				setPixel(i + m_ImagePosX, j + m_ImagePosY, daImage->getRGB(i, j));
			}
		}
	}
	//Postprocessing
	//if (m_ColsRendered % 20 == 19 || _state == RenderingState::SETUP)
	if (_state == RenderingState::SETUP)
	{
		daImage = renderer->getImage();
		cout << "postprocessing..." << endl;
		postPr.updateHighestExposure(*daImage, m_ResolutionX, m_ResolutionY);
		postPr.controlExposure(*daImage, m_ResolutionX, m_ResolutionY, AUTO);
		float duration = (std::clock() - start) / (float)CLOCKS_PER_SEC;
		cout << endl << "render completed!" << endl << "time: " << duration << " seconds" << endl;

		//Update SDL texture
		for (int i = 0; i < m_ResolutionX; i++)
		{
			for (int j = 0; j < m_ResolutionY; j++)
			{
				setPixel(i + m_ImagePosX, j + m_ImagePosY, daImage->getRGB(i,j));
			}
		}
	}
	m_ColsRendered++;
}

void ApplicationRoot::drawTexture(){
	if (scPtr->textures.size() >0)
	{
		Texture tex = scPtr->textures[dispTexIdx];
		{
			for (int i = 0; i < m_TexResolutionX; i++)
			{
				for (int j = 0; j < m_TexResolutionY; j++)
				{
					setPixel(i + m_TexPosX, j + m_TexPosY, tex.getRGB(((float)i / (float)m_TexResolutionX), ((float)j / (float)m_TexResolutionY)));
				}
			}
		}
	}
	else
	{
		int texSizeX = 10, texSizeY = 10;
		Texture tex = Texture(string("test texture"), texSizeX, texSizeY);
		tex.setInterpolationMode(INTPOL_LINEAR);
		tex.setQuadraticFittingMode(FIT_STRETCHXY);
		for (int x = 0; x < texSizeX; x++)
		{
			for (int y = 0; y < texSizeY; y++)
			{
				tex.setRGB(colRGB(x / (float)texSizeX, y / (float)texSizeY, 1), x, y);
			}
		}
		for (int i = 0; i < m_TexResolutionX; i++)
		{
			for (int j = 0; j < m_TexResolutionY; j++)
			{
				float x = (float)i / (float)m_TexResolutionX;
				float y = (float)j / (float)m_TexResolutionY;
				colRGB thisPixel = tex.getRGB(x, y);
				setPixel(i + m_TexPosX, j + m_TexPosY, thisPixel);
			}
		}
	}
}

void ApplicationRoot::renderText(const std::string &message, TTF_Font *daFont,
	SDL_Color color, int fontSize, int posX, int posY)
{
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(daFont, message.c_str(), color);
	if (surf == nullptr){
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, surf);
	if (texture == nullptr){
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);

	SDL_Rect texture_rect;
	texture_rect.x = posX;  //the x coordinate
	texture_rect.y = posY; // the y coordinate
	texture_rect.w = message.length()*fontSize/2; //the width of the texture
	texture_rect.h = fontSize;

	SDL_RenderCopy(sdlRenderer, texture, NULL, &texture_rect);

	SDL_DestroyTexture(texture);
}

void ApplicationRoot::displaySceneInfo(){
	SDL_Color color = { 255, 255, 255, 255 };
	int x = m_ImagePosX, dy = 30, fSize = 20, y = m_ImagePosY + m_ResolutionY + dy;
	for (size_t i = 0; i < sceneInfo.size(); i++){
		renderText(sceneInfo[i], m_ConsoleFontRegularPtr, color, fSize, x, y);
		y += dy;
	}
}

void ApplicationRoot::drawImage()
{
	SDL_UpdateTexture(renderTex, NULL, pixels, _screenWidth *sizeof(Uint32));

	SDL_RenderClear(sdlRenderer);
	SDL_RenderCopy(sdlRenderer, renderTex, NULL, NULL);

	displaySceneInfo();

	SDL_RenderPresent(sdlRenderer);

}

void ApplicationRoot::setPixel(int x, int y, colRGB col)
{
	if (y >= 0 && y < _screenHeight && x >= 0 && x < _screenWidth)
	{
		int red = (int)(col.red * 255) << 16;
		int green = (int)(col.green * 255) << 8;
		int blue = (int)(col.blue * 255);
		Uint32 uint = Uint32(red + green + blue);
		pixels[y * _screenWidth + x] = uint;
	}
}
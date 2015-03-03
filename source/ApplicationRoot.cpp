#include "ApplicationRoot.h"
#include <stdexcept>

ApplicationRoot::ApplicationRoot()
{
	_window = nullptr;
	_screenWidth = 1280;
	_screenHeight = 720;
	_state = RenderingState::SETUP;

	m_ResolutionX = 128;
	m_ResolutionY = 64;
	m_ResolutionX = 640;
	m_ResolutionY = 360;
	m_ImagePosX = _screenWidth - (m_ResolutionX + 50);
	m_ImagePosY = 100;
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
				cout << "rendering.... " << endl;
				start = clock();
				break;
			case SDLK_RETURN:
				_state = RenderingState::RENDER;
				cout << "rendering.... " << endl;
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
					cout << endl << "Scene info:" << endl;
					cout << "Vertices: " << scPtr->vertCount << endl;
					cout << "Triangles: " << scPtr->triCount << endl;
					cout << "Objects: " << scPtr->objCount << endl;
					cout << "Lights: " << scPtr->lightCount << endl << endl;
				}
				break;
			case SDLK_t:
				cout << "loading test scene..." << endl;
				scPtr->loadTestScene();
				cout << "scene loaded! " << endl;
				scPtr->updateSceneInfo();
				cout << endl << "Scene info:" << endl;
				cout << "Vertices: " << scPtr->vertCount << endl;
				cout << "Triangles: " << scPtr->triCount << endl;
				cout << "Objects: " << scPtr->objCount << endl;
				cout << "Lights: " << scPtr->lightCount << endl << endl;
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
					imgExp->saveBMP((fileName + string(".bmp")).c_str(), m_ResolutionX, m_ResolutionY, 72, daImage);
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
	}
	//render
	if (renderer->renderNextRow())
	{
		_state = RenderingState::SETUP;
		isSceneLoaded = false;
	}
	//Postprocessing
	daImage = renderer->getImage();
	//if (m_ColsRendered % 20 == 19 || _state == RenderingState::SETUP)
	if (_state == RenderingState::SETUP)
	{
		cout << "postprocessing..." << endl;
		postPr.updateHighestExposure(daImage, m_ResolutionX, m_ResolutionY);
		daImage = postPr.controlExposure(daImage, m_ResolutionX, m_ResolutionY, AUTO);
		double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		cout << endl << "render completed!" << endl << "time: " << duration << " seconds" << endl;

		//Update SDL texture
		for (int i = 0; i < m_ResolutionX; i++)
		{
			for (int j = 0; j < m_ResolutionY; j++)
			{
				setPixel(i + m_ImagePosX, j + m_ImagePosY, daImage[i][j]);
			}
		}
	}
	m_ColsRendered++;
	if (_mode == PerformanceMode::VIEW)
	{
		for (int i = 0; i < m_ResolutionX; i++)
		{
			for (int j = 0; j < m_ResolutionY; j++)
			{
				setPixel(i + m_ImagePosX, j + m_ImagePosY, daImage[i][j]);
			}
		}
	}
}

void ApplicationRoot::drawImage()
{
	SDL_UpdateTexture(renderTex, NULL, pixels, _screenWidth *sizeof(Uint32));

	SDL_RenderClear(sdlRenderer);
	SDL_RenderCopy(sdlRenderer, renderTex, NULL, NULL);
	SDL_RenderPresent(sdlRenderer);

}

void ApplicationRoot::setPixel(int x, int y, colRGB col)
{
	int red = (int)(col.red * 255) << 16;
	int green = (int)(col.green * 255) << 8;
	int blue = (int)(col.blue * 255);
	Uint32 uint = Uint32(red + green + blue);
	pixels[y * _screenWidth + x] = uint;
}
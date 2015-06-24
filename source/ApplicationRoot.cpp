#include "ApplicationRoot.hpp"
#include <stdexcept>
#include "GUI\GuiEngine.hpp"

//-----------------------------------------------------------------
// Defines
//-----------------------------------------------------------------
#define GUI_ENGINE (GuiEngine::GetSingleton())

ApplicationRoot::ApplicationRoot()
{
}


ApplicationRoot::~ApplicationRoot()
{
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
	delete m_Renderer;
	m_Renderer = nullptr;
	delete imgExp;
	imgExp = nullptr;
	delete m_BmpResultPtr;
	m_BmpResultPtr = nullptr;
	delete m_BmpTexturePtr;
	m_BmpTexturePtr = nullptr;
}

void ApplicationRoot::initSystems()
{
	GUI_ENGINE->Init();

	ilInit();
	iluInit();
	ilutRenderer(ILUT_DIRECT3D9);

	m_Renderer = new LWRenderer();
	imgExp = new ImageExporter();


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

		m_Renderer->init((exeDirectory + string("Resources/sceneDescriptionFiles/sceneCornell.lwtf")).c_str());
		m_ResolutionX = m_Renderer->m_Settings.resolutionX;
		m_ResolutionY = m_Renderer->m_Settings.resolutionY;
		m_ImagePosX = GUI_ENGINE->GetWidth() - (m_ResolutionX + 50);
		m_ImagePosY = 50;
		m_BmpResultPtr = new Bitmap(m_ResolutionX, m_ResolutionY);

		m_TexResolutionX = 400;
		m_TexResolutionY = 400;
		m_TexPosX = 50;
		m_TexPosY = GUI_ENGINE->GetHeight() - (m_TexResolutionX + 50);
		m_BmpTexturePtr = new Bitmap(m_TexResolutionX, m_TexResolutionY);

		m_ConsoleFontRegularPtr = TTF_OpenFont((exeDirectory + string("Resources/fonts/Inconsolata-Regular.ttf")).c_str(), 20);
		m_ConsoleFontBoldPtr = TTF_OpenFont((exeDirectory + string("Resources/fonts/Inconsolata-Bold.ttf")).c_str(), 20);

		cout << "    fonts loaded!" << endl;
	}
	else cout << "    font loading failed!" << endl;
	cout << "high performance mode" << endl;
	if (m_Renderer->m_Settings.backfaceCulling == false){
		cout << "backface culling disabled" << endl;
	}
	else{
		cout << "backface culling enabled" << endl;
	}
}

void ApplicationRoot::functionLoop()
{
	while(exit == false)
	{
		//Handle user input
		processInput();
		switch (m_Renderer->m_RenderStatus)
		{
		case LW_RUNNING:
			//Render a few samples
			updateImage();
			break;
		}
		//Draw Scene Info
		SDL_Color color = { 255, 255, 255, 255 };
		int x = m_ImagePosX, dy = 30, fSize = 20, y = m_ImagePosY - 30;
		GUI_ENGINE->DrawString(m_Renderer->m_Status, m_ConsoleFontRegularPtr, color, fSize, x, y);
		y = m_ImagePosY + m_ResolutionY + dy;
		for (size_t i = 0; i < sceneInfo.size(); i++){
			GUI_ENGINE->DrawString(sceneInfo[i], m_ConsoleFontRegularPtr, color, fSize, x, y);
			y += dy;
		}
		//Draw Texture Bitmap
		if (m_DrawTexture)
		{
			GUI_ENGINE->DrawBitmap(m_TexPosX, m_TexPosY, m_BmpTexturePtr);
		}
		//Draw Rendered image
		GUI_ENGINE->DrawBitmap(m_ImagePosX, m_ImagePosY, m_BmpResultPtr);
		//Make SDL render the GUI
		GUI_ENGINE->Paint();
	}
}


void ApplicationRoot::processInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)){
		switch (evnt.type) {
		case SDL_QUIT:
			exit = true;
			cout << "quitting.... " << endl;
			break; 
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_KP_ENTER:
				cout << endl << "starting render.... " << endl;
				m_Renderer->preprocess();
				break;
			case SDLK_RETURN:
				cout << endl << "starting render.... " << endl;
				m_Renderer->preprocess();
				break;
			case SDLK_l:
				fileName = GUI_ENGINE->GetWindow()->GetFileName();
				if ((fileName==string("nofile")))
				{
					cout << "loading file canceled " << endl;
				}
				else
				{
					cout << "loading file: " << fileName.c_str() << endl;
					m_Renderer->importExternal(fileName.c_str());
					cout << "file loaded! " << endl;
					sceneInfo.clear();
					sceneInfo.push_back(string("Scene info:\n"));
					sceneInfo.push_back(string("Vertices: ") + to_string(m_Renderer->m_Scene->vertCount) + string("\n"));
					sceneInfo.push_back(string("Triangles: ") + to_string(m_Renderer->m_Scene->triCount) + string("\n"));
					sceneInfo.push_back(string("Objects: ") + to_string(m_Renderer->m_Scene->objCount) + string("\n"));
					sceneInfo.push_back(string("Lights: ") + to_string(m_Renderer->m_Scene->lightCount) + string("\n"));
				}
				break;
			case SDLK_t:
				cout << "loading test scene..." << endl;
				m_Renderer->loadLWTF((exeDirectory + string("Resources/sceneDescriptionFiles/sceneCornell.lwtf")).c_str());
				cout << "scene loaded! " << endl;
				sceneInfo.clear();
				sceneInfo.push_back(string("Scene info:\n"));
				sceneInfo.push_back(string("Vertices: ") + to_string(m_Renderer->m_Scene->vertCount) + string("\n"));
				sceneInfo.push_back(string("Triangles: ") + to_string(m_Renderer->m_Scene->triCount) + string("\n"));
				sceneInfo.push_back(string("Objects: ") + to_string(m_Renderer->m_Scene->objCount) + string("\n"));
				sceneInfo.push_back(string("Lights: ") + to_string(m_Renderer->m_Scene->lightCount) + string("\n"));
				break;
			case SDLK_c:
				cout << "clearing scene..." << endl;
				m_Renderer->clear();
				cout << "scene cleared! " << endl;
				break;
			case SDLK_s:
				fileName = GUI_ENGINE->GetWindow()->SaveFileName();
				if ((fileName == string("nofile")))
				{
					cout << "saving file canceled " << endl;
				}
				else
				{
					cout << "saving file: " << (fileName+string(".bmp")).c_str() << endl;
					imgExp->saveBMP((fileName + string(".bmp")).c_str(), m_Renderer->m_Settings.resolutionX, m_Renderer->m_Settings.resolutionY, 72, *m_Renderer->m_TonemappedImage);
					cout << "file saved! " << endl;
				}
				break;
			case SDLK_b:
				if (m_Renderer->m_Settings.backfaceCulling == false){
					cout << "backface culling enabled" << endl;
					m_Renderer->m_Settings.backfaceCulling = true;
				}
				else{
					cout << "backface culling disabled" << endl;
					m_Renderer->m_Settings.backfaceCulling = false;
				}
				break;
			case SDLK_d:
				m_DrawTexture = !m_DrawTexture;
				if (m_DrawTexture)
				{
					drawTexture();
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
	//render
	if (m_Renderer->updateRender())
	{
		m_Renderer->postProcessRender();
		for (int i = 0; i < m_ResolutionX; i++)
		{
			for (int j = 0; j < m_ResolutionY; j++)
			{
				m_BmpResultPtr->SetPixel(i, j, m_Renderer->m_TonemappedImage->getRGB(i, j));
			}
		}
	}
}

void ApplicationRoot::drawTexture(){
	if (m_Renderer->m_Scene->textures.size() >0)
	{
		Texture tex = m_Renderer->m_Scene->textures[dispTexIdx];
		for (int i = 0; i < m_TexResolutionX; i++)
		{
			for (int j = 0; j < m_TexResolutionY; j++)
			{
				m_BmpTexturePtr->SetPixel(i, j, tex.getRGB(((float)i / (float)m_TexResolutionX), ((float)j / (float)m_TexResolutionY)));
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
				m_BmpTexturePtr->SetPixel(i, j, thisPixel);
			}
		}
	}
}
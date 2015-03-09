#include "winmain.hpp"


winmain::winmain(int lX, int lY)
{
	m_Pixels = new Uint32[lX * lY];
}


winmain::~winmain()
{
}

string winmain::getFileName(HWND wnd)
{
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = wnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "3d model files (*.dae, *.3DS)\0*.dae;*.3DS";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if(GetOpenFileName(&ofn))
	return string(ofn.lpstrFile);
	else return string("nofile");
}

string winmain::getTTFName(HWND wnd)
{
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = wnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Font files (*.ttf)\0*.ttf";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
		return string(ofn.lpstrFile);
	else return string("nofile");
}

string winmain::saveFileName(HWND wnd)
{
	OPENFILENAME ofn;
	char szFile[256];
	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = wnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Bitmap Images (*.bmp)\0*.bmp\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetSaveFileName(&ofn))
		return string(ofn.lpstrFile);
	else return string("nofile");
}
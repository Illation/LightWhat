//version 0.4.1
//by Robert Lindner
//using SDL2, Assimp and tinyxml2

#include "ApplicationRoot.h"
#include <iostream>

int main(int argc, char** argv)
{
	ApplicationRoot *renderApp = new ApplicationRoot();
	renderApp->run();
	delete renderApp;
	return 0;
}
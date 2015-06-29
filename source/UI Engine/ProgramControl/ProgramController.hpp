#pragma once

#include <iostream>
#include <ctime>

#include "AbstractProgram.hpp"
//-----------------------------------------------------------------
// ProgramController Class
//
//Responsible for the program Event flow and Correct init order
//-----------------------------------------------------------------
class ProgramController
{
public:
	ProgramController(AbstractProgram *programPtr);
	~ProgramController();

private:
	void Init();
	void EventLoop();

	bool m_Exit = false;
	AbstractProgram *m_ProgramPtr = nullptr;
};


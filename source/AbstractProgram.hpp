#pragma once
//-----------------------------------------------------------------
// Abstract Program Class
//-----------------------------------------------------------------
class AbstractProgram
{
public:
	AbstractProgram();
	virtual ~AbstractProgram();

	// C++11 make the class non-copyable
	AbstractProgram(const AbstractProgram&) = delete;
	AbstractProgram& operator=(const AbstractProgram&) = delete;

	virtual void Init(){}
	virtual void Tick(){}
	virtual void Paint(){}
};


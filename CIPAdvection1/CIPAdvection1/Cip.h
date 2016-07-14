#pragma once
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Config.h"
#include "AdvectionScheme.h"

using namespace std;

class CCip : public CAdvectionScheme
{
public:
private:
	double* _g_t;		//”÷•ª’n—p
	double* _g_t_1;		//t+ƒ¢1‚Ì”÷•ª’l
	virtual void keisan();
public:
	virtual void keisan_auto();
	virtual void keisan_step();
	virtual double getDataAt(int index);
	virtual const double* const getData();
	virtual int getSize();
	virtual bool isEnd();
	CCip(const Config* const config);
	virtual ~CCip();
};


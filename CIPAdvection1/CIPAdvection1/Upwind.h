#pragma once
#include "Config.h"
#include "AdvectionScheme.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

class CUpwind : public CAdvectionScheme
{
private:
	double c;
	virtual void keisan();
public:
	virtual void keisan_auto();
	virtual void keisan_step();
	virtual double getDataAt(int index);
	virtual const double* const getData();
	virtual int getSize();
	virtual bool isEnd();
	CUpwind(const Config* const config);
	virtual ~CUpwind();
};


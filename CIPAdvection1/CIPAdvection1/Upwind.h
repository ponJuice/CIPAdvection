#pragma once
#include "Config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

class CUpwind
{
private:
	string* _name;
	int _endStep;		//シミュレーション終わりステップ数
	int _stepCounter;	//ステップカウンタ
	double* _data_t;	//tの計算格子（一次元）
	double* _data_t_1;	//t+Δ1の計算格子
	int _x_size;		//計算格子数
	double _x_length;	//計算空間の長さ
	double _deltaTime;	//シミュレーション間隔
	double _deltaX;		//格子の間隔
	double _speed;		//移流速度
	double c;
	void keisan();
public:
	void createGnuplotAnimationConfig(string *fileName,string *title) const;
	void keisan_auto();
	void keisan_step();
	double getDataAt(int index);
	const double* getData();
	int getSize();
	bool isEnd();
	CUpwind(Config* config);
	~CUpwind();
};


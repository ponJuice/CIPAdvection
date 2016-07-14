#pragma once
#include <string>
#include <fstream>
#include <algorithm>
#include "Config.h"

using namespace std;

class CAdvectionScheme
{
protected:
	string* _name;		//スキーム名
	int _endStep;		//シミュレーション終わりステップ数
	int _stepCounter;	//ステップカウンタ
	double* _data_t;	//tの計算格子（一次元）
	double* _data_t_1;	//t+Δ1の計算格子
	int _x_size;		//計算格子数
	double _x_length;	//計算空間の長さ
	double _deltaTime;	//シミュレーション間隔
	double _deltaX;		//格子の間隔
	double _speed;		//移流速度
	virtual void keisan() = 0;
public:
	virtual void createGnuplotAnimationConfig(const string* const fileName, const string* const title,int dataColmun) const;
	virtual void keisan_auto() = 0;
	virtual void keisan_step() = 0;
	virtual double getDataAt(int index) = 0;
	virtual const double* const getData() = 0;
	virtual int getSize() = 0;
	virtual bool isEnd() = 0;
	virtual string* getSchemeName();
	void createGnuplotAnimationConfig(const string* const fileName, const string* const title,CAdvectionScheme** cas_array,int size) {
		string _fileName(*fileName);
		replace(_fileName.begin(), _fileName.end(), '\\', '/');
		string gpFile(*fileName);
		gpFile += "_animation_Mix.gp";
		string pltFile(*fileName);
		pltFile += "_animation_Mix.plt";
		ofstream gp(gpFile);
		ofstream plt(pltFile);
		replace(pltFile.begin(), pltFile.end(), '\\', '/');

		string text;

		int num = 2;

		text += "plot \"";
		text += _fileName.c_str();
		text += ".dat\" index n using 1:";
		text += std::to_string(num);
		text += " title \"";
		text += "Exact";
		text += "\" w lp,";

		for (int l = 0; l < size; l++) {
			text += "\"";
			text += _fileName.c_str();
			text += ".dat\" index n using 1:";
			text += std::to_string(num+1+l);
			text += " title \"";
			text += *(cas_array[l]->getSchemeName());
			text += "\" w lp,";
		}
		text.pop_back();

		gp << "reset" << std::endl
			<< "set term gif animate" << std::endl
			<< "set output \"" << _fileName.c_str() << "_Mix_.gif\"" << std::endl
			<< "set title \"" << title->c_str() << "\"" << std::endl
			<< "set yrange [0:1.1]" << std::endl
			<< "value_start = " << "0" << std::endl
			<< "value_max = " << _endStep << std::endl
			<< "value_delta = " << "1" << std::endl
			<< "load \"" << pltFile.c_str() << "\"" << std::endl;

		plt << "if(exist(\"n\") == 0 || n<0) n = value_start" << std::endl
			<< "label(n) = sprintf(\"Step:%d\",n)" << std::endl
			<< "set label 1 label(n)  at 2, 1.15 left" << std::endl
			<< text << std::endl
			<< "n = n + value_delta" << std::endl
			<< "if ( n < value_max ) reread" << std::endl
			<< "undefine n" << std::endl;

	}
public:
	CAdvectionScheme() {};
	CAdvectionScheme(const Config* const config);
	virtual ~CAdvectionScheme();
};


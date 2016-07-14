#include "AdvectionScheme.h"



CAdvectionScheme::CAdvectionScheme(const Config* const config)
{
	_name = nullptr;
	//設定用構造体の中身をコピー
	_endStep = config->endStep;					//終わりステップ数
	_x_size = config->x_size + 2;				//格子数
	_x_length = config->x_length;				//計算空間の長さ
	_speed = config->speed;						//移流速度
	_deltaTime = config->deltaTime;				//シミュレーション間隔
	_deltaX = _x_length / (double)(_x_size - 2);//格子間隔
	_data_t = new double[_x_size];				//計算格子作成
	_data_t_1 = new double[_x_size];			//　⇒外側の事も考慮する

	_stepCounter = 0;							//計算ステップカウンタ初期化

	//データ配列を0初期化
	for (int n = 0; n < _x_size; n++) {
		_data_t[n] = 0;
		_data_t_1[n] = 0;
	}

	//初期値をコピー
	for (int n = 0; n < _x_size - 2; n++) {
		_data_t[n + 1] = config->initData[n];
	}
}


CAdvectionScheme::~CAdvectionScheme()
{
	if (_name != nullptr)
		delete _name;
	delete[] _data_t;
	delete[] _data_t_1;
}

string* CAdvectionScheme::getSchemeName() {
	return _name;
}

void CAdvectionScheme::createGnuplotAnimationConfig(const string* const fileName,const string* const title,int dataColumn) const{
	string _fileName(*fileName);
	replace(_fileName.begin(), _fileName.end(), '\\', '/');
	string gpFile(*fileName);
	gpFile += "_animation_" + (*_name) + ".gp";
	string pltFile(*fileName);
	pltFile += "_animation_" + (*_name) + ".plt";
	ofstream gp(gpFile);
	ofstream plt(pltFile);
	replace(pltFile.begin(), pltFile.end(), '\\', '/');

	gp << "reset" << std::endl
		<< "set term gif animate" << std::endl
		<< "set output \"" << _fileName.c_str() << "_Exact_and_" << _name->c_str() << ".gif\"" << std::endl
		<< "set title \"" << title->c_str() << "\"" << std::endl
		<< "set yrange [0:1.1]" << std::endl
		<< "value_start = " << "0" << std::endl
		<< "value_max = " << _endStep << std::endl
		<< "value_delta = " << "1" << std::endl
		<< "load \"" << pltFile.c_str() << "\"" << std::endl;

	plt << "if(exist(\"n\") == 0 || n<0) n = value_start" << std::endl
		<< "label(n) = sprintf(\"Step:%d\",n)" << std::endl
		<< "set label 1 label(n)  at 2, 1.15 left" << std::endl
		<< "plot \"" << _fileName.c_str() << ".dat\" index n using 1:2 title \"Exact\" w lp,"
		<< "\"" << _fileName.c_str() << ".dat\" index n using 1:" << dataColumn << " title \"" << _name->c_str() << "\" w lp" << std::endl
		<< "n = n + value_delta" << std::endl
		<< "if ( n < value_max ) reread" << std::endl
		<< "undefine n" << std::endl;
}
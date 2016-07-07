#include "Upwind.h"



CUpwind::CUpwind(Config* config)
{
	if (config->name == nullptr)
		_name = new string("");
	_name = config->name;
	//設定用構造体の中身をコピー
	_endStep = config->endStep;			//終わりステップ数
	_x_size = config->x_size+2;			//格子数
	_x_length = config->x_length;		//計算空間の長さ
	_speed = config->speed;				//移流速度
	_deltaTime = config->deltaTime;		//シミュレーション間隔
	_deltaX = _x_length / (double)(_x_size-2);			//格子間隔
	_data_t = new double[_x_size];	//計算格子作成
	_data_t_1 = new double[_x_size];	//外側の事も考慮する
	_stepCounter = 0;
	c = _speed * _deltaTime / _deltaX;
	for (int n = 0; n < _x_size; n++) {
		_data_t[n] = 0;
		_data_t_1[n] = 0;
	}
	for (int n = 0; n < _x_size - 2; n++) {
		_data_t[n + 1] = config->initData[n];
	}
}


CUpwind::~CUpwind()
{
	if(_name != nullptr)
		delete _name;
	delete[] _data_t;
	delete[] _data_t_1;
}

void CUpwind::createGnuplotAnimationConfig(string *fileName,string *title) const{
	string _fileName(*fileName);
	replace(_fileName.begin(), _fileName.end(), '\\', '/');
	string gpFile(*fileName);
	gpFile += "_animation.gp";
	string pltFile(*fileName);
	pltFile += "_animation.plt";
	ofstream gp(gpFile);
	ofstream plt(pltFile);
	replace(pltFile.begin(), pltFile.end(), '\\', '/');

	gp << "reset" << std::endl
		<< "set term gif animate" << std::endl
		<< "set output \"" << _fileName.c_str() << ".gif\"" << std::endl
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
		<< "\"" << _fileName.c_str() << ".dat\" index n using 1:3 title \"" << _name->c_str() << "\" w lp" << std::endl
		<< "n = n + value_delta" << std::endl
		<< "if ( n < value_max ) reread" << std::endl
		<< "undefine n" << std::endl;

}

void CUpwind::keisan() {
	for (int n = 1; n < _x_size-1; n++) {
		_data_t_1[n] = _data_t[n] - 0.5 * c * (_data_t[n + 1] - _data_t[n - 1])
			+ 0.5 * c * (_data_t[n + 1] - 2.0 * _data_t[n] + _data_t[n - 1]);
	}

	//入れ替え
	double* temp = _data_t;
	_data_t = _data_t_1;
	_data_t_1 = temp;
}

void CUpwind::keisan_auto() {
	while (!isEnd()) {
		keisan();
		_stepCounter++;
	}
}

void CUpwind::keisan_step() {
	if (isEnd())
		return;
	keisan();
	_stepCounter++;
}

const double* CUpwind::getData() {
	return &(_data_t[1]);
}

double CUpwind::getDataAt(int index) {
	if (index < 0 && index >= _x_size)
		return -1;
	return _data_t[index+1];
}

int CUpwind::getSize() {
	return _x_size;
}

bool CUpwind::isEnd() {
	return _stepCounter >= _endStep;
}
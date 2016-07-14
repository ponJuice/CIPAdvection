#include "Cip.h"

CCip::CCip(const Config* const config) : CAdvectionScheme(config)
{
	_name = new string("CIP");		//スキーマ名

	//微分用格子作成
	_g_t = new double[_x_size];
	_g_t_1 = new double[_x_size];

	//微分用格子初期化
	for (int n = 0; n < _x_size; n++) {
		_g_t[n] = 0;
		_g_t_1[n] = 0;
	}
}


CCip::~CCip()
{
	delete[] _g_t;
	delete[] _g_t_1;
}

void CCip::keisan() {
	double a = 0;
	double b = 0;
	double D = -1.0 * _deltaX;
	double ip = -1.0 * _speed * _deltaTime;
	for (int i = 1; i < _x_size - 1; i++) {
		a = (_g_t[i] + _g_t[i - 1]) / (D*D) + (2.0 * (_data_t[i] - _data_t[i - 1])) / (D*D*D);
		
		b = (3.0 * (_data_t[i - 1] - _data_t[i]) / (D*D)) - (2.0 * _g_t[i] + _g_t[i - 1]) / D;
		
		_data_t_1[i] = a*(ip*ip*ip) + b*(ip*ip) + _g_t[i] * ip + _data_t[i];
		
		_g_t_1[i] = 3.0*a*(ip*ip) + 2.0*b*ip + _g_t[i];
	}

	//入れ替え
	double* temp = _data_t;
	_data_t = _data_t_1;
	_data_t_1 = temp;

	temp = _g_t;
	_g_t = _g_t_1;
	_g_t_1 = temp;
}

void CCip::keisan_auto() {
	while (!isEnd()) {
		keisan();
		_stepCounter++;
	}
}

void CCip::keisan_step() {
	if (isEnd())
		return;
	keisan();
	_stepCounter++;
}

const double* const CCip::getData() {
	return &(_data_t[1]);
}

double CCip::getDataAt(int index) {
	if (index < 0 && index >= _x_size)
		return -1;
	return _data_t[index + 1];
}

int CCip::getSize() {
	return _x_size;
}

bool CCip::isEnd() {
	return _stepCounter >= _endStep;
}
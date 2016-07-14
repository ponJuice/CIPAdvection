#include "Upwind.h"



CUpwind::CUpwind(const Config* const config) : CAdvectionScheme(config)
{
	_name = new string("UPWIND");
	c = _speed * _deltaTime / _deltaX;
}


CUpwind::~CUpwind()
{
}

void CUpwind::keisan() {
	for (int n = 1; n < _x_size-1; n++) {
		_data_t_1[n] = _data_t[n] - 0.5 * c * (_data_t[n + 1] - _data_t[n - 1])
			+ 0.5 * c * (_data_t[n + 1] - 2.0 * _data_t[n] + _data_t[n - 1]);
	}

	//“ü‚ê‘Ö‚¦
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

const double* const CUpwind::getData() {
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
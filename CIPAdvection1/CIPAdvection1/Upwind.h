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
	int _endStep;		//�V�~�����[�V�����I���X�e�b�v��
	int _stepCounter;	//�X�e�b�v�J�E���^
	double* _data_t;	//t�̌v�Z�i�q�i�ꎟ���j
	double* _data_t_1;	//t+��1�̌v�Z�i�q
	int _x_size;		//�v�Z�i�q��
	double _x_length;	//�v�Z��Ԃ̒���
	double _deltaTime;	//�V�~�����[�V�����Ԋu
	double _deltaX;		//�i�q�̊Ԋu
	double _speed;		//�ڗ����x
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


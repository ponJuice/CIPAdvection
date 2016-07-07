#pragma once
#include <string>

class Config {
public:
	std::string *name;		//�X�L�[����
	int endStep;		//�I���X�e�b�v��
	double speed;		//�ڗ����x
	double deltaTime;	//�V�~�����[�V�����Ԋu
	int x_size;			//�v�Z�i�q��
	double x_length;	//�v�Z��Ԃ̒���
	double* initData;	//�����l
	Config() {
		name = nullptr;
	}
	~Config() {
	}
};
#pragma once
#include "Upwind.h"
#include "Cip.h"
#include "Config.h"
#include "GnuplotFileManager.h"

//�L�^�p�N���X
CGnuplotFileManager cgf;

int upCut(double n) {
	if (n / 10.0 != 0)
		return n + 1;
	else
		return n;
}

void genmitsuCalc(double* data, const Config* conf, int step,double deltaX,double length_height) {
	for (int n = 0; n < conf->x_size; n++) {
		if (n*deltaX > conf->speed*conf->deltaTime*((double)step) && n*deltaX < (conf->speed*conf->deltaTime*((double)step) + length_height*deltaX))
			data[n] = 1;
		else
			data[n] = 0;
	}
}

void test(CAdvectionScheme* cas) {
	return;
}

void main() {
	//�t�@�C����
	std::string fileName("C:\\common files\\Gnuplot\\CIP\\test");
	//�\�^�C�g��
	std::string title("numerical diffusion");

	cgf.openFile(&fileName, CGnuplotFileManager::MODE::NEW);
	if (cgf.isOpen(&fileName) == CGnuplotFileManager::FILE::NOTOPEN) {
		printf("�t�@�C�����J���܂���ł���\n");
		return;
	}

	const double* upWind;
	const double* cip;
	Config config;
	config.name = new string("Upwind");
	config.endStep = 100;
	config.speed = 0.5;
	config.deltaTime = 0.001;
	config.x_length = 0.1;
	config.x_size = 100;

	double deltaX = config.x_length / (double)config.x_size;
	double length_height = 20;	//��`�g�̏�̕����̒���

	/************���������̍쐬**************/
	double* initData = new double[config.x_size];
	for (int n = 0; n < config.x_size; n++) {
		if (n*deltaX < (config.speed*config.deltaTime + length_height*deltaX))
			initData[n] = 1;
		else
			initData[n] = 0;
	}
	config.initData = initData;
	/***************************************/

	//�v�Z�X�L�[���N���X�̍쐬
	CUpwind uw(&config);

	config.name = new string("CIP");
	CCip cp(&config);

	/***************�v�Z���L�^****************/
	int step = 0;
	double* dataset = new double[4];
	do {
		//�v�Z�f�[�^�擾
		upWind = uw.getData();
		cip = cp.getData();

		//�L�^
		for (int n = 1; n < config.x_size + 1; n++) {
			dataset[0] = n;
			dataset[1] = initData[n - 1];
			dataset[2] = upWind[n - 1];
			dataset[3] = cip[n - 1];
 			cgf.writeData(&fileName, dataset, 4);
		}

		//���ԋ�؂�iGnuplot�p�j
		cgf.nextDataBlock(&fileName);

		//�v�Z
		genmitsuCalc(initData, &config, step,deltaX,length_height);
		uw.keisan_step();
		cp.keisan_step();

		step++;
	} while (!uw.isEnd());

	uw.createGnuplotAnimationConfig(&fileName,&title,3);
	cp.createGnuplotAnimationConfig(&fileName, &title,4);

	CAdvectionScheme** cas_array = new CAdvectionScheme*[2];
	cas_array[0] = &uw;
	cas_array[1] = &cp;

	cp.createGnuplotAnimationConfig(&fileName, &title,cas_array,2);

	delete[] dataset;
	delete[] cas_array;
	delete[] initData;
}

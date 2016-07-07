#pragma once
#include "Upwind.h"
#include "Config.h"
#include "GnuplotFileManager.h"

//記録用クラス
CGnuplotFileManager cgf;

int upCut(double n) {
	if (n / 10.0 != 0)
		return n + 1;
	else
		return n;
}

void genmitsuCalc(double* data, const Config* conf, int step,double deltaX) {
	for (int n = 0; n < conf->x_size; n++) {
		if (n*deltaX > conf->speed*conf->deltaTime*((double)step) && n*deltaX < (conf->speed*conf->deltaTime*((double)step) + 5.0*deltaX))
			data[n] = 1;
		else
			data[n] = 0;
	}
}

void main() {
	//ファイル名
	std::string fileName("C:\\Users\\Goto\\Documents\\Gnuplot\\Data\\CIP\\test");
	//表タイトル
	std::string title("numerical diffusion");

	cgf.openFile(&fileName, CGnuplotFileManager::MODE::NEW);
	if (cgf.isOpen(&fileName) == CGnuplotFileManager::FILE::NOTOPEN) {
		printf("ファイルが開けませんでした\n");
		return;
	}

	const double* upWind;
	Config config;
	config.name = new string("Upwind");
	config.endStep = 100;
	config.speed = 0.5;
	config.deltaTime = 0.001;
	config.x_length = 0.1;
	config.x_size = 100;

	double deltaX = config.x_length / (double)config.x_size;

	/************初期条件の作成**************/
	double* initData = new double[config.x_size];
	for (int n = 0; n < config.x_size; n++) {
		if (n*deltaX < (config.speed*config.deltaTime + 5.0*deltaX))
			initData[n] = 1;
		else
			initData[n] = 0;
	}
	config.initData = initData;
	/***************************************/

	CUpwind uw(&config);

	/***************計算＆記録****************/
	int step = 0;
	double* dataset = new double[3];
	do {
		upWind = uw.getData();

		//記録
		for (int n = 1; n < config.x_size + 1; n++) {
			dataset[0] = n;
			dataset[1] = initData[n - 1];
			dataset[2] = upWind[n - 1];
			cgf.writeData(&fileName, dataset, 3);
		}

		cgf.nextDataBlock(&fileName);

		//計算
		genmitsuCalc(initData, &config, step,deltaX);
		uw.keisan_step();

		step++;
	} while (!uw.isEnd());

	uw.createGnuplotAnimationConfig(&fileName,&title);

	delete[] dataset;
	delete[] initData;
}

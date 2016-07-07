#pragma once
#include <string>

class Config {
public:
	std::string *name;		//スキーム名
	int endStep;		//終わりステップ数
	double speed;		//移流速度
	double deltaTime;	//シミュレーション間隔
	int x_size;			//計算格子数
	double x_length;	//計算空間の長さ
	double* initData;	//初期値
	Config() {
		name = nullptr;
	}
	~Config() {
	}
};
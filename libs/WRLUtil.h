#pragma once
#pragma once
const double WRL_PI = 3.14159265358979;

//符号を返す(-1 or 1, int)
int Sign(double n);

//percentage %の確率でtrueを返す
bool Chance(int percentage);

/*
乱数を最低値と最高値を設定して返す。
RNG(1,3)なら1, 2, 3のどれかが返る。preciseModeをtrueにするとより精密に乱数を取る。
*/
int RNG(int min, int max, bool preciseMode = false);

/*Clip(100,0,5) -> 5, Clip(-5,0,5) -> 0*/
template<class T> T Clip(T base, T min, T max)
{
	return T(base < min ? min : base > max ? max : base);
}

//いつも公式忘れるやつら
//距離と角度(度数法)からXを求める
double ConvertDegDistToX(double dist, double deg);

//距離と角度(度数法)からYを求める
double ConvertDegDistToY(double dist, double deg);


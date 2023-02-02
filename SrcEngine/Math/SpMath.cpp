#include "SpMath.h"

//ラジアンから角度
float ConvertRadianToAngle(float radian) {
	float angle = 180 / PIf * radian;
	return angle;
}

//角度からラジアン
float ConvertAngleToRadian(float angle) {
	float radian = PIf / 180 * angle;
	return radian;
}
#include "SpMath.h"

//���W�A������p�x
float ConvertRadianToAngle(float radian) {
	float angle = 180 / PIf * radian;
	return angle;
}

//�p�x���烉�W�A��
float ConvertAngleToRadian(float angle) {
	float radian = PIf / 180 * angle;
	return radian;
}
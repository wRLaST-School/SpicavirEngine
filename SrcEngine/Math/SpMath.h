#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Float2.h"
#include "Float3.h"
#include "Float4.h"
#include "Matrix.h"
#include <Quaternion.h>
#include "Ray.h"

#ifndef PI
#define PI 3.14159265359
#endif

#ifndef PIf
#define PIf 3.14159265359f
#endif

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

//���W�A������p�x
float ConvertRadianToAngle(float radian);

//�p�x���烉�W�A��
float ConvertAngleToRadian(float angle);
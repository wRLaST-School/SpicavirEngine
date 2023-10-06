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

//ラジアンから角度
float ConvertRadianToAngle(float radian);

//角度からラジアン
float ConvertAngleToRadian(float angle);
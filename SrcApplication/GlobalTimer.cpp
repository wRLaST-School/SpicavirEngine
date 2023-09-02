#include "stdafx.h"
#include "GlobalTimer.h"
int64_t GlobalTimer::sTime = 0;
void GlobalTimer::Update()
{
	sTime++;
}

#pragma once
#include <stdafx.h>

#define RegisterScript(classname) extern "C" __declspec(dllexport) classname* classname ## _Create() {return new classname ## ();};

//#define RegisterScript(classname) classname* classname ## _Create() {return new classname ## ();};
#pragma once

#define Register(classname) extern "C" __declspec(dllexport) classname* classname ## _Create();


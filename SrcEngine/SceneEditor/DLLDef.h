#pragma once

#ifdef ENGINE_DLL_
#define DLLExport __declspec( dllexport )
#else
#define DLLExport __declspec( dllimport )
#endif

#pragma once

#ifdef ROADDATA_DLL
#define ROADDATA_API __declspec(dllexport)
#else         
#define ROADDATA_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"RoadDatad.lib")
#pragma message("Automatically linking with RoadDatad.dll")
#else
#pragma comment(lib,"RoadData.lib")
#pragma message("Automatically linking with RoadData.dll")
#endif
#endif

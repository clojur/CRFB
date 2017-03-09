#pragma once

#ifdef YJOSG_DLL
#define YJOSG_API __declspec(dllexport)
#else         
#define YJOSG_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"YjOSGd.lib")
#pragma message("Automatically linking with YjOSGd.dll")
#else
#pragma comment(lib,"YjOSG.lib")
#pragma message("Automatically linking with YjOSG.dll")
#endif
#endif

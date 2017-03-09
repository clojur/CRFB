#pragma once

#ifdef  DEFAULTPARAM_DLL
#define  DEFAULTPARAM_API __declspec(dllexport)
#else         
#define  DEFAULTPARAM_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"DefaultParamd.lib")
#pragma message("Automatically linking with DefaultParamd.dll")
#else
#pragma comment(lib,"DefaultParam.lib")
#pragma message("Automatically linking with DefaultParam.dll")
#endif
#endif

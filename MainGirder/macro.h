#pragma once

#ifdef  MAINGIRDER_DLL
#define  MAINGIRDER_API __declspec(dllexport)
#else         
#define  MAINGIRDER_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"MainGirderd.lib")
#pragma message("Automatically linking with MainGirderd.dll")
#else
#pragma comment(lib,"MainGirder.lib")
#pragma message("Automatically linking with MainGirder.dll")
#endif
#endif
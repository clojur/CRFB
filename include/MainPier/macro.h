#pragma once

#ifdef  MAINPIER_DLL
#define  MAINPIER_API __declspec(dllexport)
#else         
#define  MAINPIER_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"MainPierd.lib")
#pragma message("Automatically linking with MainPierd.dll")
#else
#pragma comment(lib,"MainPier.lib")
#pragma message("Automatically linking with MainPier.dll")
#endif
#endif

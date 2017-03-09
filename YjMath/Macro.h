#pragma once

#ifdef  YJMATH_DLL
#define  YJMATH_API __declspec(dllexport)
#else         
#define  YJMATH_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"YjMathd.lib")
#pragma message("Automatically linking with YjMathd.dll")
#else
#pragma comment(lib,"YjMath.lib")
#pragma message("Automatically linking with YjMath.dll")
#endif
#endif

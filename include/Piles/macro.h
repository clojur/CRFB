#pragma once

#ifdef  PILESBAR_DLL
#define  PILESBAR_API __declspec(dllexport)
#else         
#define  PILESBAR_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"Pilesd.lib")
#pragma message("Automatically linking with Pilesd.dll")
#else
#pragma comment(lib,"Piles.lib")
#pragma message("Automatically linking with Piles.dll")
#endif
#endif
#pragma once

#ifdef STDSECTION_DLL
#define STDSECTION_API __declspec(dllexport)
#else         
#define STDSECTION_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"StdSectiond.lib")
#pragma message("Automatically linking with StdSectiond.dll")
#else
#pragma comment(lib,"StdSection.lib")
#pragma message("Automatically linking with StdSection.dll")
#endif
#endif

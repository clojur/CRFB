#pragma once

#ifdef  RIGIDFRAMEBRIDGE_DLL
#define  RIGIDFRAMEBRIDGE_API __declspec(dllexport)
#else         
#define  RIGIDFRAMEBRIDGE_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"RigidFrameBridged.lib")
#pragma message("Automatically linking with RigidFrameBridged.dll")
#else
#pragma comment(lib,"RigidFrameBridge.lib")
#pragma message("Automatically linking with RigidFrameBridge.dll")
#endif
#endif

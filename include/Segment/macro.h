#pragma once

#ifdef SEGMENT_DLL
#define SEGMENT_API __declspec(dllexport)
#else         
#define SEGMENT_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"Segmentd.lib")
#pragma message("Automatically linking with Segmentd.dll")
#else
#pragma comment(lib,"Segment.lib")
#pragma message("Automatically linking with Segment.dll")
#endif
#endif

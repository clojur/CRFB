#ifdef  CDIP_BAR_DLL
#define  CDIP_BAR_API __declspec(dllexport)
#else         
#define  CDIP_BAR_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"CDIPBard.lib")
#pragma message("Automatically linking with CDIPBard.dll...")
#else
#pragma comment(lib,"CDIPBar.lib")
#pragma message("Automatically linking with CDIPBar.dll...")
#endif
#endif
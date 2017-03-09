#if defined(DXFLIB_DLL)
#   ifdef _WIN32
#       if defined(DXFLIB_LIBRARY)
#           define DXFLIB_EXPORT __declspec(dllexport)
#       else
#           define DXFLIB_EXPORT __declspec(dllimport)
#       endif
#   else
#       define DXFLIB_EXPORT
#   endif
#else
#   define DXFLIB_EXPORT
#   ifdef _DEBUG
#   pragma comment(lib,"DxfMakerd.lib")
#   pragma message("Automatically linking with DxfMakerd.dll")
#   else
#   pragma comment(lib,"DxfMaker.lib")
#   pragma message("Automatically linking with DxfMakerd.dll")
#   endif
#endif

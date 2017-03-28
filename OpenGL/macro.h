#pragma once

#ifdef WinOpenGL_DLL
#define WinOpenGL_API __declspec(dllexport)
#else         
#define WinOpenGL_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"WinOpenGLd.lib")
#pragma message("Automatically linking with WinOpenGLd.dll")
#else
#pragma comment(lib,"RouteCross.lib")
#pragma message("Automatically linking with WinOpenGL.dll")
#endif
#endif
#ifndef _GCAD_PLATFORMWIN32BASE_H_
#define _GCAD_PLATFORMWIN32BASE_H_

#ifdef _MSC_VER
  #pragma warning (disable : 4251)
#endif

#ifdef WIN32
  #ifdef GCAD_DLL_LIBRARY
    #define GCAD_WIN32_EXPORT __declspec(dllexport)
  #elif defined (GCAD_LIB)
    #define GCAD_WIN32_EXPORT
  #else
    #define GCAD_WIN32_EXPORT __declspec(dllimport)
  #endif
#else
  #define GCAD_WIN32_EXPORT
#endif

#endif
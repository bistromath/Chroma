//
// Config.hpp
//
// Common macro definitions for library API export.
//
// Copyright (c) 2013-2014 Josh Blum
// SPDX-License-Identifier: BSL-1.0
//

#pragma once
// http://gcc.gnu.org/wiki/Visibility
// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define CHROMA_HELPER_DLL_IMPORT __declspec(dllimport)
  #define CHROMA_HELPER_DLL_EXPORT __declspec(dllexport)
  #define CHROMA_HELPER_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define CHROMA_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define CHROMA_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define CHROMA_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define CHROMA_HELPER_DLL_IMPORT
    #define CHROMA_HELPER_DLL_EXPORT
    #define CHROMA_HELPER_DLL_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define CHROMA_API and CHROMA_LOCAL.
// CHROMA_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// CHROMA_LOCAL is used for non-api symbols.

#define CHROMA_DLL //always building a DLL

#ifdef CHROMA_DLL // defined if POTHOS is compiled as a DLL
  #ifdef CHROMA_DLL_EXPORTS // defined if we are building the CHROMA DLL (instead of using it)
    #define CHROMA_API CHROMA_HELPER_DLL_EXPORT
    #define CHROMA_EXTERN
  #else
    #define CHROMA_API CHROMA_HELPER_DLL_IMPORT
    #define CHROMA_EXTERN extern
  #endif // CHROMA_DLL_EXPORTS
  #define CHROMA_LOCAL CHROMA_HELPER_DLL_LOCAL
#else // CHROMA_DLL is not defined: this means CHROMA is a static lib.
  #define CHROMA_API
  #define CHROMA_LOCAL
  #define CHROMA_EXTERN
#endif // CHROMA_DLL

#ifdef _MSC_VER
  #ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES //math.h M_* constants
  #endif //_USE_MATH_DEFINES
#endif //_MSC_VER

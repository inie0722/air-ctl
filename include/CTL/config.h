#pragma once

#if defined(CTL_DLL_BUILD)
#if defined(WIN32) || defined(_WIN32)
#define CTL_API __declspec(dllexport)
#else
#define CTL_API
#endif
#else
#if (defined(WIN32) || defined(_WIN32)) && defined(CTL_DLL)
#define CTL_API __declspec(dllimport)
#else
#define CTL_API
#endif
#endif
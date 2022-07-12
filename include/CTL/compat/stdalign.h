#pragma once

#if defined __STDC_VERSION__
#if __STDC_VERSION__ < 201112L
#define alignas(a)
#else
#include <stdalign.h>
#endif
#elif defined __cplusplus
#if __cplusplus < 201103L
#define alignas(a)
#endif
#endif
#ifndef CORE_DEFINES_H
#define CORE_DEFINES_H

#ifndef CORE_PLATFORM_WIN32
    #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
        #define CORE_PLATFORM_WIN32 1
    #else
        #define CORE_PLATFORM_WIN32 0
    #endif
#endif

#ifdef CORE_EXPORT
    #ifdef WIN32
        #define CORE_API __declspec(dllexport)
    #else
        #define CORE_API __attribute__((visibility("default")))
    #endif
#else // CORE_IMPORT
    #ifdef WIN32
        #define CORE_API __declspec(dllimport)
    #else
        #define CORE_API
    #endif
#endif

#endif

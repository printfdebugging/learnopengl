#ifndef LOADER_DEFINES_H
#define LOADER_DEFINES_H

#ifndef LOADER_PLATFORM_WIN32
    #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
        #define LOADER_PLATFORM_WIN32 1
    #else
        #define LOADER_PLATFORM_WIN32 0
    #endif
#endif

#ifdef LOADER_EXPORT
    #ifdef WIN32
        #define LOADER_API __declspec(dllexport)
    #else
        #define LOADER_API __attribute__((visibility("default")))
    #endif
#else  // LOADER_IMPORT
    #ifdef WIN32
        #define LOADER_API __declspec(dllimport)
    #else
        #define LOADER_API
    #endif
#endif

#endif

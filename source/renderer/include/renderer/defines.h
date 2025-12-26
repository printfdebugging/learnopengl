#ifndef RENDERER_DEFINES_H
#define RENDERER_DEFINES_H

#ifndef RENDERER_PLATFORM_WIN32
    #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
        #define RENDERER_PLATFORM_WIN32 1
    #else
        #define RENDERER_PLATFORM_WIN32 0
    #endif
#endif

#ifdef RENDERER_STATIC
    #define RENDERER_API
#else
    #ifdef RENDERER_EXPORT
        #if RENDERER_PLATFORM_WIN32
            #define RENDERER_API __declspec(dllexport)
        #else
            #define RENDERER_API __attribute__((visibility("default")))
        #endif
    #elif defined(RENDERER_IMPORT)
        #if RENDERER_PLATFORM_WIN32
            #define RENDERER_API __declspec(dllimport)
        #else
            #define RENDERER_API
        #endif
    #else
        #define RENDERER_API
    #endif
#endif

#endif

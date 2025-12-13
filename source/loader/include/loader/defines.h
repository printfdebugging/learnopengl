#ifndef LOADER_DEFINES_H
#define LOADER_DEFINES_H

#ifdef LOADER_EXPORT
    #ifdef _MSC_VER
        #define API_CALL __declspec(dllexport)
    #else
        #define API_CALL __attribute__((visibility("default")))
    #endif
#endif

#ifdef LOADER_IMPORT
    #ifdef _MSC_VER
        #define API_CALL __declspec(dllimport)
    #else
        #define API_CALL
    #endif
#endif

#endif

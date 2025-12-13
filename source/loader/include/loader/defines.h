#ifndef LOADER_DEFINES_H
#define LOADER_DEFINES_H

#ifdef LOADER_EXPORT
    #ifdef WIN32
        #define API_CALL __declspec(dllexport)
    #else
        #define API_CALL __attribute__((visibility("default")))
    #endif
#endif

#ifdef LOADER_IMPORT
    #ifdef WIN32
        #define API_CALL __declspec(dllimport)
    #else
        #define API_CALL
    #endif
#endif

#endif

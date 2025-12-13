#ifndef CORE_DEFINES_H
#define CORE_DEFINES_H

#ifdef CORE_EXPORT
    #ifdef _MSC_VER
        #define API_CALL __declspec(dllexport)
    #else
        #define API_CALL __attribute__((visibility("default")))
    #endif
#endif

#ifdef CORE_IMPORT
    #ifdef _MSC_VER
        #define API_CALL __declspec(dllimport)
    #else
        #define API_CALL
    #endif
#endif

#endif

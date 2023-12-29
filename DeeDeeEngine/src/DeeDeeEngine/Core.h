#pragma once

#ifdef DEE_PLATFORM_WINDOWS
    #ifdef DEE_BUILD_DLL
        #define DEE_API __declspec(dllexport)
    #else
        #define DEE_API __declspec(dllimport)
    #endif // 

#else
    #error Dee only support Windows
#endif // 


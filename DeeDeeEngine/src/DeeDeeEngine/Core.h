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

#ifdef DEE_ENABLE_ASSERTS
    #define DEE_ASSERT(x,...) {if(!(x)){DEE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
    #define DEE_CORE_ASSERT(x,...) {if(!(x)){DEE_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
#else
    #define DEE_ASSERT(x,...)
    #define DEE_CORE_ASSERT(x,...)
#endif

#define BIT(X)(1<<X)

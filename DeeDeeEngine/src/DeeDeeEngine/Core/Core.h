#pragma once
#include <memory>
#ifdef DEE_PLATFORM_WINDOWS
#if DEE_DYNAMIC_LINKS
    #ifdef DEE_BUILD_DLL
        #define DEE_API __declspec(dllexport)
    #else
        #define DEE_API __declspec(dllimport)
    #endif // 
#else
    #define DEE_API
#endif
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

#define DEE_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)

namespace DeeDeeEngine {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T>
	using Ref = std::shared_ptr<T>;

	

}

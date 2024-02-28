#pragma once
#include "deepch.h"
#include "DeeDeeEngine/Core.h"
#include "DeeDeeEngine/Events/Event.h"

namespace DeeDeeEngine {
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "DeeDeeEngine",
			unsigned int width = 1280,
			unsigned int height = 720)
			:Title(title), Width(width), Height(height) {}
	};

	class DEE_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());//该函数可以在不创建任何WindowProps对象的情况下调用，
		//因为参数使用了默认参数WindowProps()。如果没有提供props参数，
		//	将使用默认构造函数创建一个WindowProps对象。
	};
}
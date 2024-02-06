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

		static Window* Create(const WindowProps& props = WindowProps());//�ú��������ڲ������κ�WindowProps���������µ��ã�
		//��Ϊ����ʹ����Ĭ�ϲ���WindowProps()�����û���ṩprops������
		//	��ʹ��Ĭ�Ϲ��캯������һ��WindowProps����
	};
}
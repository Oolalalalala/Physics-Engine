#pragma once

#include "Event/Event.h"

struct GLFWwindow;

namespace Olala {

	struct WindowProps {
		std::string title;
		unsigned int width, height;
		bool vSync;
		WindowProps() = default;
		WindowProps(const std::string& title, unsigned int width = 800, unsigned int height = 600, bool vSync = true)
			: title(title), width(width), height(height), vSync(vSync) {}
	};


	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		static Scope<Window> Create(const WindowProps& props);

		Window(const WindowProps& props);

		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }
		bool IsVSync() const { return m_Data.VSync; }
		void SetVSync(bool enabled);

		void OnUpdate();

		void* GetNativeWindow() const { return m_GLFWwindow; }
		void SetEventCallback(EventCallbackFn callback) { m_Data.EventCallback = callback; }

	private:
		void Init(const WindowProps& props);
		void ShutDown();

	private:
		GLFWwindow* m_GLFWwindow;

		struct WindowData 
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;

		static uint32_t s_WindowCount;
	};

}
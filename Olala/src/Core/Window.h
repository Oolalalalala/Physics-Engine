#pragma once

#include "Event/Event.h"

struct GLFWwindow;

namespace Olala {

	struct WindowSpecs {
		std::string Title = "";
		unsigned int Width = 1280, Height = 720;
		std::string IconImagePath = "";
		std::string SmallIconImagePath = "";
		bool VSync = true;
		WindowSpecs() = default;
		WindowSpecs(const std::string& title, unsigned int width = 800, unsigned int height = 600, bool vSync = true)
			: Title(title), Width(width), Height(height), VSync(vSync) {}
	};


	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		static Scope<Window> Create(const WindowSpecs& props);

		Window(const WindowSpecs& props);

		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }
		bool IsVSync() const { return m_Data.VSync; }
		void SetVSync(bool enabled);

		void OnUpdate();

		void* GetNativeWindow() const { return m_GLFWwindow; }
		void SetEventCallback(EventCallbackFn callback) { m_Data.EventCallback = callback; }

	private:
		void Init(const WindowSpecs& props);
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
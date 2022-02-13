#pragma once

#include "Window.h"
#include "Timer.h"
#include "Event/ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

namespace Olala {

	struct ApplicationSpecs {
		std::string Name = "Application";
		uint32_t Width = 1280, Height = 720;
		std::string IconImagePath = "";
		std::string SmallIconImagePath = "";
		bool VSync = true;
	};
	
	class Application
	{
	public:
		Application(const ApplicationSpecs& specs);

		void Run();
		void Close();

		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static Application& Get();
		Window& GetWindow() { return *m_Window; }

	protected:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		void Init(const ApplicationSpecs& specs);

	private:
		bool m_Running;
		std::string m_ApplicationName;

		ImGuiLayer* m_ImGuiLayer;

		Scope<Window> m_Window;
		Scope<Timer> m_Timer;
		Scope<LayerStack> m_LayerStack;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
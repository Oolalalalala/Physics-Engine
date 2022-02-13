#include "pch.h"
#include "Application.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

namespace Olala {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecs& specs)
		: m_ApplicationName(specs.Name)
	{
		s_Instance = this;
		Init(specs);
	}

	void Application::Init(const ApplicationSpecs& specs)
	{
		Log::Init();

		WindowSpecs windowSpecs(specs.Name, specs.Width, specs.Height, specs.VSync);
		windowSpecs.IconImagePath = specs.IconImagePath;
		windowSpecs.SmallIconImagePath = specs.SmallIconImagePath;

		m_Window = Window::Create(windowSpecs);
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_Timer = CreateScope<Timer>();

		RenderCommand::Init();
		Renderer2D::Init();

		m_LayerStack = CreateScope<LayerStack>();
		m_ImGuiLayer = new ImGuiLayer();
		m_LayerStack->PushOverlay(m_ImGuiLayer);

		m_Running = true;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			for (Layer* layer : *m_LayerStack)
			{
				layer->OnUpdate(m_Timer->GetDeltaTime());
			}
			

			m_ImGuiLayer->Begin();
			for (Layer* layer : *m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
		Renderer2D::ShutDown();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		if (dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose))) OLA_CORE_TRACE("Application closed");

		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); it++)
		{
			(*it)->OnEvent(e);
			if (e.Handled()) break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		/// Unneeded because of ImGui, probably should be changed to modify framebuffer and camera aspect ratio
		//RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack->PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack->PushOverlay(overlay);
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

}
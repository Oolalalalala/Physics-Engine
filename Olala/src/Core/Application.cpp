#include "pch.h"
#include "Application.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

namespace Olala {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
		: m_ApplicationName(name)
	{
		s_Instance = this;
		Init();
	}

	void Application::Init()
	{
		Log::Init();

		m_Window = Window::Create(WindowProps(m_ApplicationName));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_Timer = CreateScope<Timer>();

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
		if (dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose))) CORE_LOG_TRACE("Window Close Event");
		if (dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize))) CORE_LOG_TRACE("Window Resize Event");

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
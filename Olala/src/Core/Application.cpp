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

		Renderer2D::Init();

		m_LayerStack = CreateScope<LayerStack>();

		m_Running = true;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float dt = 0.166f;
			for (Layer* layer : *m_LayerStack)
			{
				layer->OnUpdate(dt);
			}

			m_Window->OnUpdate();
		}
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
		m_Running = false;
		Renderer2D::ShutDown();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
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
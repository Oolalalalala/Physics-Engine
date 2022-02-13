#include "pch.h"
#include "Window.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Renderer/Texture2D.h"

namespace Olala {

	uint32_t Window::s_WindowCount = 0;

	Scope<Window> Window::Create(const WindowSpecs& props)
	{
		return CreateScope<Window>(props);
	}

	Window::Window(const WindowSpecs& props)
	{
		Init(props);
		s_WindowCount++;
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_GLFWwindow);
	}

	void Window::Init(const WindowSpecs& specs)
	{
		m_Data.Title = specs.Title;
		m_Data.Width = specs.Width;
		m_Data.Height = specs.Height;
		m_Data.VSync = specs.VSync;

		if (!s_WindowCount)
		{
			if (!glfwInit())
				OLA_CORE_CRITICAL("GLFW initialization failure");
		}

		m_GLFWwindow = glfwCreateWindow(specs.Width, specs.Height, specs.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_GLFWwindow);

		if (!s_WindowCount)
		{
			if (glewInit() != GLEW_OK && s_WindowCount)
				OLA_CORE_CRITICAL("GLEW initialization failure");
		}

		// Icons
		GLFWimage images[2];
		if (specs.IconImagePath.size() && specs.SmallIconImagePath.size())
		{
			Texture2D::LoadToGLFWImage(images[0], specs.IconImagePath);
			Texture2D::LoadToGLFWImage(images[1], specs.SmallIconImagePath);
			glfwSetWindowIcon(m_GLFWwindow, 2, images);
			Texture2D::FreeGLFWImage(images[0]);
			Texture2D::FreeGLFWImage(images[1]);
		}
		else if (specs.IconImagePath.size())
		{
			Texture2D::LoadToGLFWImage(images[0], specs.IconImagePath);
			glfwSetWindowIcon(m_GLFWwindow, 1, images);
			Texture2D::FreeGLFWImage(images[0]);
		}
		else if (specs.SmallIconImagePath.size())
		{
			Texture2D::LoadToGLFWImage(images[0], specs.SmallIconImagePath);
			glfwSetWindowIcon(m_GLFWwindow, 1, images);
			Texture2D::FreeGLFWImage(images[0]);
		}


		glfwSetWindowUserPointer(m_GLFWwindow, &m_Data);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_GLFWwindow, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_GLFWwindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_GLFWwindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false, mods);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true, mods);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_GLFWwindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_GLFWwindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_GLFWwindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_GLFWwindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

		SetVSync(specs.VSync);
	}

	void Window::ShutDown()
	{
		glfwDestroyWindow(m_GLFWwindow);

		if (!--s_WindowCount)
			glfwTerminate();
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

}
#include "pch.h"
#include "Camera.h"

namespace Olala {

	PerspectiveCamera::PerspectiveCamera()
		: Camera(glm::perspective(glm::radians(60.f), 1.776f, 0.01f, 1000.f))
	{
		m_Specs = { glm::radians(60.f), 1.776f, 0.01f, 1000.f };
	}

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
		: Camera(glm::perspective(fov, aspectRatio, nearPlane, farPlane))
	{
		m_Specs = { fov, aspectRatio, nearPlane, farPlane };
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		m_Projection = glm::perspective(m_Specs.Fov, m_Specs.AspectRatio, m_Specs.NearPlane, m_Specs.FarPlane);
	}

	OrthographicCamera::OrthographicCamera()
		: Camera(glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -1.0f, 1.0f))
	{
		m_Specs = { -400.0f, 400.0f, -300.0f, 300.0f };
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom)
		: Camera(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
		m_Specs = { left, right, bottom, top };
	}

	void OrthographicCamera::RecalculateProjectionMatrix()
	{
		m_Projection = glm::ortho(m_Specs.Left, m_Specs.Right, m_Specs.Bottom, m_Specs.Top);
	}
}
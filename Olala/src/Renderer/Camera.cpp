#include "pch.h"
#include "Camera.h"

namespace Olala {

	PerspectiveCamera::PerspectiveCamera()
		: Camera(glm::perspective(glm::radians(60.f), 1.776f, 0.01f, 1000.f))
	{
		m_Specs = { 60.f, 1.776f, 0.01f, 1000.f };
	}

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
		: Camera(glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane))
	{
		m_Specs = { fov, aspectRatio, nearPlane, farPlane };
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		m_Projection = glm::perspective(glm::radians(m_Specs.Fov), m_Specs.AspectRatio, m_Specs.NearPlane, m_Specs.FarPlane);
	}

	OrthographicCamera::OrthographicCamera()
		: Camera(glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -1.0f, 1.0f))
	{
		m_Specs = { 800, 600, -1, 1 };
	}

	OrthographicCamera::OrthographicCamera(float width, float height, float nearPlane, float farPlane)
		: Camera(glm::ortho(-width / 2.f, width / 2.f, -height / 2.f, height / 2.f, nearPlane, farPlane))
	{
		m_Specs = { width, height, nearPlane, farPlane };
	}

	void OrthographicCamera::RecalculateProjectionMatrix()
	{
		m_Projection = glm::ortho(-m_Specs.Width / 2.f, m_Specs.Width / 2.f, -m_Specs.Height / 2.f, m_Specs.Height / 2.f,
			m_Specs.NearPlane, m_Specs.FarPlane);
	}
}
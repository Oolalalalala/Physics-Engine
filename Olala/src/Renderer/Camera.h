#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Olala {

	class Camera
	{
	public:
		Camera() = default;
		Camera(glm::mat4 projection)
			: m_Projection(projection) {}

		virtual ~Camera() {}

		void SetProjectionMatrix(glm::mat4 projection) { m_Projection = projection; }
		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetDirection(glm::vec3 direction) { m_Direction = direction; }
		void SetUpDirection(glm::vec3 upDirection) { m_UpDirection = upDirection; }

		const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
		const glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Direction, m_UpDirection); }
		const glm::mat4 GetViewProjectionMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); }

	private:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	};

	class PerspectiveCamera : public Camera
	{
	public:

	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();
		OrthographicCamera(float left, float right, float top, float bottom);
	};

}
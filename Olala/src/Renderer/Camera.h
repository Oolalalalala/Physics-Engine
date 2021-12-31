#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Olala {

	enum class CameraProjectionType
	{
		Perspective = 0,
		Orthographic = 1
	};

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

		const glm::vec3& GetDirection() const { return m_Direction; }
		const glm::vec3& GetUpDirection() const { return m_UpDirection; }
		const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
		const glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Direction, m_UpDirection); }
		const glm::mat4 GetViewProjectionMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); }

		virtual const CameraProjectionType GetProjectionType() const = 0;
		virtual void RecalculateProjectionMatrix() = 0;

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	};

	class PerspectiveCamera : public Camera
	{
	private:
		struct Specs {
			float Fov, AspectRatio, NearPlane, FarPlane;
			bool operator==(const Specs& other)
			{
				return Fov == other.Fov && AspectRatio == other.AspectRatio &&
					   NearPlane == other.NearPlane && FarPlane == other.FarPlane;
			}
			bool operator!=(const Specs& other) { return !(*this == other); }
		};

	public:
		PerspectiveCamera();
		PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);

		Specs& GetSpecs() { return m_Specs; }
		
		void RecalculateProjectionMatrix() override;
		const CameraProjectionType GetProjectionType() const override { return CameraProjectionType::Perspective; }

	private:
		Specs m_Specs;
	};

	class OrthographicCamera : public Camera
	{
	private:
		struct Specs {
			float Width, Height, NearPlane, FarPlane;
			bool operator==(const Specs& other) 
			{
				return Width == other.Width && Height == other.Height && NearPlane == other.NearPlane && FarPlane == other.FarPlane;
			}
			bool operator!=(const Specs& other) { return !(*this == other); }
		};

	public:
		OrthographicCamera();
		OrthographicCamera(float width, float height, float nearPlane, float farPlane);

		Specs& GetSpecs() { return m_Specs; }

		void RecalculateProjectionMatrix() override;
		const CameraProjectionType GetProjectionType() const override { return CameraProjectionType::Orthographic; }

	private:
		Specs m_Specs;
	};

}
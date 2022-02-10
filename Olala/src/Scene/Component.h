#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "Core/Base.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Framebuffer.h"
#include "Physics/Collider.h"

#include <string>


namespace Olala {

	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
			: Position(position), Rotation(rotation), Scale(scale)
		{
		}

		glm::mat4 GetTransform()
		{
			return glm::translate(glm::mat4(1.0f), Position)
				* glm::toMat4(glm::quat(Rotation))
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct CameraComponent
	{
		Ref<Camera> Camera = nullptr;
		bool IsMain = false, IsOn = true;
		Ref<Framebuffer> RenderTarget = nullptr;

		CameraComponent() = default;
		CameraComponent(Ref<::Olala::Camera> camera, Ref<Framebuffer> renderTarget = nullptr, bool isMain = true, bool isOn = true)
			: Camera(camera), RenderTarget(renderTarget), IsMain(isMain), IsOn(isOn) {}
	};

	struct EditorCameraControllerComponent
	{
		GLenum ForwardKey   = GLFW_KEY_W;
		GLenum BackKey      = GLFW_KEY_S;
		GLenum LeftKey      = GLFW_KEY_A;
		GLenum RightKey     = GLFW_KEY_D;
		float MovementSpeed = 8.f;
		bool IsOn = false;
		
		EditorCameraControllerComponent() = default;
		EditorCameraControllerComponent(GLenum forwardKey, GLenum backKey, GLenum leftKey, GLenum rightKey, float movementSpeed)
			: ForwardKey(forwardKey), BackKey(backKey), LeftKey(leftKey), RightKey(rightKey), MovementSpeed(movementSpeed) {}
	};

	struct SpriteRendererComponent
	{
		glm::vec2 Size = glm::vec2(10, 10);
		glm::vec4 Color = glm::vec4(1.0f);
		Ref<Texture2D> Texture = nullptr;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(glm::vec2 size, glm::vec4 color, Ref<Texture2D> texture)
			: Size(size), Color(color), Texture(texture) {}
	};

	struct Rigidbody2DComponent
	{
		glm::vec2 Velocity = glm::vec2(0.f);
		glm::vec2 CenterOfMass = glm::vec2(0.f);
		float AngularVelocity = 0.f;

		float Mass = 1.f;
		bool IsStatic = false;
		bool ApplyGravity = true;

#ifdef USE_BOX2D
		void* RuntimeBody;
#else
		PhysicsID PhysicsHandle = 0;
#endif

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(float mass, bool applyGravity = true, bool isStatic = false)
			: Mass(mass), ApplyGravity(applyGravity), IsStatic(isStatic) {}
	};

	struct Collider2DComponent
	{
		glm::vec2 Center = glm::vec2(0.f, 0.f);

		Collider2DComponent() = default;
		Collider2DComponent(glm::vec2 center = glm::vec2(0.f, 0.f))
			: Center(center) {}
	};

	struct BoxCollider2DComponent : public Collider2DComponent
	{
		glm::vec2 Size = glm::vec2(10.f, 10.f);
		float AngularOffset = 0.f;

		BoxCollider2DComponent() : Collider2DComponent(glm::vec2(0.f, 0.f)) {}
		BoxCollider2DComponent(glm::vec2 size, glm::vec2 center = glm::vec2(0.f, 0.f))
			: Size(size), Collider2DComponent(center) {}
	};

	struct CircleCollider2DComponent : public Collider2DComponent
	{
		float Radius = 1.f;

		CircleCollider2DComponent() : Collider2DComponent(glm::vec2(0.f, 0.f)) {}
		CircleCollider2DComponent(float radius, glm::vec2 center = glm::vec2(0.f, 0.f))
			: Radius(radius), Collider2DComponent(center) {}
	};

}
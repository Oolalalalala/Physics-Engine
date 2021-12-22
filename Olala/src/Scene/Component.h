#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "Core/Base.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Framebuffer.h"

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
		CameraComponent(Ref<::Olala::Camera> camera, bool isMain = true, bool isOn = true)
			: Camera(camera), IsMain(isMain), IsOn(isOn) {}
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

}
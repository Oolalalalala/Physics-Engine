#include "pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

namespace Olala {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate()
	{
		std::vector<Entity> cameras;

		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				camera.Camera->SetPosition(transform.Position);

				if (camera.IsOn)
					cameras.emplace_back(entity, this);
			}
		}

		// Render the scene to all camera
		{
			auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
			

			for (Entity& camera : cameras)
			{
				auto& cameraComponent = camera.GetComponent<CameraComponent>();

				RenderCommand::SetRenderTarget(cameraComponent.RenderTarget);
				RenderCommand::SetClearColor({ 0.0f, 0.0f, 1.0f, 1.0f });
				RenderCommand::Clear();

				Renderer2D::BeginScene(*cameraComponent.Camera);

				for (auto entity : group)
				{
					auto& transform = group.get<TransformComponent>(entity);
					auto& sprite = group.get<SpriteRendererComponent>(entity);

					Renderer2D::DrawQuad(glm::vec2{ transform.Position.x, transform.Position.y }, sprite.Size * glm::vec2{ transform.Scale.x, transform.Scale.y }, sprite.Texture, sprite.Color);
					//CORE_LOG_INFO("{} drawn, position = {}, {}", m_Registry.get<TagComponent>(entity).Tag, transform.Position.x, transform.Position.y);
				}

				Renderer2D::EndScene();
			}
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TagComponent>(name.empty() ? "Unnamed" : name);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

}

#include "pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "Core/IO.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

namespace Olala {

	Scene::Scene()
	{
		m_PhysicsWorld = CreateRef<PhysicsWorld>();
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(float dt)
	{
		// Editor Camera Controller
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent, EditorCameraControllerComponent>();
			for (auto entity : group)
			{
				auto [transform, camera, cameraController] = group.get<TransformComponent, CameraComponent, EditorCameraControllerComponent>(entity);
				if (cameraController.IsOn)
				{
					if (IO::IsKeyPressed(cameraController.ForwardKey))
						transform.Position += camera.Camera->GetDirection() * cameraController.MovementSpeed * dt;
					if (IO::IsKeyPressed(cameraController.BackKey))
						transform.Position -= camera.Camera->GetDirection() * cameraController.MovementSpeed * dt;
					if (IO::IsKeyPressed(cameraController.LeftKey))
						transform.Position -= glm::normalize(glm::cross(camera.Camera->GetDirection(), camera.Camera->GetUpDirection())) * cameraController.MovementSpeed * dt;
					if (IO::IsKeyPressed(cameraController.RightKey))
						transform.Position += glm::normalize(glm::cross(camera.Camera->GetDirection(), camera.Camera->GetUpDirection())) * cameraController.MovementSpeed * dt;
				}
			}
		}

		// Physics
		m_PhysicsWorld->OnUpdate(dt);

		// Retrieve and Set Data From and To Physics
		auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
		for (auto e : view)
		{
			auto [transform, rigidbody2d] = view.get<TransformComponent, Rigidbody2DComponent>(e);
			auto& physicsBody = m_PhysicsWorld->GetPhysicsBody(rigidbody2d.PhysicsHandle);
			transform.Position.x = physicsBody.Position.x;// TODO : set the position to center of mass instead of the raw mass
			transform.Position.y = physicsBody.Position.y;
			transform.Rotation.y = physicsBody.Rotation;
			rigidbody2d.Velocity = physicsBody.Velocity;
		}

		// Get All Cameras
		std::vector<Entity> cameras;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				camera.Camera->SetPosition(transform.Position);
				camera.Camera->SetDirection(glm::quat(glm::radians(transform.Rotation)) * glm::vec3(0.0f, 0.0f, -1.0f)); // Not sure if correct

				if (camera.IsOn && camera.RenderTarget != nullptr)
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
				RenderCommand::SetClearColor({ 0.125f, 0.125f, 0.125f, 1.0f });
				RenderCommand::Clear();

				Renderer2D::BeginScene(*cameraComponent.Camera);

				for (auto entity : group)
				{
					auto& transform = group.get<TransformComponent>(entity);
					auto& sprite = group.get<SpriteRendererComponent>(entity);

					Renderer2D::DrawQuad((glm::vec2)transform.Position, sprite.Size * (glm::vec2)transform.Scale, transform.Rotation.z, sprite.Texture, sprite.Color);
				}

				Renderer2D::EndScene();
			}

			RenderCommand::SetRenderTarget(nullptr);
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

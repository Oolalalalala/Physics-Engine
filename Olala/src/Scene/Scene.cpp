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
		m_AssetManager = CreateRef<AssetManager>();
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

	void Scene::OnRuntimeUpdate(float dt)
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
			transform.Rotation.z = physicsBody.Rotation;
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

	void Scene::DestroyEntity(Entity entity)
	{
		if (entity.HasComponent<Rigidbody2DComponent>())
			m_PhysicsWorld->RemovePhysicsBody(entity.GetComponent<Rigidbody2DComponent>().PhysicsHandle);

		m_Registry.destroy(entity.m_EntityID);
	}

	void Scene::InitializePhysics()
	{
		m_PhysicsWorld = CreateRef<PhysicsWorld>();

		auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
		for (auto e : view)
		{
			auto [transform, rb2d] = view.get<TransformComponent, Rigidbody2DComponent>(e);
			Entity entity(e, this);

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				rb2d.PhysicsHandle = m_PhysicsWorld->CreatePhysicsBody(ColliderType::BoundingCircle);
				auto& circleCollider = entity.GetComponent<CircleCollider2DComponent>();
				auto boundingCircle = std::static_pointer_cast<BoundingCircle>(entity.GetPhysicsBody().Collider);
				boundingCircle->Radius = circleCollider.Radius;
				boundingCircle->Offset = circleCollider.Center;
			}
			else if (entity.HasComponent<BoxCollider2DComponent>())
			{
				rb2d.PhysicsHandle = m_PhysicsWorld->CreatePhysicsBody(ColliderType::BoundingBox);
				auto& boxCollider = entity.GetComponent<BoxCollider2DComponent>();
				auto boundingBox = std::static_pointer_cast<BoundingBox>(entity.GetPhysicsBody().Collider);
				boundingBox->SetSize(boxCollider.Size);
				boundingBox->Offset = boxCollider.Center;
			}
			else
				rb2d.PhysicsHandle = m_PhysicsWorld->CreatePhysicsBody();
			
			auto& physicsBody = entity.GetPhysicsBody();
			physicsBody.Position = transform.Position;
			physicsBody.Rotation = transform.Rotation.z;
			physicsBody.Velocity = rb2d.IsStatic ? glm::vec2(0.f) : rb2d.Velocity;
			physicsBody.AngularVelocity = rb2d.IsStatic ? 0.f : rb2d.AngularVelocity;
			physicsBody.InvMass = rb2d.IsStatic ?  0.f : 1.f / rb2d.Mass;
			physicsBody.ApplyGravity = rb2d.ApplyGravity;
			physicsBody.Restitution = 1.f;
		}
	}

	void Scene::Reset()
	{
		m_Name = "Untitled";
		m_Registry.clear();
		m_PhysicsWorld = nullptr;
		m_AssetManager = CreateRef<AssetManager>();
	}

	Ref<Scene> Scene::Copy(Ref<Scene> source)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_Name = source->m_Name;

		auto tagView = source->m_Registry.view<TagComponent>();
		for (auto e : tagView)
		{
			Entity newEntity = newScene->CreateEntity(tagView.get<TagComponent>(e).Tag);
			Entity oldEntity(e, source.get());

			if (oldEntity.HasComponent<TransformComponent>())        newEntity.GetComponent<TransformComponent>()        = oldEntity.GetComponent<TransformComponent>();
			if (oldEntity.HasComponent<SpriteRendererComponent>())   newEntity.AddComponent<SpriteRendererComponent>()   = oldEntity.GetComponent<SpriteRendererComponent>();
			if (oldEntity.HasComponent<CircleCollider2DComponent>()) newEntity.AddComponent<CircleCollider2DComponent>() = oldEntity.GetComponent<CircleCollider2DComponent>();
			if (oldEntity.HasComponent<BoxCollider2DComponent>())    newEntity.AddComponent<BoxCollider2DComponent>()    = oldEntity.GetComponent<BoxCollider2DComponent>();
			if (oldEntity.HasComponent<EditorCameraControllerComponent>())
				newEntity.AddComponent<EditorCameraControllerComponent>() = oldEntity.GetComponent<EditorCameraControllerComponent>();

			if (oldEntity.HasComponent<CameraComponent>())
			{
				auto& camera = newEntity.AddComponent<CameraComponent>() = oldEntity.GetComponent<CameraComponent>();
				camera.RenderTarget = Framebuffer::Create(camera.RenderTarget->GetSpecs());
			}

			if (oldEntity.HasComponent<Rigidbody2DComponent>())
			{
				auto& rb2d = newEntity.AddComponent<Rigidbody2DComponent>() = oldEntity.GetComponent<Rigidbody2DComponent>();
				rb2d.PhysicsHandle = 0;
			}
		}

		return newScene;
	}

}

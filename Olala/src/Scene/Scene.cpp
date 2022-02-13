#include "pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "Core/IO.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"

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
				camera.Camera->SetDirection(glm::inverse(glm::toMat4(glm::quat(glm::radians(transform.Rotation)))) * glm::vec4(0.f, 0.f, 1.f, 0.0f));

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

	void Scene::OnUpdateRuntime(float dt)
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
		{
#ifdef USE_BOX2D
			m_PhysicsWorld->Step(dt, 6, 2);

			auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
			for (auto e : view)
			{
				auto [transform, rb2d] = view.get<TransformComponent, Rigidbody2DComponent>(e);
				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				
				transform.Position.x = body->GetPosition().x;
				transform.Position.y = body->GetPosition().y;
				transform.Rotation.z = glm::degrees(body->GetAngle());
				rb2d.Velocity.x = body->GetLinearVelocity().x;
				rb2d.Velocity.y = body->GetLinearVelocity().y;
				rb2d.AngularVelocity = glm::degrees(body->GetAngularVelocity());
			}

#else
			m_PhysicsWorld->OnUpdate(dt);

			// Retrieve and Set Data From and To Physics
			auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
			for (auto e : view)
			{
				auto [transform, rb2d] = view.get<TransformComponent, Rigidbody2DComponent>(e);
				auto& physicsBody = m_PhysicsWorld->GetPhysicsBody(rb2d.PhysicsHandle);
				glm::vec2 newPos = physicsBody.Position - glm::rotate(rb2d.CenterOfMass, glm::radians(physicsBody.Rotation));
				transform.Position.x = newPos.x;
				transform.Position.y = newPos.y;
				transform.Rotation.z = physicsBody.Rotation;
				rb2d.Velocity = physicsBody.Velocity;
			}
#endif
		}

		// Get All Cameras
		std::vector<Entity> cameras;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				camera.Camera->SetPosition(transform.Position);
				camera.Camera->SetDirection(glm::inverse(glm::toMat4(glm::quat(glm::radians(transform.Rotation)))) * glm::vec4(0.f, 0.f, 1.f, 0.0f));

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
#ifdef USE_BOX2D
			if (m_PhysicsWorld && entity.HasComponent<Rigidbody2DComponent>())
				m_PhysicsWorld->DestroyBody((b2Body*)entity.GetComponent<Rigidbody2DComponent>().RuntimeBody);
#else
			if (m_PhysicsWorld && entity.HasComponent<Rigidbody2DComponent>())
				m_PhysicsWorld->RemovePhysicsBody(entity.GetComponent<Rigidbody2DComponent>().PhysicsHandle);
#endif

		m_Registry.destroy(entity.m_EntityID);
	}

	void Scene::InitializePhysics()
	{
#ifdef USE_BOX2D

		m_PhysicsWorld = CreateRef<b2World>(b2Vec2(0.f, -9.81f));

		auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
		for (auto e : view)
		{
			auto [transform, rb2d] = view.get<TransformComponent, Rigidbody2DComponent>(e);
			Entity entity(e, this);

			b2BodyDef def;
			def.position.Set(transform.Position.x, transform.Position.y);
			def.angle = glm::radians(transform.Rotation.z);
			def.linearVelocity.Set(rb2d.Velocity.x, rb2d.Velocity.y);
			def.angularVelocity = glm::radians(rb2d.AngularVelocity);
			def.gravityScale = rb2d.ApplyGravity ? 1.f : 0.f;
			def.type = rb2d.IsStatic ? b2BodyType::b2_staticBody : b2BodyType::b2_dynamicBody;

			b2MassData massData;
			massData.mass = rb2d.Mass;
			massData.center.Set(rb2d.CenterOfMass.x, rb2d.CenterOfMass.y);
			massData.I = 1.f; // temp

			// TODO : add properties to rigidbodycomponent
			b2FixtureDef fixtureDef;
			fixtureDef.density;
			fixtureDef.friction;
			fixtureDef.restitution = 0.6f;
			fixtureDef.restitutionThreshold = 10.f;

			b2Body* body = m_PhysicsWorld->CreateBody(&def);
			body->SetMassData(&massData);

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
				b2CircleShape circle;
				circle.m_p.Set(cc2d.Center.x, cc2d.Center.y);
				circle.m_radius = cc2d.Radius;
				

				fixtureDef.shape = &circle;
				body->CreateFixture(&fixtureDef);
			}
			else if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				b2PolygonShape box;
				box.SetAsBox(bc2d.Size.x * transform.Scale.x * 0.5f, bc2d.Size.y * transform.Scale.y * 0.5f, b2Vec2(bc2d.Center.x, bc2d.Center.y), glm::radians(bc2d.AngularOffset));

				fixtureDef.shape = &box;
				body->CreateFixture(&fixtureDef);
			}


			rb2d.RuntimeBody = body;
		}

#else
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
				boundingCircle->Offset = -rb2d.CenterOfMass + circleCollider.Center;
			}
			else if (entity.HasComponent<BoxCollider2DComponent>())
			{
				rb2d.PhysicsHandle = m_PhysicsWorld->CreatePhysicsBody(ColliderType::BoundingBox);
				auto& boxCollider = entity.GetComponent<BoxCollider2DComponent>();
				auto boundingBox = std::static_pointer_cast<BoundingBox>(entity.GetPhysicsBody().Collider);
				boundingBox->SetSize(boxCollider.Size);
				boundingBox->Offset = -rb2d.CenterOfMass + boxCollider.Center;
			}
			else
				rb2d.PhysicsHandle = m_PhysicsWorld->CreatePhysicsBody();
			
			auto& physicsBody = entity.GetPhysicsBody();
			physicsBody.Position = (glm::vec2)transform.Position + glm::rotate(rb2d.CenterOfMass, transform.Rotation.z);
			physicsBody.Rotation = transform.Rotation.z;
			physicsBody.Velocity = rb2d.IsStatic ? glm::vec2(0.f) : rb2d.Velocity;
			physicsBody.AngularVelocity = rb2d.IsStatic ? 0.f : rb2d.AngularVelocity;
			physicsBody.InvMass = rb2d.IsStatic ?  0.f : 1.f / rb2d.Mass;
			physicsBody.ApplyGravity = rb2d.ApplyGravity;
			physicsBody.Restitution = 1.f;
		}
#endif

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
				//rb2d.PhysicsHandle = 0;
			}
		}

		return newScene;
	}

}

#include "PropertyPanel.h"

PropertyPanel::PropertyPanel()
	: Panel("Property")
{
}

PropertyPanel::~PropertyPanel()
{
}

void PropertyPanel::OnUpdate(float dt)
{
}

void PropertyPanel::OnImGuiRender()
{
	if (m_IsOpen)
	{
		ImGui::Begin(m_Name.c_str(), &m_IsOpen);
		if (m_DisplayedEntity)
		{
			if (m_IsRuntime)
				DrawContextRuntime();
			else
				DrawContext();
		}

		m_IsFocused = ImGui::IsWindowFocused();
		ImGui::End();
	}
}

template<typename T>
static void DrawComponent(const std::string& name, Olala::Entity& entity, std::function<void(T&)> uiFunction)
{
	constexpr ImGuiTreeNodeFlags componentFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;

	if (entity.HasComponent<T>())
	{
		bool opened = ImGui::CollapsingHeader(name.c_str(), componentFlags);
		bool shouldRemove = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::Selectable("Remove")) shouldRemove = true;
			ImGui::EndPopup();
		}

		if (opened) uiFunction(entity.GetComponent<T>());
		if (shouldRemove) entity.RemoveComponent<T>();
	}
}

template<typename T>
static void AddComponent(Olala::Entity& entity)
{
	static_assert(false);
}
template<>
static void AddComponent<Olala::TransformComponent>(Olala::Entity& entity)
{
	entity.AddComponent<Olala::TransformComponent>();
}
template<>
static void AddComponent<Olala::SpriteRendererComponent>(Olala::Entity& entity)
{
	entity.AddComponent<Olala::SpriteRendererComponent>();
}
template<>
static void AddComponent<Olala::CameraComponent>(Olala::Entity& entity)
{
	auto& camera = entity.AddComponent<Olala::CameraComponent>();
	camera.Camera = Olala::CreateRef<Olala::PerspectiveCamera>();
}
template<>
static void AddComponent<Olala::Rigidbody2DComponent>(Olala::Entity& entity)
{
	auto& rigidbody2d = entity.AddComponent<Olala::Rigidbody2DComponent>();
	auto physicsWorld = entity.GetPhysicsWorld();
	rigidbody2d.PhysicsHandle = physicsWorld->CreatePhysicsBody();

	auto& physicsBody = physicsWorld->GetPhysicsBody(rigidbody2d.PhysicsHandle);
	auto& transform = entity.GetComponent<Olala::TransformComponent>();
	physicsBody.Position = (glm::vec2)transform.Position;
	physicsBody.Rotation = transform.Rotation.z;
}
template<>
static void AddComponent<Olala::BoxCollider2DComponent>(Olala::Entity& entity)
{
	entity.AddComponent<Olala::BoxCollider2DComponent>();
	entity.GetPhysicsBody().SetColliderType(Olala::ColliderType::BoundingBox);
}
template<>
static void AddComponent<Olala::CircleCollider2DComponent>(Olala::Entity& entity)
{
	entity.AddComponent<Olala::CircleCollider2DComponent>();
	entity.GetPhysicsBody().SetColliderType(Olala::ColliderType::BoundingCircle);
}

void PropertyPanel::DrawContext()
{
	auto& entity = m_DisplayedEntity;

	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	constexpr float addButtonWidth = 50.f;

	// Entity Name
	if (entity.HasComponent<Olala::TagComponent>())
	{
		auto& tag = entity.GetComponent<Olala::TagComponent>();
		constexpr size_t maxLength = 30;
		char name[maxLength];
		strcpy_s(name, tag.Tag.c_str());
		ImGui::PushItemWidth(contentRegionAvailable.x - addButtonWidth - 10.f);
		if (ImGui::InputText("###EntityName", name, maxLength))
		{
			if (strlen(name))
				tag.Tag = name;
		}
		ImGui::PopItemWidth();
	}
	
	// Add component
	{
		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("Add", { addButtonWidth, 20.f }))
		{
			ImGui::OpenPopup("Add component");
		}

		constexpr char* componentList[] = { "Transform ", "Camera", "Sprite Renderer 2D", "Rigidbody 2D", "Circle Collider 2D", "Box Collider 2D"};
		if (ImGui::BeginPopup("Add component"))
		{
			for (int i = 0; i < 6; i++)
			{
				if (ImGui::Selectable(componentList[i]))
				{
					switch (i)
					{
						case 0:
						{
							if (!entity.HasComponent<Olala::TransformComponent>())
								entity.AddComponent<Olala::TransformComponent>();
							break;
						}
						case 1:
						{
							if (!entity.HasComponent<Olala::CameraComponent>())
								entity.AddComponent<Olala::CameraComponent>();
							break;
						}
						case 2:
						{
							if (!entity.HasComponent<Olala::SpriteRendererComponent>())
								entity.AddComponent<Olala::SpriteRendererComponent>();
							break;
						}
						case 3:
						{
							if (!entity.HasComponent<Olala::Rigidbody2DComponent>())
								entity.AddComponent<Olala::Rigidbody2DComponent>();
							break;
						}
						case 4:
						{
							if (!entity.HasAnyComponent<Olala::BoxCollider2DComponent, Olala::CircleCollider2DComponent>())
							{
								if (!entity.HasComponent<Olala::Rigidbody2DComponent>())
									entity.AddComponent<Olala::Rigidbody2DComponent>();
								entity.AddComponent<Olala::CircleCollider2DComponent>();
							}
							break;
						}
						case 5:
						{
							if (!entity.HasAnyComponent<Olala::BoxCollider2DComponent, Olala::CircleCollider2DComponent>())
							{
								if (!entity.HasComponent<Olala::Rigidbody2DComponent>())
									entity.AddComponent<Olala::Rigidbody2DComponent>();
								entity.AddComponent<Olala::BoxCollider2DComponent>();
							}
							break;
						}
					}
				}
			}
			ImGui::EndPopup();
		}
	}

	ImGui::Separator();

	// Transform Component // TODO : transform should be undestroyable
	DrawComponent<Olala::TransformComponent>("Transform", entity, [](auto& transform)
	{
		ImGui::DragFloat3("Position", (float*)&transform.Position, 2);
		ImGui::DragFloat3("Rotation", (float*)&transform.Rotation, 2);
		ImGui::DragFloat3("Scale", (float*)&transform.Scale, 2);
	});

	// Camera Component
	DrawComponent<Olala::CameraComponent>("Camera", entity, [](auto& camera)
	{
		constexpr const char* projectionTypes[] = { "Perspective", "Orthographic" };
		int selectedProjectionType = (int)camera.Camera->GetProjectionType();

		ImGui::Checkbox("On", &camera.IsOn);
		ImGui::Checkbox("Main", &camera.IsMain);
		if (ImGui::BeginCombo("Projection type", projectionTypes[selectedProjectionType]))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = selectedProjectionType == i;
				if (ImGui::Selectable(projectionTypes[i], &isSelected))
				{
					// Change the camera type
					if (i == (int)Olala::CameraProjectionType::Perspective)
						camera.Camera = Olala::CreateRef<Olala::PerspectiveCamera>();
					else
						camera.Camera = Olala::CreateRef<Olala::OrthographicCamera>();
				}
			}
			ImGui::EndCombo();
		}

		if (camera.Camera->GetProjectionType() == Olala::CameraProjectionType::Perspective)
		{
			auto& specs = std::static_pointer_cast<Olala::PerspectiveCamera>(camera.Camera)->GetSpecs();
			bool modified = false;
			modified |= ImGui::DragFloat("FOV", &specs.Fov, 1.f, 10.f, 180.f);
			modified |= ImGui::DragFloat("Aspect Ratio", &specs.AspectRatio, 1.f, 0.1f, 10.f);
			modified |= ImGui::DragFloat("Near Plane", &specs.NearPlane);
			modified |= ImGui::DragFloat("Far Plane", &specs.FarPlane);

			if (modified)
				camera.Camera->RecalculateProjectionMatrix();
		}
		else
		{
			auto& specs = std::static_pointer_cast<Olala::OrthographicCamera>(camera.Camera)->GetSpecs();
			bool modified = false;
			modified |= ImGui::InputFloat("Width", &specs.Width);
			modified |= ImGui::InputFloat("Height", &specs.Height);
			modified |= ImGui::InputFloat("Near Plane", &specs.NearPlane);
			modified |= ImGui::InputFloat("Far Plane", &specs.FarPlane);

			if (modified)
				camera.Camera->RecalculateProjectionMatrix();
		}
	});
	
	// Sprite Renderer Component
	DrawComponent<Olala::SpriteRendererComponent>("Sprite Renderer", entity, [](auto& sprite)
	{
		ImGui::DragFloat2("Size##Sprite", (float*)&sprite.Size, 1.f, 0.001f, 1000.f);
		ImGui::ColorEdit4("Color", (float*)&sprite.Color);
	});

	// Rigidbody 2D
	DrawComponent<Olala::Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& rigidbody2d)
	{
		ImGui::DragFloat("Mass", &rigidbody2d.Mass, 1.0f, 0.001f, 1000.f);
		ImGui::DragFloat2("Velocity", (float*)&rigidbody2d.Velocity);
		ImGui::Checkbox("Static", &rigidbody2d.IsStatic);
		ImGui::Checkbox("Gravity", &rigidbody2d.ApplyGravity);
	});

	// Box Collider 2D
	DrawComponent<Olala::BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& boxCollider2d)
	{
		ImGui::DragFloat2("Center", (float*)&boxCollider2d.Center);
		ImGui::DragFloat2("Size##bc2d", (float*)&boxCollider2d.Size, 1.f, 0.001f, 1000.f);
	});

	// Circle Collider 2D
	DrawComponent<Olala::CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& circleCollider2d)
	{
		ImGui::DragFloat2("Center", (float*)&circleCollider2d.Center);
		ImGui::DragFloat("Radius", &circleCollider2d.Radius, 1.f, 0.001f, 1000.f);
	});
}

void PropertyPanel::DrawContextRuntime()
{
	auto& entity = m_DisplayedEntity;

	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	constexpr float addButtonWidth = 50.f;

	// Entity Name
	if (entity.HasComponent<Olala::TagComponent>())
	{
		auto& tag = entity.GetComponent<Olala::TagComponent>();
		constexpr size_t maxLength = 30;
		char name[maxLength];
		strcpy_s(name, tag.Tag.c_str());
		ImGui::PushItemWidth(contentRegionAvailable.x - addButtonWidth - 10.f);
		if (ImGui::InputText("###EntityName", name, maxLength))
		{
			if (strlen(name))
				tag.Tag = name;
		}
		ImGui::PopItemWidth();
	}

	// Add component
	{
		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("Add", { addButtonWidth, 20.f }))
		{
			ImGui::OpenPopup("Add component");
		}

		constexpr char* componentList[] = { "Transform ", "Camera", "Sprite Renderer 2D", "Rigidbody 2D", "Box Collider 2D", "Circle Collider 2D" };
		if (ImGui::BeginPopup("Add component"))
		{
			for (int i = 0; i < 6; i++)
			{
				if (ImGui::Selectable(componentList[i]))
				{
					switch (i)
					{
					case 0:
					{
						if (!entity.HasComponent<Olala::TransformComponent>())
							AddComponent<Olala::TransformComponent>(entity);
						break;
					}
					case 1:
					{
						if (!entity.HasComponent<Olala::CameraComponent>())
							AddComponent<Olala::CameraComponent>(entity);
						break;
					}
					case 2:
					{
						if (!entity.HasComponent<Olala::SpriteRendererComponent>())
							AddComponent<Olala::SpriteRendererComponent>(entity);
						break;
					}
					case 3:
					{
						if (!entity.HasComponent<Olala::Rigidbody2DComponent>())
							AddComponent<Olala::Rigidbody2DComponent>(entity);
						break;
					}
					case 4:
					{
						if (!entity.HasAnyComponent<Olala::BoxCollider2DComponent, Olala::CircleCollider2DComponent>())
						{
							if (!entity.HasComponent<Olala::Rigidbody2DComponent>())
								AddComponent<Olala::Rigidbody2DComponent>(entity);
							AddComponent<Olala::BoxCollider2DComponent>(entity);
						}
						break;
					}
					case 5:
					{
						if (!entity.HasAnyComponent<Olala::BoxCollider2DComponent, Olala::CircleCollider2DComponent>())
						{
							if (!entity.HasComponent<Olala::Rigidbody2DComponent>())
								AddComponent<Olala::Rigidbody2DComponent>(entity);
							AddComponent<Olala::CircleCollider2DComponent>(entity);
						}
						break;
					}
					}
				}
			}
			ImGui::EndPopup();
		}
	}

	ImGui::Separator();

	// Transform Component // TODO : transform should be undestroyable
	DrawComponent<Olala::TransformComponent>("Transform", entity, [&](auto& transform)
		{
			if (ImGui::DragFloat3("Position", (float*)&transform.Position, 2))
			{
				if (entity.HasComponent<Olala::Rigidbody2DComponent>())
				{
					auto& physicsBody = entity.GetPhysicsBody();
					physicsBody.Position = transform.Position;
					physicsBody.Velocity = glm::vec2(0.f);
				}
			}
			if (ImGui::DragFloat3("Rotation", (float*)&transform.Rotation, 2))
			{
				if (entity.HasComponent<Olala::Rigidbody2DComponent>())
					entity.GetPhysicsBody().Rotation = transform.Rotation.z;
			}
			ImGui::DragFloat3("Scale", (float*)&transform.Scale, 2);
		});

	// Camera Component
	DrawComponent<Olala::CameraComponent>("Camera", entity, [](auto& camera)
		{
			constexpr const char* projectionTypes[] = { "Perspective", "Orthographic" };
			int selectedProjectionType = (int)camera.Camera->GetProjectionType();

			ImGui::Checkbox("On", &camera.IsOn);
			ImGui::Checkbox("Main", &camera.IsMain);
			if (ImGui::BeginCombo("Projection type", projectionTypes[selectedProjectionType]))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = selectedProjectionType == i;
					if (ImGui::Selectable(projectionTypes[i], &isSelected))
					{
						// Change the camera type
						if (i == (int)Olala::CameraProjectionType::Perspective)
							camera.Camera = Olala::CreateRef<Olala::PerspectiveCamera>();
						else
							camera.Camera = Olala::CreateRef<Olala::OrthographicCamera>();
					}
				}
				ImGui::EndCombo();
			}

			if (camera.Camera->GetProjectionType() == Olala::CameraProjectionType::Perspective)
			{
				auto& specs = std::static_pointer_cast<Olala::PerspectiveCamera>(camera.Camera)->GetSpecs();
				bool modified = false;
				modified |= ImGui::DragFloat("FOV", &specs.Fov, 1.f, 10.f, 180.f);
				modified |= ImGui::DragFloat("Aspect Ratio", &specs.AspectRatio, 1.f, 0.1f, 10.f);
				modified |= ImGui::DragFloat("Near Plane", &specs.NearPlane);
				modified |= ImGui::DragFloat("Far Plane", &specs.FarPlane);

				if (modified)
					camera.Camera->RecalculateProjectionMatrix();
			}
			else
			{
				auto& specs = std::static_pointer_cast<Olala::OrthographicCamera>(camera.Camera)->GetSpecs();
				bool modified = false;
				modified |= ImGui::InputFloat("Width", &specs.Width);
				modified |= ImGui::InputFloat("Height", &specs.Height);
				modified |= ImGui::InputFloat("Near Plane", &specs.NearPlane);
				modified |= ImGui::InputFloat("Far Plane", &specs.FarPlane);

				if (modified)
					camera.Camera->RecalculateProjectionMatrix();
			}
		});

	// Sprite Renderer Component
	DrawComponent<Olala::SpriteRendererComponent>("Sprite Renderer", entity, [](auto& sprite)
		{
			ImGui::DragFloat2("Size##Sprite", (float*)&sprite.Size, 1.f, 0.001f, 1000.f);
			ImGui::ColorEdit4("Color", (float*)&sprite.Color);
		});

	// Rigidbody 2D
	DrawComponent<Olala::Rigidbody2DComponent>("Rigidbody 2D", entity, [&](auto& rigidbody2d)
		{
			if (ImGui::DragFloat("Mass", &rigidbody2d.Mass, 1.0f, 0.001f, 1000.f)) entity.GetPhysicsBody().InvMass = 1.f / rigidbody2d.Mass;

			if (ImGui::DragFloat2("Velocity", (float*)&rigidbody2d.Velocity)) entity.GetPhysicsBody().Velocity = rigidbody2d.Velocity;

			if (ImGui::Checkbox("Static", &rigidbody2d.IsStatic))
			{
				auto& physicsBody = entity.GetPhysicsBody();
				if (rigidbody2d.IsStatic)
				{
					physicsBody.InvMass = 0.f;
					physicsBody.Velocity = glm::vec2(0.f);
				}
				else
					physicsBody.InvMass = 1 / rigidbody2d.Mass;
			}

			if (ImGui::Checkbox("Gravity", &rigidbody2d.ApplyGravity)) entity.GetPhysicsBody().ApplyGravity = rigidbody2d.ApplyGravity;
		});

	// Box Collider 2D
	DrawComponent<Olala::BoxCollider2DComponent>("Box Collider 2D", entity, [&](auto& boxCollider2d)
		{
			ImGui::DragFloat2("Center", (float*)&boxCollider2d.Center);
			if (ImGui::DragFloat2("Size##bc2d", (float*)&boxCollider2d.Size, 1.f, 0.001f, 1000.f))
				std::static_pointer_cast<Olala::BoundingBox>(entity.GetPhysicsBody().Collider)->SetSize(boxCollider2d.Size);
		});

	// Circle Collider 2D
	DrawComponent<Olala::CircleCollider2DComponent>("Circle Collider 2D", entity, [&](auto& circleCollider2d)
		{
			ImGui::DragFloat2("Center", (float*)&circleCollider2d.Center);
			if (ImGui::DragFloat("Radius", &circleCollider2d.Radius, 1.f, 0.001f, 1000.f))
				std::static_pointer_cast<Olala::BoundingCircle>(entity.GetPhysicsBody().Collider)->Radius = circleCollider2d.Radius;
		});
}

void PropertyPanel::SetIsRuntime(bool isRuntime)
{
	m_IsRuntime = isRuntime;
}

void PropertyPanel::SetDisplayedEntity(const Olala::Entity& entity)
{
	m_DisplayedEntity = entity;
}

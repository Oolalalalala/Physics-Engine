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

		if (m_DisplayedScene)
			DrawSceneContext();
		else if (m_DisplayedEntity)
		{
			if (m_IsRuntime)
				DrawEntityContextRuntime();
			else
				DrawEntityContext();
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
			if (ImGui::MenuItem("Remove##component")) shouldRemove = true;
			ImGui::EndPopup();
		}

		if (opened) uiFunction(entity.GetComponent<T>());
		if (shouldRemove) entity.RemoveComponent<T>();
	}
}

void PropertyPanel::DrawEntityContext()
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
		ImGui::DragFloat("Angular Velocity", &rigidbody2d.AngularVelocity);
		ImGui::DragFloat2("Center Of Mass", (float*)&rigidbody2d.CenterOfMass);
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


#ifdef USE_BOX2D
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
	// TODO : create add_to_physics_world function in scene.h
	auto& rigidbody2d = entity.AddComponent<Olala::Rigidbody2DComponent>();
	auto& transform = entity.GetComponent<Olala::TransformComponent>();
	auto physicsWorld = entity.GetPhysicsWorld();

	b2BodyDef def;
	def.type = rigidbody2d.IsStatic ? b2BodyType::b2_staticBody : b2BodyType::b2_dynamicBody;
	def.position.Set(transform.Position.x, transform.Position.y);
	def.angle = transform.Rotation.z;
	rigidbody2d.RuntimeBody = physicsWorld->CreateBody(&def);
}
template<>
static void AddComponent<Olala::BoxCollider2DComponent>(Olala::Entity& entity)
{
	auto& bc2d = entity.AddComponent<Olala::BoxCollider2DComponent>();
	auto& rb2d = entity.GetComponent<Olala::Rigidbody2DComponent>();

	b2PolygonShape box;
	box.SetAsBox(bc2d.Size.x * 0.5f, bc2d.Size.y * 0.5f, b2Vec2(bc2d.Center.x, bc2d.Center.y), bc2d.AngularOffset);

	b2FixtureDef fixtureDef;
	// TODO : set the restitution and friction
	fixtureDef.restitution = 0.6f;
	fixtureDef.shape = &box;

	((b2Body*)rb2d.RuntimeBody)->CreateFixture(&fixtureDef);
}
template<>
static void AddComponent<Olala::CircleCollider2DComponent>(Olala::Entity& entity)
{
	auto& cc2d = entity.AddComponent<Olala::CircleCollider2DComponent>();
	auto& rb2d = entity.GetComponent<Olala::Rigidbody2DComponent>();

	b2CircleShape circle;
	circle.m_p.Set(cc2d.Center.x, cc2d.Center.y);
	circle.m_radius = cc2d.Radius;

	b2FixtureDef fixtureDef;
	// TODO : set the restitution and friction
	fixtureDef.shape = &circle;

	((b2Body*)rb2d.RuntimeBody)->CreateFixture(&fixtureDef);
}

void PropertyPanel::DrawEntityContextRuntime()
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
					auto& rb2d = entity.GetComponent<Olala::Rigidbody2DComponent>();
					physicsBody.SetTransform(b2Vec2(transform.Position.x, transform.Position.y), glm::radians(transform.Rotation.z));
					physicsBody.SetLinearVelocity(b2Vec2(0.f, 0.f));
				}
			}
			if (ImGui::DragFloat3("Rotation", (float*)&transform.Rotation, 2))
			{
				if (entity.HasComponent<Olala::Rigidbody2DComponent>())
				{
					auto& physicsBody = entity.GetPhysicsBody();
					auto& rb2d = entity.GetComponent<Olala::Rigidbody2DComponent>();
					physicsBody.SetTransform(b2Vec2(transform.Position.x, transform.Position.y), glm::radians(transform.Rotation.z));
				}
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
	
	//// Rigidbody 2D
	DrawComponent<Olala::Rigidbody2DComponent>("Rigidbody 2D", entity, [&](auto& rigidbody2d)
		{
			if (ImGui::DragFloat("Mass", &rigidbody2d.Mass, 1.0f, 0.001f, 1000.f))
			{
				b2MassData data;
				data.mass = rigidbody2d.Mass;
				data.center.Set(rigidbody2d.CenterOfMass.x, rigidbody2d.CenterOfMass.y);
				data.I; // TODO : inertia
				entity.GetPhysicsBody().SetMassData(&data);
			}
	
			if (ImGui::DragFloat2("Velocity", (float*)&rigidbody2d.Velocity)) entity.GetPhysicsBody().SetLinearVelocity({ rigidbody2d.Velocity.x, rigidbody2d.Velocity.y });
	
			if (ImGui::DragFloat("Angular Velocity", &rigidbody2d.AngularVelocity)) entity.GetPhysicsBody().SetAngularVelocity(glm::radians(rigidbody2d.AngularVelocity));
	
			if (ImGui::Checkbox("Static", &rigidbody2d.IsStatic))
			{
				auto& physicsBody = entity.GetPhysicsBody();
				
				physicsBody.SetType(rigidbody2d.IsStatic ? b2BodyType::b2_staticBody : b2BodyType::b2_dynamicBody);
			}
	
			if (ImGui::DragFloat2("Center Of Mass", (float*)&rigidbody2d.CenterOfMass))
			{
				auto& physicsBody = entity.GetPhysicsBody();
	
				b2MassData data;
				data.mass = rigidbody2d.Mass;
				data.center.Set(rigidbody2d.CenterOfMass.x, rigidbody2d.CenterOfMass.y);
				data.I;
	
				physicsBody.SetMassData(&data);
			}
	
			if (ImGui::Checkbox("Gravity", &rigidbody2d.ApplyGravity)) entity.GetPhysicsBody().SetGravityScale(rigidbody2d.ApplyGravity ? 1.f : 0.f);
		});
	
	//// Box Collider 2D
	DrawComponent<Olala::BoxCollider2DComponent>("Box Collider 2D", entity, [&](auto& bc2d)
	{
		bool modified = false;
		modified |= ImGui::DragFloat2("Center", (float*)&bc2d.Center);
		modified |= ImGui::DragFloat2("Size##bc2d", (float*)&bc2d.Size, 1.f, 0.001f, 1000.f);

		if (modified)
		{
			// Recreate Fixture
			auto& body = entity.GetPhysicsBody();
			body.DestroyFixture(body.GetFixtureList());

			b2FixtureDef fixtureDef;
			fixtureDef.density;
			fixtureDef.friction;
			fixtureDef.restitution = 1.f;
			fixtureDef.restitutionThreshold;

			b2PolygonShape box;
			box.SetAsBox(bc2d.Size.x * 0.5f, bc2d.Size.y * 0.5f, { bc2d.Center.x, bc2d.Center.y }, glm::radians(bc2d.AngularOffset));
			fixtureDef.shape = &box;

			body.CreateFixture(&fixtureDef);
		}
	});
	
	// Circle Collider 2D
	DrawComponent<Olala::CircleCollider2DComponent>("Circle Collider 2D", entity, [&](auto& cc2d)
	{
		bool modified = false;
		modified |= ImGui::DragFloat2("Center", (float*)&cc2d.Center);
		modified |= ImGui::DragFloat("Radius", &cc2d.Radius, 1.f, 0.001f, 1000.f);

		if (modified)
		{
			// Recreate fixture
			auto& body = entity.GetPhysicsBody();
			body.DestroyFixture(body.GetFixtureList());

			b2FixtureDef fixtureDef;
			fixtureDef.density;
			fixtureDef.friction;
			fixtureDef.restitution = 1.f;
			fixtureDef.restitutionThreshold;

			b2CircleShape circle;
			circle.m_p.Set(cc2d.Center.x, cc2d.Center.y);
			circle.m_radius = cc2d.Radius;
			fixtureDef.shape = &circle;

			body.CreateFixture(&fixtureDef);
		}
	});
}

#else

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

void PropertyPanel::DrawEntityContextRuntime()
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
				auto& rb2d = entity.GetComponent<Olala::Rigidbody2DComponent>();
				physicsBody.Position = (glm::vec2)transform.Position + rb2d.CenterOfMass;
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

		if (ImGui::DragFloat("Velocity", &rigidbody2d.AngularVelocity)) entity.GetPhysicsBody().AngularVelocity = rigidbody2d.AngularVelocity;

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

		if (ImGui::DragFloat2("Center Of Mass", (float*)&rigidbody2d.CenterOfMass))
		{
			auto& physicsBody = entity.GetPhysicsBody();
			auto& transform = entity.GetComponent<Olala::TransformComponent>();
			physicsBody.Position = (glm::vec2)transform.Position + rigidbody2d.CenterOfMass;
			if (entity.HasComponent<Olala::BoxCollider2DComponent>())
				physicsBody.Collider->Offset = -rigidbody2d.CenterOfMass + entity.GetComponent<Olala::BoxCollider2DComponent>().Center;
			if (entity.HasComponent<Olala::CircleCollider2DComponent>())
				physicsBody.Collider->Offset = -rigidbody2d.CenterOfMass + entity.GetComponent<Olala::CircleCollider2DComponent>().Center;
		}

		if (ImGui::Checkbox("Gravity", &rigidbody2d.ApplyGravity)) entity.GetPhysicsBody().ApplyGravity = rigidbody2d.ApplyGravity;
	});

	// Box Collider 2D
	DrawComponent<Olala::BoxCollider2DComponent>("Box Collider 2D", entity, [&](auto& boxCollider2d)
		{
			if (ImGui::DragFloat2("Center", (float*)&boxCollider2d.Center))
			{
				entity.GetPhysicsBody().Collider->Offset = -entity.GetComponent<Olala::Rigidbody2DComponent>().CenterOfMass + boxCollider2d.Center;
			}

			if (ImGui::DragFloat2("Size##bc2d", (float*)&boxCollider2d.Size, 1.f, 0.001f, 1000.f))
				std::static_pointer_cast<Olala::BoundingBox>(entity.GetPhysicsBody().Collider)->SetSize(boxCollider2d.Size);
		});

	// Circle Collider 2D
	DrawComponent<Olala::CircleCollider2DComponent>("Circle Collider 2D", entity, [&](auto& circleCollider2d)
		{
			if (ImGui::DragFloat2("Center", (float*)&circleCollider2d.Center))
			{
				entity.GetPhysicsBody().Collider->Offset = -entity.GetComponent<Olala::Rigidbody2DComponent>().CenterOfMass + circleCollider2d.Center;
			}

			if (ImGui::DragFloat("Radius", &circleCollider2d.Radius, 1.f, 0.001f, 1000.f))
				std::static_pointer_cast<Olala::BoundingCircle>(entity.GetPhysicsBody().Collider)->Radius = circleCollider2d.Radius;
		});
}

#endif

void PropertyPanel::DrawSceneContext()
{
	constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;

	if (ImGui::CollapsingHeader("Scene", flags))
	{
		char sceneName[25];
		strcpy_s(sceneName, m_DisplayedScene->GetName().c_str());
		if (ImGui::InputText("Name", sceneName, 25))
		{
			if (strlen(sceneName))
			{
				m_DisplayedScene->GetName() = sceneName;

			}
		}
	}
	
}

void PropertyPanel::SetIsRuntime(bool isRuntime)
{
	m_IsRuntime = isRuntime;
}

void PropertyPanel::DisplayEntity(const Olala::Entity& entity)
{
	m_DisplayedEntity = entity;
	m_DisplayedScene = nullptr;
}

void PropertyPanel::DisplayScene(Olala::Ref<Olala::Scene> scene)
{
	m_DisplayedScene = scene;
}
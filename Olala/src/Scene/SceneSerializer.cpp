#include "pch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Component.h"
#include <yaml-cpp/yaml.h>


namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Olala {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4 v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	SceneSerializer::SceneSerializer(const Ref<Scene>& scene, const fs::path& filepath)
		: m_Scene(scene)
	{
		m_SceneFilePath = filepath;
		m_DirectoryPath = filepath.parent_path();
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity, std::unordered_map<Ref<Texture2D>, uint32_t>& textureMap)
	{
		out << YAML::BeginMap;

		auto& tag = entity.GetComponent<TagComponent>();
		out << YAML::Key << "Name" << YAML::Value << tag.Tag;

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& camera = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.Camera->GetProjectionType();
			if (camera.Camera->GetProjectionType() == CameraProjectionType::Perspective)
			{
				auto specs = std::static_pointer_cast<PerspectiveCamera>(camera.Camera)->GetSpecs();
				out << YAML::Key << "NearPlane" << YAML::Value << specs.NearPlane;
				out << YAML::Key << "FarPlane" << YAML::Value << specs.FarPlane;
				out << YAML::Key << "FieldOfView" << YAML::Value << specs.Fov;
				out << YAML::Key << "AspectRatio" << YAML::Value << specs.AspectRatio;
			}
			else
			{
				auto specs = std::static_pointer_cast<OrthographicCamera>(camera.Camera)->GetSpecs();
				out << YAML::Key << "NearPlane" << YAML::Value << specs.NearPlane;
				out << YAML::Key << "FarPlane" << YAML::Value << specs.FarPlane;
				out << YAML::Key << "Width" << YAML::Value << specs.Width;
				out << YAML::Key << "Height" << YAML::Value << specs.Height;
			}
			out << YAML::EndMap;

			out << YAML::Key << "IsMain" << camera.IsMain;
			out << YAML::Key << "IsOn" << camera.IsOn;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Size" << YAML::Value << sprite.Size;
			out << YAML::Key << "Color" << YAML::Value << sprite.Color;
			out << YAML::Key << "Texture" << YAML::Value << textureMap[sprite.Texture];

			out << YAML::EndMap;
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap;

			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "Mass" << YAML::Value << rb2d.Mass;
			out << YAML::Key << "Velocity" << YAML::Value << rb2d.Velocity;
			out << YAML::Key << "Static" << YAML::Value << rb2d.IsStatic;
			out << YAML::Key << "Gravity" << YAML::Value << rb2d.ApplyGravity;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Center" << YAML::Value << bc2d.Center;
			out << YAML::Key << "AngularOffset" << YAML::Value << bc2d.AngularOffset;
			out << YAML::Key << "Size" << YAML::Value << bc2d.Size;

			out << YAML::EndMap;
		}
		
		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Center" << YAML::Value << cc2d.Center;
			out << YAML::Key << "Radius" << YAML::Value << cc2d.Radius;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize()
	{
		// Rename
		if (m_DirectoryPath.filename() != m_Scene->GetName())
		{
			fs::rename(m_SceneFilePath, m_DirectoryPath / (m_Scene->m_Name + ".olala"));
			fs::rename(m_DirectoryPath, m_DirectoryPath.parent_path() / m_Scene->m_Name);
			m_DirectoryPath = m_DirectoryPath.parent_path() / m_Scene->m_Name;
			m_SceneFilePath = m_DirectoryPath / (m_Scene->m_Name + ".olala");
		}



		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_Name;


		// Textures
		out << YAML::Key << "Textures" << YAML::Value << YAML::BeginMap;

		std::unordered_map<Ref<Texture2D>, uint32_t> textureMap;
		textureMap[nullptr] = 0;
		uint32_t idx = 1;

		auto& texturePool = m_Scene->m_AssetManager->GetPool<Texture2D>();
		for (const auto& [textureName, texture] : texturePool.GetAll())
		{
			out << YAML::Key << textureName << YAML::Value << idx;
			if (textureMap.find(texture) == textureMap.end())
				textureMap[texture] = idx++;
		}

		out << YAML::EndMap;


		// Entities
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity(entityID, m_Scene.get());
			if (!entity)
				return;

			SerializeEntity(out, entity, textureMap);
		});
		YAML::EndSeq;
		YAML::EndMap;

		std::ofstream fout(m_SceneFilePath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize()
	{
		std::ifstream stream(m_SceneFilePath);
		YAML::Node data = YAML::Load(stream);

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();


		// Load Textures
		m_Scene->m_AssetManager->Load(m_DirectoryPath);
		auto& texturePool = m_Scene->m_AssetManager->GetPool<Texture2D>();

		std::unordered_map<uint32_t, Ref<Texture2D>> textureMap;
		textureMap[0] = nullptr;


		auto textures = data["Textures"];
		for (auto texture : textures)
		{
			textureMap[texture.second.as<uint32_t>()] = texturePool.Get(texture.first.as<std::string>());
		}


		auto entities = data["Entities"];
		for (auto entity : entities)
		{
			std::string name = entity["Name"].as<std::string>();
			Entity deserializedEntity = m_Scene->CreateEntity(name);

			if (auto transformComponent = entity["TransformComponent"])
			{
				auto& transform = deserializedEntity.GetComponent<TransformComponent>();
				transform.Position = transformComponent["Position"].as<glm::vec3>();
				transform.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				transform.Scale = transformComponent["Scale"].as<glm::vec3>();
			}
			if (auto cameraComponent = entity["CameraComponent"])
			{
				auto& camera = deserializedEntity.AddComponent<CameraComponent>();

				if (cameraComponent["Camera"])
				{
					if (cameraComponent["Camera"]["ProjectionType"].as<int>() == (int)CameraProjectionType::Perspective)
					{
						float nearPlane   =  cameraComponent["Camera"]["NearPlane"].as<float>();
						float farPlane    =  cameraComponent["Camera"]["FarPlane"].as<float>();
						float fieldOfView =  cameraComponent["Camera"]["FieldOfView"].as<float>();
						float aspectRatio =  cameraComponent["Camera"]["AspectRatio"].as<float>();
						camera.Camera = CreateRef<PerspectiveCamera>(nearPlane, farPlane, fieldOfView, aspectRatio);
					}
					else
					{
						float nearPlane =  cameraComponent["Camera"]["NearPlane"].as<float>();
						float farPlane  =  cameraComponent["Camera"]["FarPlane"].as<float>();
						float width     =  cameraComponent["Camera"]["Width"].as<float>();
						float height    =  cameraComponent["Camera"]["Height"].as<float>();
						camera.Camera = CreateRef<OrthographicCamera>(width, height, nearPlane, farPlane);
					}
				}

				camera.IsMain = cameraComponent["IsMain"].as<bool>();
				camera.IsOn = cameraComponent["IsOn"].as<bool>();

				FramebufferSpecs specs;
				specs.Width = 800, specs.Height = 600;
				camera.RenderTarget = Framebuffer::Create(specs);
			}
			if (auto spriteRendererComponent = entity["SpriteRendererComponent"])
			{
				auto& sprite = deserializedEntity.AddComponent<SpriteRendererComponent>();
				sprite.Size = spriteRendererComponent["Size"].as<glm::vec2>();
				sprite.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				sprite.Texture = textureMap[spriteRendererComponent["Texture"].as<uint32_t>()];
			}
			if (auto rigidbody2DComponent = entity["Rigidbody2DComponent"])
			{
				auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
				rb2d.Mass = rigidbody2DComponent["Mass"].as<float>();
				rb2d.Velocity = rigidbody2DComponent["Velocity"].as<glm::vec2>();
				rb2d.IsStatic = rigidbody2DComponent["Static"].as<bool>(); 
				rb2d.ApplyGravity = rigidbody2DComponent["Gravity"].as<bool>();
			}
			if (auto boxCollider2DComponent = entity["BoxCollider2DComponent"])
			{
				auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
				bc2d.Center = boxCollider2DComponent["Center"].as<glm::vec2>();
				bc2d.AngularOffset = boxCollider2DComponent["AngularOffset"].as<float>();
				bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
			}
			if (auto circleCollider2DComponent = entity["CircleCollider2DComponent"])
			{
				auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
				cc2d.Center = circleCollider2DComponent["Center"].as<glm::vec2>();
				cc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
			}
		}



		m_Scene->m_Name = sceneName;

		return true;
	}

	void SceneSerializer::SetFilePath(const fs::path& filepath)
	{
		m_SceneFilePath = filepath;
		m_DirectoryPath = filepath.parent_path();
	}

	bool SceneSerializer::CraeteDirectory(const fs::path& path, Ref<Scene> scene)
	{
		const std::string& sceneName = scene->GetName();

		bool success = true;
		success &= fs::create_directory(path / sceneName);
		success &= fs::create_directory(path / sceneName / "Texture");
		SceneSerializer(scene, path / sceneName / (sceneName + ".olala")).Serialize();

		return success;
	}

	void SceneSerializer::CopyAssets(const fs::path& srcDirectory, const fs::path& dstDirectory)
	{
		fs::copy(srcDirectory / "Texture", dstDirectory / "Texture");
	}

	template<typename T>
	bool SceneSerializer::Import(const fs::path& filepath)
	{
		static_assert(false);
	}
	template<>
	bool SceneSerializer::Import<Texture2D>(const fs::path& filepath)
	{
		fs::path newPath = m_DirectoryPath / "Texture" / filepath.filename();
		bool success = fs::copy_file(filepath, newPath);

		if (!success)
			return false;

		m_Scene->GetAssetManager()->GetPool<Texture2D>().Add(filepath.filename().string(), Texture2D::Create(newPath.string()));

		return true;
	}

}
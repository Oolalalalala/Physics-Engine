#pragma once

#include "Scene.h"

namespace Olala {

	namespace fs = std::filesystem;

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene, const fs::path& filepath);

		void Serialize();
		bool Deserialize();

		void SetFilePath(const fs::path& filepath);

		template<typename T>
		bool Import(const fs::path& filepath);

		static bool CraeteDirectory(const fs::path& path, Ref<Scene> scene);

	private:
		Ref<Scene> m_Scene;

		fs::path m_DirectoryPath;
		fs::path m_SceneFilePath;
	};

}
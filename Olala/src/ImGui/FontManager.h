#pragma once

#include "imgui/imgui.h"

namespace Olala {

	class FontManager
	{
	public:
		FontManager();
		~FontManager();

		void AddFont(const std::string& name, const std::string& filepath, float size = 18.f);
		ImFont* GetFont(const std::string& name);
		void RemoveAllFonts();

	private:
		std::unordered_map<std::string, ImFont*> m_Fonts;
	};

}
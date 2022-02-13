#include "pch.h"
#include "FontManager.h"

Olala::FontManager::FontManager()
{
}

Olala::FontManager::~FontManager()
{
	RemoveAllFonts();
}

void Olala::FontManager::AddFont(const std::string& name, const std::string& filepath, float size)
{
	ImGuiIO& io = ImGui::GetIO();
	m_Fonts[name] = io.Fonts->AddFontFromFileTTF(filepath.c_str(), size);
}

ImFont* Olala::FontManager::GetFont(const std::string& name)
{
	return m_Fonts[name];
}

void Olala::FontManager::RemoveAllFonts()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->ClearFonts();
	m_Fonts.clear();
}

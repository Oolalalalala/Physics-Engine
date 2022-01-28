#pragma once

namespace Olala {

	class FileDialog
	{
	public:
		static std::string OpenFile(const std::string& title, const std::vector<std::string>& filters = { "All Files", "*" });
		static std::string SaveFile(const std::string& title, const std::vector<std::string>& filters = { "All Files", "*" });
		static std::string SelectFolder(const std::string& title);
		static void Message(const std::string& title, const std::string& message);
	};
}
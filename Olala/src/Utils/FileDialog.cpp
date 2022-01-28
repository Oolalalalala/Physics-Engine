#include "pch.h"
#include "FileDialog.h"

#include "portable_file_dialogs/portable_file_dialogs.h"

namespace Olala {

	std::string FileDialog::OpenFile(const std::string& title, const std::vector<std::string>& filters)
	{
		auto result = pfd::open_file(title, "", filters).result();

		if (result.empty()) 
			return std::string();

		return result[0];
	}

	std::string FileDialog::SaveFile(const std::string& title, const std::vector<std::string>& filters)
	{
		return pfd::save_file(title, "", filters).result();
	}

	std::string FileDialog::SelectFolder(const std::string& title)
	{
		return pfd::select_folder(title).result();
	}

	void FileDialog::Message(const std::string& title, const std::string& message)
	{
		auto result = pfd::message(title, message, pfd::choice::ok).result();
	}

}
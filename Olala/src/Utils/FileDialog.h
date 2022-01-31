#pragma once

namespace Olala {

	class FileDialog
	{
	public:

		enum class Result {
			cancel = -1,
			ok,
			yes,
			no,
			abort,
			retry,
			ignore
		};

		static std::string OpenFile(const std::string& title, const std::vector<std::string>& filters = { "All Files", "*" });
		static std::string SaveFile(const std::string& title, const std::vector<std::string>& filters = { "All Files", "*" });
		static std::string SelectFolder(const std::string& title);
		static Result MessageOK(const std::string& title, const std::string& message);
		static Result MessageYesNo(const std::string& title, const std::string& message);
		static Result MessageYesNoCancel(const std::string& title, const std::string& message);
	};
}
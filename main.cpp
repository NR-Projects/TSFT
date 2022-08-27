<<<<<<< HEAD
#pragma once

#include "Config.h"
#include "FileHandler.h"
#include <cstring>

std::string TrimSpaces(std::string str) {
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ') + 1);
	return str;
}

std::vector<std::string> GetArgAsList(std::string arg_list) {
	// Check If List Or Not
	std::string args = arg_list;
	std::vector<std::string> args_as_list;

	size_t checkForList = args.find(',');

	if (checkForList == std::string::npos) {
		args_as_list.push_back(TrimSpaces(args));
	}
	else {
		size_t pos = 0;
		while ((pos = args.find(',')) != std::string::npos) {
			args_as_list.push_back(TrimSpaces(args.substr(0, pos)));
			args.erase(0, pos + 1);
		}
		args_as_list.push_back(TrimSpaces(args));
	}

	return args_as_list;
}

bool isArgEqual(const char* arg1, const char* arg2) {
	return (strcmp(arg1, arg2) == 0);
}

void PrintHelp()
{
	std::cout << "This is all the available commands for tsft.exe file helper" << std::endl;
	std::cout << "Source Path is automatically set to current directory (If None Is Passed)" << std::endl;
	std::cout << "\nAll parameters with :opt are optional and will have a default value that is indicated below" << std::endl;
	Config::PrintHelp();
	FileHandler::PrintHelp();
}

int main(int argc, char* argv[])
{
	// Load Config
	Config config;
	config.LoadConfig();

	// Initialize FileHandler
	FileHandler filehandler(config);

	// No Arguments Passed
	if (argc == 1) {
		PrintHelp();
		return 0;
	}

	// Check For Config Op
	if (argc >= 2 && isArgEqual(argv[1], "config"))
	{
		if (argc == 2) {
			Config::PrintHelp();
			return 0;
		}

		if (isArgEqual(argv[2], "/s")) {
			config.ShowConfig();
			return 0;
		}

		if (isArgEqual(argv[2], "/e")) {
			config.EditConfig();
			return 0;
		}
	}

	/// Check For Files
	// Check For Create Op
#pragma region Create Commands
	if (argc > 2 && isArgEqual(argv[1], "+fl"))
	{
		std::vector<std::string> files = GetArgAsList(argv[2]);
		filehandler.CreateFiles(files);
		return 0;
	}

	if (argc > 2 && isArgEqual(argv[1], "+fd"))
	{
		std::vector<std::string> folders = GetArgAsList(argv[2]);
		filehandler.CreateFolders(folders);
		return 0;
	}
#pragma endregion
	
	// Check For Show Op
#pragma region Show Commands
	if (argc >= 2 && isArgEqual(argv[1], "-p")) {
		if (argc == 2) {
			filehandler.ShowFilesAndFolders(
				filehandler.GetAllFile()
			);
			return 0;
		}

		if (argc == 3) {
			filehandler.ShowFilesAndFolders(
				filehandler.GetAllFile("", std::stoi(argv[2]))
			);
			return 0;
		}

		if (argc == 4) {
			if (isArgEqual(argv[2], "-sf") || isArgEqual(argv[2], "-cf") || isArgEqual(argv[2], "-ef")) {
				filehandler.ShowFilesAndFolders(
					filehandler.GetAllFnFCategoryFiltered(GetArgAsList(argv[3]))
				);
				return 0;
			}
		}

		if (argc == 5) {
			if (isArgEqual(argv[3], "-sf") || isArgEqual(argv[3], "-cf") || isArgEqual(argv[3], "-ef")) {
				std::vector<std::string> substrfilters = GetArgAsList(argv[4]);
				filehandler.ShowFilesAndFolders(
					filehandler.GetAllFNFNameFiltered(
						substrfilters,
						"",
						std::stoi(argv[2])
					)
				);
				return 0;
			}
		}
	}
#pragma endregion

	// Check For Clone Op
#pragma region Clone Command
	if (argc >= 3 && isArgEqual(argv[1], "-c")) {
		if (argc == 3) {
			filehandler.CloneFiles(
				filehandler.GetAllFile(
					"",
				 0
				),
				argv[2]
			);
		}
		if (argc == 4) {
			filehandler.CloneFiles(
				filehandler.GetAllFile(
					"",
					std::stoi(argv[2])
				),
				argv[3]
			);
		}
		if (argc == 5) {
			if (isArgEqual(argv[3], "-sf") || isArgEqual(argv[3], "-cf") || isArgEqual(argv[3], "-ef")) {
				filehandler.CloneFiles(
					filehandler.GetAllFNFNameFiltered(
						GetArgAsList(argv[4]),
						"",
						std::stoi(argv[2])
					),
					argv[5]
				);
			}
		}
		if (argc == 6) {
			if (isArgEqual(argv[3], "-sf") || isArgEqual(argv[3], "-cf") || isArgEqual(argv[3], "-ef")) {
				filehandler.CloneFiles(
					filehandler.GetAllFNFNameFiltered(
						GetArgAsList(argv[4]),
						argv[6],
						std::stoi(argv[2])
					),
					argv[5]
				);
			}
		}
	}
#pragma endregion

	// Check For Remove Op
#pragma region Remove Command
	if (argc > 2 && isArgEqual(argv[1], "-fl"))
	{
		std::vector<std::string> files = GetArgAsList(argv[2]);
		filehandler.RemoveFiles(files);
		return 0;
	}

	if (argc > 2 && isArgEqual(argv[1], "-fd"))
	{
		std::vector<std::string> folders = GetArgAsList(argv[2]);
		filehandler.RemoveFolders(folders);
		return 0;
	}
#pragma endregion

	std::cout << "Unfortunately, that command, does not exist [yet], here are the commands you might want to use\n\n" << std::endl;
	PrintHelp();

	return 0;
=======
#pragma once

#include "Config.h"
#include "FileHandler.h"
#include <cstring>

std::string TrimSpaces(std::string str) {
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ') + 1);
	return str;
}

std::vector<std::string> GetArgAsList(std::string arg_list) {
	// Check If List Or Not
	std::string args = arg_list;
	std::vector<std::string> args_as_list;

	size_t checkForList = args.find(',');

	if (checkForList == std::string::npos) {
		args_as_list.push_back(TrimSpaces(args));
	}
	else {
		size_t pos = 0;
		while ((pos = args.find(',')) != std::string::npos) {
			args_as_list.push_back(TrimSpaces(args.substr(0, pos)));
			args.erase(0, pos + 1);
		}
		args_as_list.push_back(TrimSpaces(args));
	}

	return args_as_list;
}

bool isArgEqual(const char* arg1, const char* arg2) {
	return (strcmp(arg1, arg2) == 0);
}

void PrintHelp()
{
	std::cout << "This is all the available commands for tsft.exe file helper" << std::endl;
	std::cout << "Source Path is automatically set to current directory (If None Is Passed)" << std::endl;
	std::cout << "\nAll parameters with :opt are optional and will have a default value that is indicated below" << std::endl;
	Config::PrintHelp();
	FileHandler::PrintHelp();
}

int main(int argc, char* argv[])
{
	// Load Config
	Config config;
	config.LoadConfig();

	// Initialize FileHandler
	FileHandler filehandler(config);

	// No Arguments Passed
	if (argc == 1) {
		PrintHelp();
		return 0;
	}

	// Check For Config Op
	if (argc >= 2 && isArgEqual(argv[1], "config"))
	{
		if (argc == 2) {
			Config::PrintHelp();
			return 0;
		}

		if (isArgEqual(argv[2], "/s")) {
			config.ShowConfig();
			return 0;
		}

		if (isArgEqual(argv[2], "/e")) {
			config.EditConfig();
			return 0;
		}
	}

	/// Check For Files
	// Check For Create Op
#pragma region Create Commands
	if (argc > 2 && isArgEqual(argv[1], "+fl"))
	{
		std::vector<std::string> files = GetArgAsList(argv[2]);
		filehandler.CreateFiles(files);
		return 0;
	}

	if (argc > 2 && isArgEqual(argv[1], "+fd"))
	{
		std::vector<std::string> folders = GetArgAsList(argv[2]);
		filehandler.CreateFolders(folders);
		return 0;
	}
#pragma endregion
	
	// Check For Show Op
#pragma region Show Commands
	if (argc >= 2 && isArgEqual(argv[1], "-p")) {
		if (argc == 2) {
			filehandler.ShowFilesAndFolders(
				filehandler.GetAllFile()
			);
			return 0;
		}

		if (argc == 3) {
			filehandler.ShowFilesAndFolders(
				filehandler.GetAllFile("", std::stoi(argv[2]))
			);
			return 0;
		}

		if (argc == 4) {
			if (isArgEqual(argv[2], "-sf") || isArgEqual(argv[2], "-cf") || isArgEqual(argv[2], "-ef")) {
				filehandler.ShowFilesAndFolders(
					filehandler.GetAllFnFCategoryFiltered(GetArgAsList(argv[3]))
				);
				return 0;
			}
		}

		if (argc == 5) {
			if (isArgEqual(argv[3], "-sf") || isArgEqual(argv[3], "-cf") || isArgEqual(argv[3], "-ef")) {
				std::vector<std::string> substrfilters = GetArgAsList(argv[4]);
				filehandler.ShowFilesAndFolders(
					filehandler.GetAllFNFNameFiltered(
						substrfilters,
						"",
						std::stoi(argv[2])
					)
				);
				return 0;
			}
		}
	}
#pragma endregion

	// Check For Clone Op
#pragma region Clone Command
	if (argc >= 3 && isArgEqual(argv[1], "-c")) {
		if (argc == 3) {
			filehandler.CloneFiles(
				filehandler.GetAllFile(
					"",
				 0
				),
				argv[2]
			);
		}
		if (argc == 4) {
			filehandler.CloneFiles(
				filehandler.GetAllFile(
					"",
					std::stoi(argv[2])
				),
				argv[3]
			);
		}
		if (argc == 5) {
			if (isArgEqual(argv[3], "-sf") || isArgEqual(argv[3], "-cf") || isArgEqual(argv[3], "-ef")) {
				filehandler.CloneFiles(
					filehandler.GetAllFNFNameFiltered(
						GetArgAsList(argv[4]),
						"",
						std::stoi(argv[2])
					),
					argv[5]
				);
			}
		}
		if (argc == 6) {
			if (isArgEqual(argv[3], "-sf") || isArgEqual(argv[3], "-cf") || isArgEqual(argv[3], "-ef")) {
				filehandler.CloneFiles(
					filehandler.GetAllFNFNameFiltered(
						GetArgAsList(argv[4]),
						argv[6],
						std::stoi(argv[2])
					),
					argv[5]
				);
			}
		}
	}
#pragma endregion

	// Check For Remove Op
#pragma region Remove Command
	if (argc > 2 && isArgEqual(argv[1], "-fl"))
	{
		std::vector<std::string> files = GetArgAsList(argv[2]);
		filehandler.RemoveFiles(files);
		return 0;
	}

	if (argc > 2 && isArgEqual(argv[1], "-fd"))
	{
		std::vector<std::string> folders = GetArgAsList(argv[2]);
		filehandler.RemoveFolders(folders);
		return 0;
	}
#pragma endregion

	std::cout << "Unfortunately, that command, does not exist [yet], here are the commands you might want to use\n\n" << std::endl;
	PrintHelp();

	return 0;
>>>>>>> 6822adcdac7fb86e054535633119486a9acfa7de
}
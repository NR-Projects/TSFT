#pragma once
#include <algorithm>
#include <iostream>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "Config.h"


struct FileStruct {
	std::filesystem::directory_entry _entry;
	int _depth;

	FileStruct(std::filesystem::directory_entry entry, int depth) {
		_entry = entry;
		_depth = depth;
	}
};


class FileHandler
{
public:
	static void PrintHelp();

	FileHandler(Config config);
	std::vector<FileStruct> GetAllFile(std::string srcPath = "", int depth = 0);
	std::vector<FileStruct> GetAllFNFNameFiltered(std::vector<std::string> Filters, std::string srcPath = "", int depth = 0);
	std::vector<FileStruct> GetAllFnFCategoryFiltered(std::vector<std::string> Filters, std::string srcPath = "", int depth = 0);
	std::vector<FileStruct> GetAllFnFExtensionFiltered(std::vector<std::string> Filters, std::string srcPath = "", int depth = 0);

	void CreateFiles(std::vector<std::string> files);
	void CreateFolders(std::vector<std::string> folders);
	void ShowFilesAndFolders(std::vector<FileStruct> FnF);
	void CloneFiles(std::vector <FileStruct> FnF, std::string destPath);
	void RemoveFiles(std::vector<std::string> files);
	void RemoveFolders(std::vector<std::string> folders);
	
private:
	void _RecursiveFileSearch(std::vector<FileStruct>& path_list, std::string dir_path, int curr_depth, int max_depth);

private:
	Config _config;
};

#pragma once

#include <algorithm>
#include <iostream>
#include <chrono>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "Config.h"

struct file_struct {
	std::filesystem::directory_entry _entry;
	int _depth;

	file_struct(std::filesystem::directory_entry entry, int depth) {
		_entry = entry;
		_depth = depth;
	}
};


class FileHandler
{
public:

	enum fnf_type { FILE_TYPE, FOLDER_TYPE };

	static void help();

	FileHandler(Config config);

	// Exposed Actions
	void create_fnf(std::vector<std::string> _path_names, fnf_type _type);
	void show_fnf(std::vector<file_struct> _fnf);
	void clone_fnf(std::vector <file_struct> _fnf, std::string _dest_path);
	void remove_fnf(std::vector<std::string> _path_names, fnf_type _type);

	// Filters
	std::vector<file_struct> get_all_fnf(std::string _src_path = "", int _depth = 0);
	std::vector<file_struct> get_all_fnf_name_filtered(std::vector<std::string> _filters, std::string _src_path = "", int _depth = 0);
	std::vector<file_struct> get_all_fnf_category_filtered(std::vector<std::string> _filters, std::string _src_path = "", int _depth = 0);
	std::vector<file_struct> get_all_fnf_extension_filtered(std::vector<std::string> _filters, std::string _src_path = "", int _depth = 0);

private:
	void _recursive_fnf_search(std::vector<file_struct>& _path_list, std::string _dir_path, int _curr_depth, int _max_depth);

private:
	Config _config;
};


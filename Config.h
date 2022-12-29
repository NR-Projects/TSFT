#pragma once

#include <chrono>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>


class Config
{
public:
	static void help();

	void load_config();
	void print_config();
	void edit_config();

private:
	void _generate_config();
	bool _is_config_exist();

	void _parse_config_line(std::string _line);
	void _parse_categories(std::vector<std::string> _categories);

	bool _str2bool(std::string _strbool);

private:
	std::string default_path = "C:\\TunaSalmon\\tsft\\";
	std::string config_path = default_path + "tsftconfig.txt";
	std::string categories_path = "C:\\TunaSalmon\\tsft\\categories.txt"; // Can be override by config file

public:
	int print_mode = 1; // 0 -> Structure, 1 -> Detail
	bool enable_logs = false;
	bool show_time_to_execute = false;

	std::map<std::string, std::vector<std::string>> categories;
};


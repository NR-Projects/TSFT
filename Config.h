#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <vector>

class Config
{
public:
	static void PrintHelp();

	Config();
	void LoadConfig();
	void ShowConfig();
	void EditConfig();

private:
	void _GenerateDefaultPath();
	void _GenerateConfig();
	void _GenerateCategories();
	bool _IsConfigExists();
	void _ParseConfigLine(std::string line);
	void _ParseCategory(std::string CategoryType, std::vector<std::string> categories);

private:
	const char* DefaultAppPath = "C:\\TunaSalmon\\tsft";
	const char* ConfigPath = "C:\\TunaSalmon\\tsft\\tsftconfig.txt";
	const char* BaseCategoryDefaultPath = "basecategorypath.txt";
	// Props
	std::string BaseCategoriesPath = "";
	std::string ExtendedCategoriesPath = "";

public:
	std::map<std::string, std::vector<std::string>> BaseCategories;
	std::map<std::string, std::vector<std::string>> ExtendedCategories;
};


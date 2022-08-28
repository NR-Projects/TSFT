#include "Config.h"

void Config::PrintHelp() {
	std::cout
		<< "    Show Config:                        tsft config /s                          \n"
		<< "    Edit Config:                        tsft config /e                          \n"
		<< std::endl;
}

Config::Config()
{
}

void Config::LoadConfig()
{
	_GenerateDefaultPath();

	if (!_IsConfigExists()) {
		_GenerateConfig();
		_GenerateCategories();
	}


	// Load Config
	std::ifstream r_file;
	std::string f_line;

	r_file.open(ConfigPath);
	while (std::getline(r_file, f_line))
	{
		_ParseConfigLine(f_line);
	}
	r_file.close();
	
	// Load Base Category
	r_file.open(BaseCategoriesPath);
	std::vector<std::string> tbvec;
	while (std::getline(r_file, f_line))
	{
		tbvec.push_back(f_line);
	}
	_ParseCategory("Base", tbvec);
	r_file.close();


	// Load Extended Category (If Exists)
	if (ExtendedCategoriesPath == "") return;

	r_file.open(ExtendedCategoriesPath);
	std::vector<std::string> tevec;
	while (std::getline(r_file, f_line))
	{
		tevec.push_back(f_line);
	}
	_ParseCategory("Extended", tevec);
	r_file.close();
}

void Config::ShowConfig()
{
	// Show Config
	std::cout
		<< "[Config] \n"
		<< "BaseCategoryPath=" << BaseCategoriesPath << "\n"
		<< "ExtendedCategoryPath=" << ExtendedCategoriesPath << "\n"
		<< "\n\n";
	
	std::cout<< "[Base Categories] \n";

	for (auto const& x : BaseCategories)
	{
		std::cout << "->" << x.first << "\n";
		for (std::string cat : x.second) {
			std::cout << cat << "\n";
		}

		std::cout << "\n";
	}

	std::cout << "\n\n";

	std::cout << "[Extended Categories] \n";

	for (auto const& x : ExtendedCategories)
	{
		std::cout << "->" << x.first << "\n";
		for (std::string cat : x.second) {
			std::cout << cat << "\n";
		}

		std::cout << "\n";
	}
}

void Config::EditConfig()
{
	std::cout << "Not Yet Available" << std::endl;
}

void Config::_GenerateDefaultPath()
{
	if (!std::filesystem::exists(DefaultAppPath)) {
		std::filesystem::create_directories(DefaultAppPath);
	}
}

void Config::_GenerateConfig()
{
	std::ofstream w_file;
	w_file.open(ConfigPath);

	w_file << "* This is a comment \n";
	w_file << "* Use NONE to indicate that no path has been placed \n";
	w_file << "BaseCategoryPath=basecategorypath.txt" << "\n";
	w_file << "ExtendedCategoryPath=NONE" << "\n";

	w_file.close();
}

void Config::_GenerateCategories()
{
	std::ofstream w_file;
	w_file.open(DefaultAppPath + std::string("\\") + BaseCategoryDefaultPath);

	w_file << "* This is a comment \n";
	w_file << "* categories are represented by -><category name> \n";
	w_file << "* Only put extensions without dot(.) \n";
	w_file << "->img" << "\n";
	w_file << "png" << "\n";
	w_file << "jpg" << "\n";
	w_file << "\n\n";
	w_file << "->vid" << "\n";
	w_file << "mp4" << "\n";

	w_file.close();
}

bool Config::_IsConfigExists()
{
	std::ifstream infile(ConfigPath);
	return infile.good();
}

void Config::_ParseConfigLine(std::string line)
{
	// Ignore Comments
	if (line.length() > 0)
		if (line[0] == '*')
			return;

	size_t pos = line.find("=");
	std::string first_part = line.substr(0, pos);
	std::string second_part = line.substr(pos + 1, line.length());

	if (second_part == "NONE") return;

	if (first_part == "BaseCategoryPath")
		BaseCategoriesPath = DefaultAppPath + std::string("\\") + second_part;
	else if (first_part == "ExtendedCategoryPath")
		ExtendedCategoriesPath = DefaultAppPath + std::string("\\") + second_part;
}

void Config::_ParseCategory(std::string CategoryType, std::vector<std::string> categories)
{
	std::map<std::string, std::vector<std::string>> tmap;

	bool lookForCategoryHead = true;

	std::string tvec_lbl;
	
	for (unsigned int i = 0; i < categories.size(); i++) {
		std::string line = categories[i];

		// Ignore Comments
		if (line.length() > 0)
			if (line[0] == '*')
				continue;

		if (lookForCategoryHead) {
			// Look For ->
			if (line.length() > 2 && line.substr(0, 2) == "->") {
				lookForCategoryHead = false;
				tvec_lbl = line.substr(2, line.length()-2);
				tmap.insert(
					std::make_pair(
						tvec_lbl,
						std::vector<std::string>()
					)
				);
			}
		}
		else {
			// Get All Extensions
			if (line.length() > 1) {
				// Get Current Category
				auto it = tmap.find(tvec_lbl);

				// Push Extension
				it->second.push_back(line);
			}
			else {
				// If Clear Str is Encountered
				lookForCategoryHead = true;
			}
		}
	}

	// Overwrite Data of Selected Type
	if (CategoryType == "Base") BaseCategories = tmap;
	else if (CategoryType == "Extended") ExtendedCategories = tmap;
}
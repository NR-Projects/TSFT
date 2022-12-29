#include "Config.h"

void Config::help() {
	std::cout
		<< "    Show Config:                        tsft config -s								\n"
		<< "    Edit Config:                        tsft config -e								\n"
		<< std::endl;
}

void Config::load_config()
{
	if (!this->_is_config_exist()) this->_generate_config();

	// Load Config
	std::ifstream r_file;
	std::string f_line;

	// Open Config
	r_file.open(this->config_path);
	while (std::getline(r_file, f_line))
	{
		this->_parse_config_line(f_line);
	}
	r_file.close();

	// Open Category
	r_file.open(this->categories_path);
	std::vector<std::string> tbvec;
	while (std::getline(r_file, f_line))
	{
		tbvec.push_back(f_line);
	}
	this->_parse_categories(tbvec);
	r_file.close();
}

void Config::print_config()
{
	// Show Config
	std::cout
		<< "[Config] \n"
		<< "category_path=" << this->categories_path << "\n"
		<< "print_mode=" << this->print_mode << "\n"
		<< "enable_logs=" << std::boolalpha << this->enable_logs << "\n"
		<< "show_time_to_execute=" << std::boolalpha << this->show_time_to_execute << "\n"
 		<< "\n";

	std::cout << "[Categories] \n";

	for (auto const& x : this->categories)
	{
		std::cout << "->" << x.first << "\n";
		for (std::string cat : x.second) {
			std::cout << cat << "\n";
		}

		std::cout << "\n";
	}

	std::cout << "\n\n";
}

void Config::edit_config()
{
	std::cout << "Opening notepad to edit config and categories... \n";
	system(std::string("notepad " + this->config_path).c_str());
	system(std::string("notepad " + this->categories_path).c_str());
}

void Config::_generate_config()
{
	if (!std::filesystem::exists(this->default_path)) {
		std::filesystem::create_directories(this->default_path);
	}

	if (!std::filesystem::exists(this->config_path)) {
		std::ofstream config_file;
		config_file.open(this->config_path);
		config_file << "* This is a comment \n";
		config_file << "* Use NONE to indicate that no path has been placed \n";
		config_file << "* For missing properties, it will be set as default in the initial generation of this file \n";
		config_file << "category_path=C:\\TunaSalmon\\tsft\\categories.txt" << "\n";
		config_file << "* For print mode, 0 -> Structure, 1 -> Detail \n";
		config_file << "print_mode=1" << "\n";
		config_file << "enable_logs=false" << "\n";
		config_file << "show_time_to_execute=false" << "\n";
		config_file.close();
	}

	if (!std::filesystem::exists(this->categories_path)) {
		std::ofstream categories_file;
		categories_file.open("C:\\TunaSalmon\\tsft\\categories.txt");
		categories_file << "* This is a comment \n";
		categories_file << "* categories are represented by -><category name> \n";
		categories_file << "* Only put extensions without dot(.) \n";
		categories_file << "->img" << "\n";
		categories_file << "png" << "\n";
		categories_file << "jpg" << "\n";
		categories_file << "\n\n";
		categories_file << "->vid" << "\n";
		categories_file << "mp4" << "\n";
		categories_file.close();
	}
}

bool Config::_is_config_exist()
{
	std::ifstream infile(this->config_path);
	return infile.good();
}

void Config::_parse_config_line(std::string _line)
{
	// Ignore Comments
	if (_line.length() > 0)
		if (_line[0] == '*')
			return;

	size_t pos = _line.find("=");
	std::string arg1 = _line.substr(0, pos);
	std::string arg2 = _line.substr(pos + 1, _line.length());

	if (arg2 == "NONE") return;

	// Parse and Load
	else if (arg1 == "category_path") this->categories_path = arg2;
	else if (arg1 == "print_mode") this->print_mode = std::stoi(arg2);
	else if (arg1 == "enable_logs") this->enable_logs = this->_str2bool(arg2);
	else if (arg1 == "show_time_to_execute") this->show_time_to_execute = this->_str2bool(arg2);
}

void Config::_parse_categories(std::vector<std::string> _categories)
{
	std::map<std::string, std::vector<std::string>> tmap;

	bool look_for_category_head = true;

	std::string tvec_lbl;

	for (unsigned int i = 0; i < _categories.size(); i++) {
		std::string line = _categories[i];

		// Ignore Comments
		if (line.length() > 0)
			if (line[0] == '*')
				continue;

		if (look_for_category_head) {
			// Look For ->
			if (line.length() > 2 && line.substr(0, 2) == "->") {
				look_for_category_head = false;
				tvec_lbl = line.substr(2, line.length() - 2);
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
				look_for_category_head = true;
			}
		}
	}

	this->categories = tmap;
}

bool Config::_str2bool(std::string _strbool)
{
	if (_strbool == "true") return true;
	return false;
}

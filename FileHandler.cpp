#include "FileHandler.h"

void FileHandler::help()
{
	std::cout
		<< "    Create New File:                    tsft -a fl <name>							\n"
		<< "    Create New Files:                   tsft -a fl \"<name_1>, <name_2>, ...\"		\n"
		<< "    Create New Folder:                  tsft -a fd <name>							\n"
		<< "    Create New Folders:                 tsft -a fd \"<name_1>, <name_2>, ...\"		\n"
		<< "\n"

		<< "    Remove Existing File:               tsft -r fl <name>							\n"
		<< "    Remove Existing Files:              tsft -r fl \"<name_1>, <name_2>, ...\"		\n"
		<< "    Remove Existing Folder:             tsft -r fd <name>							\n"
		<< "    Remove Existing Folders:            tsft -r fd \"<name_1>, <name_2>, ...\"		\n"
		<< "\n"

		<< "    Show All Files and Folders:         tsft -p [depth]									\n"
		<< "\n"
		<< "        Substring Filters:              tsft -p [depth] -sf <name_1>					\n"
		<< "                                        tsft -p [depth] -sf \"<name_1>, <name_2>, ...\"	\n"
		<< "        Category Filters:               tsft -p [depth] -cf <name_1>					\n"
		<< "                                        tsft -p [depth] -cf \"<name_1>, <name_2>, ...\"	\n"
		<< "        Extension Filters:              tsft -p [depth] -ef <name_1>					\n"
		<< "                                        tsft -p [depth] -ef \"<name_1>, <name_2>, ...\"	\n"
		<< "\n"

		<< "    Clone Path:                         tsft -c [depth] (dest) [src]						\n"
		<< "\n"
		<< "        Clone with Substring																\n"
		<< "        Substring Filters:              tsft -c [depth] -sf \"<name(s), ...>\" (dest) [src]	\n"
		<< "        Category Filters:               tsft -c [depth] -cf \"<name(s), ...>\" (dest) [src]	\n"
		<< "        Extension Filters:              tsft -c [depth] -ef \"<name(s), ...>\" (dest) [src]	\n"

		<< std::endl;
}

FileHandler::FileHandler(Config config)
{
	this->_config = config;
}

void FileHandler::create_fnf(std::vector<std::string> _path_names, fnf_type _type)
{
	switch (_type) {
		case FILE_TYPE:
			for (std::string file : _path_names) {
				std::ofstream c_file;
				c_file.open(file);
				c_file.close();
				std::cout << "File -> " << file << " successfully created" << std::endl;
			}
			break;
		case FOLDER_TYPE:
			for (std::string folder : _path_names) {
				std::filesystem::create_directory(folder);
				std::cout << "Directory -> " << folder << " successfully created" << std::endl;
			}
			break;
	}
}

void FileHandler::show_fnf(std::vector<file_struct> _fnf)
{
	std::string t_lbl;

	switch (this->_config.print_mode) {
		case 0:
			for (file_struct file : _fnf) {
				std::filesystem::directory_entry entry = file._entry;
				
				std::cout << std::string(file._depth, ' ') << "|-" << entry.path().filename().string() << "\n";
			}
			break;
		case 1:
			for (file_struct file : _fnf) {
				std::filesystem::directory_entry entry = file._entry;

				if (entry.is_directory()) t_lbl = "[Directory]";
				else if (entry.is_block_file()) t_lbl = "[Block File]";
				else if (entry.is_character_file()) t_lbl = "[Character File]";
				else if (entry.is_regular_file()) t_lbl = "[File]";

				std::cout << std::string(file._depth, '-') << t_lbl << " -> " << "[" << file._depth << "] " << entry.path().filename().string() << "\n";
			}
			break;
	}
}

void FileHandler::clone_fnf(std::vector<file_struct> _fnf, std::string _dest_path)
{
	std::cout << "Starting Creating Clones For:" << '\n';
	for (file_struct file : _fnf) {
		std::filesystem::path dir(_dest_path);
		std::filesystem::path c_file(file._entry.path().filename().string());
		std::filesystem::path full_path = dir / c_file;

		if (file._entry.is_directory()) continue;

		try {
			std::filesystem::copy_file(file._entry.path(), full_path);
			std::cout
				<< file._entry.path().filename().string()
				<< " at depth of "
				<< file._depth
				<< " is copied successfully"
				<< '\n';
		}
		catch (std::filesystem::filesystem_error& err) {
			std::cout
				<< "Unable to copy " << file._entry.path().filename().string() << " -> "
				<< err.what()
				<< '\n';
		}
	}
}

void FileHandler::remove_fnf(std::vector<std::string> _path_names, fnf_type _type)
{
	switch (_type) {
		case FILE_TYPE:
			for (std::string file : _path_names) {
				if (std::filesystem::remove(file)) std::cout << "File -> " << file << " successfully deleted" << std::endl;
				if (std::filesystem::remove(file)) std::cout << "Unable to delete" << file << std::endl;
			}
			break;
		case FOLDER_TYPE:
			for (std::string folder : _path_names) {
				if (std::filesystem::remove_all(folder)) std::cout << "Directory -> " << folder << " successfully deleted" << std::endl;
				if (std::filesystem::remove_all(folder)) std::cout << "Unable to delete" << folder << std::endl;
			}
			break;
	}
}

std::vector<file_struct> FileHandler::get_all_fnf(std::string _src_path, int _depth)
{
	std::vector<file_struct> fnf_vec;

	if (_src_path == "") _src_path = std::filesystem::current_path().string();

	this->_recursive_fnf_search(fnf_vec, _src_path, 0, _depth + 1);

	return fnf_vec;
}

std::vector<file_struct> FileHandler::get_all_fnf_name_filtered(std::vector<std::string> _filters, std::string _src_path, int _depth)
{
	std::vector<file_struct> fnf_vec = this->get_all_fnf(_src_path, _depth);
	std::vector<file_struct> nf_vec;

	for (file_struct file : fnf_vec) {
		std::string fnf_entry = file._entry.path().filename().string();
		for (std::string filter : _filters) {
			if (fnf_entry.find(filter) != std::string::npos) {
				nf_vec.push_back(file);
			}
		}
	}

	return nf_vec;
}

std::vector<file_struct> FileHandler::get_all_fnf_category_filtered(std::vector<std::string> _filters, std::string _src_path, int _depth)
{
	std::vector<file_struct> fnf_vec = this->get_all_fnf(_src_path, _depth);
	std::vector<file_struct> cf_vec;

	// Get Filters from Category
	std::vector<std::string> all_cat_filters;

	for (std::string cat : _filters) {
		auto itc = this->_config.categories.find(cat);
		if (itc != this->_config.categories.end())
			all_cat_filters.insert(all_cat_filters.end(), itc->second.begin(), itc->second.end());
	}

	// Remove Extension Duplicates
	std::sort(all_cat_filters.begin(), all_cat_filters.end());
	all_cat_filters.erase(std::unique(all_cat_filters.begin(), all_cat_filters.end()), all_cat_filters.end());


	for (file_struct file : fnf_vec) {
		std::string fnf_entry = file._entry.path().filename().extension().string();
		for (std::string catfilter : all_cat_filters) {
			if ("." + catfilter == fnf_entry) {
				cf_vec.push_back(file);
				break;
			}
		}
	}

	return cf_vec;
}

std::vector<file_struct> FileHandler::get_all_fnf_extension_filtered(std::vector<std::string> _filters, std::string _src_path, int _depth)
{
	std::vector<file_struct> fnf_vec = this->get_all_fnf(_src_path, _depth);
	std::vector<file_struct> ef_vec;

	for (file_struct file : fnf_vec) {
		std::string ext = file._entry.path().filename().extension().string();
		for (std::string extfilter : _filters) {
			if ("." + extfilter == ext) {
				ef_vec.push_back(file);
			}
		}
	}

	return ef_vec;
}

void FileHandler::_recursive_fnf_search(std::vector<file_struct>& _path_list, std::string _dir_path, int _curr_depth, int _max_depth)
{
	if (_curr_depth >= _max_depth && _max_depth != -1) return;

	for (const auto& entry : std::filesystem::directory_iterator(_dir_path)) {
		if (entry.is_directory()) {
			_path_list.push_back(file_struct(entry, _curr_depth));
			_recursive_fnf_search(_path_list, entry.path().string(), _curr_depth + 1, _max_depth);
		}
		else {
			_path_list.push_back(file_struct(entry, _curr_depth));
		}
	}
}

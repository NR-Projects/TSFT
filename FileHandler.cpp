#include "FileHandler.h"

void FileHandler::PrintHelp()
{
	std::cout
		<< "    Create New File:                    tsft +fl <name>							\n"
		<< "    Create New Files:                   tsft +fl \"<name_1>, <name_2>, ...\"	\n"
		<< "\n\n"
		<< "    Create New Folder:                  tsft +fd <name>							\n"
		<< "    Create New Folders:                 tsft +fd \"<name_1>, <name_2>, ...\"	\n"
		<< "\n\n"
		<< "    Show Stuffs:                        tsft -p <depth:opt>		                \n"
		<< "        Show All with Filter (included optional depth after -p)								\n"
		<< "\n"
		<< "        Substring Filters:              tsft -p <depth:opt> -sf <name_1>					\n"
		<< "                                        tsft -p <depth:opt> -sf \"<name_1>, <name_2>, ...\"	\n"
		<< "        Category Filters:               tsft -p <depth:opt> -cf <name_1>					\n"
		<< "                                        tsft -p <depth:opt> -cf \"<name_1>, <name_2>, ...\"	\n"
		<< "        Extension Filters:              tsft -p <depth:opt> -ef <name_1>					\n"
		<< "                                        tsft -p <depth:opt> -ef \"<name_1>, <name_2>, ...\"	\n"
		<< "\n\n"
		<< "    Clone Path:                         tsft -c <depth:opt> <dest> <src:opt>				\n"
		<< "        without passing depth explicitly, depth will be set to 0							\n"
		<< "\n"
		<< "        Clone with Substring													            \n"
		<< "        Substring Filters:              tsft -c <depth> -sf \"<name(s), ...>\" <dest> <src:opt>	\n"
		<< "        Category Filters:               tsft -c <depth> -cf \"<name(s), ...>\" <dest> <src:opt>	\n"
		<< "        Extension Filters:              tsft -c <depth> -ef \"<name(s), ...>\" <dest> <src:opt>	\n"
		<< "\n\n"
		<< "    Remove Existing File:               tsft -fl <name>							\n"
		<< "    Remove Existing Files:              tsft -fl \"<name_1>, <name_2>, ...\"	\n"
		<< "\n\n"
		<< "    Remove Existing Folder:             tsft -fd <name>							\n"
		<< "    Remove Existing Folders:            tsft -fd \"<name_1>, <name_2>, ...\"	\n"
		<< std::endl;
}

FileHandler::FileHandler(Config config)
{
	this->_config = config;
}

std::vector<FileStruct> FileHandler::GetAllFile(std::string srcPath, int depth)
{
	std::vector<FileStruct> FnF;

	if (srcPath == "") srcPath = std::filesystem::current_path().string();

	_RecursiveFileSearch(FnF, srcPath, 0, depth+1);

	return FnF;
}

std::vector<FileStruct> FileHandler::GetAllFNFNameFiltered(std::vector<std::string> Filters, std::string srcPath, int depth)
{
	std::vector<FileStruct> file_vec = GetAllFile(srcPath, depth);
	std::vector<FileStruct> ret_vec;

	for (FileStruct file : file_vec) {
		std::string en = file._entry.path().filename().string();
		for (std::string filter : Filters) {
			if (en.find(filter) != std::string::npos) {
				ret_vec.push_back(file);
			}
		}
	}

	return ret_vec;
}

std::vector<FileStruct> FileHandler::GetAllFnFCategoryFiltered(std::vector<std::string> Filters, std::string srcPath, int depth)
{
	std::vector<FileStruct> file_vec = GetAllFile(srcPath, depth);
	std::vector<FileStruct> ret_vec;

	// Get Filters from Category
	std::vector<std::string> all_ext_filters;

	for (std::string cat : Filters) {
		auto itb = this->_config.BaseCategories.find(cat);
		if (itb != this->_config.BaseCategories.end())
			all_ext_filters.insert(all_ext_filters.end(), itb->second.begin(), itb->second.end());

		auto ite = this->_config.ExtendedCategories.find(cat);
		if (ite != this->_config.ExtendedCategories.end())
			all_ext_filters.insert(all_ext_filters.end(), ite->second.begin(), ite->second.end());
	}

	// Remove Extension Duplicates
	std::sort(all_ext_filters.begin(), all_ext_filters.end());
	all_ext_filters.erase(std::unique(all_ext_filters.begin(), all_ext_filters.end()), all_ext_filters.end());


	for (FileStruct file : file_vec) {
		std::string ext = file._entry.path().filename().extension().string();

		for (std::string catfilter : all_ext_filters) {
			if ("." + catfilter == ext) {
				ret_vec.push_back(file);
				break;
			}
		}
	}

	return ret_vec;
}

std::vector<FileStruct> FileHandler::GetAllFnFExtensionFiltered(std::vector<std::string> Filters, std::string srcPath, int depth)
{
	std::vector<FileStruct> file_vec = GetAllFile(srcPath, depth);
	std::vector<FileStruct> ret_vec;

	for (FileStruct file : file_vec) {
		std::string ext = file._entry.path().filename().extension().string();
		for (std::string extfilter : Filters) {
			if ("." + extfilter == ext) {
				ret_vec.push_back(file);
			}
		}
	}

	return ret_vec;
}

void FileHandler::CreateFiles(std::vector<std::string> files)
{
	for (std::string file : files) {
		std::ofstream c_file;
		c_file.open(file);
		c_file.close();
	}
}

void FileHandler::CreateFolders(std::vector<std::string> folders)
{
	for (std::string folder : folders) {
		std::filesystem::create_directory(folder);
	}
}

void FileHandler::ShowFilesAndFolders(std::vector<FileStruct> FnF)
{
	std::string t_lbl;

	for (FileStruct file : FnF) {
		std::filesystem::directory_entry entry = file._entry;

		if (entry.is_directory()) t_lbl = "[Directory]";
		else if (entry.is_block_file()) t_lbl = "[Block File]";
		else if (entry.is_character_file()) t_lbl = "[Character File]";
		else if (entry.is_regular_file()) t_lbl = "[File]";

		std::cout << std::string(file._depth, '-') << t_lbl << " -> " << "[" << file._depth << "] " << entry.path().filename().string() << "\n";
	}
}

void FileHandler::CloneFiles(std::vector<FileStruct> FnF, std::string destPath)
{
	std::cout << "Starting Creating Clones For:" << '\n';
	for (FileStruct file : FnF) {
		std::filesystem::path dir(destPath);
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

void FileHandler::RemoveFiles(std::vector<std::string> files)
{
	for (std::string file : files) {
		if (std::filesystem::remove(file)) std::cout << "File -> " << file << " successfully deleted" << std::endl;
		if (std::filesystem::remove(file)) std::cout << "Unable to delete" << file << std::endl;
	}
}

void FileHandler::RemoveFolders(std::vector<std::string> folders)
{
	for (std::string folder : folders) {
		if (std::filesystem::remove_all(folder)) std::cout << "Directory -> " << folder << " successfully deleted" << std::endl;
		if (std::filesystem::remove_all(folder)) std::cout << "Unable to delete" << folder << std::endl;
	}
}

void FileHandler::_RecursiveFileSearch(std::vector<FileStruct>& path_list, std::string dir_path, int curr_depth, int max_depth)
{
	if (curr_depth >= max_depth && max_depth != -1) return;

	for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
		if (entry.is_directory()) {
			path_list.push_back(FileStruct(entry, curr_depth));
			_RecursiveFileSearch(path_list, entry.path().string(), curr_depth + 1, max_depth);
		}
		else {
			path_list.push_back(FileStruct(entry, curr_depth));
		}
	}
}
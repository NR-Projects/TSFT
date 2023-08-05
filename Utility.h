#pragma once

#include "libs.h"


namespace utility {
	std::string trim_spaces(std::string str);
	std::vector<std::string> get_arg_list(std::string arg_list);
	bool is_arg_eq(const char* arg1, const char* arg2);
};
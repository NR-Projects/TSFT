#include "Utility.h"

std::string utility::trim_spaces(std::string str) {
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
    return str;
}

std::vector<std::string> utility::get_arg_list(std::string arg_list) {
    // Check If List Or Not
    std::string args = arg_list;
    std::vector<std::string> args_as_list;

    size_t checkForList = args.find(',');

    if (checkForList == std::string::npos) {
        args_as_list.push_back(trim_spaces(args));
    }
    else {
        size_t pos = 0;
        while ((pos = args.find(',')) != std::string::npos) {
            args_as_list.push_back(trim_spaces(args.substr(0, pos)));
            args.erase(0, pos + 1);
        }
        args_as_list.push_back(trim_spaces(args));
    }

    return args_as_list;
}

bool utility::is_arg_eq(const char* arg1, const char* arg2) {
    return (strcmp(arg1, arg2) == 0);
}
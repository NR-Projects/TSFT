// tsft.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstdio>
#include <ctime>
#include <iostream>

#include "Config.h"
#include "FileHandler.h"


void print_help() {

    std::cout << "This is all the available commands for tsft.exe file helper"  << "\n"
              << "Default Values:"                                              << "\n"
              << "                  \"Dest\" = current directory"               << "\n"
              << "                  \"Depth\" = 0"                              << "\n";

    Config::help();
    FileHandler::help();
}

std::string trim_spaces(std::string str) {
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
    return str;
}

std::vector<std::string> get_arg_list(std::string arg_list) {
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

bool is_arg_eq(const char* arg1, const char* arg2) {
    return (strcmp(arg1, arg2) == 0);
}


int main(int argc, char *argv[])
{
    // Time Start
    auto start_time = std::chrono::steady_clock::now();

    // main vars
    bool has_executed_command = false;

    // Load Config
    Config config;
    config.load_config();

    // Initialize FileHandler
    FileHandler filehandler(config);

#pragma region Standard Options
    // Check for no arguments passed
    if (argc == 1) {
        std::cout << "Output may be affected depending on priveleges" << "\n";
        std::cout << "Type --version to see version info" << "\n";
        std::cout << "Type --help to see all commands"    << "\n";
        has_executed_command = true;
    }

    // Check for "--version" or "--help" requests
    else if (argc == 2) {
        if (is_arg_eq(argv[1], "--version")) {
            std::cout << "v1.0.0" << "\n";
            has_executed_command = true;
        }
        else if (is_arg_eq(argv[1], "--help")) {
            print_help();
            has_executed_command = true;
        }
    }
#pragma endregion

    if (argc >= 2) {
        // Config Actions
        if (is_arg_eq(argv[1], "config")) {
            if (is_arg_eq(argv[2], "-s")) {
                config.print_config();
                has_executed_command = true;
            }

            else if (is_arg_eq(argv[2], "-e")) {
                config.edit_config();
                has_executed_command = true;
            }
        }

        // File Actions
#pragma region Adding FnF
        else if (is_arg_eq(argv[1], "-a")) {
            // Adding FnF
            if (is_arg_eq(argv[2], "fl")) {
                filehandler.create_fnf(
                    get_arg_list(argv[3]),
                    FileHandler::FILE_TYPE
                );
                has_executed_command = true;
            }
            else if (is_arg_eq(argv[2], "fd")) {
                filehandler.create_fnf(
                    get_arg_list(argv[3]),
                    FileHandler::FOLDER_TYPE
                );
                has_executed_command = true;
            }
        }
#pragma endregion

#pragma region Removing FnF
        else if (argv[1] == "-r") {
            // Removing FnF
            if (is_arg_eq(argv[2], "fl")) {
                filehandler.remove_fnf(
                    get_arg_list(argv[3]),
                    FileHandler::FILE_TYPE
                );
                has_executed_command = true;
            }
            else if (is_arg_eq(argv[2], "fl")) {
                filehandler.remove_fnf(
                    get_arg_list(argv[3]),
                    FileHandler::FOLDER_TYPE
                );
                has_executed_command = true;
            }
        }
#pragma endregion

#pragma region Printing FnF
        else if (is_arg_eq(argv[1], "-p")) {
            // Printing FnF
            std::cout << argc;
            if (argc == 2) {
                filehandler.show_fnf(
                    filehandler.get_all_fnf()
                );
                has_executed_command = true;
            }

            else if (argc == 3) {
                filehandler.show_fnf(
                    filehandler.get_all_fnf("", std::stoi(argv[2]))
                );
                has_executed_command = true;
            }

            else if (argc == 4) {
                if (is_arg_eq(argv[2], "-sf")) {
                    filehandler.show_fnf(
                        filehandler.get_all_fnf_name_filtered(get_arg_list(argv[3]))
                    );
                    has_executed_command = true;
                }
                else if (is_arg_eq(argv[2], "-cf")) {
                    filehandler.show_fnf(
                        filehandler.get_all_fnf_category_filtered(get_arg_list(argv[3]))
                    );
                    has_executed_command = true;
                }
                else if (is_arg_eq(argv[2], "-ef")) {
                    filehandler.show_fnf(
                        filehandler.get_all_fnf_extension_filtered(get_arg_list(argv[3]))
                    );
                    has_executed_command = true;
                }
            }

            else if (argc == 5) {
                if (is_arg_eq(argv[3], "-sf")) {
                    std::vector<std::string> substrfilters = get_arg_list(argv[4]);
                    filehandler.show_fnf(
                        filehandler.get_all_fnf_name_filtered(
                            substrfilters,
                            "",
                            std::stoi(argv[2])
                        )
                    );
                    has_executed_command = true;
                }
                else if (is_arg_eq(argv[3], "-cf")) {
                    std::vector<std::string> substrfilters = get_arg_list(argv[4]);
                    filehandler.show_fnf(
                        filehandler.get_all_fnf_category_filtered(
                            substrfilters,
                            "",
                            std::stoi(argv[2])
                        )
                    );
                    has_executed_command = true;
                }
                else if (is_arg_eq(argv[3], "-ef")) {
                    std::vector<std::string> substrfilters = get_arg_list(argv[4]);
                    filehandler.show_fnf(
                        filehandler.get_all_fnf_extension_filtered(
                            substrfilters,
                            "",
                            std::stoi(argv[2])
                        )
                    );
                    has_executed_command = true;
                }
            }
        }
#pragma endregion

#pragma region Cloning FnF
        else if (is_arg_eq(argv[1], "-c")) {
            if (argc == 3) {
                filehandler.clone_fnf(
                    filehandler.get_all_fnf(
                        "",
                        0
                    ),
                    argv[2]
                );
                has_executed_command = true;
            }
            else if (argc == 4) {
                filehandler.clone_fnf(
                    filehandler.get_all_fnf(
                        "",
                        std::stoi(argv[2])
                    ),
                    argv[3]
                );
                has_executed_command = true;
            }
            else if (argc == 5) {
                if (is_arg_eq(argv[3], "-sf")) {
                    filehandler.clone_fnf(
                        filehandler.get_all_fnf_name_filtered(
                            get_arg_list(argv[4]),
                            "",
                            std::stoi(argv[2])
                        ),
                        argv[5]
                    );
                    has_executed_command = true;
                }
                else if (is_arg_eq(argv[3], "-cf")) {
                    filehandler.clone_fnf(
                        filehandler.get_all_fnf_category_filtered(
                            get_arg_list(argv[4]),
                            "",
                            std::stoi(argv[2])
                        ),
                        argv[5]
                    );
                    has_executed_command = true;
                }
                else if (is_arg_eq(argv[3], "-ef")) {
                    filehandler.clone_fnf(
                        filehandler.get_all_fnf_extension_filtered(
                            get_arg_list(argv[4]),
                            "",
                            std::stoi(argv[2])
                        ),
                        argv[5]
                    );
                    has_executed_command = true;
                }
            }
            else if (argc == 6) {
                if (is_arg_eq(argv[3], "-sf")) {
                    filehandler.clone_fnf(
                        filehandler.get_all_fnf_name_filtered(
                            get_arg_list(argv[4]),
                            argv[6],
                            std::stoi(argv[2])
                        ),
                        argv[5]
                    );
                    has_executed_command = true;
                }
                if (is_arg_eq(argv[3], "-cf")) {
                    filehandler.clone_fnf(
                        filehandler.get_all_fnf_category_filtered(
                            get_arg_list(argv[4]),
                            argv[6],
                            std::stoi(argv[2])
                        ),
                        argv[5]
                    );
                    has_executed_command = true;
                }
                if (is_arg_eq(argv[3], "-ef")) {
                    filehandler.clone_fnf(
                        filehandler.get_all_fnf_extension_filtered(
                            get_arg_list(argv[4]),
                            argv[6],
                            std::stoi(argv[2])
                        ),
                        argv[5]
                    );
                    has_executed_command = true;
                }
            }
        }
#pragma endregion

    }

    if (!has_executed_command) {
        std::cout << "Command Not Found! Try tsft --help to see all commands" << "\n";
    }

    // Time Start
    auto end_time = std::chrono::steady_clock::now();
    
    if (config.show_time_to_execute) {
        std::cout << std::chrono::duration <double, std::milli>((end_time - start_time)).count() << " ms" << std::endl;
    }

    if (config.enable_logs) {
        // Get Time
        struct tm ltm;
        time_t now = time(0);
        localtime_s(&ltm, &now);

        char log_time[100] = {};
        sprintf_s(log_time, "[%d-%d-%d %d:%d (%d)]",
            1900 + ltm.tm_year,
            1 + ltm.tm_mon,
            ltm.tm_mday,
            5 + ltm.tm_hour,
            30 + ltm.tm_min,
            ltm.tm_sec);

        // Get Command
        std::string command = "";
        for (int i = 1; i < argc; i++) {
            command.append(
                std::string(argv[i])
            );

            if (i + 1 != argc) command.append(" ");
        }
            

        std::ofstream log_file;
        log_file.open("C:\\TunaSalmon\\tsft\\logs.txt", std::ios::app);
        log_file << log_time << "->" << command << "\n";
        log_file.close();
    }

    return 0;
}


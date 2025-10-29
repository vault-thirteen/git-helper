#include "main.hpp"
#include <iostream>
#include "actions.hpp"
#include "windows.h"

errno_t main(int argc, char** argv)
{
    if (argc != (APP_ARGS_COUNT + 1))
    {
        std::cout << "Usage: <executable> <folder> <action>" << std::endl;
        return APP_EXIT_CODE_ARGS_COUNT_IS_BAD;
    }

    errno_t err = enable_console_colours();
    if (err != 0)
    {
        return err;
    }

    //char *exe = argv[0];
    char* inf = argv[1]; // Input folder.
    std::string action = argv[2];

    if (action == "tags")
    {
        list_tags(inf);
    }
    else if (action == "versions")
    {
        list_versions(inf);
    }
    else if (action == "lver")
    {
        show_latest_version(inf);
    }
    else if (action == "newpatch")
    {
        add_new_patch_tag(inf);
    }
    else
    {
        std::cout << "Unknown action: " << action << std::endl;
        return APP_EXIT_CODE_UNKNOWN_ACTION;
    }

    return APP_EXIT_CODE_ALL_CLEAR;
}

errno_t enable_console_colours()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error getting console handle" << std::endl;
        return APP_EXIT_CODE_STDOUT_HANDLE;
    }

    DWORD dwMode;
    if (!GetConsoleMode(hConsole, &dwMode))
    {
        std::cerr << "Error getting console mode" << std::endl;
        return APP_EXIT_CODE_CONSOLE_MODE;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hConsole, dwMode))
    {
        std::cerr << "Error setting console mode" << std::endl;
        return APP_EXIT_CODE_CONSOLE_MODE;
    }

    return 0;
}

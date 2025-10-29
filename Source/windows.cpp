#include <iostream>

// Windows API.
#include <windows.h>

#include "os.hpp"

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

#ifndef GIT1_MAIN_H
#define GIT1_MAIN_H

#include <cstdlib>

#define APP_EXIT_CODE_ALL_CLEAR 0
#define APP_EXIT_CODE_STDOUT_HANDLE 1
#define APP_EXIT_CODE_CONSOLE_MODE 2
#define APP_EXIT_CODE_ARGS_COUNT_IS_BAD 3
#define APP_EXIT_CODE_UNKNOWN_ACTION 4
#define APP_EXIT_CODE_ACTION_ERROR 5

#define APP_ARGS_COUNT 2

/*
 * Program's entry point.
 *
 * @param argc          number of command line arguments
 * @param argv          textual values of command line arguments
 *
 * @return              positive error code in case of failure, otherwise 0.
 */
errno_t main(int argc, char** argv);

errno_t enable_console_colours();

#endif //GIT1_MAIN_H

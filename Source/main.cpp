#include <iostream>
#include <string>

#include "actions.hpp"
#include "os.hpp"
#include "windows.hpp"

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
        action_list_tags(inf);
    }
    else if (action == "versions")
    {
        action_list_versions(inf);
    }
    else if (action == "lver")
    {
        action_show_latest_version(inf);
    }
    else if (action == "newpatch")
    {
        action_add_new_patch_tag(inf);
    }
    else
    {
        std::cout << "Unknown action: " << action << std::endl;
        return APP_EXIT_CODE_UNKNOWN_ACTION;
    }

    return APP_EXIT_CODE_ALL_CLEAR;
}

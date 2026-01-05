#include <iostream>
#include <string>

#include "actions.hpp"
#include "interaction.hpp"
#include "os.hpp"
#include "windows.hpp"

errno_t main(int argc, char** argv)
{
	if (argc < (APP_ARGS_MIN_COUNT + 1))
	{
		show_usage_A();
		return APP_EXIT_CODE_ARGS_COUNT_IS_BAD;
	}

	errno_t err = enable_console_colours();
	if (err != 0)
	{
		return err;
	}

	//char *exe = argv[0];
	char* inf = argv[1]; // Input folder.
	std::string action = argv[2]; // Action name.

	if (action == ACTION_LIST_TAGS)
	{
		action_list_tags(inf);
	}
	else if (action == ACTION_LIST_VERSIONS)
	{
		action_list_versions(inf);
	}
	else if (action == ACTION_SHOW_LATEST_VERSION)
	{
		action_show_latest_version(inf);
	}
	else if (action == ACTION_ADD_NEW_PATCH_TAG)
	{
		action_add_new_patch_tag(inf);
	}
	else if (action == ACTION_CLONE_REPOSITORY)
	{
		if (argc < APP_ARGS_MAX_COUNT + 1)
		{
			show_usage_B();
			return APP_EXIT_CODE_ARGS_COUNT_IS_BAD;
		}

		std::string repositoryUrl = argv[3]; // Repository URL.
		action_clone_repository(inf, repositoryUrl);
	}
	else
	{
		std::cout << "Unknown action: " << action << std::endl;
		return APP_EXIT_CODE_UNKNOWN_ACTION;
	}

	return APP_EXIT_CODE_ALL_CLEAR;
}

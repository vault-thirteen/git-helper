#include <iostream>
#include <string>
#include <vector>

#include "git.hpp"
#include "std.hpp"
#include "interaction.hpp"
#include "libgit2/git2.h"
#include "classes/Version.h"

void action_list_tags(const char* infp)
{
	print_repository_folder(infp);
	init();

	git_repository* repo = nullptr;
	int error = git_repository_open(&repo, infp);
	if (error < 0)
	{
		show_last_git_error();
		fin();
		throw std::runtime_error(std::string("git_repository_open"));
	}

	std::vector<std::string> tags = read_tags(repo);

	// Print tag names and statistics.
	std::cout << "Tag names found:" << std::endl;
	for (const std::string& s : tags)
	{
		std::cout << s << std::endl;
	}
	print_count("tag", tags.size());

	git_repository_free(repo);
	fin();
}
void action_list_versions(const char* infp)
{
	print_repository_folder(infp);
	init();

	git_repository* repo = nullptr;
	int error = git_repository_open(&repo, infp);
	if (error < 0)
	{
		show_last_git_error();
		fin();
		throw std::runtime_error(std::string("git_repository_open"));
	}

	std::vector<Version> versions = read_versions(repo, 0);

	// Print versions and statistics.
	std::cout << "Versions found:" << std::endl;
	for (const Version& v : versions)
	{
		std::cout << v << std::endl;
	}
	print_count("version", versions.size());

	git_repository_free(repo);
	fin();
}
void action_show_latest_version(const char* infp)
{
	// In this action we only print the latest version.
	init();

	git_repository* repo = nullptr;
	int error = git_repository_open(&repo, infp);
	if (error < 0)
	{
		show_last_git_error();
		fin();
		throw std::runtime_error(std::string("git_repository_open"));
	}

	std::vector<Version> versions = read_versions(repo, 2);
	if (versions.empty())
	{
		std::cout << "No versions found" << std::endl;
	}
	else
	{
		Version latestVersion = versions[0];
		std::cout << latestVersion << std::endl;
	}

	git_repository_free(repo);
	fin();
}
void action_add_new_patch_tag(const char* infp)
{
	print_repository_folder(infp);
	init();

	git_repository* repo = nullptr;
	int error = git_repository_open(&repo, infp);
	if (error < 0)
	{
		show_last_git_error();
		fin();
		throw std::runtime_error(std::string("git_repository_open"));
	}

	std::vector<Version> versions = read_versions(repo, 2);
	if (versions.empty())
	{
		git_repository_free(repo);
		fin();
		std::cout << "No versions found" << std::endl;
		return;
	}

	Version latestVersion = versions[0];
	Version newVersion = latestVersion;
	newVersion.incPatch();

	std::cout << "Latest version:\t" << "\033[38;5;46m" << latestVersion << "\033[0m" << std::endl;
	std::cout << "New version:\t" << "\033[38;5;196m" << newVersion << "\033[0m" << std::endl;
	bool proceed = ask_to_proceed();
	if (!proceed)
	{
		std::cout << "Operation has been cancelled.";
		git_repository_free(repo);
		fin();
		return;
	}

	git_commit* latest_commit = get_latest_commit(repo);
	if (latest_commit == nullptr)
	{
		git_repository_free(repo);
		fin();
		throw std::runtime_error(std::string("get_latest_commit"));
	}

	const git_signature* author = git_commit_author(latest_commit);
	if (author == nullptr)
	{
		git_commit_free(latest_commit);
		git_repository_free(repo);
		fin();
		throw std::runtime_error(std::string("git_commit_author"));
	}

	std::cout << std::endl;
	std::cout << "The following commit will be tagged with new patch version:" << std::endl;
	std::cout << "Author: " << author->name << " (" << author->email << ")" << std::endl;
	std::cout << "Date/Time: " << prettyUnixTime(author->when.time, author->when.offset) << std::endl;
	std::cout << "Message: \033[38;5;214m" << git_commit_message(latest_commit) << "\033[0m" << std::endl;
	proceed = ask_to_proceed();
	if (!proceed)
	{
		git_commit_free(latest_commit);
		git_repository_free(repo);
		fin();
		std::cout << "Operation has been cancelled.";
		return;
	}

	const git_oid* commit_oid = git_commit_id(latest_commit);
	git_object* target_obj = nullptr;
	error = git_object_lookup(&target_obj, repo, commit_oid, GIT_OBJ_COMMIT);
	if (error != 0)
	{
		show_last_git_error();
		git_commit_free(latest_commit);
		git_repository_free(repo);
		fin();
		throw std::runtime_error(std::string("git_object_lookup"));
	}

	error = add_tag(repo, newVersion, target_obj);
	if (error != 0)
	{
		show_last_git_error();
		git_object_free(target_obj);
		git_commit_free(latest_commit);
		git_repository_free(repo);
		fin();
		throw std::runtime_error(std::string("add_tag"));
	}

	git_object_free(target_obj);
	git_commit_free(latest_commit);
	git_repository_free(repo);
	fin();
}
void action_clone_repository(const char* infp, std::string repositoryUrl)
{
	print_repository_folder(infp);
	init();

	// Show source and destination.
	std::cout << "Cloning a repository: \r\n" <<
		"\033[38;5;220m" << repositoryUrl << "\033[0m" << std::endl << std::endl;

	git_repository* repo = nullptr;
	const git_clone_options* options = nullptr;
	int error = git_clone(&repo, repositoryUrl.c_str(), infp, options);
	if (error < 0)
	{
		show_last_git_error();
		fin();
		throw std::runtime_error(std::string("git_clone"));
	}

	git_repository_free(repo);
	fin();
}

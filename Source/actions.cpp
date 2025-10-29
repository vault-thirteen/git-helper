#include "actions.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "std.hpp"
#include "libgit2/git2.h"
#include "classes/Version.h"

void list_tags(const char* infp)
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

std::vector<std::string> read_tags(git_repository* repo)
{
    std::vector<std::string> tags;
    git_strarray tag_names;
    int error = git_tag_list(&tag_names, repo);
    // Or to get tags matching a pattern (e.g., "v1.*"):
    // error = git_tag_list_match(&tag_names, "v1.*", repo);
    if (error < 0)
    {
        show_last_git_error();
        throw std::runtime_error(std::string("git_tag_list"));
    }

    // Read tag names from C library into normal C++ array.
    tags.reserve(tag_names.count);

    for (int i = 0; i < tag_names.count; i++)
    {
        std::string s = tag_names.strings[i];
        tags.push_back(s);
    }

    git_strarray_free(&tag_names);

    return tags;
}

void list_versions(const char* infp)
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

std::vector<Version> read_versions(git_repository* repo, char sort_type)
{
    std::vector<Version> versions;
    git_strarray tag_names;
    int error = git_tag_list(&tag_names, repo);
    // Or to get tags matching a pattern (e.g., "v1.*"):
    // error = git_tag_list_match(&tag_names, "v1.*", repo);
    if (error < 0)
    {
        show_last_git_error();
        throw std::runtime_error(std::string("git_tag_list"));
    }

    // Read tag names from C library into normal C++ array.
    versions.reserve(tag_names.count);

    for (int i = 0; i < tag_names.count; i++)
    {
        std::string s = tag_names.strings[i];
        if (Version::isAVersion(s))
            versions.emplace_back(s);
    }

    git_strarray_free(&tag_names);

    // Optional sorting.
    if (sort_type != 0)
    {
        switch (sort_type)
        {
        case 1:
            // Sort versions in ascending order.
            std::ranges::sort(versions, std::less<>());
            break;

        case 2:
            // Sort versions in descending order.
            std::ranges::sort(versions, std::greater<>());
            break;

        default:
            throw std::invalid_argument(std::string("Unknown sort order: ") + sort_type);
        }
    }

    return versions;
}

void show_latest_version(const char* infp)
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

void add_new_patch_tag(const char* infp)
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
    std::cout << "Date/Time: " << prettyTimeFromUnix(author->when.time, author->when.offset) << std::endl;
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

git_commit* get_latest_commit(git_repository* repo)
{
    git_oid head_oid;
    int error = git_reference_name_to_id(&head_oid, repo, "HEAD");
    if (error < 0)
    {
        std::cout << "Error getting HEAD OID: " << error << std::endl;
        return nullptr;
    }

    git_commit* latest_commit = nullptr;
    error = git_commit_lookup(&latest_commit, repo, &head_oid);
    if (error < 0)
    {
        std::cout << "Error looking up the commit by OID: " << error << std::endl;
        return nullptr;
    }

    return latest_commit;
}

errno_t add_tag(git_repository* repo, Version newVersion, const git_object* latestCommit)
{
    git_oid tag_oid;

    int error = git_tag_create_lightweight(
        &tag_oid, // OID of the new tag object (same as target_obj)
        repo, // Repository
        newVersion.toTagName().c_str(), // Tag name
        latestCommit, // Tagged object (commit)
        0 // Force overwrite (0 = no, 1 = yes)
    );
    if (error != 0)
    {
        const git_error* err = giterr_last();
        std::cerr << "Error creating lightweight tag: " << err->message << std::endl;
        return error;
    }

    return SUCCESS;
}

void init()
{
    git_libgit2_init();
}

void fin()
{
    git_libgit2_shutdown();
}

void print_repository_folder(const char* infp)
{
    std::cout << "Repository folder:" << std::endl;
    std::cout << "\033[38;5;220m" << infp << "\033[0m" << std::endl << std::endl;
}

void print_count(const char* object_name, int count)
{
    std::cout << std::endl << "Total " << object_name << " count: ";
    std::cout << "\033[38;5;46m" << count << "\033[0m" << std::endl;
}

bool ask_to_proceed()
{
    std::cout << "Would you like to proceed ? (yes / no): ";

    std::string userAnswer;
    std::cin >> userAnswer;
    userAnswer = ToLower(userAnswer);
    if (userAnswer == "yes")
    {
        return true;
    }

    return false;
}

void show_last_git_error()
{
    const git_error* err = giterr_last();
    std::cerr << "Git error: " << err->message << std::endl;
}

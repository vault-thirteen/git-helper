#include <algorithm>
#include <cerrno>
#include <iostream>
#include <string>
#include <vector>

#include "std.hpp"
#include "classes/Version.h"
#include "libgit2/git2.h"

void init()
{
    git_libgit2_init();
}
void fin()
{
    git_libgit2_shutdown();
}

void show_last_git_error()
{
    const git_error* err = giterr_last();
    std::cerr << "Git error: " << err->message << std::endl;
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

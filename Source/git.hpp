#ifndef GH_GIT_H
#define GH_GIT_H

#include <string>
#include <vector>

#include "classes/Version.h"
#include "libgit2/git2.h"

void init();
void fin();

void show_last_git_error();

std::vector<std::string> read_tags(git_repository* repo);
std::vector<Version> read_versions(git_repository* repo, char sort_type);
git_commit* get_latest_commit(git_repository* repo);
errno_t add_tag(git_repository* repo, Version newVersion, const git_object* latestCommit);

#endif //GH_GIT_H

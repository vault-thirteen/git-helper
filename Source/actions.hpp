#ifndef INC_GIT1_TEST_H
#define INC_GIT1_TEST_H

#include <vector>
#include "libgit2/git2.h"
#include "classes/Version.h"

#define ERROR_CODE_REPOSITORY_OPEN_ERROR 1
#define ERROR_CODE_REPOSITORY_LIST_TAGS_ERROR 2
#define ERROR_CODE_REPOSITORY_LIST_VERSIONS_ERROR 3
#define ERROR_CODE_REPOSITORY_VERSION_IS_NOT_FOUND 4
#define ERROR_CODE_REPOSITORY_GET_COMMIT 5
#define ERROR_CODE_REPOSITORY_ADD_TAG 6

void list_tags(const char* infp);
std::vector<std::string> read_tags(git_repository* repo);
void list_versions(const char* infp);
std::vector<Version> read_versions(git_repository* repo, char sort_type);
void show_latest_version(const char* infp);
void add_new_patch_tag(const char* infp);
git_commit* get_latest_commit(git_repository* repo);
errno_t add_tag(git_repository* repo, Version newVersion, const git_object* latestCommit);

void init();
void fin();

void print_repository_folder(const char* infp);
void print_count(const char* object_name, int count);
bool ask_to_proceed();
void show_last_git_error();

#endif //INC_GIT1_TEST_H

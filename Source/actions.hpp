#ifndef GH_ACTIONS_H
#define GH_ACTIONS_H

#define ERROR_CODE_REPOSITORY_OPEN_ERROR 1
#define ERROR_CODE_REPOSITORY_LIST_TAGS_ERROR 2
#define ERROR_CODE_REPOSITORY_LIST_VERSIONS_ERROR 3
#define ERROR_CODE_REPOSITORY_VERSION_IS_NOT_FOUND 4
#define ERROR_CODE_REPOSITORY_GET_COMMIT 5
#define ERROR_CODE_REPOSITORY_ADD_TAG 6

void action_list_tags(const char* infp);
void action_list_versions(const char* infp);
void action_show_latest_version(const char* infp);
void action_add_new_patch_tag(const char* infp);

#endif //GH_ACTIONS_H

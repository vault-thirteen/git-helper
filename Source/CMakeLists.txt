cmake_minimum_required(VERSION 4.0)
project(git_helper VERSION 0.1.1 DESCRIPTION "Git Helper" LANGUAGES CXX)
set_target_properties(PROPERTIES VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} meta/version.rc
        actions.cpp
        actions.hpp
        git.hpp
        git.cpp
        interaction.cpp
        interaction.hpp
        main.cpp
        main.hpp
        os.hpp
        std.cpp
        std.hpp
        windows.cpp
        windows.hpp
        classes/Version.h
)

add_library(libgit2 SHARED IMPORTED)
set_target_properties(libgit2 PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/libgit2/lib/git2.lib")
set_target_properties(libgit2 PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libgit2/bin/git2.dll")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libgit2/include")

target_link_libraries(${PROJECT_NAME} libgit2)

# Debugging.
get_property(IncludeFolders TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "IncludeFolders='${IncludeFolders}'")

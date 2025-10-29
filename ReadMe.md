# Git Helper

A tool to make usage of git a bit easier.

This tool works in a console mode.

## <a name="section_0">Contents</a>

- [Building](#section_1)
- [Installing](#section_2)
- [Usage](#section_3)
- [Version & Dependency](#section_4)
- [Build Information](#section_5)
- [Feedback](#section_6)

## <a name="section_1">Building</a>

* Install the latest version of `CMake` tool.
    * https://cmake.org/


* Install one or more IDEs with support for `CMake`:
    * `Visual Studio` by Microsoft.
        * CMake support in Visual Studio must be installed as a separate feature.
    * `CLion` by JetBrains;
        * CMake support is already built into CLion.


* Update dependencies using the available script.
    * Use the provided `get-libgit2.bat` script.


* Build the project using `Visual Studio` or via `CLion`.


* Versions of dependencies are described in [Version & Dependency](#section_4) section.


## <a name="section_2">Installing</a>

* Copy the built executable file into a desired directory. 

* Copy DLL files of third party libraries into that directory.

List of required DLL files is described in [Version & Dependency](#section_4) section.

## <a name="section_3">Usage</a>

The program works in console mode and requires two (2) command line arguments:

`<executable> <folder> <action>`

1. **Folder**.  
   Path to the folder containing a git repository.


2. **Action**.  
   Action to perform.


Available actions:
 - tags
 - versions
 - lver
 - newpatch

The `tags` action lists tag names.  
The `versions` action lists versions.  
The `lver` action shows latest version.  
The `newpatch` action creates a new patch for the latest commit.

**Usage Example**
<pre>git_helper.exe "E:\my_repo" lver</pre>

## <a name="section_4">Version & Dependency</a>

This tool does not have its own strict versioning.

This tool was built using following libraries.

* **libgit2** version `1.9.1`  
  DLL Files: git2.dll.


## <a name="section_5">Build Information</a>

[Link](./Build/ReadMe.md).

## <a name="section_6">Feedback</a>

If you have found a bug, have comments or want to suggest a new feature to be
implemented, feel free to open an issue in this GitHub repository.

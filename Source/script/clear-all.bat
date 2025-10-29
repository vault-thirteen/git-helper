@ECHO OFF

:: This batch script removes folders of third party libraries.
:: This script must be executed from the 'script' folder.

SET ScriptFolder=script

CD ..
RMDIR /Q /S "libgit2"
CD "%ScriptFolder%"

@ECHO OFF

:: This batch script downloads the 'libgit2' library.
:: This script must be executed from the 'script' folder.

:: Settings.
SET Repository=https://github.com/vault-thirteen/libgit2.git
SET LibFolder=libgit2
SET LibVersion=1.9.2
SET ScriptFolder=script

:: Download the Code.
git clone %Repository%
IF %ErrorLevel% NEQ 0 EXIT /b %ErrorLevel%

:: Prepare the code.
CD ..
MKDIR "%LibFolder%"
MKDIR "%LibFolder%\bin"
MKDIR "%LibFolder%\include"
MKDIR "%LibFolder%\include\%LibFolder%"
MKDIR "%LibFolder%\include\%LibFolder%\git2"
MKDIR "%LibFolder%\include\%LibFolder%\git2\sys"
MKDIR "%LibFolder%\lib"

MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\git*.dll" "%LibFolder%\bin\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\git*.lib" "%LibFolder%\lib\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\include\*.h" "%LibFolder%\include\%LibFolder%\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\include\git2\*.h" "%LibFolder%\include\%LibFolder%\git2\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\include\git2\sys\*.h" "%LibFolder%\include\%LibFolder%\git2\sys\"
RMDIR /Q /S "%ScriptFolder%\%LibFolder%"

CD "%ScriptFolder%"

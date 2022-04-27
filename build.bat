
:: First call this:
:: call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x64
:: Then run this bat file to compile.

@echo off
pushd .\build

if "%1" == "release" goto release

:debug
cl ..\code\main.cpp -Femain.exe -Zi -nologo -link User32.lib Opengl32.lib Gdi32.lib
goto end

:release
echo Building in release mode.
cl ..\code\main.cpp -Femain.exe -Zi -O2 -nologo -link User32.lib Opengl32.lib Gdi32.lib
goto end

:end
popd

mt.exe -manifest ".\code\main.exe.manifest" -outputresource:".\build\main.exe;1" -nologo

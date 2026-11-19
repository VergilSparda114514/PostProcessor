@echo off

pushd ..
vendor\bin\premake\Windows\premake5.exe --file=Build-Walnut-PostProcessor.lua vs2026
popd
pause
@echo off

pushd ..
if exist "build"
(
    rmdir /s /q "build"
)
popd

call generate_vs.bat

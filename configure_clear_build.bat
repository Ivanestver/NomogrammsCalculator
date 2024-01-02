rmdir /s /q build_dir
mkdir build_dir
cd ./build_dir
cmake -DCMAKE_PREFIX_PATH=%~dp0%libtorch ../

pause
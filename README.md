# CMakeP1

完整的编译流程

1.配置（就是你运行的这条命令）：
cmake . -B build -DCMAKE_TOOLCHAIN_FILE=E:\vcpkg\scripts\buildsystems\vcpkg.cmake

2.构建（你之前运行的 cmake --build）：
cmake --build build
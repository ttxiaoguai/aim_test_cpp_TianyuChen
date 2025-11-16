
# CMakeLists.txt

# 1. 设置最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 设置项目名称
project(cpp_exam VERSION 1.0)

# 3. 指定 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. 添加可执行文件 (定义目标)
add_executable(${PROJECT_NAME} main.cpp Song.cpp)

# 5. 配置头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})

# 6. 配置 CTest (必须启用测试)
enable_testing() 

# 7. ***关键步骤：将您的主程序注册为一个 CTest***
# 假设 main.cpp 运行所有的测试逻辑
add_test(NAME AllTests COMMAND ${PROJECT_NAME} --test-mode)
# 或者如果项目要求每个测试分开运行，但通常只需注册主程序
# add_test(NAME Test1 COMMAND ${PROJECT_NAME} Test1) 



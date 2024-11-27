cmake_minimum_required(VERSION 3.16)

project(myapp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找所需的 Qt6 模块
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Sql)

# 设置 Qt 项目结构
qt_standard_project_setup()

# 添加可执行文件并列出所有 .cpp 文件
qt_add_executable(myapp
    database.cpp
    window.cpp
    main.cpp
)

# 链接 Qt6 库，包括 Widgets、Charts 和 Sql
target_link_libraries(myapp PRIVATE Qt6::Widgets Qt6::Charts Qt6::Sql)

# 设置目标属性
set_target_properties(myapp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)

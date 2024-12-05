cmake_minimum_required(VERSION 3.16)

project(myapp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt6 components, including Positioning and Network for map and network operations
find_package(Qt6 REQUIRED COMPONENTS 
    Widgets
    LinguistTools
    Charts
    Sql
    Location
    Positioning
    Network
)

# 设置语言类型
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES zh en)

# 定义翻译文件的路径
set(TS_FILE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/translations")

# 生成翻译文件
qt_add_translations(myapp
    TS_FILES
        ${TS_FILE_DIR}/myapp_zh.ts
        ${TS_FILE_DIR}/myapp_en.ts
)

# Define all .cpp files
qt_add_executable(myapp
    database.cpp
    window.cpp
    main.cpp
    Pages/ComplianceDashboard.cpp
    Pages/EnvironmentalLitterIndicators.cpp
    Pages/FluorinatedCompounds.cpp
    Pages/PollutantOverview.cpp
    Pages/GeographicalHotspots.cpp
    Pages/POPs.cpp
)

target_include_directories(myapp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} # 主目录
    ${CMAKE_CURRENT_SOURCE_DIR}/Pages # Pages 文件夹
)

# Link necessary Qt libraries including Positioning and Network
target_link_libraries(myapp PRIVATE 
    Qt6::Widgets 
    Qt6::Charts 
    Qt6::Sql 
    Qt6::Positioning
    Qt6::Location
    Qt6::Network
)

# Map resources
set(qml_resource_files
    "qml/MapView.qml"
)

# Add QML resources to the project
qt_add_resources(myapp "resources"
    PREFIX "/qml"
    FILES
    ${qml_resource_files}
)

# Set target properties (if applicable)
set_target_properties(myapp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)

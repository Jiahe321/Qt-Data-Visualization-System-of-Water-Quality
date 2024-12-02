cmake_minimum_required(VERSION 3.16)

project(myapp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Sql)

qt_standard_project_setup()

# all .cpp file
qt_add_executable(myapp
    database.cpp
    methods.cpp
    ComplianceDashboard.cpp
    EnvironmentalLitterIndicators.cpp
    FluorinatedCompounds.cpp
    PollutantOverview.cpp
    POPs.cpp
    window.cpp
    main.cpp
)

target_link_libraries(myapp PRIVATE Qt6::Widgets Qt6::Charts Qt6::Sql)

set_target_properties(myapp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)

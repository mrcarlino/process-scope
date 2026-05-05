cmake_minimum_required(VERSION 3.16)
project(ProcessScope LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(ProcessScope
    ${SOURCES}
)

target_include_directories(ProcessScope PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_link_libraries(ProcessScope PRIVATE Qt6::Widgets Qt6::Charts)
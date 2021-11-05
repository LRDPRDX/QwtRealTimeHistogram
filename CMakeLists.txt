cmake_minimum_required(VERSION 3.7.0)

project(gvmepp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)

add_executable(target ${HEADERS} ${SOURCES})

target_link_libraries(target PUBLIC Qt5::Widgets)

find_library(QWT_QT5_LIBRARY qwt HINTS $ENV{QWT_ROOT}/lib)
target_link_libraries(target PUBLIC ${QWT_QT5_LIBRARY})

target_include_directories(target PUBLIC "${PROJECT_SOURCE_DIR}/inc")
target_include_directories(target PUBLIC $ENV{QWT_ROOT}/include)

set_target_properties(target
                      PROPERTIES OUTPUT_NAME real)

install(TARGETS target)

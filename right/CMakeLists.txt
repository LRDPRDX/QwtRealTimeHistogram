cmake_minimum_required(VERSION 3.7.0)

project(gvmepp VERSION 1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "~/Tools/qt/qt6-install/lib/cmake/Qt6")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)

qt_add_executable(target ${HEADERS} ${SOURCES})

find_library(QWT_QT6_LIBRARY qwt HINTS $ENV{QWT_ROOT}/lib)
target_link_libraries(target PRIVATE ${QWT_QT6_LIBRARY})
target_link_libraries(target PRIVATE Qt6::Widgets)

target_include_directories(target PRIVATE "${PROJECT_SOURCE_DIR}/inc")
target_include_directories(target PRIVATE $ENV{QWT_ROOT}/include)

set_target_properties(target
                      PROPERTIES OUTPUT_NAME real)

install(TARGETS target)


cmake_minimum_required(VERSION 2.8)

project(kohonen)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_EXE main.cpp)

set(FUNCS funcs.cpp)

set(CSOM CSOM.cpp)

set(CSOMNODE CSOMNode.cpp)

set(GITPARSER GitParser.cpp)

set(MAINTAINER Maintainer.cpp)

add_library(funcs STATIC ${FUNCS})

add_library(csom STATIC ${CSOM})

add_library(csomnode STATIC ${CSOMNODE})

add_library(gitparser STATIC ${GITPARSER})

add_library(maintainer STATIC ${MAINTAINER})

add_executable(kohonen ${SOURCE_EXE})

find_package(ImageMagick COMPONENTS Magick++)

include_directories(${ImageMagick_INCLUDE_DIRS})

target_link_libraries(kohonen funcs csom csomnode gitparser maintainer ${ImageMagick_LIBRARIES})
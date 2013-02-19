cmake_minimum_required(VERSION 2.8)

project(kohonen)

set(SOURCE_EXE main.cpp)

set(CSOM CSOM.cpp)

set(CSOMNODE CSOMNode.cpp)

set(GITPARSER GitParser.cpp)

set(MAINTAINER Maintainer.cpp)

add_library(csom STATIC ${CSOM})

add_library(csomnode STATIC ${CSOMNODE})

add_library(gitparser STATIC ${GITPARSER})

add_library(maintainer STATIC ${MAINTAINER})

add_executable(main ${SOURCE_EXE})

find_package(ImageMagick COMPONENTS Magick++)

include_directories(${ImageMagick_INCLUDE_DIRS})

target_link_libraries(main csom csomnode gitparser maintainer ${ImageMagick_LIBRARIES})
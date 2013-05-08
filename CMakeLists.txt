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

SET( CMAKE_MODULE_PATH
     "${PROJECT_SOURCE_DIR}/cmake"
     "${CMAKE_MODULE_PATH}" )

find_package(TINYXML)

include_directories(${TINYXML_INCLUDE_DIRS})

SET( TINYXML_USE_STL ON
     CACHE BOOL "tinyxml will use native STL." )
MARK_AS_ADVANCED( TINYXML_USE_STL )

IF( NOT TINYXML_FOUND )
    ADD_SUBDIRECTORY( "lib/tinyxml" )
    SET( TINYXML_FOUND       TRUE )
    SET( TINYXML_INCLUDE_DIR "${tinyxml_INCLUDE_DIR}" )
    SET( TINYXML_LIBRARIES   "tinyxml" )
ENDIF( NOT TINYXML_FOUND )

target_link_libraries(kohonen funcs csom csomnode gitparser maintainer ${ImageMagick_LIBRARIES} ${TINYXML_LIBRARIES})

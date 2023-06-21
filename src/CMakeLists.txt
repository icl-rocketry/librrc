cmake_minimum_required(VERSION 3.16.3)

add_library(librrc STATIC)

#collect source files
file(GLOB_RECURSE LIBRRC_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
target_sources(librrc PUBLIC ${LIBRRC_SRC})

#get include dcirectories relative to the current directory, i.e we have the prefix librnp/...
target_include_directories(librrc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



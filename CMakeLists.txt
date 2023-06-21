# Set minimum CMake version
cmake_minimum_required(VERSION 3.16.3)

if (ESP_PLATFORM)
    idf_component_register(INCLUDE_DIRS src
                           REQUIRES arduino librnp) #requires arduino....
    #we need some wizardry so we keep the nice original cmake structure
    #we add the src subdirectory where the static library is defined
    add_subdirectory(src) 
    #we need to explictly link t
    target_link_libraries(librrc PRIVATE idf::arduino) 
    target_link_libraries(librrc PRIVATE idf::librnp) 
    # target_link_libraries(librrc PRIVATE idf::esp_task) 
    #link the orignal cmake librnp library to the component library 
    target_link_libraries(${COMPONENT_LIB} INTERFACE librrc)
    return()

endif()


# Declare project
project(librrc LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Add source directory
add_subdirectory(src)


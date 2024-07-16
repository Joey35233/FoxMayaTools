cmake_minimum_required(VERSION 3.13)

# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME MtarImporter)

set (CMAKE_CXX_STANDARD 17)

# set SOURCE_FILES
set(SOURCE_FILES
   MtarImporter.cpp
   fox/anim/MtarHeader.cpp
   fox/anim/TrackHeader.cpp
   fox/anim/Mtar2MiniDataNode.cpp
   fox/anim/Gani2TrackHeader.cpp
)

# set linking libraries
set(LIBRARIES 
    OpenMaya
    OpenMayaAnim
    Foundation
)

# Build plugin
build_plugin()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../../../../../$<0:>"
)
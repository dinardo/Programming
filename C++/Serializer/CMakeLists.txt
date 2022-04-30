##################
# CMAKE tutorial #
##################

cmake_minimum_required(VERSION 3.20.2)
project(Serializer VERSION 1.0 DESCRIPTION "An example project with CMake" LANGUAGES CXX)

################
# C++ standard #
################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

####################
# Compiler options #
####################
target_compile_options(theServer PUBLIC -pedantic)
target_compile_options(theClient PUBLIC -pedantic)

####################################
# CmakeLists for sub-directory src #
####################################

add_executable(theServer Server.cc)
set_target_properties(theServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(theClient Client.cc)
set_target_properties(theClient PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

################################
# Include and Link directories #
################################
include_directories(theServer PRIVATE ${PROJECT_SOURCE_DIR}/include)
link_directories(theServer PRIVATE ${PROJECT_SOURCE_DIR}/src)

include_directories(theClient PRIVATE ${PROJECT_SOURCE_DIR}/include)
link_directories(theClient PRIVATE ${PROJECT_SOURCE_DIR}/src)

######################
# Internal libraries #
######################
add_library(MyLib SocketUtils.cc)

target_link_libraries(theServer PRIVATE MyLib)
target_link_libraries(theClient PRIVATE MyLib)

##############
# Find BOOST #
##############
set(BOOST_ROOT "" CACHE PATH "Boost library path")
set(Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost")
find_package(Boost 1.77.0 REQUIRED COMPONENTS serialization REQUIRED)
if(Boost_FOUND)
    message(STATUS "I found BOOST version ${Boost_VERSION}")

    include_directories(theServer PUBLIC ${Boost_INCLUDE_DIRS})
    link_directories(theServer PUBLIC ${Boost_LIBRARY_DIRS})
    target_link_libraries(theServer PUBLIC ${Boost_SERIALIZATION_LIBRARY})

    include_directories(theClient PUBLIC ${Boost_INCLUDE_DIRS})
    link_directories(theClient PUBLIC ${Boost_LIBRARY_DIRS})
    target_link_libraries(theClient PUBLIC ${Boost_SERIALIZATION_LIBRARY})
else()
    message(WARNING "I couldn't find BOOST")
endif()

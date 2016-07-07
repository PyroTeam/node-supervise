cmake_minimum_required(VERSION 2.8.3)
project(topic_relay)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
    topic_tools
)

###################################
## catkin specific configuration ##
###################################

catkin_package(
	# INCLUDE_DIRS include
    #  LIBRARIES robotino3_gazebo
    # CATKIN_DEPENDS message_runtime
    #  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

link_directories(
)

## Declare a cpp executable
add_executable(
    topic_relay

    src/topic_relay_node.cpp
)
set_target_properties(
    topic_relay

    PROPERTIES COMPILE_FLAGS "-g3 -std=c++11 -Wall -Werror"
)
target_link_libraries(
    topic_relay

    ${catkin_LIBRARIES}
)
# add_dependencies(
#     topic_relay

#     ${catkin_DEPENDENCIES}
# )

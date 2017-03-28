cmake_minimum_required(VERSION 3.5)

project(UsbHubGo)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

########################################
if(NOT DEFINED PYTHON_INCLUDE_PATH)
    SET(PYTHON_INCLUDE_PATH "/usr/include/python2.7/")
    message(STATUS "PYTHON_INCLUDE_PATH is ${PYTHON_INCLUDE_PATH}")
else()
    message(STATUS "PYTHON_INCLUDE_PATH is ${PYTHON_INCLUDE_PATH}")
endif()
########################################

# python headers
SET(PYTHON_INCLUDE_PATH "/usr/include/python2.7/")

# include
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

# all sources
SET(SOURCE_FILES
        lib.h
        lib.cpp
        stty.c
        stty.h
        usb_hub_monitor.cpp
        usb_hub_monitor.h
        usb_hub_study.cpp
        usb_hub_study.h
        common.h
        common.cpp
        usb_hub_test.cpp
        usb_hub_test.h)

# exec
#add_executable(usb_hub_go ${SOURCE_FILES})
# python
ADD_LIBRARY (UsbHubGo SHARED ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(UsbHubGo pthread)


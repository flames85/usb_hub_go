cmake_minimum_required(VERSION 3.5)

project(usb_hub_go)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )




set(SOURCE_FILES main.cpp stty.c stty.h usb_hub_monitor.cpp usb_hub_monitor.h usb_hub_study.cpp usb_hub_study.h common.h common.cpp usb_hub_test.cpp usb_hub_test.h)

# exec
#add_executable(usb_hub_go ${SOURCE_FILES})
# so
ADD_LIBRARY (usb_hub_go SHARED ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(usb_hub_go pthread)


cmake_minimum_required(VERSION 3.5)

project(UsbHubGo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )


INCLUDE_DIRECTORIES("/user/local/pythonDev/include/python2.7")

set(SOURCE_FILES lib.h lib.cpp stty.c stty.h usb_hub_monitor.cpp usb_hub_monitor.h usb_hub_study.cpp usb_hub_study.h common.h common.cpp usb_hub_test.cpp usb_hub_test.h)

# exec
#add_executable(usb_hub_go ${SOURCE_FILES})
# python
ADD_LIBRARY (UsbHubGo SHARED ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(UsbHubGo pthread)


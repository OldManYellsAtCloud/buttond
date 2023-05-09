cmake_minimum_required(VERSION 3.5)

project(buttons LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(buttons main.cpp
                volumehandler.cpp
                volumehandler.h
                powerhandler.cpp
                powerhandler.h
                buttonhandler.h)

install(TARGETS buttons RUNTIME DESTINATION /usr/bin)

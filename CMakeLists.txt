cmake_minimum_required(VERSION 3.5)

project(buttons LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SettingsLib)
find_package(LogLibrary)

add_executable(buttons src/main.cpp
                src/volumehandler.cpp
                include/volumehandler.h
                src/powerhandler.cpp
                include/powerhandler.h
                include/buttonhandler.h)

target_include_directories(buttons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(buttons PUBLIC SettingsLib LogLibrary)

install(TARGETS buttons RUNTIME DESTINATION /usr/bin)

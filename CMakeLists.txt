cmake_minimum_required(VERSION 3.5)

project(buttons LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SettingsLib)
find_package(LogLibrary)
find_package(sdbus-c++)

add_executable(buttons src/main.cpp
                include/dbusmanager.h src/dbusmanager.cpp)

target_include_directories(buttons PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(buttons PUBLIC SettingsLib LogLibrary SDBusCpp::sdbus-c++)

install(TARGETS buttons RUNTIME DESTINATION /usr/bin)

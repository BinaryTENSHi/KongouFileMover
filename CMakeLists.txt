cmake_minimum_required(VERSION 2.8.4)
project(KongouFileMover)

set(SOURCE_FILES
src/Main.cpp
src/Configuration.h
src/ConfigurationState.h
src/FileRename.h
src/KRegex.h
src/RegexState.h
src/GlobalData.h src/GlobalData.cpp
src/ConfigurationReader.h src/ConfigurationReader.cpp
src/FileNameHandler.h src/FileNameHandler.cpp

res/Resource.rc
)

set(CMAKE_CXX_FLAGS "-g -std=c++11 -Wall -Wno-unused-local-typedefs")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# BOOST #
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system filesystem regex program_options REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(KongouFileMover ${SOURCE_FILES})
    target_link_libraries(KongouFileMover ${Boost_LIBRARIES})
endif()

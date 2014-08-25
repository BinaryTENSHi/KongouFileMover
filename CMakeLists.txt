cmake_minimum_required(VERSION 2.6)
project(KongouFileMover)

find_package(Boost COMPONENTS system filesystem regex program_options REQUIRED)

set(CMAKE_CXX_FLAGS "-g -std=c++11 -Wall")
add_subdirectory("KongouFileMover")
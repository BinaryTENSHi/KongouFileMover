file(GLOB_RECURSE KongouFileMover_SOURCES "*.cpp")
file(GLOB_RECURSE KongouFileMover_HEADERS "*.h")

set(KongouFileMover_INCLUDE_DIRS "")
foreach(_headerFile ${KongouFileMover_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND KongouFileMover_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES KongouFileMover_INCLUDE_DIRS)

include_directories(,${Boost_INCLUDE_DIR},${KongouFileMover_INCLUDE_DIRS})
link_directories(${Boost_LIBRARIES})

add_executable(kongoufilemover ${KongouFileMover_SOURCES})
target_link_libraries(kongoufilemover ${Boost_LIBRARIES})
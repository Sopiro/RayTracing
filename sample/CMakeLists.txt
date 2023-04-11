file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCE_FILES})

add_executable(sample ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(sample PUBLIC
    spt
)

add_custom_command(
    TARGET sample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../res/
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/res/
)

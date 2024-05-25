# Download Assimp library
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_INSTALL OFF)
option(ASSIMP_INJECT_DEBUG_POSTFIX OFF)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.1
)

FetchContent_MakeAvailable(assimp)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCE_FILES})

add_executable(sample ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(sample PUBLIC
    bulbit
    assimp
)

set_target_properties(sample PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# add_custom_command(
#     TARGET sample POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
#     ${CMAKE_CURRENT_SOURCE_DIR}/../res/
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/res/
# )

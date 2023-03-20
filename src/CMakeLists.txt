set(HEADER_FILES
    ../include/raytracer/aabb.h
    ../include/raytracer/bitmap.h
    ../include/raytracer/bvh.h
    ../include/raytracer/camera.h
    ../include/raytracer/common.h
    ../include/raytracer/constant_medium.h
    ../include/raytracer/growable_array.h
    ../include/raytracer/hittable.h
    ../include/raytracer/material.h
    ../include/raytracer/math.h
    ../include/raytracer/mesh.h
    ../include/raytracer/model.h
    ../include/raytracer/onb.h
    ../include/raytracer/pdf.h
    ../include/raytracer/ray.h
    ../include/raytracer/raytracer.h
    ../include/raytracer/scene.h
    ../include/raytracer/sphere.h
    ../include/raytracer/texture.h
    ../include/raytracer/transform.h
    ../include/raytracer/triangle.h
    ../include/raytracer/types.h
)

set(SOURCE_FILES
    main.cpp
    material.cpp
    bvh.cpp
    scene.cpp
    sphere.cpp
    triangle.cpp
    constant_medium.cpp
    math.cpp
    mesh.cpp
    model.cpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "src" FILES ${SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include" PREFIX "include" FILES ${HEADER_FILES})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern
)

find_package(OpenMP REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    OpenMP::OpenMP_CXX
    assimp::assimp
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../res/
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/res/
)

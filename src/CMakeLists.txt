set(HEADER_FILES
    ../include/raytracer/common.h
    ../include/raytracer/bitmap.h
    ../include/raytracer/camera.h
    ../include/raytracer/constant_medium.h
    ../include/raytracer/hittable_list.h
    ../include/raytracer/hittable.h
    ../include/raytracer/material.h
    ../include/raytracer/math.h
    ../include/raytracer/ray.h
    ../include/raytracer/sphere.h
    ../include/raytracer/texture.h
    ../include/raytracer/types.h
    ../include/raytracer/triangle.h
)
set(SOURCE_FILES
    main.cpp
    material.cpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "src" FILES ${SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include" PREFIX "include" FILES ${HEADER_FILES})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern
    ${DEP_INCLUDE_DIR}
)

target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS_DEMO})

find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

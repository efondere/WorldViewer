cmake_minimum_required(VERSION 3.1)

project(openGLChernoSeries)

find_library(glfw 3.3 REQUIRED)

add_library(glad "src/vendor/glad/glad.cpp")
add_library(stb_image "src/vendor/stb_image/stb_image.cpp")

set(WORLD_VIEWER_SOURCE
    src/main.cpp
    src/World.cpp
    src/InputManager.cpp
    src/glUtil.cpp
    src/Mesh.cpp
    src/Player.cpp
    src/Shader.cpp

#    src/engine/Transformable.cpp
#    src/engine/Transform.cpp
)

add_executable(WorldViewer ${WORLD_VIEWER_SOURCE})

target_compile_options(WorldViewer PRIVATE -Wall)
target_compile_options(WorldViewer PRIVATE -Werror)
# target_compile_options(WorldViewer PRIVATE -Wextra)

target_compile_features(WorldViewer PRIVATE cxx_std_17)

target_link_libraries(WorldViewer PUBLIC glfw3 glad stb_image)
target_include_directories(WorldViewer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor ${CMAKE_CURRENT_SOURCE_DIR}/src/engine)

set(WORLD_GENERATOR_SOURCE
    src/generator/main.cpp
)

add_executable(WorldGenerator ${WORLD_GENERATOR_SOURCE})

target_compile_options(WorldGenerator PRIVATE -Wall)
target_compile_options(WorldGenerator PRIVATE -Werror)
target_compile_features(WorldGenerator PRIVATE cxx_std_17)

target_link_libraries(WorldGenerator PUBLIC glfw3 glad stb_image)
target_include_directories(WorldGenerator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor)

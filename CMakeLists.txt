cmake_minimum_required(VERSION 3.0)
project(FinalPro)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
# Find Assimp
find_package(ASSIMP REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")


add_subdirectory(external)

include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glad-opengl-3.3/include/
	external/tinygltf/
	external/
	FinalPro/
	${ASSIMP_INCLUDE_DIRS}  # Add Assimp include directories
)

add_executable(main
FinalPro/main.cpp
FinalPro/render/shader.cpp
FinalPro/render/texture.cpp

)
target_link_libraries(main
	${OPENGL_LIBRARY}
	glfw
	glad
	${ASSIMP_LIBRARIES}  # Link Assimp libraries
)

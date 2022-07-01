#remember to run sudo apt install python3-numpy
#remember to run sudo apt install python3-matplotlib

cmake_minimum_required(VERSION 3.0)

project(test_gp)

set(CMAKE_CXX_STANDARD 14) # 14 needed for meshgrid
set(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -march=native -g")

file(GLOB implementation_files "../cpp/src/*.cpp")
file(GLOB header_files "../cpp/include/*.h" "../cpp/include/*.hpp")
message("headers:" "${header_files}")
message("cpps:" "${implementation_files}")

# file(GLOB debug_files "../cpp/src/get_shapes.cpp" "../cpp/src/GPShape.cpp" "../cpp/include/GPShape.hpp" "../cpp/include/get_shapes.hpp")

# set_source_files_properties(
#   ${debug_files}
#   PROPERTIES
#   COMPILE_FLAGS "-std=c++14 -march=native -pthread -Wall -W -Wextra -Wconversion -Wno-unused-parameter -Wwrite-strings"
# )

set(Python_ADDITIONAL_VERSIONS 3.10)
find_package(PythonLibs 2.7)

SET (EIGEN3_INCLUDE_DIR "/home/taylorott/Documents/eigen-3.4.0")
message("EIGEN PATH:" "${EIGEN3_INCLUDE_DIR}")



include_directories(
        ../cpp/include 
        ${EIGEN3_INCLUDE_DIR}
        ${PYTHON_INCLUDE_DIRS}
)

add_executable(test_gp test_gp.cpp
                        ${implementation_files}
                        ${header_files})
target_link_libraries(test_gp ${PYTHON_LIBRARIES})
add_executable(test_kd test_kd.cpp
                        ${implementation_files}
                        ${header_files})

project(gp_python)
set (GP_PYTHON_SOURCE gp_python.cpp)
add_library(gp_python MODULE ${GP_PYTHON_SOURCE}
                                ${implementation_files}
                                ${header_files})
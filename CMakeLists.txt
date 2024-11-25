# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ParallelSorting VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create list of source files
set(SOURCES
    src/MergeSort.cpp
    src/QuickSort.cpp
    src/HeapSort.cpp
    src/ShellSort.cpp
    src/RadixSort.cpp
    src/Benchmark.cpp
    src/main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link OpenMP
target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)

# Set compiler flags
if(MSVC)
    # Visual Studio flags
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /O2)
else()
    # GCC/Clang flags
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O3)
endif()

# Output directory settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Install settings
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
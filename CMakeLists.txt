# CMakeLists.txt — Windows x64 build
cmake_minimum_required(VERSION 3.20)
project(TOMMY LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build type to Release for optimizations
set(CMAKE_BUILD_TYPE Release)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

add_executable(VORSIM main.cpp)


# # If you have other folders with .h files in them, add those here
# target_include_directories(${PROJECT_NAME} PUBLIC
#     # ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# target_compile_definitions(${PROJECT_NAME} PRIVATE
#     _USE_MATH_DEFINES
#     _CRT_SECURE_NO_WARNINGS
# )
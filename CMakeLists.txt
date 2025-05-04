cmake_minimum_required(VERSION 3.28)
project(VOR_simulation LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Fetch SFML 3.0 from GitHub
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Add the executable and compile features
add_executable(VORSIM main.cpp)  # Update this if your main.cpp is elsewhere
set_target_properties(VORSIM PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
target_compile_features(VORSIM PRIVATE cxx_std_17)

# Link the SFML graphics, window, and system modules
target_link_libraries(VORSIM PRIVATE SFML::Graphics SFML::Window SFML::System)

# Add post-build step to copy the assets (e.g., ui_resources folder)
add_custom_command(TARGET VORSIM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/ui_resources
        ${CMAKE_BINARY_DIR}/bin/Release/ui_resources
)

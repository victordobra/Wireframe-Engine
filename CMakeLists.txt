# Include the local .env file
include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env file included successfully.")

# Set the minumum required version
cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
message(STATUS "CMake minimum required version set to ${CMAKE_MIN_VERSION}.")

# Create the project
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})
message(STATUS "Project name set to \"${PROJECT_NAME}\".")
message(STATUS "Project version set to ${PROJECT_VERSION}.")

# Enable testing
include(CTest)
enable_testing()
message(STATUS "Testing enabled successfilly")

# Find the source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/engine/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(LENGTH SOURCES SOURCE_COUNT)

if(SOURCE_COUNT EQUAL 0)
    message(FATAL_ERROR "ERROR: No engine sources found!")
elseif(ENGINE_SOURCE_COUNT EQUAL 1)
    message(STATUS "1 engine source found.")
else()
    message(STATUS "${SOURCE_COUNT} engine sources found.")
endif()

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
message(STATUS "Executable created successfully.")

# Add the include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/engine ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/external/Core/include ${PROJECT_SOURCE_DIR}/external/Vulkan/include ${PROJECT_SOURCE_DIR}/external/stb/include)
message(STATUS "Include directories added.")

# Add the link directories and link the libraries
target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/external/Core/lib ${PROJECT_SOURCE_DIR}/external/Vulkan/lib)
message(STATUS "Link directories added.")
target_link_libraries(${PROJECT_NAME} Wireframe-Core-x86_64 vulkan-x86_64)
message(STATUS "Link libraries added.")

# Add the precompiled header
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/engine/WireframeEngine.hpp)
message(STATUS "Precompiled header added.")

# Find all shaders in the project
file(GLOB_RECURSE GLSL_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/Shaders/*.frag" "${PROJECT_SOURCE_DIR}/src/Shaders/*.vert")
set(GLSL_VALIDATOR ${PROJECT_SOURCE_DIR}/external/Vulkan/glsl/glslc.exe)
list(LENGTH GLSL_SOURCE_FILES GLSL_COUNT)

if(GLSL_COUNT EQUAL 0)
    message(STATUS "No shaders found.")
elseif(GLSL_COUNT EQUAL 1)
    message(STATUS "1 shader found.")
else()
    message(STATUS "${GLSL_COUNT} shaders found.")
endif()

# Compile shaders
foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(GLSL_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/assets/shaders/${GLSL_NAME}.spv")
    add_custom_command(OUTPUT ${SPIRV} COMMAND ${GLSL_VALIDATOR} ${GLSL} -o ${SPIRV} DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(SHADERS ALL DEPENDS ${SPIRV_BINARY_FILES})
message(STATUS "Shader compile step added successfully.")

# Add CPack components
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
message(STATUS "CPack components added.")
# Include the local .env file
include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env file included successfully.")

# Set the minumum required version
cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
message(STATUS "CMake minimum required version set to ${CMAKE_MIN_VERSION}.")

# Create the project
project(${ENGINE_NAME} VERSION ${ENGINE_VERSION_MAJOR}.${ENGINE_VERSION_MINOR}.${ENGINE_VERSION_PATCH}.0)
message(STATUS "Engine name set to \"${PROJECT_NAME}\".")
message(STATUS "Engine version set to ${PROJECT_VERSION}.")

# Configure the info header files
configure_file(${PROJECT_SOURCE_DIR}/src/Info/EngineInfo.hpp.in ${PROJECT_SOURCE_DIR}/src/Info/EngineInfo.hpp)
configure_file(${PROJECT_SOURCE_DIR}/test/Info/ProjectInfo.hpp.in ${PROJECT_SOURCE_DIR}/test/Info/ProjectInfo.hpp)
message(STATUS "Configured info header files successfully.")

# Find the engine source files
file(GLOB_RECURSE ENGINE_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(LENGTH ENGINE_SOURCES ENGINE_SOURCE_COUNT)

if(ENGINE_SOURCE_COUNT EQUAL 0)
	message(FATAL_ERROR "ERROR: No engine sources found!")
elseif(ENGINE_SOURCE_COUNT EQUAL 1)
	message(STATUS "1 engine source found.")
else()
	message(STATUS "${ENGINE_SOURCE_COUNT} engine sources found.")
endif()

# Create the engine library
add_library(${ENGINE_NAME} STATIC ${ENGINE_SOURCES})
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_20)
message(STATUS "Engine library created successfully.")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	# Add the include directories for Windows
	target_include_directories(${ENGINE_NAME} PUBLIC $ENV{VULKAN_SDK}/Include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/info)
	message(STATUS "Engine include directories added.")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	# Add the include directories for Linux
	target_include_directories(${ENGINE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/info)
	message(STATUS "Engine include directories added.")

	# Add the link libraries for Linux
	target_link_libraries(${ENGINE_NAME} X11 Xfixes xkbcommon)
	message(STATUS "Engine link libraries added.")
endif()

# Add the main engine precompiled header
target_precompile_headers(${ENGINE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/WireframeEngine.hpp)
message(STATUS "Engine precompiled header added.")

# Find the test source files
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
list(LENGTH TEST_SOURCES TEST_SOURCE_COUNT)

if(TEST_SOURCE_COUNT EQUAL 0)
	message(FATAL_ERROR "ERROR: No test sources found!")
elseif(SOURCE_COUNT EQUAL 1)
	message(STATUS "1 test source found.")
else()
	message(STATUS "${TEST_SOURCE_COUNT} test sources found.")
endif()

# Create the test project executable
add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCES})
target_compile_features(${TEST_PROJECT_NAME} PUBLIC cxx_std_20)
message(STATUS "Test project executable created successfully.")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	# Add the include directories for Windows
	target_include_directories(${TEST_PROJECT_NAME} PUBLIC $ENV{VULKAN_SDK}/Include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/info ${PROJECT_SOURCE_DIR}/src)
	message(STATUS "Test project include directories added.")

	# Add the link libraries for Windows
	target_link_libraries(${TEST_PROJECT_NAME} Wireframe-Engine)
	message(STATUS "Test project link libraries added.")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	# Add the include directories for Linux
	target_include_directories(${TEST_PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/info ${PROJECT_SOURCE_DIR}/src)
	message(STATUS "Test project include directories added.")

	# Add the link libraries for Linux
	target_link_libraries(${TEST_PROJECT_NAME} Wireframe-Engine X11 Xfixes xkbcommon)
	message(STATUS "Test project link libraries added.")
endif()

# Find all shaders in the project
file(GLOB_RECURSE GLSL_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.vert ${PROJECT_SOURCE_DIR}/src/*.frag ${PROJECT_SOURCE_DIR}/test/*.vert ${PROJECT_SOURCE_DIR}/test/*.frag)
set(GLSL_VALIDATOR glslangValidator)
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
	add_custom_command(OUTPUT ${SPIRV} COMMAND ${GLSL_VALIDATOR} ${GLSL} -V -o ${SPIRV} DEPENDS ${GLSL})
	list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(SHADERS ALL DEPENDS ${SPIRV_BINARY_FILES})
message(STATUS "Shader compile step added successfully.")

# Add CPack components
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
message(STATUS "CPack components added.")
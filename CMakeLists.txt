# Include the local .env file
include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env file included successfully.")

# Include the local .build file
include(./.build.cmake OPTIONAL RESULT_VARIABLE LOCAL_BUILD)
message(STATUS "Local .build file included successfully.")

# Set the minumum required version
cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
message(STATUS "CMake minimum required version set to ${CMAKE_MIN_VERSION}.")

# Create the project
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.0)
message(STATUS "Project name set to \"${PROJECT_NAME}\".")
message(STATUS "Project version set to ${PROJECT_VERSION}.")

# Configure the info header file
configure_file(${PROJECT_SOURCE_DIR}/info/ProjectInfo.hpp.in ${PROJECT_SOURCE_DIR}/info/ProjectInfo.hpp)
message(STATUS "Configured info header file successfully.")

# Enable testing
include(CTest)
enable_testing()
message(STATUS "Testing enabled successfully")

# Add the core subdirectory
add_subdirectory(core)

# Find the engine source files
file(GLOB_RECURSE ENGINE_SOURCES ${PROJECT_SOURCE_DIR}/engine/*.cpp)
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
	target_include_directories(${ENGINE_NAME} PUBLIC $ENV{VULKAN_SDK}/Include ${PROJECT_SOURCE_DIR}/core/include ${PROJECT_SOURCE_DIR}/engine ${PROJECT_SOURCE_DIR}/info ${PROJECT_SOURCE_DIR}/src)
	message(STATUS "Engine include directories added.")

	# Add the link directories for Windows
	target_link_directories(${ENGINE_NAME} PUBLIC ${PROJECT_BINARY_DIR}/core)
	message(STATUS "Engine link directories added.")

	# Add the link libraries for Windows
	target_link_libraries(${ENGINE_NAME} Wireframe-Core)
	message(STATUS "Engine link libraries added.")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	# Add the include directories for Linux
	target_include_directories(${ENGINE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/core/include ${PROJECT_SOURCE_DIR}/engine ${PROJECT_SOURCE_DIR}/info ${PROJECT_SOURCE_DIR}/src)
	message(STATUS "Engine include directories added.")

	# Add the link directories for Linux
	target_link_directories(${ENGINE_NAME} PUBLIC ${PROJECT_BINARY_DIR}/core)
	message(STATUS "Engine link directories added.")

	# Add the link libraries for Linux
	target_link_libraries(${ENGINE_NAME} Wireframe-Core X11 Xfixes xkbcommon)
	message(STATUS "Engine link libraries added.")
endif()

# Add the main engine precompiled header
target_precompile_headers(${ENGINE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/engine/WireframeEngine.hpp)
message(STATUS "Engine precompiled header added.")

# Find the source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(LENGTH SOURCES SOURCE_COUNT)

if(SOURCE_COUNT EQUAL 0)
	message(FATAL_ERROR "ERROR: No sources found!")
elseif(SOURCE_COUNT EQUAL 1)
	message(STATUS "1 source found.")
else()
	message(STATUS "${SOURCE_COUNT} sources found.")
endif()

# Create the project executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_20)
message(STATUS "Project executable created successfully.")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	# Add the include directories for Windows
	target_include_directories(${PROJECT_NAME} PUBLIC $ENV{VULKAN_SDK}/Include ${PROJECT_SOURCE_DIR}/core/include ${PROJECT_SOURCE_DIR}/engine ${PROJECT_SOURCE_DIR}/info ${PROJECT_SOURCE_DIR}/src)
	message(STATUS "Project include directories added.")

	if(${TARGET_ARCH} STREQUAL "x86_64")
		# Add the link directories for x86_64 Windows
		target_link_directories(${PROJECT_NAME} PUBLIC $ENV{VULKAN_SDK}/Lib32)
		message(STATUS "Project link directories added.")
	else()
		# Add the link directories for x86_64 Windows
		target_link_directories(${PROJECT_NAME} PUBLIC $ENV{VULKAN_SDK}/Lib)
		message(STATUS "Project link directories added.")
	endif()

	# Add the link libraries for Windows
	target_link_libraries(${PROJECT_NAME} Wireframe-Engine Wireframe-Core)
	message(STATUS "Project link libraries added.")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	# Add the include directories for Linux
	target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/core/include ${PROJECT_SOURCE_DIR}/engine ${PROJECT_SOURCE_DIR}/info ${PROJECT_SOURCE_DIR}/src)
	message(STATUS "Project include directories added.")

	# Add the link libraries for Linux
	target_link_libraries(${PROJECT_NAME} Wireframe-Engine Wireframe-Core X11 Xfixes xkbcommon)
	message(STATUS "Project link libraries added.")
endif()

# Find all shaders in the project
file(GLOB_RECURSE GLSL_SOURCE_FILES ${PROJECT_SOURCE_DIR}/engine/*.vert ${PROJECT_SOURCE_DIR}/engine/*.frag ${PROJECT_SOURCE_DIR}/src/*.vert ${PROJECT_SOURCE_DIR}/src/*.frag)
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
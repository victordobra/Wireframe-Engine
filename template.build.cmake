# Compilers
set(CMAKE_x86_64_C_COMPILER   "x86_64 C compiler name/path")
set(CMAKE_x86_64_CXX_COMPILER "x86_64 C compiler name/path")

set(CMAKE_x64_C_COMPILER   "x64 C compiler name/path")
set(CMAKE_x64_CXX_COMPILER "x64 C compiler name/path")

# Build info
set(TARGET_ARCH "x64")

if(${TARGET_ARCH} STREQUAL "x86_64")
	set(CMAKE_C_COMPILER   ${CMAKE_x86_64_C_COMPILER})
	set(CMAKE_CXX_COMPILER ${CMAKE_x86_64_CXX_COMPILER})
elseif(${TARGET_ARCH} STREQUAL "x64")
	set(CMAKE_C_COMPILER   ${CMAKE_x64_C_COMPILER})
	set(CMAKE_CXX_COMPILER ${CMAKE_x64_CXX_COMPILER})
else()
	message(FATAL_ERROR "ERROR: Unsupported target architecture!")
endif()
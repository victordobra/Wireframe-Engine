# Compilers
set(CMAKE_I386_C_COMPILER   "i386 C compiler name/path")
set(CMAKE_I386_CXX_COMPILER "i386 C compiler name/path")

set(CMAKE_X86_64_C_COMPILER   "x86_64 C compiler name/path")
set(CMAKE_X86_64_CXX_COMPILER "x86_64 C compiler name/path")

# Build info
set(TARGET_ARCH "Target architecture")

if(${TARGET_ARCH} STREQUAL "i386")
	set(CMAKE_C_COMPILER   ${CMAKE_I386_C_COMPILER})
	set(CMAKE_CXX_COMPILER ${CMAKE_I386_CXX_COMPILER})
elseif(${TARGET_ARCH} STREQUAL "x86_64")
	set(CMAKE_C_COMPILER   ${CMAKE_X86_64_C_COMPILER})
	set(CMAKE_CXX_COMPILER ${CMAKE_X86_64_CXX_COMPILER})
else()
	message(FATAL_ERROR "ERROR: Unsupported target architecture!")
endif()
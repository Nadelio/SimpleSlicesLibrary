cmake_minimum_required(VERSION 3.16)
project(string_slice VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "" FORCE)
add_library(string_slice src/string_slice.c)
add_library(string_slice::string_slice ALIAS string_slice)

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(string_slice PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wold-style-definition
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(string_slice PRIVATE -O3 -DNDEBUG)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(string_slice PRIVATE -O0 -g)
    endif()
    
    if(WIN32)
        if(CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
            target_compile_options(string_slice PRIVATE /W4)
        endif()
    endif()
    
    if(UNIX AND NOT APPLE)
        target_compile_options(string_slice PRIVATE -fPIC)
    endif()
else()
    message(WARNING "This project is optimized for Clang. Current compiler: ${CMAKE_C_COMPILER_ID}")
endif()

set_target_properties(string_slice PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(string_slice PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)

install(TARGETS string_slice
    EXPORT string_slice-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES include/string_slice.h 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT string_slice-targets
    FILE string_slice-targets.cmake
    NAMESPACE string_slice::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/string_slice
)

include(CMakePackageConfigHelpers)

set(CONFIG_CONTENT "
include(CMakeFindDependencyMacro)
include(\"\${CMAKE_CURRENT_LIST_DIR}/string_slice-targets.cmake\")
")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/string_slice-config.cmake" "${CONFIG_CONTENT}")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/string_slice-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/string_slice-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/string_slice-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/string_slice
)
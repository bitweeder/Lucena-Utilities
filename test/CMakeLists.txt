#[[#############################################################################

    Lucena Utilities Library
    “test/CMakeLists.txt”
    Copyright © 2018 Lucena
    All Rights Reserved

    This file is distributed under the University of Illinois Open Source
    License. See license/License.txt for details.

    This script expects to be called by the Lucena-Utilities root script; in
    particular, default values for various properties are assumed to have been
    previously set.

##############################################################################]]


#[[#############################################################################
    configuration
#]]

include (FetchContent)

find_package (Threads REQUIRED)

FetchContent_Declare (
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.1
)

FetchContent_GetProperties (googletest)

if (NOT googletest_POPULATED)
    FetchContent_Populate (googletest)

    set (CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
    add_subdirectory (
        ${googletest_SOURCE_DIR}
        ${googletest_BINARY_DIR}
        EXCLUDE_FROM_ALL
    )
    unset (CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
endif()


# SEEME This construct differentiates between single-config and multi-config
# projects (apparently), as opposed to be some sort of under-handed version
# check.
if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target (check
        COMMAND ${CMAKE_CTEST_COMMAND}
            --force-new-ctest-process
            --output-on-failure
            --build-config "$<CONFIGURATION>")
else()
    add_custom_target (check
        COMMAND ${CMAKE_CTEST_COMMAND}
            --force-new-ctest-process
            --output-on-failure)
endif()

# Prettify generated IDE projects
set_target_properties (check
    PROPERTIES
        FOLDER "scripts"
)


#[[#############################################################################
    source files

    We use the much-maligned GLOB feature of file() to generate our source
    lists, but only because we require CMake 3.12+ and CONFIGURE_DEPENDS; this
    is still not fail-safe for crufty generators and theoretical future
    generators on mythical filesystems, but I’ll take my chances,
#]]

set (lul_test_source_files
    luuTest.cpp
)

set (lul_test_header_files
    luuConfig_priv.hpp
)

file (
	GLOB lul_test_source_files
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/luu*.cpp
)

file (
	GLOB lul_test_header_files
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/luu*.hpp
)


#[[#############################################################################
    set up test target
#]]

include (GoogleTest)

add_executable (lulTest)

target_compile_features (lulTest
    PUBLIC
        cxx_std_17
            # This is redundant with the global settings, and possibly
            # undesirable if it restricts forward compatibility
            # (documentation suggests “no”).
)

target_compile_definitions (lulTest
    PRIVATE
        LUL_CONFIG_unit=1
        LUL_CONFIG_use_prefix_std=1
)

target_sources (lulTest
    PRIVATE
        ${lul_test_header_files}
        ${lul_test_source_files}
)

target_link_libraries (lulTest
    PUBLIC
        Lucena-Utilities
        gtest
        gtest_main
        gmock
)

target_include_directories (lulTest
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

gtest_discover_tests (lulTest)


#[[#############################################################################
    clean up
#]]

# Keep the cache and GUI clean.
mark_as_advanced (
    BUILD_GMOCK
    BUILD_GTEST
    BUILD_SHARED_LIBS
    gmock_build_tests
    gtest_build_samples
    gtest_build_tests
    gtest_disable_pthreads
    gtest_force_shared_crt
    gtest_hide_internal_symbols
)

# Prettify generated IDE projects
set_target_properties (gtest gtest_main gmock gmock_main
    PROPERTIES
        FOLDER "extern"
)

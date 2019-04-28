#[[#############################################################################

    Lucena Utilities Library
    “CMakeLists.txt”
    Copyright © 2018 Lucena
    All Rights Reserved

    This file is distributed under the University of Illinois Open Source
    License. See license/License.txt for details.

##############################################################################]]


#[[#############################################################################
    initialization
#]]

# This minimum is for FetchContent (3.11), CONFIGURE_DEPENDS (3.12),
# target_link_options and friends (3.13), and more.
cmake_minimum_required (VERSION 3.13 FATAL_ERROR)


# Simplify finding our internal CMake tools and start loading them.
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (LULUtils)


# Forbid poisoning the source tree, ’cuz we’re authoritarian that way. Note,
# though, that CMake will have already left a few droppings at this point.
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message (FATAL_ERROR "FATAL: In-source builds are not allowed. "
        "You should create a separate directory for build files.")
endif()


# FIXME This doesn’t actually work. Arguably, we should just have a reasonable
# default, anyway, when it’s not specified.
if ($<PLATFORM_ID:Darwin> AND NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    message (FATAL_ERROR "-DCMAKE_OSX_DEPLOYMENT_TARGET=[xx] must be an argument to CMake on Apple platforms")
endif()


# Versioning; propagates to headers
set (PROJECT_VERSION_MAJOR 2)
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)
set (PROJECT_VERSION_TWEAK "2")

if (${PROJECT_VERSION_TWEAK})
	set (LUL_PROXY_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK})
else()
	set (LUL_PROXY_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
endif()


project (Lucena-Utilities
    VERSION ${LUL_PROXY_VERSION}
    DESCRIPTION
        "C++ feature detection, utilities, and Standard Library feature implementations"
    HOMEPAGE_URL
        "https://bitweeder.com"
    LANGUAGES CXX)


# Customize paths per-platform; these may or may not be relative paths, but
# they’re guaranteed to be suitable for passing to install().
if (UNIX)
    include (GNUInstallDirs)

    set (LUL_INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/Lucena-Utilities")
    set (LUL_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
    set (LUL_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set (LUL_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set (LUL_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set (LUL_DATA_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/Lucena-Utilities")
    set (LUL_CMAKE_CONFIG_INSTALL_DIR "${LUL_DATA_INSTALL_DIR}/cmake")

elseif (WIN32)
	# SEEME Under Windows, it’s assumed that DLLs need to be in the same folder
	# as the .exe’s; this might be oversimplified.
    set (LUL_INCLUDE_INSTALL_DIR "include/Lucena-Utilities")
    set (LUL_RUNTIME_INSTALL_DIR "bin")
    set (LUL_LIBRARY_INSTALL_DIR "bin")
    set (LUL_ARCHIVE_INSTALL_DIR "lib")
    set (LUL_FRAMEWORK_INSTALL_DIR "bin")
    set (LUL_DATA_INSTALL_DIR ".")
    set (LUL_CMAKE_CONFIG_INSTALL_DIR "${LUL_DATA_INSTALL_DIR}/cmake")
else()
	message (FATAL_ERROR "Failed to identify install folders for this platform.")
endif()


# Source file macro replacement
configure_file (
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/lulConfig.hpp.in
	${CMAKE_CURRENT_SOURCE_DIR}/include/Lucena-Utilities/lulConfig.hpp
	@ONLY
	NEWLINE_STYLE UNIX
)


#[[#############################################################################
    option setup
#]]

include (CMakeDependentOption)

option (LUL_BUILD_LIBRARY "Build the Lucena Utilities Library" ON)

# Default to a static library build - we’d be header-only if we could get away
# with it - but additionally, shared library support is basically untested.
# Note that we use a proxy option to avoid breaking the global cache in case
# we’re a subdirectory.
option (LUL_BUILD_SHARED_LIBS
    "Build the Lucena Utilities Library as a shared library" OFF)

set (BUILD_SHARED_LIBS ${LUL_BUILD_SHARED_LIBS})

option (LUL_BUILD_TESTS "Build unit tests" ON)

option (LUL_BUILD_DOCS "Build documentation" ON)

# FIXME Note that this won’t actually work. In particular, when providing
# reference implementations for Standard Library features that are not
# header-only, this will never work. Additionally, some of the utilities
# provided with the library still need to be re-written to eliminate the need
# for object file support (e.g., Status).
option (LUL_CONFIG_HEADERS_ONLY "Suppress generation of object files" OFF)
if (${LUL_CONFIG_HEADERS_ONLY})
    set(_LUL_CONFIG_HEADERS_ONLY 1)
else()
    set(_LUL_CONFIG_HEADERS_ONLY 0)
endif()

option (LUL_CONFIG_FORCE_LOCAL_ANY
    "Alias stdproxy::any to the reference implementation even if std::any is available" OFF)
if (${LUL_CONFIG_FORCE_LOCAL_ANY})
    set(_LUL_CONFIG_FORCE_LOCAL_ANY 1)
else()
    set(_LUL_CONFIG_FORCE_LOCAL_ANY 0)
endif()

option (LUL_CONFIG_FORCE_LOCAL_BIT_CAST
    "Alias stdproxy::bit_cast to the reference implementation even if std::bit_cast is available" OFF)
if (${LUL_CONFIG_FORCE_LOCAL_BIT_CAST})
    set(_LUL_CONFIG_FORCE_LOCAL_BIT_CAST 1)
else()
    set(_LUL_CONFIG_FORCE_LOCAL_BIT_CAST 0)
endif()

option (LUL_CONFIG_FORCE_LOCAL_FILESYSTEM
    "Alias stdproxy::filesystem to the reference implementation even if std::filesystem is available" OFF)
if (${LUL_CONFIG_FORCE_LOCAL_FILESYSTEM})
    set(_LUL_CONFIG_FORCE_LOCAL_FILESYSTEM 1)
else()
    set(_LUL_CONFIG_FORCE_LOCAL_FILESYSTEM 0)
endif()

option (LUL_CONFIG_FORCE_LOCAL_OBSERVER_PTR
    "Alias stdproxy::observer_ptr to the reference implementation even if std::observer_ptr is available" OFF)
if (${LUL_CONFIG_FORCE_LOCAL_OBSERVER_PTR})
    set(_LUL_CONFIG_FORCE_LOCAL_OBSERVER_PTR 1)
else()
    set(_LUL_CONFIG_FORCE_LOCAL_OBSERVER_PTR 0)
endif()

option (LUL_CONFIG_FORCE_LOCAL_OPTIONAL
    "Alias stdproxy::optional to the reference implementation even if std::optional is available" OFF)
if (${LUL_CONFIG_FORCE_LOCAL_OPTIONAL})
    set(_LUL_CONFIG_FORCE_LOCAL_OPTIONAL 1)
else()
    set(_LUL_CONFIG_FORCE_LOCAL_OPTIONAL 0)
endif()

option (LUL_CONFIG_FORCE_LOCAL_SPAN
    "Alias stdproxy::span to the reference implementation even if std::span is available" OFF)
if (${LUL_CONFIG_FORCE_LOCAL_SPAN})
    set(_LUL_CONFIG_FORCE_LOCAL_SPAN 1)
else()
    set(_LUL_CONFIG_FORCE_LOCAL_SPAN 0)
endif()

option (LUL_CONFIG_FORCE_LOCAL_VARIANT
    "Alias stdproxy::variant to the reference implementation even if std::variant is available" OFF)
if (${LUL_CONFIG_FORCE_LOCAL_VARIANT})
    set(_LUL_CONFIG_FORCE_LOCAL_VARIANT 1)
else()
    set(_LUL_CONFIG_FORCE_LOCAL_VARIANT 0)
endif()

option (LUL_CONFIG_USE_BOOST_NETWORKING
    "Alias stdproxy::net symbols to the Boost implementation unless <net> is available" OFF)
CMAKE_DEPENDENT_OPTION (LUL_CONFIG_FORCE_BOOST_NETWORKING
    "Alias stdproxy::net symbols to the Boost implementation even if <net> is available" OFF
        "LUL_CONFIG_USE_BOOST_NETWORKING" OFF)
if (${LUL_CONFIG_USE_BOOST_NETWORKING})
    set(_LUL_CONFIG_USE_BOOST_NETWORKING 1)
else()
    set(_LUL_CONFIG_USE_BOOST_NETWORKING 0)
endif()
if (${LUL_CONFIG_FORCE_BOOST_NETWORKING})
    set(_LUL_CONFIG_FORCE_BOOST_NETWORKING 1)
else()
    set(_LUL_CONFIG_FORCE_BOOST_NETWORKING 0)
endif()

option (LUL_CONFIG_USE_SHARED_LOCK
    "Use std::shared_lock in convenience type aliases where available and appropriate" ON)
if (${LUL_CONFIG_USE_SHARED_LOCK})
    set(_LUL_CONFIG_USE_SHARED_LOCK 1)
else()
    set(_LUL_CONFIG_USE_SHARED_LOCK 0)
endif()

option (LUL_CONFIG_MACOS_TARGET
    "Force the build to target macOS; requires -DCMAKE_OSX_DEPLOYMENT_TARGET=[xx]" OFF)

option (LUL_CONFIG_IOS_TARGET
    "Force the build to target iOS; requires -DCMAKE_OSX_DEPLOYMENT_TARGET=[xx]" OFF)

option (LUL_CONFIG_TVOS_TARGET
    "Force the build to target tvOS; requires -DCMAKE_OSX_DEPLOYMENT_TARGET=[xx]" OFF)

option (LUL_CONFIG_WATCHOS_TARGET
    "Force the build to target watchOS; requires -DCMAKE_OSX_DEPLOYMENT_TARGET=[xx]" OFF)


#[[#############################################################################
    global configuration
#]]

# Symbol visibility defaults; note that the source code overrides these, as
# necessary.
set (CMAKE_CXX_VISIBILITY_PRESET hidden)
set (CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# Listing out individual features is a bit silly as it creates too much
# coupling, and attempting to get by with proxies is brittle. Note that
# CXX_STANDARD appears to be completely useless for its intended purpose
# since it’s viewed as an unenforced suggestion; we set it anyway. Also,
# note that this is probably unreliable under MSVC, but we can just invoke
# “/permissive-” explicitly there.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_EXTENSIONS OFF)


#[[#############################################################################
    source files

    We use the much-maligned GLOB feature of file() to generate our source
    lists, but only because we require CMake 3.12+ and CONFIGURE_DEPENDS; this
    is still not fail-safe for crufty generators and theoretical future
    generators on mythical filesystems, but I’ll take my chances,
#]]

file (
	GLOB lul_public_includes
    RELATIVE ${CMAKE_SOURCE_DIR}/
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/include/Lucena-Utilities/lul*.hpp
)

set (lul_interface_includes
    ${CMAKE_SOURCE_DIR}/include/Lucena-Utilities/Lucena-Utilities.hpp
)

file (
	GLOB lul_detail_includes
    RELATIVE ${CMAKE_SOURCE_DIR}/
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/include/Lucena-Utilities/details/lul*.hpp
)

file (
	GLOB lul_library_source_files
    RELATIVE ${CMAKE_SOURCE_DIR}/
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/lul/Lucena-Utilities.cpp
    ${CMAKE_SOURCE_DIR}/src/lul/lul*.cpp
)

file (
	GLOB lul_library_header_files
    RELATIVE ${CMAKE_SOURCE_DIR}/
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/lul/lul*.hpp
)


#[[#############################################################################
    set up library target

    We build a static library by default, as per the LUL_BUILD_SHARED_LIBS
    option.
#]]

if (LUL_BUILD_LIBRARY)
    add_library (Lucena-Utilities)

    target_compile_features (Lucena-Utilities
        PUBLIC
            cxx_std_17
                # This is redundant with the global settings, and possibly
                # undesirable if it restricts forward compatibility
                # (documentation suggests “no”).
    )

    target_compile_definitions (Lucena-Utilities
        PUBLIC
             LUL_CONFIG_headers_only=${_LUL_CONFIG_HEADERS_ONLY}

             LUL_CONFIG_force_local_any=${_LUL_CONFIG_FORCE_LOCAL_ANY}
             LUL_CONFIG_force_local_bit_cast=${_LUL_CONFIG_FORCE_LOCAL_BIT_CAST}
             LUL_CONFIG_force_local_filesystem=${_LUL_CONFIG_FORCE_LOCAL_FILESYSTEM}
             LUL_CONFIG_force_local_observer_ptr=${_LUL_CONFIG_FORCE_LOCAL_OBSERVER_PTR}
             LUL_CONFIG_force_local_optional=${_LUL_CONFIG_FORCE_LOCAL_OPTIONAL}
             LUL_CONFIG_force_local_span${_LUL_CONFIG_FORCE_LOCAL_SPAN}
             LUL_CONFIG_force_local_variant=${_LUL_CONFIG_FORCE_LOCAL_VARIANT}

             LUL_CONFIG_use_boost_networking=${_LUL_CONFIG_USE_BOOST_NETWORKING}
             LUL_CONFIG_force_boost_networking=${_LUL_CONFIG_FORCE_BOOST_NETWORKING}

             LUL_CONFIG_use_shared_lock=${_LUL_CONFIG_USE_SHARED_LOCK}
        PRIVATE
            LUL_CONFIG_utility_lib=1
            LUL_CONFIG_use_prefix_std=1

            LUL_PACKAGING_PRODUCT_IDENTIFIER="${CMAKE_PROJECT_NAME}"
            LUL_PACKAGING_SIGNATURE="luul"
            LUL_PACKAGING_COPYRIGHT_HOLDER="Lucena"
            LUL_PACKAGING_COPYRIGHT_DATE="2015-2018"
            LUL_PACKAGING_PRODUCT_DOMAIN="com.lucena"
            LUL_PACKAGING_PREFIX="lul"
            LUL_PACKAGING_DESCRIPTION="${CMAKE_PROJECT_DESCRIPTION}"
            LUL_PACKAGING_BUILD="${PROJECT_VERSION_TWEAK}"
            LUL_PACKAGING_VERSION="${PROJECT_VERSION}"
            $<$<CONFIG:DEBUG>:LUL_CONFIG_debug=1>
    )

    if ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
        # CMake, unhelpfully, does not differentiate between Apple platforms at
        # this time, though a patch is in the works. For now, we assume macOS
        # unless the user has specifically indicated otherwise through options.
        if (${LUL_CONFIG_IOS_TARGET})
            target_compile_definitions (Lucena-Utilities
                PUBLIC
                    LUL_PACKAGING_API="iOS"
                    LUL_TARGET_OS_IOS=1
            )
        elseif (${LUL_CONFIG_IOS_SIM_TARGET})
            target_compile_definitions (Lucena-Utilities
                PUBLIC
                    LUL_PACKAGING_API="iOS"
                    LUL_TARGET_OS_IOS_SIM=1
            )
        elseif (${LUL_CONFIG_TVOS_TARGET})
            message ("LUL does not currently support tvOS targets")
            target_compile_definitions (Lucena-Utilities
                PUBLIC
                    LUL_PACKAGING_API="iOS"
                    LUL_TARGET_OS_IOS=1
            )
        elseif (${LUL_CONFIG_WATCHOS_TARGET})
            message ("LUL does not currently support watchOS targets")
            target_compile_definitions (Lucena-Utilities
                PUBLIC
                    LUL_PACKAGING_API="iOS"
                    LUL_TARGET_OS_IOS=1
            )
        else()
            target_compile_definitions (Lucena-Utilities
                PUBLIC
                    LUL_PACKAGING_API="OSX"
                    LUL_TARGET_OS_OSX=1
            )
        endif()
    elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
        target_compile_definitions (Lucena-Utilities
            PUBLIC
                LUL_PACKAGING_API="WinAPI"
                LUL_TARGET_OS_WIN=1
        )
    elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux" OR
            "${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD" OR
            "${CMAKE_SYSTEM_NAME}" MATCHES "NetBSD" OR
            "${CMAKE_SYSTEM_NAME}" MATCHES "OpenBSD")

        target_compile_definitions (Lucena-Utilities
            PUBLIC
                LUL_PACKAGING_API="X11"
                LUL_TARGET_OS_X11=1
        )
    else()
        message (FATAL_ERROR "LUL does not appear to support this platform")
    endif()

    target_sources (Lucena-Utilities
        PUBLIC
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/$<JOIN:${lul_public_includes} ${lul_detail_includes}, $<INSTALL_PREFIX>/>>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/$<JOIN:${lul_public_includes} ${lul_detail_includes}, ${CMAKE_CURRENT_SOURCE_DIR}/>>

        INTERFACE
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/$<JOIN:${lul_interface_includes}, $<INSTALL_PREFIX>/>>

        PRIVATE
            ${lul_library_header_files}
            ${lul_library_source_files}
    )

    # These are the same headers, just referenced different ways depending on
    # whether we’re building or installed/imported. As indicated, these are
    # treated as system headers.
    target_include_directories (Lucena-Utilities SYSTEM
        PUBLIC
            $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

    target_include_directories (Lucena-Utilities
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # FIXME This list was built lazily; actual comand line args from the
    # existing projects need to be examined and cross-referenced with CMake to
    # determine canonical methods for setting various flags, whether explicitly
    # here or via some CMake command-line switch or via a variable/property
    # setting.
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_compile_options (Lucena-Utilities
            PRIVATE
                -Wmain
                -Wall
                -Wno-multichar
                -Wclobbered
                -Wempty-body
                -Wignored-qualifiers
                -Wsign-compare
                -Wtype-limits
                -Wuninitialized
                -Wunused-parameter
                -Wunused-but-set-parameter
        )
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
         target_compile_options (Lucena-Utilities
            PRIVATE
                /wd4800                         # truncation to bool warning
        )
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        # Note that this also (correctly) picks up “AppleClang”

        target_compile_options (Lucena-Utilities
            PRIVATE
                -Werror=non-modular-include-in-framework-module
                -Wno-trigraphs
                -Werror=incompatible-pointer-types
                -Wmissing-field-initializers
                -Wmissing-prototypes
                -Werror=return-type
                -Wdocumentation
                -Wunreachable-code
                -Werror=deprecated-objc-isa-usage
                -Werror=objc-root-class
                -Wno-non-virtual-dtor
                -Woverloaded-virtual
                -Wno-exit-time-destructors
                -Wmissing-braces
                -Wparentheses
                -Wswitch
                -Wunused-function
                -Wunused-label
                -Wunused-parameter
                -Wno-unused-variable
                -Wunused-value
                -Wempty-body
                -Wuninitialized
                -Wconditional-uninitialized
                -Wno-unknown-pragmas
                -Wshadow
                -Wno-four-char-constants
                -Wconversion
                -Wconstant-conversion
                -Wint-conversion
                -Wbool-conversion
                -Wenum-conversion
                -Wfloat-conversion
                -Wnon-literal-null-conversion
                -Wobjc-literal-conversion
                -Wassign-enum
                -Wsign-compare
                -Wshorten-64-to-32
                -Wnewline-eof
                -Wc++11-extensions
        )
    endif()
endif()


#[[#############################################################################
    set up packages
#]]

include (CMakePackageConfigHelpers)

write_basic_package_version_file (
    ${CMAKE_CURRENT_BINARY_DIR}/Lucena-Utilities-config-version.cmake
    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file (
    ${PROJECT_SOURCE_DIR}/cmake/Lucena-Utilities-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/Lucena-Utilities-config.cmake
    INSTALL_DESTINATION
        ${LUL_CMAKE_CONFIG_INSTALL_DIR}
)


#[[#############################################################################
    install targets
#]]

#if (LUL_HAS_IMPORTED_TARGETS)
    install (
        TARGETS
            Lucena-Utilities
        EXPORT
            Lucena-Utilities-export
        ARCHIVE
            DESTINATION
                ${LUL_ARCHIVE_INSTALL_DIR}
            OPTIONAL
        LIBRARY
            DESTINATION
                ${LUL_LIBRARY_INSTALL_DIR}
            OPTIONAL
        INCLUDES
            DESTINATION
                ${LUL_INCLUDE_INSTALL_DIR}
    )

    # SEEME For some inexplicable and no doubt completely CMake-ian reason,
    # the above line does *not* actually install the public headers; we still
    # need to explicitly copy them. Note that the alternative PUBLIC_HEADERS
    # option above will fail on Apple platforms - intentionally! - regardless
    # of the target type.
    install (
        DIRECTORY ${CMAKE_SOURCE_DIR}/include
            DESTINATION
                ${LUL_INCLUDE_INSTALL_DIR}
    )

    install (EXPORT Lucena-Utilities-export
    	FILE
    		Lucena-Utilities-export.cmake
    	NAMESPACE
    		Lucena-Utilities::
        DESTINATION
            ${LUL_CMAKE_CONFIG_INSTALL_DIR}
    )

    install (
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/Lucena-Utilities-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/Lucena-Utilities-config-version.cmake
        DESTINATION
            ${LUL_CMAKE_CONFIG_INSTALL_DIR}
    )
#endif()


#[[#############################################################################
    set up subdirectories
#]]

if (LUL_BUILD_TESTS)
    enable_testing()
    add_subdirectory (test)
endif()

if (LUL_BUILD_DOCS)
    add_subdirectory (docs)
endif()

#[[#############################################################################

    Lucena Utilities Library
    “cmake/LULUtils.cmake”
    Copyright © 2018 Lucena
    All Rights Reserved

    This file is distributed under the University of Illinois Open Source
    License. See license/License.txt for details.

	This file is modified from QuickCppLibUtils.cmake in QuickCppLib 0.1.0.0
	(<https://github.com/ned14/quickcpplib>) and used under the Boost Software
	License 1.0. See “licenses/QuickCppLib License.txt” for details. Mostly, we
	just strip stuff out that we have no need of to reduce the maintenance
	surface and avoid implying bunch of functionality we don’t support.
	Additionally, the file is renamed to avoid conflicts and it sits in a
	(different) namespace.

##############################################################################]]


if(LULUtilsIncluded)
  return()
endif()
set(LULUtilsIncluded ON)

# Returns a path with forward slashes replaced with backslashes on WIN32
function(NativisePath outvar)
  if(WIN32)
    string(REPLACE "/" "\\" new ${ARGN})
  else()
    set(new ${ARGN})
  endif()
  set(${outvar} ${new} PARENT_SCOPE)
endfunction()

# Escape a string into a regex matching that string
function(escape_string_into_regex outvar)
  string(REGEX REPLACE "(\\^|\\$|\\.|\\[|\\]|\\*|\\+|\\?|\\(|\\)|\\\\)" "\\\\\\1" out ${ARGN})
  set(${outvar} ${out} PARENT_SCOPE)
endfunction()

# Indents a message by a global variable amount of whitespace
function(indented_message type)
  message(${type} "${MESSAGE_INDENT}" ${ARGN})
endfunction()

# Executes an external process, fatal erroring if it fails
function(checked_execute_process desc)
  execute_process(${ARGN} RESULT_VARIABLE result)
  if(NOT result EQUAL 0)
    message(FATAL_ERROR "FATAL: ${desc} failed with error '${result}'")
  endif()
endfunction()

# Apply OpenMP to a given target. Add REQUIRED to make it mandatory.
function(target_uses_openmp target)
  find_package(OpenMP)
  if(OPENMP_FOUND)
    if(MSVC AND CLANG)
      # Currently doesn't work
    elseif(MSVC)
      target_compile_options(${target} PRIVATE ${OpenMP_CXX_FLAGS})
      return()
    else()
      target_compile_options(${target} PRIVATE ${OpenMP_CXX_FLAGS})
      set_target_properties(${target} PROPERTIES LINK_FLAGS -fopenmp)
      return()
    endif()
  endif()
  list(FIND ARGN "REQUIRED" required_idx)
  if(${required_idx} GREATER -1)
    indented_message(FATAL_ERROR "FATAL: Target ${target} requires OpenMP")
  endif()
endfunction()

# Configures a CTest script with a sensible set of defaults
# for doing a configure, build, test and submission run
macro(CONFIGURE_CTEST_SCRIPT_FOR_CDASH projectname bindir)
  set(CTEST_PROJECT_NAME "${projectname}")
  set(CTEST_SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  set(CTEST_BINARY_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${bindir}")
  set(CTEST_CONFIGURATION_TYPE Release)
  if(WIN32)
    if(NOT DEFINED CTEST_CMAKE_GENERATOR)
      # TODO Figure out how to use winclang via adding in -T v140_clang_c2
      set(CTEST_CMAKE_GENERATOR "Visual Studio 15 2017 Win64")
    endif()
    set(CTEST_CMAKE_CI_BIN_DIR "${bindir}/bin/${CTEST_CONFIGURATION_TYPE}")
    set(CTEST_SITE $ENV{COMPUTERNAME})
  else()
    if(NOT DEFINED CTEST_CMAKE_GENERATOR)
      set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
    endif()
    set(CTEST_CMAKE_CI_BIN_DIR "${bindir}/bin")
    set(CTEST_SITE $ENV{NAME})
  endif()
  set(CTEST_BUILD_NAME "${CMAKE_SYSTEM}-${CMAKE_SYSTEM_PROCESSOR}")

  find_program(CTEST_PYTHON_COMMAND NAMES python)
endmacro()

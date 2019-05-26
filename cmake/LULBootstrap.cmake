#[[#############################################################################

    Lucena Utilities Library
    “cmake/LULUtils.cmake”
    Copyright © 2018-2019 Lucena
    All Rights Reserved

    This file is distributed under the University of Illinois Open Source
    License. See license/License.txt for details.

	This file is modified from QuickCppLibBootstrap.cmake in
	QuickCppLib 0.1.0.0 (<https://github.com/ned14/quickcpplib>) and used under
	the Boost Software License 1.0. See “licenses/QuickCppLib License.txt” for
	details. Mostly, we just strip stuff out that we have no need of.
	Additionally, the file is renamed to avoid conflicts and it sits in a
	(different) namespace.

##############################################################################]]


cmake_minimum_required (VERSION 3.13 FATAL_ERROR)

# If necessary bring in the LUL cmake tooling
set(lul_done OFF)
foreach(item ${CMAKE_MODULE_PATH})
  if(item MATCHES "lul/cmake")
    set(lul_done ON)
  endif()
endforeach()
if(NOT lul_done)
  # CMAKE_SOURCE_DIR is the very topmost parent cmake project
  # CMAKE_CURRENT_SOURCE_DIR is the current cmake subproject

  # If there is a magic .lul_use_siblings directory above the topmost project, use sibling edition
  if(EXISTS "${CMAKE_SOURCE_DIR}/../.lul_use_siblings")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../lul/cmake")
  elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../.lul_use_siblings")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../lul/cmake")
  elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.gitmodules")
    # Read in .gitmodules and look for myself
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.gitmodules" GITMODULESCONTENTS)
    if(GITMODULESCONTENTS MATCHES ".*\\n?\\[submodule \"([^\"]+\\/lul)\"\\]")
      set(lulpath "${CMAKE_MATCH_1}")
      if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${lulpath}/cmake")
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/${lulpath}/cmake")
      else()
        message(WARNING "WARNING: ${lulpath}/cmake does not exist, attempting git submodule update --init --recursive ...")
        include(FindGit)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
          WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${lulpath}/cmake")
          set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/${lulpath}/cmake")
        else()
          message(FATAL_ERROR "FATAL: ${lulpath}/cmake does not exist and git submodule update --init --recursive did not make it available, bailing out")
        endif()
      endif()
    else()
      message(FATAL_ERROR "FATAL: A copy of lul cannot be found, and cannot find a lul submodule in this git repository")
    endif()
  else()
    message(FATAL_ERROR "FATAL: A copy of lul cannot be found, and there are no git submodules to search")
  endif()
endif()
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

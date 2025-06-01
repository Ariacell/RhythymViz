cmake_minimum_required(VERSION 3.10)

project(VulkanAudioApp)
set(EXECUTABLE_OUTPUT_PATH "bin")
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

# Vulkan
find_package(Vulkan REQUIRED)

# OpenAL
find_package(OpenAL REQUIRED)

# GLFW
find_package(glfw3 3.3 REQUIRED)

# libsndfile
# Use pkg-config to get hints about paths
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(LIBSNDFILE_PKGCONF sndfile)
endif(PKG_CONFIG_FOUND)

# Include dir
find_path(LIBSNDFILE_INCLUDE_DIR
        NAMES sndfile.h
        PATHS ${LIBSNDFILE_PKGCONF_INCLUDE_DIRS}
        )

# Library
find_library(LIBSNDFILE_LIBRARY
        NAMES sndfile libsndfile-1
        PATHS ${LIBSNDFILE_PKGCONF_LIBRARY_DIRS}
        )

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(LibSndFile  DEFAULT_MSG  LIBSNDFILE_LIBRARY LIBSNDFILE_INCLUDE_DIR)

if(LIBSNDFILE_FOUND)
    set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_LIBRARY})
    set(LIBSNDFILE_INCLUDE_DIRS ${LIBSNDFILE_INCLUDE_DIR})
endif(LIBSNDFILE_FOUND)

mark_as_advanced(LIBSNDFILE_LIBRARY LIBSNDFILE_LIBRARIES LIBSNDFILE_INCLUDE_DIR LIBSNDFILE_INCLUDE_DIRS)

include(FindPkgConfig)
pkg_search_module(SndFile REQUIRED sndfile)

include_directories(${LIBSNDFILE_INCLUDE_DIRS})

message(STATUS "sndfile include dirs path: ${LIBSNDFILE_INCLUDE_DIRS}")
message(STATUS "sndfile libs path: ${LIBSNDFILE_LIBRARIES}")

# Add source files
add_executable(VulkanAudioApp
    src/main.cpp
    src/AudioPlayer.h
)

# Link libraries
target_link_libraries(VulkanAudioApp
    Vulkan::Vulkan
    OpenAL::OpenAL
    ${LIBSNDFILE_LIBRARIES}
    glfw
  )


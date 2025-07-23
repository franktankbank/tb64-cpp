cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    tb64
    VERSION 0.1.0
    DESCRIPTION "A niche tool to encode text in base64 3 times. (cpp version)"
    HOMEPAGE_URL "https://github.com/franktankbank/tb64-cpp"
    LANGUAGES CXX
)

if(DEFINED ENV{VCPKG_ROOT})
  set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
  include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
else()
  message(FATAL_ERROR "vcpkg not found. Please set \'VCPKG_ROOT\'.")
endif()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Disable clip examples and tests
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

# Add clip subdirectory to compile the library
add_subdirectory(third_party/clip)

# ---- Declare executable ----

add_executable(tb64_exe src/box.cpp src/tb64.cpp)
add_executable(tb64::exe ALIAS tb64_exe)

set_property(TARGET tb64_exe PROPERTY OUTPUT_NAME tb64)

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(tb64_exe PRIVATE Microsoft.GSL::GSL)

target_link_libraries(tb64_exe PRIVATE clip)
target_include_directories(tb64_exe PRIVATE ${VCPKG_INSTALLED_DIR}/${VCPKG_HOST_TRIPLET}/include)
target_include_directories(tb64_exe PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(tb64_exe PRIVATE ${CMAKE_SOURCE_DIR}/third_party)
target_compile_features(tb64_exe PRIVATE cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT tb64_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of tb64"
  )
endif()

include(cmake/dev-mode.cmake)

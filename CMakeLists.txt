cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    tb64
    DESCRIPTION "A niche tool to encode text in base64 3 times. (cpp version)"
    HOMEPAGE_URL "https://github.com/franktankbank/tb64-cpp"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Disable clip examples, tests, and image support
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")
set(CLIP_ENABLE_IMAGE OFF CACHE BOOL "Enables the support to copy/paste images")
set(CLIP_INSTALL OFF CACHE BOOL "Enable clip installation")

include(FetchContent)
FetchContent_Declare(
  clip
  GIT_REPOSITORY https://github.com/dacap/clip.git
  GIT_TAG        7a60eabaad167d9dc91eae439cb2c1ab9d49c6ba # v1.10
)

# Declare turbo-base64 dependency
FetchContent_Declare(
  turbo_base64
  GIT_REPOSITORY https://github.com/franktankbank/Turbo-Base64.git
  GIT_TAG        master  # or a specific tag/commit for stability
)

# Download and make the target available
FetchContent_MakeAvailable(turbo_base64)

FetchContent_GetProperties(clip)
if(NOT clip_POPULATED)
  FetchContent_Populate(clip)

  # This gives you:
  # - clip_SOURCE_DIR (path to source)
  # - clip_BINARY_DIR (out-of-source build dir for it)

  add_subdirectory(${clip_SOURCE_DIR} ${clip_BINARY_DIR})
endif()

# ---- Declare executable ----

add_executable(tb64_exe src/box.cpp src/tb64.cpp src/base64.cpp)
add_executable(tb64::exe ALIAS tb64_exe)

set_property(TARGET tb64_exe PROPERTY OUTPUT_NAME tb64)

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(tb64_exe PRIVATE Microsoft.GSL::GSL)

target_include_directories(tb64_exe PRIVATE ${turbo_base64_SOURCE_DIR})
target_link_libraries(tb64_exe PRIVATE base64)

target_link_libraries(tb64_exe PRIVATE clip)
target_include_directories(tb64_exe PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(tb64_exe PRIVATE ${clip_SOURCE_DIR})
target_compile_features(tb64_exe PRIVATE cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

include("${CMAKE_SOURCE_DIR}/cmake/CPackConfig.cmake")
include(CPack)

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

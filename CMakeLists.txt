cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    tb64-cpp
    DESCRIPTION "A niche tool to encode text in base64 3 times. (cpp version)"
    HOMEPAGE_URL "https://github.com/franktankbank/tb64-cpp"
    LANGUAGES CXX
)

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  find_package(Git)
  if (NOT GIT_FOUND)
    message(FATAL_ERROR "Git is required by this project")
  endif(NOT GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
    RESULT_VARIABLE RESULT
    OUTPUT_VARIABLE OUTPUT
  )
  string(REGEX MATCH "v[0-9].[0-9].[0-9]" LATEST_TAG "${OUTPUT}")
  string(REPLACE "\n" ""  LATEST_TAG "${LATEST_TAG}")
  string(REPLACE "v" ""  TB64_VERSION "${LATEST_TAG}")
else()
  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" CONTENTS)
  string(STRIP "${CONTENTS}" LATEST_TAG)
  string(REPLACE "v" ""  TB64_VERSION "${LATEST_TAG}")
endif()

configure_file("include/version.h.in" "version.h" @ONLY)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(HOMEBREW "Building through homebrew" OFF)

if(HOMEBREW)
  option(CLIP_ROOT "Clip root" "")
  option(TURBO_BASE64_ROOT "Turbo-Base64 root" "")
else()
  # Configure clip
  set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples" FORCE)
  set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests" FORCE)
  set(CLIP_ENABLE_IMAGE OFF CACHE BOOL "Enables the support to copy/paste images" FORCE)
  set(CLIP_INSTALL OFF CACHE BOOL "Enable clip installation" FORCE)
  set_property(CACHE CLIP_EXAMPLES PROPERTY TYPE INTERNAL)
  set_property(CACHE CLIP_TESTS PROPERTY TYPE INTERNAL)
  set_property(CACHE CLIP_ENABLE_IMAGE PROPERTY TYPE INTERNAL)
  set_property(CACHE CLIP_INSTALL PROPERTY TYPE INTERNAL)

  # Configure CLI11
  set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree." FORCE)
  set(CLI11_BOOST OFF CACHE BOOL "Turn on boost test (currently may fail with Boost 1.70)" FORCE)
  set(CLI11_BUILD_EXAMPLES OFF CACHE BOOL "Build CLI11 examples" FORCE)
  set(CLI11_BUILD_EXAMPLES_JSON OFF CACHE BOOL "Build CLI11 json example" FORCE)
  set(CLI11_BUILD_TESTS OFF CACHE BOOL "Build CLI11 tests" FORCE)
  set(CLI11_CUDA_TESTS OFF CACHE BOOL "" FORCE)
  set(CLI11_INSTALL OFF CACHE BOOL "Install the CLI11 folder to include during install process" FORCE)
  set(CLI11_PRECOMPILED ON CACHE BOOL "" FORCE)
  set(CLI11_SANITIZERS OFF CACHE BOOL "Download the sanitizers CMake config" FORCE)
  set(CLI11_SINGLE_FILE OFF CACHE BOOL "Generate a single header file" FORCE)
  set(CLI11_WARNINGS_AS_ERRORS OFF CACHE BOOL "Turn all warnings into errors (for CI)" FORCE)
  set_property(CACHE BUILD_TESTING PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_BOOST PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_BUILD_EXAMPLES PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_BUILD_EXAMPLES_JSON PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_BUILD_TESTS PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_CUDA_TESTS PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_INSTALL PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_PRECOMPILED PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_SANITIZERS PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_SINGLE_FILE PROPERTY TYPE INTERNAL)
  set_property(CACHE CLI11_WARNINGS_AS_ERRORS PROPERTY TYPE INTERNAL)

  # Configure Turbo Base64
  set(BUILD_APP OFF CACHE BOOL "Build executables" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
  set(FULLCHECK OFF CACHE BOOL "Enable full base64 checking" FORCE)
  set(NAVX512 OFF CACHE BOOL "Disable AVX512" FORCE)
  set(NCHECK OFF CACHE BOOL "Disable checking for faster decoding" FORCE)
  set_property(CACHE BUILD_APP PROPERTY TYPE INTERNAL)
  set_property(CACHE BUILD_SHARED_LIBS PROPERTY TYPE INTERNAL)
  set_property(CACHE FULLCHECK PROPERTY TYPE INTERNAL)
  set_property(CACHE NAVX512 PROPERTY TYPE INTERNAL)
  set_property(CACHE NCHECK PROPERTY TYPE INTERNAL)

  include(FetchContent)

  # Declare clip dependency
  FetchContent_Declare(
    clip
    GIT_REPOSITORY https://github.com/dacap/clip.git
    GIT_TAG        7a60eabaad167d9dc91eae439cb2c1ab9d49c6ba # v1.10
    EXCLUDE_FROM_ALL
  )

  # Declare turbo-base64 dependency
  FetchContent_Declare(
    turbo_base64
    GIT_REPOSITORY https://github.com/franktankbank/Turbo-Base64.git
    GIT_TAG        master  # master
    EXCLUDE_FROM_ALL
  )

  FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        bfffd37e1f804ca4fae1caae106935791696b6a9  # v2.6.1
    EXCLUDE_FROM_ALL
  )

  FetchContent_MakeAvailable(turbo_base64)
  FetchContent_MakeAvailable(cli11)
  FetchContent_MakeAvailable(clip)
endif()

# ---- Declare executable ----

add_executable(tb64_exe src/box.cpp src/tb64.cpp src/base64.cpp)
add_executable(tb64::exe ALIAS tb64_exe)

set_property(TARGET tb64_exe PROPERTY OUTPUT_NAME tb64)

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(tb64_exe PRIVATE Microsoft.GSL::GSL)

if(HOMEBREW)
  add_compile_definitions(WITH_HOMEBREW)

  # cli11
  find_package(CLI11 CONFIG REQUIRED)

  # clip
  target_include_directories(tb64_exe PRIVATE ${CLIP_ROOT}/include)
  target_link_directories(tb64_exe PRIVATE ${CLIP_ROOT}/lib)

  # turbo-base64
  target_include_directories(tb64_exe PRIVATE ${TURBO_BASE64_ROOT}/include)
  target_link_directories(tb64_exe PRIVATE ${TURBO_BASE64_ROOT}/lib)

  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(tb64_exe PRIVATE "-framework AppKit")
  endif()

else()
  target_include_directories(tb64_exe PRIVATE ${turbo_base64_SOURCE_DIR})
  target_include_directories(tb64_exe PRIVATE ${clip_SOURCE_DIR})
endif()
target_link_libraries(tb64_exe PRIVATE clip)
target_link_libraries(tb64_exe PRIVATE base64)
target_link_libraries(tb64_exe PRIVATE CLI11::CLI11)
target_include_directories(tb64_exe PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(tb64_exe PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(tb64_exe PRIVATE cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

include("${CMAKE_SOURCE_DIR}/cmake/CPackConfig.cmake")
include(CPack)

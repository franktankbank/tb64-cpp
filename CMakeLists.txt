cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    tb64
    DESCRIPTION "A niche tool to encode text in base64 3 times. (cpp version)"
    HOMEPAGE_URL "https://github.com/franktankbank/tb64-cpp"
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(HOMEBREW "Building through homebrew" OFF)

if(HOMEBREW)
  option(CLIP_ROOT "Clip root" "")
  option(TURBO_BASE64_ROOT "Turbo-Base64 root" "")
  option(CLI11_ROOT "CLI11 root" "")
else()
  # Disable clip examples, tests, and image support
  set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
  set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")
  set(CLIP_ENABLE_IMAGE OFF CACHE BOOL "Enables the support to copy/paste images")
  set(CLIP_INSTALL OFF CACHE BOOL "Enable clip installation")

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
  target_include_directories(tb64_exe PRIVATE ${TURBO_BASE64_ROOT}/include)
  target_link_directories(tb64_exe PRIVATE ${TURBO_BASE64_ROOT}/lib)

  add_compile_definitions(WITH_HOMEBREW)

  target_include_directories(tb64_exe PRIVATE ${CLIP_ROOT}/include)
  target_link_directories(tb64_exe PRIVATE ${CLIP_ROOT}/lib)

  target_include_directories(tb64_exe PRIVATE ${CLI11_ROOT}/include)
  target_link_directories(tb64_exe PRIVATE ${CLI11_ROOT}/lib)

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
target_include_directories(tb64_exe PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(tb64_exe PRIVATE cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

include("${CMAKE_SOURCE_DIR}/cmake/CPackConfig.cmake")
include(CPack)

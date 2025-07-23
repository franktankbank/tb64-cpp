include(cmake/folders.cmake)

add_custom_target(
    run-exe
    COMMAND tb64_exe
    VERBATIM
)
add_dependencies(run-exe tb64_exe)

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)

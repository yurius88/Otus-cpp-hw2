cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(homework_2 VERSION ${PROJECT_VESRION})

add_executable(homework_2_cli main.cpp)

if (MSVC)
    target_compile_options(homework_2_cli PRIVATE
        /W4
    )   
else ()
    target_compile_options(homework_2_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )   
endif()

install(TARGETS homework_2_cli RUNTIME DESTINATION bin)

add_custom_command(
        TARGET homework_2_cli POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/ip_filter.tsv
                ${PROJECT_BINARY_DIR}/ip_filter.tsv)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)


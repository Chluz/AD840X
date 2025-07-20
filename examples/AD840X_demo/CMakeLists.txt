# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)
set(component_srcs "main.c")
idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS ".")

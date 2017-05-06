cmake_minimum_required (VERSION 2.6)
project (smshell)
add_executable(smshell smshell.c)
set(CMAKE_C_FLAGS "-std=c99")

add_custom_target(run
     COMMAND smshell
     WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

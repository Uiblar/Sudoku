cmake_minimum_required(VERSION 3.10)
project(Sudoku C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Source files
set(SRC_FILES
        main.c
        sudoku_cli.c
        sudoku_hint.c
        sudoku_solver.c
        save_load.c
        save_load.h
)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_executable(sudoku ${SRC_FILES})

# Optional: target properties
set_target_properties(sudoku PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

cmake_minimum_required(VERSION 3.20)
project(alleno)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/database.cpp
    src/workout.cpp
    src/file.cpp
)

add_executable(alleno ${SOURCES})

target_compile_options(alleno PRIVATE -Werror -Wall -Wextra)
target_include_directories(alleno PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_libraries(alleno PRIVATE ${SQLite3_LIBRARIES})


cmake_minimum_required(VERSION 3.20)
project(DungeonDelvers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Enable testing" OFF)

find_package(SDL2 REQUIRED)

add_executable(DungeonDelvers
    src/main.cpp
    src/game/Game.cpp
)

target_include_directories(DungeonDelvers PRIVATE
    ${SDL2_INCLUDE_DIRS}
    include
)

target_link_libraries(DungeonDelvers PRIVATE ${SDL2_LIBRARIES})

if (WIN32)
    target_compile_definitions(DungeonDelvers PRIVATE NOMINMAX)
endif()

if (MSVC)
    target_compile_options(DungeonDelvers PRIVATE /W4 /permissive-)
else()
    target_compile_options(DungeonDelvers PRIVATE -Wall -Wextra -Wpedantic)
endif()

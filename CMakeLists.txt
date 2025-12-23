cmake_minimum_required(VERSION 3.20)
project(DungeonDelvers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)

add_executable(DungeonDelvers
  src/main.cpp

  src/core/App.h
  src/core/App.cpp
  src/core/Log.h
  src/core/Log.cpp
  src/core/Time.h
  src/core/Time.cpp

  src/platform/SDLContext.h
  src/platform/SDLContext.cpp
  src/platform/Input.h
  src/platform/Input.cpp

  src/render/GL33.h
  src/render/GL33.cpp
  src/render/Camera.h
  src/render/Camera.cpp
  src/render/Shader.h
  src/render/Shader.cpp
  src/render/Texture.h
  src/render/Texture.cpp
  src/render/SpriteBatch.h
  src/render/SpriteBatch.cpp

  src/sim/World.h
  src/sim/World.cpp
  src/sim/DungeonCore.h
  src/sim/DungeonCore.cpp
  src/sim/Spawner.h
  src/sim/Spawner.cpp
  src/sim/Mob.h
  src/sim/Invader.h
)

target_include_directories(DungeonDelvers PRIVATE src)

# IMPORTANT: no SDL2main
target_link_libraries(DungeonDelvers PRIVATE SDL2::SDL2 opengl32)

# Copy assets to build output folder on build
add_custom_command(TARGET DungeonDelvers POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:DungeonDelvers>/assets
)

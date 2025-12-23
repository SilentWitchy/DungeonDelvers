# Dungeon Delvers (Skeleton)

Windows-only. C++20 + SDL2 + OpenGL 3.3. Minimal dependencies: SDL2 only.

## Controls
- WASD: pan camera
- Q/E: zoom out/in
- Mouse wheel: change active Z-level
- PageUp/PageDown: change active Z-level
- Esc: quit

## Build
Requires SDL2 dev package.

### CMake (example)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

If CMake can’t find SDL2, set SDL2_DIR to the folder containing SDL2Config.cmake,
or install SDL2 via vcpkg (recommended for least friction).

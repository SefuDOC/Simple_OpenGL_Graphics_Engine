# Vroom — OpenGL + Lua

Small real-time 3D engine project.

## Features
- OpenGL rendering (GLFW + GLAD + GLM)
- Scene graph (`SceneNode`) + hierarchical transforms
- XML scene loading (tinyxml2) + OBJ loading (tinyobjloader)
- Textures (stb) + shader wrapper
- Input + camera
- ImGui simple UI
- Lua scripting + event queue (`EventBus`) dispatched to Lua

## Setup (Windows, CMake-GUI + vcpkg)
This project uses **vcpkg** for dependencies.

1) Install deps with vcpkg:
- glfw3, glad, glm, tinyobjloader, tinyxml2, imgui, stb, lua

2) Open **CMake-GUI**:
- **Where is the source code:** the repo folder
- **Where to build the binaries:** `build/` (create it)

3) Add CMake entry:
- **Name:** `CMAKE_TOOLCHAIN_FILE`
- **Type:** `FILEPATH`
- **Value:** `C:/vcpkg/scripts/buildsystems/vcpkg.cmake`

4) Configure → Generate → Open Project → Build/Run.

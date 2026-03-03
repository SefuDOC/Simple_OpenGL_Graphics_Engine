cmake_minimum_required(VERSION 3.15)
project(Vroom)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

find_package(Lua REQUIRED)

message(STATUS "GLFW include dir: ${glfw3_DIR}")
message(STATUS "GLM include dir: ${glm_DIR}")
message(STATUS "TinyObjLoader include dir: ${tinyobjloader_DIR}")
message(STATUS "STB include dir: ${Stb_INCLUDE_DIRS}")
message(STATUS "Lua include dir: ${LUA_INCLUDE_DIR}")
message(STATUS "Lua libraries: ${LUA_LIBRARIES}")

add_executable(Vroom 
	src/main.cpp
	src/Mesh.cpp
	src/Mesh.h
	src/Shader.cpp
	src/Shader.h
	src/ObjLoader.h
	src/ObjLoader.cpp
	src/Input.h
	src/Input.cpp
	src/Camera.h
	src/Camera.cpp
	src/Texture.cpp
	src/Texture.h
	src/Scene.h
	src/SceneNode.h
	src/Transform.h
	src/Frustum.h
	src/SceneLoader.h
	src/ScriptSystem.cpp
	src/ScriptSystem.h
	src/EventBus.h
	)

file(COPY ${CMAKE_SOURCE_DIR}/src/Shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(Vroom PRIVATE ${LUA_INCLUDE_DIR})

target_link_libraries(Vroom PRIVATE
    glad::glad
    glfw
    glm::glm
    tinyobjloader::tinyobjloader
    tinyxml2::tinyxml2
    imgui::imgui
    ${LUA_LIBRARIES}
)
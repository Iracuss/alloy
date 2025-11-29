cmake_minimum_required(VERSION 3.15)
project(alloy_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------
# GLFW
# --------------------------------
add_subdirectory(libs/glfw)

# --------------------------------
# GLAD
# --------------------------------
add_library(glad libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

# --------------------------------
# Source files
# --------------------------------
file(GLOB CORE_FILES "engine/core/*.cpp")
file(GLOB GRAPHIC_FILES "engine/graphics/*.cpp")
file(GLOB ASSET_FILES "engine/assets/*.cpp")
file(GLOB MATH_FILES "engine/math/*.cpp")

add_executable(alloy_engine ${CORE_FILES} ${GRAPHIC_FILES} ${ASSET_FILES} ${MATH_FILES})

# --------------------------------
# GLM (header-only)
# --------------------------------
target_include_directories(alloy_engine PUBLIC libs/glm)
target_include_directories(alloy_engine PUBLIC libs/stb)

# --------------------------------
# Link libs
# --------------------------------
target_link_libraries(alloy_engine
    glfw
    glad
)

# macOS frameworks
if(APPLE)
    find_library(COCOA Cocoa)
    find_library(OpenGL OpenGL)
    find_library(IOKIT IOKit)
    target_link_libraries(alloy_engine ${COCOA} ${OpenGL} ${IOKIT})
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(alloy_engine OpenGL::GL)
endif()

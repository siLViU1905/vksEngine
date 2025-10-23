cmake_minimum_required(VERSION 3.31)
project(vksEngine)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG")
elseif (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif ()

#if your vulkan version is different specify it here
set(VULKAN_SDK "C:/VulkanSDK/1.4.328.1")

set(Vulkan_ROOT "${VULKAN_SDK}")

set(Vulkan_INCLUDE_DIR "${VULKAN_SDK}/Include")

set(GLFW_DIR "${CMAKE_SOURCE_DIR}/external/GLFW")

set(GLM_DIR "${CMAKE_SOURCE_DIR}/external/glm")

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

set(IMNODES_DIR "${CMAKE_SOURCE_DIR}/external/imnodes")

set(STB_IMAGE_DIR "${CMAKE_SOURCE_DIR}/external/stbimage")

set(ASSIMP_DIR "${CMAKE_SOURCE_DIR}/external/assimp")

find_package(Vulkan REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_DIR})

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${ASSIMP_DIR})

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

set(IMNODES_SOURCES
        ${IMNODES_DIR}/imnodes.cpp
)

add_executable(vksEngine
        main.cpp

        app/handlers/VulkanHandler.cpp

        app/Window.cpp

        app/Application.cpp

        app/objects/Vertex.cpp

        app/objects/Camera.cpp

        app/objects/texture/Texture.cpp

        app/objects/mesh/Mesh.cpp

        app/objects/UniformBuffer.cpp

        app/objects/lights/Light.cpp

        app/objects/lights/DirectionalLight.cpp

        app/objects/lights/PointLight.cpp

        ${IMGUI_SOURCES}

        ${IMNODES_SOURCES}

        app/handlers/ImGuiHandler.cpp

        app/objects/menus/PointLightMenu.cpp

        app/objects/menus/DirectionalLightMenu.cpp

        app/objects/menus/MeshMenu.cpp

        app/scene/Scene.cpp

        app/scene/FileExplorer.cpp

        app/scene/components/MeshComponent.cpp

        app/scene/components/DirectionalLightComponent.cpp

        app/scene/components/PointLightComponent.cpp

        app/objects/timer/Clock.cpp

        app/objects/input/Key.cpp

        app/handlers/InputHandler.cpp

        app/objects/input/Button.cpp

        app/objects/menus/Menu.cpp

        app/scene/menus/SceneFunctionsMenu.cpp

        app/scene/menus/SceneComponentsMenu.cpp

        app/scene/menus/SceneComponentPropertiesMenu.cpp

        app/scene/components/ComponentType.cpp

        app/objects/texture/TextureType.cpp

        app/scene/menus/SceneInfoMenu.cpp

        app/scene/menus/SceneEventsMenu.cpp

        app/objects/Material.cpp
)


target_include_directories(vksEngine PRIVATE
        ${Vulkan_INCLUDE_DIR}
        ${GLFW_DIR}
        ${GLM_DIR}
        ${IMGUI_DIR}
        ${IMNODES_DIR}
        ${IMGUI_DIR}/backends
        ${STB_IMAGE_DIR}
)

target_link_libraries(vksEngine PRIVATE
        Vulkan::Vulkan
        glfw
        assimp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(vksEngine PRIVATE stdc++exp)
    target_link_options(vksEngine PRIVATE 
        -static-libgcc 
        -static-libstdc++
        -Wl,-Bstatic
        -lstdc++
        -lwinpthread
    )
endif()

set_target_properties(vksEngine PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

add_custom_command(TARGET vksEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:glfw>
        $<TARGET_FILE_DIR:vksEngine>
)

add_custom_command(TARGET vksEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:assimp>
        $<TARGET_FILE_DIR:vksEngine>
)

add_custom_command(TARGET vksEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/app/shaders/compiled
        $<TARGET_FILE_DIR:vksEngine>/shaders
        COMMENT "Copying shaders to build directory"
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Vulkan SDK: ${VULKAN_SDK}")
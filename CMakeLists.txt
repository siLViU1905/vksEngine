cmake_minimum_required(VERSION 3.31)
project(vksEngine)

set(CMAKE_CXX_STANDARD 23)

#if your vulkan version is different specify it here
set(VULKAN_SDK "C:/VulkanSDK/1.4.328.1")

set(Vulkan_ROOT "${VULKAN_SDK}")

set(Vulkan_INCLUDE_DIR "${VULKAN_SDK}/Include")

set(GLFW_DIR "${CMAKE_SOURCE_DIR}/external/GLFW")

set(GLM_DIR "${CMAKE_SOURCE_DIR}/external/glm")

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

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

add_executable(vksEngine
        main.cpp

        app/handlers/VulkanHandler.cpp

        app/Window.cpp

        app/Application.cpp

        app/objects/Vertex.cpp

        app/objects/Camera.cpp

        app/objects/Texture.cpp

        app/objects/Mesh.cpp

        app/objects/UniformBuffer.cpp

        app/objects/lights/Light.cpp

        app/objects/lights/DirectionalLight.cpp

        app/objects/lights/PointLight.cpp

        ${IMGUI_SOURCES}

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
)


target_include_directories(vksEngine PRIVATE
        ${Vulkan_INCLUDE_DIR}
        ${GLFW_DIR}
        ${GLM_DIR}
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${STB_IMAGE_DIR}
)

target_link_libraries(vksEngine
        Vulkan::Vulkan
        glfw
        assimp
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

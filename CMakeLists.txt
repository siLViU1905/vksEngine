cmake_minimum_required(VERSION 3.31)
project(vksEngine)

set(CMAKE_CXX_STANDARD 23)

set(VULKAN_SDK "C:/VulkanSDK/1.4.321.1")

set(Vulkan_ROOT "${VULKAN_SDK}")

set(GLFW_INCLUDE_DIRS "C:/GLFW/include")

set(GLM_INCLUDE_DIRS "C:/glm/include")

set(STB_IMAGE_INCLUDE_DIRS "C:/stbimage/include")

set(ASSIMP_INCLUDE_DIRS "C:/assimp/include")

set(ASSIMP_LIBRARIES "C:/assimp/lib/assimp-vc143-mtd.lib")

set(IMGUI_INCLUDE_DIRS "C:/imgui/docking")

if (MSVC)
    set(GLFW_LIBRARIES "C:/GLFW/lib/glfw3.lib")
elseif (MINGW OR CMAKE_COMPILER_IS_GNUCXX)
    set(GLFW_LIBRARIES "C:/GLFW/lib/libglfw3.a")
endif ()

find_package(Vulkan REQUIRED)

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

        ${IMGUI_INCLUDE_DIRS}/imgui/imgui.cpp

        ${IMGUI_INCLUDE_DIRS}/imgui/imgui_draw.cpp

        ${IMGUI_INCLUDE_DIRS}/imgui/imgui_widgets.cpp

        ${IMGUI_INCLUDE_DIRS}/imgui/imgui_tables.cpp

        ${IMGUI_INCLUDE_DIRS}/imgui/imgui_impl_glfw.cpp

        ${IMGUI_INCLUDE_DIRS}/imgui/imgui_impl_vulkan.cpp

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
        ${GLFW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${STB_IMAGE_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIRS}
        ${IMGUI_INCLUDE_DIRS}
)

target_link_libraries(vksEngine
        Vulkan::Vulkan
        ${GLFW_LIBRARIES}
        ${ASSIMP_LIBRARIES}
)

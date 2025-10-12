# VKSEngine

Simple 3D graphics engine with a scene and components(meshes, lights).  

## About the Engine

It contains shaders without if's for better parallelization.
The engine is using multiple threads for command buffer recording , updating scene components and updating unifrom buffers.
Thanks to ImGui all UI is also rendered on the GPU.

## 🏗️ Architecture & Design Patterns

I took an OOP aproach.

### Class Hierarchy

```
# VKSEngine

Simple 3D graphics engine with a scene and components(meshes, lights).  

## About the Engine

It contains shaders without if's for better parallelization.
The engine is using multiple threads for command buffer recording , updating scene components and updating unifrom buffers.
Thanks to ImGui all UI is also rendered on the GPU.

## 🏗️ Architecture & Design Patterns

I took an OOP aproach.

### Class Hierarchy

```
vksengine
│   CMakeLists.txt
│   main.cpp
│
├───app
│   │   Application.cpp
│   │   Application.h
│   │   Window.cpp
│   │   Window.h
│   │
│   ├───handlers
│   │       ImGuiHandler.cpp
│   │       ImGuiHandler.h
│   │       InputHandler.cpp
│   │       InputHandler.h
│   │       VulkanHandler.cpp
│   │       VulkanHandler.h
│   ├───objects
│   │   │   Camera.cpp
│   │   │   Camera.h
│   │   │   Mesh.cpp
│   │   │   Mesh.h
│   │   │   Texture.cpp
│   │   │   Texture.h
│   │   │   UniformBuffer.cpp
│   │   │   UniformBuffer.h
│   │   │   Vertex.cpp
│   │   │   Vertex.h
│   │   │
│   │   ├───input
│   │   │       Button.cpp
│   │   │       Button.h
│   │   │       Key.cpp
│   │   │       Key.h
│   │   │
│   │   ├───lights
│   │   │       DirectionalLight.cpp
│   │   │       DirectionalLight.h
│   │   │       Light.cpp
│   │   │       Light.h
│   │   │       PointLight.cpp
│   │   │       PointLight.h
│   │   │
│   │   ├───menus
│   │   │       ComponentMenu.h
│   │   │       DirectionalLightMenu.cpp
│   │   │       DirectionalLightMenu.h
│   │   │       Menu.cpp
│   │   │       Menu.h
│   │   │       MeshMenu.cpp
│   │   │       MeshMenu.h
│   │   │       PointLightMenu.cpp
│   │   │       PointLightMenu.h
│   │   │
│   │   └───timer
│   │           Clock.cpp
│   │           Clock.h
│   │           TimeType.h
│   │
│   ├───scene
│   │   │   FileExplorer.cpp
│   │   │   FileExplorer.h
│   │   │   Scene.cpp
│   │   │   Scene.h
│   │   │   SceneComponent.h
│   │   │
│   │   ├───components
│   │   │       ComponentEntry.h
│   │   │       ComponentType.cpp
│   │   │       ComponentType.h
│   │   │       DirectionalLightComponent.cpp
│   │   │       DirectionalLightComponent.h
│   │   │       MeshComponent.cpp
│   │   │       MeshComponent.h
│   │   │       PointLightComponent.cpp
│   │   │       PointLightComponent.h
│   │   │
│   │   ├───menus
│   │   │       SceneComponentPropertiesMenu.cpp
│   │   │       SceneComponentPropertiesMenu.h
│   │   │       SceneComponentsMenu.cpp
│   │   │       SceneComponentsMenu.h
│   │   │       SceneFunctionsMenu.cpp
│   │   │       SceneFunctionsMenu.h
│   │   │
│   │   └───ubocomponents
│   │           UBOcounters.h
│   │           UBOvp.h
│   │
│   └───shaders
│       │   compile.bat
│       │   glslc.exe
│       │
│       ├───compiled
│       │   ├───complexmesh
│       │   │       ComplexMeshShaderFrag.spv
│       │   │       ComplexMeshShaderVert.spv
│       │   │
│       │   └───simplemesh
│       │           SimpleMeshShaderFrag.spv
│       │           SimpleMeshShaderVert.spv
│       │
│       └───glsl
│           ├───complexmesh
│           │       ComplexMeshShader.frag
│           │       ComplexMeshShader.vert
│           │
│           └───simplemesh
│                   SimpleMeshShader.frag
│                   SimpleMeshShader.vert
|

```
## 🛠️ Technical Implementation

### Core Technologies
- **C++23**
- **Vulkan** for custom graphics rendering
- **ImGui** for UI

```
## 🛠️ Technical Implementation

### Core Technologies
- **C++ 20** language features
- **Vulkan** for custom graphics rendering
- **ImGui** for UI

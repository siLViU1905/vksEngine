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
├───vksEngine
│   ├───app
│   │   ├───handlers
│   │   ├───objects
│   │   │   ├───input
│   │   │   ├───lights
│   │   │   ├───menus
│   │   │   └───timer
│   │   ├───scene
│   │   │   ├───components
│   │   │   ├───menus
│   │   │   └───ubocomponents
│   │   └───shaders
│   │       ├───compiled
│   │       │   ├───complexmesh
│   │       │   └───simplemesh
│   │       └───glsl
│   │           ├───complexmesh
│   │           └───simplemesh

```
## 🛠️ Technical Implementation

### Core Technologies
- **C++ 20** language features
- **Vulkan** for custom graphics rendering
- **ImGui** for UI

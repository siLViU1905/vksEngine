# VKSEngine

Simple 3D graphics engine with a scene and components(meshes, lights).

## About the Engine

It contains shaders without if's for better parallelization.
The engine is using multiple threads for command buffer recording, updating scene components and updating uniform buffers.
Thanks to ImGui all UI is also rendered on the GPU.

## 🚀 Installation & Setup

### Prerequisites

- **CMake** 3.31 or higher
- **C++23** compatible compiler (MSVC 2022, GCC 13+, or Clang 16+)
- **Vulkan SDK** 1.3 or higher
- **Git** with submodule support

### Windows Installation

1. **Install Vulkan SDK**
    - Download from [vulkan.lunarg.com](https://vulkan.lunarg.com/)
    - Install to default location or set `VULKAN_SDK` environment variable

2. **Clone the repository with submodules**
   ```bash
   git clone --recursive https://github.com/siLViU1905/vksEngine.git
   cd vksEngine
   ```

3. **Configure and build**
   ```bash
   cmake -B build
   cmake --build build
   ```

4. **Run the engine**
   ```bash
   build/Debug/vksEngine.exe
   ```
   or
   ```bash
   build/Release/vksEngine.exe
   ```

### Troubleshooting

**Missing submodules:**
```bash
git submodule update --init --recursive
```

**CMake can't find Vulkan SDK:**
- Windows: Set environment variable `VULKAN_SDK` to your installation path
- Linux/macOS: Install vulkan-headers and vulkan-loader packages

**Missing DLLs on Windows:**
- The build process automatically copies required DLLs (Assimp, GLFW)
- If missing, run `cmake --build build` again

## 🏗️ Architecture & Design Patterns

I took an OOP approach.

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
│   └───external (git submodules)
│       ├───glfw
│       ├───glm
│       ├───imgui (docking branch)
│       ├───assimp
│       └───stbimage
```

## 🛠️ Technical Implementation

### Core Technologies
- **C++23** language features
- **Vulkan 1.4** for custom graphics rendering
- **ImGui** (docking branch) for UI
- **GLFW** for window management
- **GLM** for mathematics
- **Assimp** for model loading
- **stb_image** for texture loading

### Dependencies (Managed via Git Submodules)
All external dependencies are included as git submodules and built automatically:
- GLFW 3.x
- GLM 1.x
- Dear ImGui (docking branch)
- Assimp 5.x
- stb_image

## 📝 Development

### Building for Development
```bash
# Debug build with validation layers
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Release build optimized
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Updating Dependencies
```bash
# Update all submodules to latest commits
git submodule update --remote --merge
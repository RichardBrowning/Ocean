# Vulkna Game Engine with Practical Components

## Abstract
This is a project report for the Capstone Project. The project develops an independent game engine using Vulkan graphics API. The game engine is capable of performing some basic tasks of a gram engine. The project also implements some components based on the base engine. The components includes user input listener, lighting system, etc. A simple shader system was also implemented as a central part of the project. 

## Introduction

### Part 1 Overview: Vulkan Game Engine

The game engien uses Vulkan as the graphics API and was written in c++. Vulkan is a cross-platform graphics API by Khoronos group. The API has many advantages over the previous generation of graphics API such as OpenGL. Vulkan is a low-level API which means it is closer to the hardware and provides more control over the hardware. Vulkan has advanced concurrency support which provides better performance than its predecessor OpenGL does. Vulkan can run on different platforms such as Windows, Linux, macOS, etc.

The procesure of developing the game engine using Vulkan is complicated comparing with using other APIs. Vulkan is a low-level API which provides marely some basic functions that manage core componets including the device, the instance, the graphics pipeine, the swapchain, and the renderpass, etc. All higher-level fucntions need to be implemented. 

The development of the engine begins with a basic structure. The basic struacture includes 4 component, the initialization process, the game loop, and the clean up process. The initialization process is responsible for creating the instance, the device, the swapchain, the renderpass, the graphics pipeline, and the command pool. The game loop is responsible for rendering the scene and handling user input. The clean up process is responsible for destroying the created window, instance, logical device, etc.

#### Initialization Process
The initialization process contains the following mean components:
1. Create the window
2. Create the Vulkan Instance
3. Create the surface
4. Create the physical device
5. Create the logical device
6. Create the swapchain
7. Create the renderpass
8. Create the graphics pipeline
9. Create the command pool
10. Create the command buffers

The window used here is the GLFW window object which is a OpenGL window but is the official default window interface for Vulkan. After creating the window, the GLFW windows instance was passed to the device constructor. The device class was composed by Branden Galea. It is reseponsible for creating the instance, as well as selecting optimal physical GPU and create logical device with it. The device class also contains some helper functions for creating the swapchain, the renderpass, the graphics pipeline, and the command pool. The command buffers are created in the main loop. 
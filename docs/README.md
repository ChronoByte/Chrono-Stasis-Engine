
# **Chrono Stasis Engine** 

Chrono Stasis Engine is a 3D game engine developed/coded from scratch with OpenGL in C++.

The engine contains all the basic [sub-systems]() of a modern 3D engine and we have also added support to handle [particle system](). Although this project has been done with educational purposes it will be used (along other engines from our class) to produce a game in the Project 3 subject in the first half of 2020.

This project has been done by two students of CITM - UPC (Terrasa, Spain) for the subject Game Engines.

> ### *Download the last version of the engine [HERE](https://github.com/ChronoByte/Chrono-Stasis-Engine/releases/tag/Assigment3.0)*

***

## Terminology & Origin
As his name says, Chrono Stasis comes from the greek term **Chronostasis** (*chrónos* "Time" and *stásis* "standing"), a temporal illusion in which the first impression following the introduction of a new event or task-demand to the brain can appear to be extended in time. 

That is to say, is an illusion where that person perceives an instance of time as an extremely long or even infinite state of time, hence the word "standing time." Therefore, we can affirm that it is an allegory in the computational world and videogames, where for a user or player an instance of time is a millisecond of time spent playing, whereas for us it is an instance where millions of instructions, operations and calculations happen internally in the engine. 

Metaphorically for us (software engineers and game developers), we live in a chronostasis loop measuring and observing the behavior and time performance in each frame at all the time until the game ends. In addition, we decide to make Chrono Stasis as compound word because it contains the acronym of our names: **C**hrono (Carlos) & **S**tasis (Sebas).

***
## **Team**


<a href="url"><img src="https://raw.githubusercontent.com/ChronoByte/Chrono-Stasis-Engine/master/docs/foto_carlos.jpg" align="right" height="200" width="200">
</a> 

### _Peña Hernando, Carlos_

* Github Account:
   * [GitHub account](https://github.com/CarlosUPC)
   
 * Resposabilities:  
    * Own Format File
    * Resource Manager
    * Scene Serialization
    * File System
    * Refresh System (detection of dropped files)
    * Counting Reference
    * Asset Project Window 
    * Unity like camera controls
    * Folder Configuration (creation, renaming, deletion)
    * .Meta Checking System (to detect new files and generate his own files + resources)
    * Component Particle System
    * Component Particle System Serialization as a template file
    * Component Particle System Serialization within a scene file
    * Texture Resource Browser (to configure particle material)


<a href="url"><img src="https://github.com/ChronoByte/Chrono-Stasis-Engine/blob/master/docs/foto_sebas.JPG" align="right" height="200" width="200" ></a>

### _López Tenorio, Sebastián_

* Github Account:
   * [GitHub account](https://github.com/Sebi-Looper)
  
* Resposabilities: 
    * Mouse Picking
    * Unity like camera controls
    * ImgGui editor
    * Component Camera
    * Frustum Culling
    * Octree System
    * ImGuizmo UI
    * Bounding Box 
    * Z-Buffer
    * Real-time multiple views scene rendering
    * Time Management (Game Mode with Start / Pause / Step based on dt modifier)
    * Component Particle System
    * Particle Emmitter
    * Particle Burst
    * Particle Structure
    
    
***

## **MAIN CORE SUB-SYSTEMS**

Our game engine code is structured in modules. The main module (*csApp.cpp*) manages all the other modules calling in a loop its respective awake, preupdate, update, postupdate, cleanup that they share thorugh a base class *csModule*. 

On the following sections we will explain the main core sub-systems of our engine.

### 3D Renderer

Our engine has a module renderer that handles all the drawing of the program. We don't use shaders. Instead we use OpenGL Vertex Arrays with indices (except for the debug draw of the primitives which are rendered in direct mode). Everything is drawn in the post update of the module. There, the gameobjects are filtered and we only render the ones that are inside the fustrum. 

### Geometry loader

When we detect a file in our Assets/Models/ directory we read the file with Assimp and extract all the information we need (vertices, indices, uvs, name...) in order to render the mesh on screen. Then we save the resource in our own binary format.

### Gameobject structure

Gameobjects have a tree structure so the transformations affect all the children correctly. Each gameobject has a transform component and if it has a mesh, we calculate its AABB so that it can be filtered by the frustum culling or the quadtree. We follow the same structure as Unity, using the Decoupling Design Pattern.

### Binary files format

All assets in Assets/ are saved as resources in our own binary format inside the Library directory. Doing this we only save the data we need and when we have to read the process is easier and much faster than if we had to read the original asset file such as .fbx or .png. A new resource in our own binary format is generated each time we detect a new assets in Assets/.

Our engine accepts the following assets formats:
* Meshes: FBX and OBJ.
* Textures: png, jpg, jpeg, dds and tga.

### 3D Camera & frustum culling

Our engine has two cameras: the editor camera (which can be controlled as in any other 3D software) and the game camera. The game camera is treated as a component of the gameobject 'Main Camera' and has frusutm culling using a quadtree so gameobjects outisde its frustum won't be drawn. 

### Quadtree

We've implement a quadtree container in our engine to speed things up. All the gameobjects in the scene that are marked as static in the inspector are inserted inside the quadtree. The quadtree will subdivide itself in four as many times as it needs to redistribute the gameobjects and match the specified bucket size. Doing this we can safely discard gameobjects whose quadtree node does not intersect with the camera frustum speeding up the frustum culling process.

### Data serialization

Our engine serializes the entire hierarchy of objects scene through all the gameobjects and their corresponding components. For this we use UUIDs (Universally unique identifier) to be able to handle all the information when serializing and deserializing a scene. For our engine the UUIDs are generated by random numbers of 32 bits.

When using the engine, one of the most important features for the user is to be able of saving the scene in the computer. By doing this the user can save all his work to continue later and share it with other people.

### Importer
Once the user adds a new mesh to the scene we store it in our own file format. By doing this we acomplish great performance when loading scenes, for example when loading a level.

### Mouse picking

Mouse picking is done using the Raycast method. When the left mouse button is clicked, the camera module calculates a ray that will start in the corresponding clicked point in the near plane and will project until it reaches the far plane. Once we have that we just need to do:

- Collect all the gameobjects (both dynamic and static) that are inside the fustrum and intersect with the projected ray. The static object collection is accelerated with the quadtree.
- Loop through all this gameobjects. If they have mesh we calculate all its triangles and for each of them we test collision against the raycast (in the local space of the gameobject). We do this until we've loop through all gameobjects.


### Time managment & game mode

When the engine is in game mode, the scene is seen though the game camera (which has frustum culling). We also keep an internal game clock (aside from the app clock) that will only run when the game mode is activated. Its dt will be passed to all the updates instead of the real time dt from the app.

User can play, pause and tick and even modify the speed of the time in game. To manAge all of the time propierties we have a time manager to set the time in all levels of the engine .

### Resource Management

In our resource system we have imitated the management of resources that Unity has. When starting the engine we generate specific resources (Mesh, material, bones ...) and in case a resource is duplicated we only send the information (Vertices of a mesh for example) to the gpu once. As in unity, we generate .meta files that link the original file (.png) with the own format file that we generate once it has been imported (.dds). In addition, our .meta files also store information on how to perform the import.

We have developed a resource manager to improve the performance, especially the memory performance, in the engine. No matter how many times a mesh is in the scene, it will only be loaded once in memory.
***

### Octree Development

We use an adaptable Octree to speed up the drawing process and mouse picking, quickly discarding irrelevant GameObjects when we apply the camera culling.

## User Interface
We have used the ImGui library for our Engine UI.
We have been able to do all the menus, hierarchy window, buttons and texts. It has helped us with all the Engine UI options 

# **HIGH-LEVEL SYSTEM: PARTICLE SYSTEM**

We have developed a particle system for our engine, but before we started to develop the particulate system we had to add different  engine functionalities like the billboard and transparencies 

## Billboarding

You can add a component Billboard to any GameObject.
Once the component is added you can choose between one of the following alignments:

### Screen Aligned
Objects follows camera rotation

<p align="center">
<img src="https://media.giphy.com/media/SsNinkZCb9dyjwKgXA/giphy.gif" width="400">
</p>

### World Aligned
Objects look towards the camera. 

<p align="center">
<img src="https://media.giphy.com/media/H1wp7aKn9slX279Ei8/giphy.gif" width="400">
</p>

### Axially Aligned
Objects look towards the camera, but user can lock one of the axis. 

<p align="center">
<img src="https://media.giphy.com/media/gH8142A7h5guaLSkE9/giphy.gif" width="400">
</p>


### Transparencies

### Blending Color

### Particle Properties

### Particle Shape

### Particle Color

### Particle Burst

### Particle Texture

### Particle Serialization 

## **Engine Demo**

<iframe width="1280" height="720" src="" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

***

## Installation

* Go to our github release page: [Click Here!](https://github.com/ChronoByte/Chrono-Stasis-Engine/releases)
* Download the last version **.zip**
* Extract .zip files inside a folder 
* Execute **.exe** file

***

## **Notes on performance**

There are three main known cases where the overall perfomance might decrease significantly:


***

## **Controls & Instructions**

~~~~~~~~~~~~~~~
Controls

GameObjects
 - Right Click: Select it
 - Left Click (Hierarchy): Delete it
 
Hierarchy panel:
  - Create -> Empty Object
  - Create -> 3D Object (cube, shpere, etc)
  - Create -> Camera (enables Game view)
  - Create -> Efects -> Particle System

Gizmos
- W/E/R: Change gizmo to Translate, Rotation and Scale

Scene Serialization

 - Save Scene (File bar menu): Select Scenes folder and input a name (without typing extension!) -> Save button
 - Load Scene (File bar menu): Select Scenes folder and select a file-> Load button
 
 Assets Project Window:
 
 - Scenes: Select Scene folder from project window and double-click it to load a whole scene
 - Models: Select Models folder from project window and double-click it to load a mesh
 - Textures / Icons / Particles: They are just visual
 
Particle System (Inspector):

- Emmitter: Lot of emmitter customization include its shape and burst
- Particle: Lot of particle properties customization
- Renderer:
   - Billboard: Choose a billboard mode -> Screen Aligned / World Aligned / Axially Aligned
   - Texture Button: Choose a texture for the particle quad
   - Blending: Customize blending color modes and Blend Equation options
- Serialization:
   - Save Particle System Button: Select Particles folder and input a name for the file (without typing extension!) -> Save button
   - Load Particle System Button: Select Particles and select a template particle file-> Load button
 Particle System (Scene View):
   - Preview Panel: When a Particle System object is selected, a preview panel is displayed to manipulate its time managment (Play / Pause / Restart)
   
 Particle System (Game Mode):
    - 1 Key: Fireworks shooting
    
Camera

- Mouse Right + WASD: fps-like movement and free look around
- Mouse Right: Move Camera with static position
- Mouse Left + Alt: Move Camera around object (Orbit)
- Mouse wheel: Move Camera Zoom in and Zoom out
- F: Center the Camera on an object when is selected
- SHIFT: Duplicates movement speed.

Performance:

 Z key: Z-Buffer

~~~~~~~~~~~~~~~

***

## Programs & Dependencies used
* [Microsoft Visual Studio 2019](https://visualstudio.microsoft.com/es/vs/)
* [OpenGL3](https://opengl.org/)
* [SDL v2.0](https://www.libsdl.org/license.php)
* [STL](https://docs.oracle.com/database/bdb181/html/installation/win_build_stl.html)
* [MathGeoLib v1.5](https://github.com/juj/MathGeoLib)
* [PCG v0.9](http://www.pcg-random.org/)
* [Mmgr](http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml)
* [ImGui](https://github.com/ocornut/imgui)
* [JSON parser v3.7](https://github.com/nlohmann/json)
* [Glew v2](https://github.com/nigels-com/glew)
* [par_shape.h](https://prideout.net/shapes)
* [Assimp v4.1](http://www.assimp.org/) 
* [DevIL v1.8](http://openil.sourceforge.net/) 
* [GPU detect v1.1](https://github.com/MatthewKing/DeviceId)


***
## Repository Link

* 3D Engine Engine Repository: [Chrono Stasis Engine](https://github.com/ChronoByte/Chrono-Stasis-Engine)
* Our Organization: [ChronoByte](https://github.com/ChronoByte)

***


## **License**

~~~~~~~~~~~~~~~

MIT License

      Copyright (c) 2019 ChronoByte

      Permission is hereby granted, free of charge, to any person obtaining a copy
      of this software and associated documentation files (the "Software"), to deal
      in the Software without restriction, including without limitation the rights
      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
      copies of the Software, and to permit persons to whom the Software is
      furnished to do so, subject to the following conditions:

      The above copyright notice and this permission notice shall be included in all
      copies or substantial portions of the Software.

      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
      SOFTWARE.

~~~~~~~~~~~~~~~


## **Project overview** 

Chrono Stasis Engine is a 3D game engine developed/coded from scratch with OpenGL in C++.

The engine contains all the basic [sub-systems]() of a modern 3D engine and we have also added support to handle [particle system](). Although this project has been done with educational purposes it will be used (along other engines from our class) to produce a game in the Project 3 subject in the first half of 2020.

This project has been done by two students of CITM - UPC (Terrasa, Spain) for the subject Game Engines.

> ### *Download the last version of the engine [HERE]()*

***

## **Our Team**

<a href="url"><img src="" align="right" height="150" width="150" ></a>_Peña Hernando, Carlos responsability and GitHub account_

* Resource Manager
* Quadtree & Frustum Culling
* Binary scene serialization
* GameObjects hierarchy
* Persistence (Json)
* File system
* Camera
* Gizmo UI
* Particle System
* [GitHub account]()


<a href="url"><img src="" align="right" height="150" width="150" ></a>_López Nosequemás, Sebastián responsability and GitHub account_

* Time Management
* Mouse Picking
* Unity like camera controls
* ImgGui editor
* File system
* Gizmo UI
* Camera
* Particle System
* [GitHub account]()

***

## **Main core subsystems**

Our game engine code is structured in modules. The main module (called csApp.cpp) manages all the other modules calling in a loop its respective awake, preupdate, update, postupdte, cleanup that they share thorugh a base class trModule. Down below a basic scheme of this structure (only some example modules are showed here):

![dibujo sin titulo](https://user-images.githubusercontent.com/25589509/50381846-84d3ac00-0691-11e9-8390-3f2fc765b614.png)

On the following sections we will explain the main core sub-systems of this engine.

### 3D Renderer

Our engine has a module renderer that handles all the drawing of the proram. We don't use shaders. Instead we use OpenGL Vertex Arrays with indices (except for the debug draw of the primitives which are rendered in direct mode). Everything is drawn in the post update of the module. There, the gameobjects are filtered and we only render the ones that are inside the fustrum. 

### Geometry loader

When we detect a file in our Assets/Models/ directory we read the file with Assimp and extract all the information we need (vertices, indices, uvs, name...) in order to render the mesh on screen. Then we save the resource in our own binary format.

Each time a resource mesh is generated a scene is generated in our own binary format too.

### Gameobject structure

Gameobjects have a tree structure so the transformations affect all the children correctly. Each gameobject has a transform component and if it has a mesh, we calculate its AABB so that it can be filtered by the frustum culling or the quadtree. We follow the same structure as Unity, using the Decoupling Design Pattern.

### Binary files format

All assets in Assets/ are saved as resources in our own binary format inside the Library directory. Doing this we only save the data we need and when we have to read the process is easier and much faster than if we had to read the original asset file such as .fbx or .png. A new resource in our own binary format is generated each time we detect a new assets in Assets/.

All the assets generate a resource in our own binary format except the textures which will be saved as DDS files for speed and standard convention reasons.

Our engine accepts the following assets formats:
* Meshes: FBX and Collada (.dae).
* Texture: png, jpg, jpeg, dds and tga.
* Animations: Collada (.dae)

### 3D Camera & frustum culling

Our engine has two cameras: the editor camera (which can be controlled as in any other 3D software) and the game camera. The game camera is treated as a component of the gameobject 'Main Camera' and has frusutm culling using a quadtree so gameobjects outisde its frustum won't be drawn. 

### Quadtree

We've implement a quadtree container in our engine to speed things up. All the gameobjects in the scene that are marked as static in the inspector are inserted inside the quadtree. The quadtree will subdivide itself in four as many times as it needs to redistribute the gameobjects and match the specified bucket size. Doing this we can safely discard gameobjects whose quadtree node does not intersect with the camera frustum speeding up the frustum culling process.

### Data serialization

Our engine serializes the entire hierarchy of objects scene through all the gameobjects and their corresponding components. For this we use UUIDs (Universally unique identifier) to be able to handle all the information when serializing and deserializing a scene. For our engine the UUIDs are generated by random numbers of 32 bits.

### Mouse picking

Mouse picking is done using the Raycast method. When the left mouse button is clicked, the camera module calculates a ray that will start in the corresponding clicked point in the near plane and will project until it reaches the far plane. Once we have that we just need to do:

- Collect all the gameobjects (both dynamic and static) that are inside the fustrum and intersect with the projected ray. The static object collection is accelerated with the quadtree.
- Loop through all this gameobjects. If they have mesh we calculate all its triangles and for each of them we test collision against the raycast (in the local space of the gameobject). We do this until we've loop through all gameobjects.

_NOTE:_ Collected gameobjects are sorted by distance and we constantly check for minimum distance to avoid generating unnecessary triangles and speed up the process.

### Time managment & game mode

When the engine is in game mode, the scene is seen though the game camera (which has frustum culling). We also keep an internal game clock (aside from the app clock) that will only run when the game mode is activated. Its dt will be passed to all the updates instead of the real time dt from the app.

### Resource Management

In our resource system we have imitated the management of resources that Unity has. When starting the engine we generate specific resources (Mesh, material, bones ...) and in case a resource is duplicated we only send the information (Vertices of a mesh for example) to the gpu once. As in unity, we generate .meta files that link the original file (.png) with the own format file that we generate once it has been imported (.dds). In addition, our .meta files also store information on how to perform the import.

***

## **Particles subsystem**


## **Engine Demo**

<iframe width="1280" height="720" src="" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

***

## **Installation instructions**

Download the zip file and unzip it. Open the folder, execute the .exe and enjoy!

_IMPORTANT: do not modify, change or add any folder or file as you might not be able to execute the game._

***

## **Notes on performance**

There are three main known cases where the overall perfomance might decrease significantly:


***

## **Controls**

~~~~~~~~~~~~~~~
Controls

- Select object: Left click

Gizmos

- W/E/R: Change gizmo to Translate, Rotation and Scale

Assets controls

- Load scene: Double left clic

Camera

- Free look around: Right Click Mouse Button
- Orbit around object: LALT + Left Click Mouse Button
- Zoom in/out Mouse wheel
- Pan: Middle mouse button
- Duplicate movement speeed: Hold Shift key
- Focus camera on object: F key

~~~~~~~~~~~~~~~

***

## **Tools used**

* IDE: Microsoft Visual Studio 2017
* Language: C++
* Containers: STL
* Input and audio: SDL 2.0.8
* Graphics: OpenGL
* Math: MathGeoLib 1.5
* Random Number Generator: PCG 0.9 (Minimal C Edition)
* GUI: Deat ImGui 1.65
* 3D Model / Animation importer: Assimp 1.4.0
* Image loader: Developer's Image Library (DevIL) 1.8.0
* Data persistance: JSON parser - Parson 2017
* Profiler: Brofiler 1.1.2
* Memory manager: mmgr
* File system: PHYSFS 3.0.1
* Code repository: GitHub
* Others: Adobe Photoshop CS6 / MS Paint / Aseprite


***

## **License**

~~~~~~~~~~~~~~~

MIT License

Copyright (c) 2018

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

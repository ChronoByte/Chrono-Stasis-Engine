# Chrono-Stasis-Engine

Chrono Stasis Engine is a 3D game engine created by two students in the third year of Videogame Design and Development degree at CITM UPC from Terrassa, Spain. 

## Terminology & Origin
As his name says, Chrono Stasis comes from the greek term **Chronostasis** (*chrónos* "Time" and *stásis* "standing"), a temporal illusion in which the first impression following the introduction of a new event or task-demand to the brain can appear to be extended in time. 

That is to say, is an illusion where that person perceives an instance of time as an extremely long or even infinite state of time, hence the word "standing time." Therefore, we can affirm that it is an allegory in the computational world and videogames, where for a user or player an instance of time is a millisecond of time spent playing, whereas for us it is an instance where millions of instructions, operations and calculations happen internally in the engine. 

Metaphorically for us (software engineers and game developers), we live in a chronostasis loop measuring and observing the behavior and time performance in each frame at all the time until the game ends. In addition, we decide to make Chrono Stasis as compound word because it contains the acronym of our names: **C**hrono (Carlos) & **S**tasis (Sebas).

## Features
* Geometry Creation using par_shapes (Cube, Sphere, Cilinder,...)
* FBX Importer with Drag and Drop functionality
* Texture Importer with Drag and drop functionality
* Load and Save editor data from serialized JSON file
* Camera Unity-like controls (Rotation, Orbitation, FPS Movement, Padding, Zoom)
* Focus Camera on GameObject when is selected
* Game Object and Component Design Pattern
* Transform, Mesh and Material Components generated when model is loaded
* Inspector window to display all GameObject info from its Components
* Hierarchy window to list in a tree node all GameObjects created or loaded from model importation
* Reading logs from Dependencies & Libraries in the console
* Editor Custom Theme and Size style system in Configuration window
* Bounding Box
* Docking for model windows and Scene
* Framebuffer to renders the world scene as a texture
* Editor Windows (Console, Configuration, About)
* GameObjects Transformations from Inspector Window
* Inspector show all properties for each component on selected GameObject
* GameObjects can be picked from the world using Mouse Picking with ImGuizmo
* Internal list of static Gameobjects that is accelerated using a quadtree visualized in the editor.
* Camera is a component with modificable FOV
* Scene/Game rendering window adapts to the new aspect ratio
* AABB implementation in all meshes that can be discarded using Frustum Culling visualized in the editor.
* Meshes and Textures saved in an own format file on Library/
* Scene and Model Serialization into json file that can be loaded.
* Resource Management (Material and Mesh resource)
* Counting Reference
* Engine can Start / Pause / Stop properly and recover its original state
* Assets Project window that display all assets file (not meta data)
* Folders management:  create / remove / rename folders
* Resources not managed inside Assets and are dropped to engine, are copied and metadata + own format files are generated automatically

## Members Task

* Carlos Peña
    * Development of own format files
    * Resource Management 
    * Model Serialization into json file
    * Scene Serialization into json file
    * File System implementation to generate directories & files 
    * Refresh System to detect new droppable files (Textures, FBX, etc)
    * Resource Mesh & Material loading own format files from memory 
    * Counting Reference implementation to check own format files used at every component of every GameObject
    * Assets Project Window to display Assets files
    * Creation, renaming and deletion of folders into Project Window
    * .Meta checking system to detect new files and generate his own format files + meta + resources
    
* Sebastián López
    * Real time view of the z-buffer
    * Implementation of a static Octree system
    * Component Camera with Frustum Culling (optimizable with Octree)
    * System that renders in real time multiple views of the scene
    * Bounding Box functionality
    * Mouse picking functionality (optimizable with Octree)
    * Game mode that can Start / Pause / Step with a speed modifier (dt modifier).
    * ImGuizmo implementation


## Innovation 
* Free costumization of **ambient color, diffuse color and specular color on lighting render** at runtime. You can find it and use it in configuration window at renderer section.
* Free costumization of **Material Color** into GameObject material section. You can find it and use it in inspector window at material component section.
* Free customization of **GUI theme and size styles**. You can find it and use it in configuration window at GUI section.
* Octree Development
* Real time view of Z-buffer
* Project Asset Panel to display the file management in a visual way
* Refresh file system to new droppable files detection. When a file is dropped, that file is copied if isn't found in Assets folder and copied to Models folder. In the next refresh time, the file is added to file system internally and its own format files are generated with his uuid's and meta file respectively.
* Adaptable docking resolution for mouse picking raycast
* Updatable Parenting Bounding Box


## Installation
* Go to our github release page: [Click Here!](https://github.com/ChronoByte/Chrono-Stasis-Engine/releases)
* Download the last version **.zip**
* Extract .zip files inside a folder 
* Execute **.exe** file

## Authors
* Carlos Peña [CarlosUPC](https://github.com/CarlosUPC)
* Sebastián López [Sebi Lopez](https://github.com/Sebi-Lopez)

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

## Repository Link

* 3D Engine Engine Repository: [Chrono Stasis Engine](https://github.com/ChronoByte/Chrono-Stasis-Engine)
* Our Organization: [ChronoByte](https://github.com/ChronoByte)

## Unity-Like Camera Controls
* **Mouse Right + WASD:** fps-like movement and free look around
* **Mouse Right:** Move Camera with static position
* **Mouse Left + Alt:** Move Camera around object (Orbit)
* **Mouse wheel:** Move Camera Zoom in and Zoom out
* **F**: Center the Camera on an object when is selected
* **SHIFT:** Duplicates movement speed.

## Updates & Changelog

* 0.3.0
     * Generate a stable & operative release Build for First Assignment
* 0.2.9
     * Fix Bugs from loading file directories with different path slashes (//, /, \)
     * Fix Bugs from loading different texture extensions according to its coordinates origin
     * Fix Bugs from Bounding Box not detected
     * Fix Bugs from par_shapes objects creation
     * Loading several fbx with diffuse texture in DDS format
* 0.2.8
     * Implemented Bounding Box with AABB from MathGeoLib to improve focus Camera functionality
     * Fix memory leaks from Face Normals and improved its calculations
     * Added libraries and dependencies inicialization logs in console window
* 0.2.7
     * Improved GameObject creation with par_shapes
     * Implemented Bounding Box with AABB from MathGeoLib to improve focus Camera functionality
* 0.2.6
     * FrameBuffer into gameplay scene window to generate a texture from its renderer
     * Implemented Docking on GUI windows and framebuffer scene
     * Solved bugs from framebuffer scene resizing
     * Save functionality only with Application data in serialized config.json file
* 0.2.5
     * Added icon.exe
     * Implemented Camera Orbitation movement & solved bugs related with it
     * Implemented Camera Panning movement
     * Implemented Camera Zoom movement 
     * Focus Camera on GameObject selected functionality (F)
     * Improved Camera rotation movement code & solved bugs related with it
* 0.2.4
     * Matching and linking System with GameObject children from parent
     * Creation and Assignment of components from model loader to create a GameObject
     * Developed Tree node to list all GameObjects created and loaded
* 0.2.3
     * Developed Mesh Component to display info in inspector window
     * Developed Transform Component to display info in inspector window
     * Developed Material Component to display info in inspector window
     * Added Hire checkbox option to Enable / Disable GameObject visualization
* 0.2.2
     * Implemented Ambient Color option to modify lighting render at runtime
     * Implemented Diffuse Color option to modify lighting render at runtime
     * Fix bugs related with lighting color enabling / disabling
* 0.2.1
     * Implemented Custom theme and sizing style system from Imgui code
     * Added different hand-made themes into system
     * Added different hand-made sizes into system
* 0.2.0
     * Added Basic GameObject  Structure 
     * Reestructured Model Importer 
     * Added Inspector Window
     * Component Design Pattern 
        > Transform, Material, Mesh

* 0.1.9
    * Implemented DevIL 
     * Texture Loader
     * Drag & Drop funcionality (to import DDS/PNG textures).


* 0.1.8
    * Implemented Assimp  
    * FBX Model Loader
       > Draw vertices, indices, normals.
    * Drag & Drop funcionality (to import FBX models).


* 0.1.7
     * Add ImGui::Dock (changed later) 
     * 3D Geometry Creation
       > Vertex Buffers, Index Buffers, Par shapes
     
      
* 0.1.5
   * Implemented JSON Parser
      > Load / Save editor configuration
   * Implemented OpenGL Context with GLEW (init & renderer)
      > Initialization, Renderer, Shutdown 
   * Implemented "Render" options inside "Configuration" window
      > Depth test, lightning, color material, wireframe, axis, smooth polygons and fog mode.
    * Congifuration Window development
      > OpenGL + GLEW info.

* 0.1.0
    * Implemented "Configuration" Window:
      >Frame rate options. Window options (resizable, fullscreen, etc). Renderer options. Hardware info 
    * Added "About us..." Modal Window:
      >Name, description, libraries used.
    * "Console" Window:
      >Logs & Engine inner performance.
      
 * 0.0.5
   * Implemented MathGeoLib Framework
     > Cube, Sphere, Cilinders, Frustum, AABB, etc
   * Implemented PCG Pseudo-Random Helper
     > Float between 0.0 and 1.0. 
     > Int between any min/max
## License

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


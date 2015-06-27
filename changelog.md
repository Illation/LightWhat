0.1 (Basis for rendering)
* vector math
* Triangle Meshes
* Intersection
* Point lights
* Shadows
* Phong Shader
* Background Shader

0.2.1 (basic usability)
* file input
* tinyxml2
* collada Import
* new shading setup

0.2.2
* removed unnecessary game engine
* ported to SDL2
* efficient application loop setup
* using native c++ string library

0.2.3
* preventing window from freezing during render
* choosing files to load from with Open file Dialogue
* basic window layout and user control with hotkeys
* setup for later adding a GUI
* watching the image render
* progress feedback from console
* building release versions

0.2.4
* added image saving class
* added saving to bitmap function
* added save file dialog
* added file filters to open and save file dialogs to prevent loading of illegal file types
	
0.3.1 (code maintainability)
* bundled vertex math structures into common.h and common.cpp for easy re-use in other projects and more sensible file structure
* bundled Raytracing helper structures into commonR.h and commonR.cpp to remove clutter
* changed names of classes and functions to more generic names for better readability (Vertex -> vec3 with typedef point3 | RayColor -> colRGB)
* added line and plane structure. line plane intersection is handled within planes directly
* moved faces directly to the mesh class and called them tri, because it is basically part of the primitive and wont be used in other primitives. 
* removed unnecessary edgehandling of meshes to save memory
* meshes now have to be placed on heap for better memory allocation
* removed bug where importing collada meshes with a space after the last vcount number would crash the application

0.3.2
* added shape base class for all types of shapes
* made meshes derive from shapes
* main renderer class doesnt handle specific intersections anymore, but gets the results from shapes that are stored in lists
* moved current mesh intersection into meshes
* added differential geometry structure that provides information for shading, now including baryacentric UV coordinates

0.3.3
* added sphere and plane shapes
* added option to load a test scene with spheres and a plane by pressing "T"
* removed weird distortion from camera ray generation
* fixed diffuse shader and gave it shadow tests
* added image postprocessor that removes shader banding by fixing the exposure

0.3.4
* added a scene class thay handles all objects and provides scene information -> render class now actually only does rendering and no scene handling
* added option to clear scenes of all objects and lights ("C")
* improved general racast structure
* added earlier rejections to meshes and spheres in intersection test
* made triangles precalculate intersection data to improve the speed
* added backface culling option
* added performance options to user controls ("B" to toggle backface culling, "P" to toggle performance mode)
* => performance improvment: up to 2.6 times as fast

0.4.1 (Basic Shading and Textures)
* fixed phong shader
* added reflection shader
* removed collada importer and included assimp
* restructured meshes to support more data like vertex normals with polylists
* added support for multiple materials
* added mesh smoothing

0.4.2
* added 2D vectors and points
* added UV sets to Meshes 
* calculating UV coordinates for mesh intersection
* added UV loading to sceneLoader
* added camera transformation

0.4.3
* added Texture class
* added texture loading with devIL
* added diffuse map rendering
* added text display with SDL_TFF
* added texture display in application ("D")

0.4.4
* added linear texture interpolation
* added tangent space data to meshes
* added specular map texture rendering
* added tangent space interpolation to mesh intersections
* added normal map rendering
* it is now possible to load larger models

0.5 (Performance)
* inlined vector math
* .h files -> .hpp files
* added Axis aligned bounding Boxes (AABB)
* added bounding volume hierachies
* added building of bounding volume hierachies from meshes
* replaced linear raycasting with raycasting through bounding volume hierachies
* using floats instead of doubles to reduce massive overhead
* added multithreaded tile rendering
* displaying the render time in the application (SDL text rendering)

0.6 (Global Illumination)
* fixed Spheres
* Made test scene a cornell box
* Polymorphism to support multiple light types, Using both point and area lights
* Polymorphism for shaders, Shaders now use the visitor pattern
* created basic monteCarlo library for Global Illumination
* Shaders now are BDSFs -> Global Illumination
* created Diffuse, Glossy, Glass, Emission, Mix and Background Shaders
* added basic XML parser and dom
* render settings are now loaded from .lwtf files
* renderer is now split into specific LWrenderer and traceUnit

0.7.1
* created GUI engine singleton
* created Window manager
* moved SDL window code from application root to window manager
* moved window drawing code to gui engine
* added Event manager to GUI engine for input handling
* modified Window handling code
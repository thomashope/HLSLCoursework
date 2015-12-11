# Graphics Programming with Shaders
#### Coursework Documentation
#### Thomas Hope: 1302495

## Preface

This document is a description of the techniques

## Overview
### Controls
* WASD: forward, left, back, right
* E: up
* Q: down
* Arrow Keys: rotate camera
* NUMPAD 0: show bonus scene
* NUMPAD 1: toggle normal display
* NUMPAD 2: increase tessellation
* NUMPAD 3: decrease tessellation
### Features
* multiple lights, ambient and diffuse
* per light attenuation
* per light shadow mapping
* per light specular highlights (see bonus scene)
* diffuse texturing
* normal mapping
* tessellation
* transparency
* vertex manipulation
* refraction
* light transmission
* geometry generation
* depth of field
* fog
### Objects
* Floor
* Hellknight model
* Magic Sphere
### Buffers
* Shadow Map x 2
* Scene with light and shadow
* Magic sphere diffuse colours
* Magic sphere normals
* Scene with light and shadow, blended with magic sphere
* Blurred version of the blended scene
### Shaders
* Depth shader, renders depth values to texture alpha
* Texture shader, renders unlit mesh with texture
* Shadow shader, renders objects with light and shadow
* Normal shadow shader, renders objects with normal map, light and shadow
* Tess depth shader, renders tint information for a tessellated shape for shadow blending
* Tess colour shader, renders colour information for tessellated shape and depth to alpha
* Tess normal shader, renders normal information for tessellated shape to texture RGB
* Blend Shader, blends two textures with refraction
* Blur shader, modified box blur shader using sub pixel sampling
* Depth of field shader, blends a sharp and blurred texture based on depth from the screen centre 
## Detail

## References
* Paul Robertson, what a gent
* [RasterTek Tutorials](http://www.rastertek.com/tutdx11.html)
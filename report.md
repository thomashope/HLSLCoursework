# Graphics Programming with Shaders
#### Coursework Documentation
#### AG0904A15
#### Thomas Hope: 1302495

## Preface

This document is a description of the techniques used for my submission

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
### Hellknight
### Floor
The scene floor is renderd into shadow maps 1 and 2, so it can receive shadows from the other objects in the scene, and into the lit scene with the shadows and normal maps applied.
#### Normal Maps
Normal maps are similar to the usual coloured (diffuse) textures in that they store additional information about the surface they are applied to. However instead of storing colour information they store information about the normal to the surface at each pixel.  
The XYZ direcition of the normal is stored as an RGB colour value in tangent space. Tangent space is a coordinate system relative to the surface the normal map is being applied to. In tangent space 'up' is normally represented by the actual normal of the polygon and the other to axies (tangent and binormal) lie on the surface of the polygon perpendicular to each other and the normal.  
This additonal tangent and binormal information needs to be passed to the pixel shader somehow, it could be incorporated into the mesh or calculated in a previous shader stage (of which the vertex or geometry shader are the most likely candidates). In the case of the floor I added the tangent and binormal information to the mesh. The normal, tangent and binormal are then multiplied by the worldMatrix in the vertex shader so they are orientated correctly to the surface.  
The normal map is passed into the pixel shader along with the diffuse texture and sampled at the same UV coordinate. The normal sample is then transformed from the range [0,1] to [-1,1], tansformed from tangent space, and normalised (if the values in the texture have not been normalised already). All further lighting calculating are then done with this normal.
### Magic Sphere

## References
* Paul Robertson, what a gent
* with help from [RasterTek Tutorials](http://www.rastertek.com/tutdx11.html)

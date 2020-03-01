# Hikari
This is a physically based renderer I implemented from scratch in C++.  
Hikari (ひかり, 光) means light, radiance in Japanese. Also, it's the name of one of my favorite characters from game [*Xenoblade Chronicles 2*](https://en.wikipedia.org/wiki/Xenoblade_Chronicles_2). 

### Features Checklist

- [X] Simple Primitives (Sphere, Box, Plane) 
- [X] Vector class
- [X] Camera class
- [X] Ray class
- [X] Intersection class(position, normal, textrure coordinates...)
- [X] Material class (diffuse, specular, phong, glossy...)
- [X] Russian Roulette
- [X] Triangle Meshes
- [X] Bounding volume hierarchy(BVH, for triangle mesh only)
- [ ] Use a new powerful scene loader, support different type of files (.obj, .pbrt, etc.)
- [ ] Importance Sampling of glossy/Phong Material
- [ ] Lighting/Area light interface
- [ ] BSSRDF
- [ ] Bi-Directional Path Tracing
- [ ] Multiple importance samlping (MIS)

### Gallery
Coming soon...
### Reference
1. [Physically Based Rendering](https://www.pbrt.org/)
2. [The Rendering Equation](http://www.cse.chalmers.se/edu/year/2016/course/TDA361/rend_eq.pdf) by Kajiya
3. ScratchPixel [https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing](https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing)
4. smallpt [http://www.kevinbeason.com/smallpt/](http://www.kevinbeason.com/smallpt/)

# ~Ray~ Path Tracing

Michael Asper - ma53285
Daniel Mancia - dam4754


## Path Tracing

To use path tracing, you need to supply

path samples amount = w

monte carlo depth = x

light sampling radius (recommended for 1) = y

lgith samples amount = z

then do `-p "w.x.y.z"`

If you incorrectly format it, the path tracing will override it with dummy
values.

The demos was done on path.ray and a lot of samples images are in
/path\_images/

Anything with ray in the file name, is not path traced.

## Extra Credit

We did the course evaluation on CIS


## Old ray tracing stuff

## Completed Features

* (60 pts) BVH
* (10 pts) Texture Mapping
* (10 pts) Cube Mapping
* (5 pts) Adaptive Termination 
* (5 pts) Stocahstic supersampling
* (10 pts) New geometry: dedicated torii and general quadrics
* (20 pts) Depth of field
    * (+10) Soft Shadows (scenes in assets/scenes/soft)
* (20 pts) 3D images for viewing with glasses (Anaglyph)
* (varied pts) Implement different modes of shading: cel shading
* (varied pts) Creative scene - assets/scenes/torus/torus1.ray


### Adaptive Termination

Use `-z [threshold]`

### Stocahstic supersampling

Use `-f [samples]`

### Geometry supersampling

We have scenes in the quad/torus folders

### Depth of field

Use `-q [focalDistance]` and `-e [aperature size]`

`./bin/ray -r 5 -q 6 -e 0.3 assets/scenes/reflection2.ray output.png`

Must use q!

### 3D Images

Use `-d [3dmode]`

#### 3D modes:

0. Gray scale
1. Optimized half color

`./bin/ray -r 5 -d 0 assets/scenes/reflection2.ray output.png`

### Toon shading

Use `-n`

## Creative Scene

Simpsons's Donut

`./bin/ray -r 5 -n -a 3 -c ../assets/cubemaps/Simpsons/negx.png   ../assets/scenes/torus/torus1.ray out1.bmp; open out1.bmp`


# Path Tracer

This is a CPU-only path tracer written in C++

## Features
- Unidirectional path tracing
- BVH accelerated ray tracing
- Multiple importance sampling
- Physically based materials
- Light sources
  - Directional light
  - Environment light
  - Area lights
- Volume rendering
- Modeling loading with Assimp
- Parallel processing with OpenMP

## Building
- Install [CMake](https://cmake.org/install/)
- Ensure CMake is in the system `PATH`
- Clone the repository `git clone --recursive https://github.com/Sopiro/PathTracer`
- Run CMake build script depend on your system
  - Visual Studio: Run `build.bat`
  - Otherwise: Run `build.sh`
  
## Todo
- Memory management
- Better material (BSDF and BSSRDF)
- Path tracing on GPU

## References
- https://raytracing.github.io
- https://learnopengl.com
- https://boksajak.github.io/blog/BRDF
- https://www.pbr-book.org
- https://agraphicsguynotes.com/posts/sample_microfacet_brdf/
- https://schuttejoe.github.io/post/ggximportancesamplingpart2/
- https://simonstechblog.blogspot.com/2020/01/note-on-sampling-ggx-distribution-of.html
- https://hal.inria.fr/hal-01024289/file/Heitz2014Slides.pdf
- https://jcgt.org/published/0007/04/01/


## Samples
![CornellBox](.github/image/render_1000x1000_s1024_d2147483647_t327.607s.png)
![RTIOW](.github/image/render_1920x1080_s1080_d2147483647_t263.396s.png)
![Materials](.github/image/render_1920x1080_s2048_d2147483647_t885.34s.png)
![MIS](.github/image/render_1920x1080_s128_d2147483647_t191.966s.png)
![Sponza](.github/image/render_1920x1080_s1024_d2147483647_t4680.33s.png)
![DamagedHelmet](.github/image/render_1920x1080_s1024_d2147483647_t134.453s.png)
|![Lucy1](.github/image/render_1000x1000_s1024_d2147483647_t524.58s.png)|![Lucy2](.github/image/render_1000x1000_s1024_d2147483647_t663.434s.png)|
|--|--|

![AntiqueCamera](.github/image/render_1920x1080_s1024_d2147483647_t233.835s.png)
![StanfordModels](.github/image/render_1920x1080_s1024_d2147483647_t490.157s.png)
|![Statue1](.github/image/render_1000x1000_s1024_d2147483647_t366.618s.png)|![Statue2](.github/image/render_1000x1000_s1024_d2147483647_t369.879s.png)|
|--|--|  

![Ship](.github/image/render_1600x1200_s2048_d2147483647_t2203.37s.png)
|![Volume1](.github/image/render_1000x1000_s1024_d2147483647_t1027.93s.png)|![Volume2](.github/image/render_1000x1000_s1024_d2147483647_t1133.39s.png)|
|--|--|
|![Volume3](.github/image/render_1000x1000_s1024_d2147483647_t1243.96s.png)|![Volume4](.github/image/render_1000x1000_s1024_d2147483647_t1321.27s.png)|
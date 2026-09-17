# Host asset pipeline

64DS-DX can render developer-authored host meshes without converting them to
Nintendo DS BMD files. The lightweight interchange pair is Wavefront OBJ for
geometry and binary PPM (`P6`, 8-bit RGB) for an optional texture.

`ntr::HostMesh` lives in `port/ntr/include/ntr/host_mesh.h`. Call `load()` once,
then call `draw()` while the desired game matrices are active. Polygon faces
are triangulated as a fan. The intentionally small OBJ subset is `v`, `vt`, and
positive one-based `f v/vt` indices.

```cpp
#include "ntr/host_mesh.h"

ntr::HostMesh mesh;
if (!mesh.load("mods/my_character/body.obj", "mods/my_character/body.ppm"))
    std::fprintf(stderr, "mesh: %s\n", mesh.error().c_str());

// From a render hook with the correct DS matrices active:
mesh.draw(0.01f);
```

The scale argument converts conventional modeling units into DS scene units,
allowing artists to retain sensible OBJ units. This pipeline is host-only QoL:
retail-matching `src/` remains untouched, and missing assets can fall back to
the normal DS model.

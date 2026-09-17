#ifndef NTR_HOST_MESH_H
#define NTR_HOST_MESH_H

#include <stdint.h>
#include <string>
#include <vector>

namespace ntr {

// Developer-facing host mesh: Wavefront OBJ geometry plus an optional binary
// PPM (P6) texture. This deliberately bypasses Nitro BMD dictionaries while
// still submitting through Tango's existing camera, clipping, and rasterizer.
class HostMesh {
public:
    bool load(const char *obj_path, const char *texture_path = nullptr);
    void draw(float scale = 1.0f) const;
    void draw_model(const int model[12], float scale = 1.0f) const;
    bool empty() const { return triangles_.empty(); }
    const std::string &error() const { return error_; }

private:
    struct Vertex { float x, y, z, u, v; };
    struct Triangle { Vertex v[3]; };
    std::vector<Triangle> triangles_;
    std::vector<uint32_t> texture_;
    int texture_width_ = 0;
    int texture_height_ = 0;
    std::string error_;
};

} // namespace ntr

#endif

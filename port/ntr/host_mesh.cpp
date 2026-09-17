#include "ntr/host_mesh.h"
#include "ntr/gx.h"

#include <cstdio>
#include <fstream>
#include <sstream>

namespace ntr {
namespace {
struct P3 { float x, y, z; };
struct P2 { float u, v; };

bool obj_index(const std::string &word, int &v, int &t) {
    v = t = 0;
    return std::sscanf(word.c_str(), "%d/%d", &v, &t) >= 1 && v > 0;
}

bool load_ppm(const char *path, std::vector<uint32_t> &pixels, int &w, int &h,
              std::string &error) {
    std::ifstream in(path, std::ios::binary);
    std::string magic;
    int maxv = 0;
    if (!(in >> magic >> w >> h >> maxv) || magic != "P6" || w <= 0 || h <= 0 ||
        maxv != 255) {
        error = "texture is not an 8-bit binary PPM (P6)";
        return false;
    }
    in.get();
    std::vector<unsigned char> rgb((size_t)w * h * 3);
    if (!in.read(reinterpret_cast<char *>(rgb.data()), rgb.size())) {
        error = "PPM pixel data is truncated";
        return false;
    }
    pixels.resize((size_t)w * h);
    for (size_t i = 0; i < pixels.size(); ++i)
        pixels[i] = 0xff000000u | (uint32_t(rgb[i * 3]) << 16) |
                    (uint32_t(rgb[i * 3 + 1]) << 8) | rgb[i * 3 + 2];
    return true;
}
} // namespace

bool HostMesh::load(const char *obj_path, const char *texture_path) {
    triangles_.clear(); texture_.clear(); error_.clear();
    texture_width_ = texture_height_ = 0;
    std::ifstream in(obj_path);
    if (!in) { error_ = "cannot open OBJ"; return false; }
    std::vector<P3> positions;
    std::vector<P2> texcoords;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream row(line);
        std::string kind;
        row >> kind;
        if (kind == "v") {
            P3 p{}; if (row >> p.x >> p.y >> p.z) positions.push_back(p);
        } else if (kind == "vt") {
            P2 p{}; if (row >> p.u >> p.v) texcoords.push_back(p);
        } else if (kind == "f") {
            std::vector<Vertex> face;
            std::string word;
            while (row >> word) {
                int vi, ti;
                if (!obj_index(word, vi, ti) || vi > (int)positions.size()) {
                    error_ = "OBJ face contains an invalid vertex index";
                    triangles_.clear(); return false;
                }
                const P3 &p = positions[vi - 1];
                P2 uv{};
                if (ti > 0 && ti <= (int)texcoords.size()) uv = texcoords[ti - 1];
                face.push_back({p.x, p.y, p.z, uv.u, 1.0f - uv.v});
            }
            for (size_t i = 2; i < face.size(); ++i)
                triangles_.push_back({face[0], face[i - 1], face[i]});
        }
    }
    if (triangles_.empty()) { error_ = "OBJ contains no faces"; return false; }
    if (texture_path && *texture_path &&
        !load_ppm(texture_path, texture_, texture_width_, texture_height_, error_)) {
        triangles_.clear(); return false;
    }
    return true;
}

void HostMesh::draw(float scale) const {
    gx_bind_texture(texture_.empty() ? nullptr : texture_.data(),
                    texture_width_, texture_height_);
    for (const Triangle &triangle : triangles_) {
        float xyz[9], uv[6];
        for (int i = 0; i < 3; ++i) {
            xyz[i * 3] = triangle.v[i].x * scale;
            xyz[i * 3 + 1] = triangle.v[i].y * scale;
            xyz[i * 3 + 2] = triangle.v[i].z * scale;
            uv[i * 2] = triangle.v[i].u * texture_width_;
            uv[i * 2 + 1] = triangle.v[i].v * texture_height_;
        }
        gx_submit_host_triangle(xyz, uv, 0xffffffffu);
    }
}

} // namespace ntr

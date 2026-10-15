#include <cstdlib>
#include <new>
#include <cstdio>
#include <chrono>

static size_t allocated_bytes;
void *operator new(size_t size) {
    void *p = std::malloc(size ? size : 1);
    if (!p) throw std::bad_alloc();
    allocated_bytes += size;
    return p;
}
void operator delete(void *p) noexcept { std::free(p); }
void operator delete(void *p, size_t) noexcept { std::free(p); }

#ifdef STORAGE_BASELINE_GX
#include STORAGE_BASELINE_GX
#else
#include "../ntr/gx.cpp"
#endif

// No HD pack or display device is involved in the VRAM binding proof.
namespace ntr {
bool io_ready() { return false; }
bool rt_window_rows(int, const uint32_t **) { return false; }
void io_gxstat_publish() {}
bool hdtex_wants_work() { return false; }
uint64_t hdtex_name(uint32_t, uint32_t) { return 0; }
void hdtex_dump(uint64_t, int, int, const uint32_t *) {}
int hdtex_lookup(uint64_t, int, int, std::vector<uint32_t> &) { return 0; }
}

int main() {
    void *memory = VirtualAlloc(reinterpret_cast<void *>(0x06800000), 0x100000,
                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (memory != reinterpret_cast<void *>(0x06800000)) return 1;
    auto *pixels = static_cast<uint16_t *>(memory);
    for (int i = 0; i < 64; ++i) pixels[i] = static_cast<uint16_t>(0x8000u | (i * 491u));
    for (int mode = 0; mode <= 2; ++mode) {
        ntr::gx_invalidate_textures();
        ntr::gx_configure_texture_filter(mode);
        ntr::g_teximage = 7u << 26;
        const size_t before = allocated_bytes;
        const auto start = std::chrono::steady_clock::now();
        for (unsigned palette = 0; palette < 2048; ++palette) {
            ntr::g_plttbase = palette;
            ntr::bind_from_vram();
        }
        const double ms = std::chrono::duration<double, std::milli>(
            std::chrono::steady_clock::now() - start).count();
        uint32_t hash = 2166136261u;
        for (const auto &entry : ntr::g_vram_tex_cache) {
            for (uint32_t p : entry.second.px) hash = (hash ^ p) * 16777619u;
            for (const auto &level : entry.second.mip)
                for (uint32_t p : level) hash = (hash ^ p) * 16777619u;
        }
        if (ntr::g_vram_tex_cache.size() != 2048) return 2;
#ifndef STORAGE_BASELINE_GX
        if (ntr::g_tex_by_px.size() != (mode == 2 ? 2048u : 0u)) return 3;
#endif
        std::printf("filter=%d textures=%zu lookup_nodes=%zu allocated=%zu hash=%08x ms=%.3f\n",
                    mode, ntr::g_vram_tex_cache.size(), ntr::g_tex_by_px.size(),
                    allocated_bytes - before, hash, ms);
    }
    ntr::gx_invalidate_textures();
    VirtualFree(memory, 0, MEM_RELEASE);
    return 0;
}

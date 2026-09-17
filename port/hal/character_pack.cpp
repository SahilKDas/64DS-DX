#include "character_pack.h"

#include "ntr/host_mesh.h"

#include <cstdio>
#include <cstdlib>

namespace {
struct Pack {
    bool attempted;
    bool loaded;
    ntr::HostMesh mesh;
};

Pack g_packs[6];
const char *const g_names[6] = {"mario", "luigi", "wario", "yoshi", "waluigi", "toad"};

Pack &load_pack(int character) {
    Pack &pack = g_packs[character];
    if (pack.attempted) return pack;
    pack.attempted = true;
    const char *root = std::getenv("SM64DS_CHARACTER_PACK_DIR");
    if (!root || !*root) return pack;
    char model[1024];
    std::snprintf(model, sizeof model, "%s/%s/model-parts.obj", root,
                  g_names[character]);
    pack.loaded = pack.mesh.load(model);
    if (pack.loaded)
        std::fprintf(stderr, "[character-pack] loaded %s from %s\n",
                     g_names[character], model);
    else
        std::fprintf(stderr, "[character-pack] %s unavailable: %s\n",
                     g_names[character], pack.mesh.error().c_str());
    return pack;
}
} // namespace

bool port_character_pack_draw(int character, const int model[12]) {
    const char *preview = std::getenv("SM64DS_CHARACTER_PACK_PREVIEW");
    if (!preview || !*preview || *preview == '0') return false;
    if (character < 0 || character >= 6) return false;
    Pack &pack = load_pack(character);
    if (!pack.loaded) return false;
    pack.mesh.draw_model(model, 0.01f);
    return true;
}

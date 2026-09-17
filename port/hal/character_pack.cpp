#include "character_pack.h"

#include "ntr/host_mesh.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
struct Pack {
    bool attempted;
    bool loaded;
    ntr::HostMesh mesh;
};

Pack g_packs[6];
const char *const g_names[6] = {"mario", "luigi", "wario", "yoshi", "waluigi", "toad"};

const char *pack_root() {
    const char *configured = std::getenv("SM64DS_CHARACTER_PACK_DIR");
    if (configured && *configured) return configured;
#ifdef _WIN32
    /* A packaged build must work when its EXE is double-clicked directly.  The
       old path was supplied only by launch-waluigi.cmd, so doing exactly that
       silently disabled every imported model.  Resolve the sibling directory
       from the executable rather than the caller's working directory. */
    static char sibling[MAX_PATH];
    static bool resolved;
    if (!resolved) {
        resolved = true;
        const DWORD n = GetModuleFileNameA(0, sibling, MAX_PATH);
        if (n && n < MAX_PATH) {
            char *slash = std::strrchr(sibling, '\\');
            if (!slash) slash = std::strrchr(sibling, '/');
            if (slash && (size_t)(slash - sibling) + 12 < sizeof sibling)
                std::strcpy(slash + 1, "characters");
            else
                sibling[0] = '\0';
        } else {
            sibling[0] = '\0';
        }
    }
    return sibling[0] ? sibling : 0;
#else
    return "characters";
#endif
}

Pack &load_pack(int character) {
    Pack &pack = g_packs[character];
    if (pack.attempted) return pack;
    pack.attempted = true;
    const char *root = pack_root();
    if (!root || !*root) return pack;
    char model[1024];
    std::snprintf(model, sizeof model, "%s/%s/model.obj", root,
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
    /* Presence of a packaged character directory is the default opt-in.  The
       explicit value 0 remains an escape hatch back to the native DS model. */
    if (preview && *preview == '0') return false;
    if (character < 0 || character >= 6) return false;
    Pack &pack = load_pack(character);
    if (!pack.loaded) return false;
    pack.mesh.draw_model(model, 0.01f);
    return true;
}

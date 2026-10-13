#include "resource_pack.h"

#include <cstdio>
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::fprintf(stderr, "usage: resource_pack_probe <pack-root>\n");
        return 2;
    }
    std::string error;
    if (!sm64ds::packs::load_all(argv[1], error)) {
        std::fprintf(stderr, "%s", error.c_str());
        return 1;
    }
    if (!sm64ds::packs::characters().empty()) return 3;
    if (sm64ds::packs::textures().size() != 1 ||
        sm64ds::packs::textures()[0].target_hash != 0x0123456789abcdefULL)
        return 5;
    if (sm64ds::packs::packs().size() != 1 ||
        !sm64ds::packs::packs()[0].loaded ||
        !sm64ds::packs::packs()[0].show_healthbar ||
        !sm64ds::packs::packs()[0].show_hitbox ||
        sm64ds::packs::registry_fingerprint().size() != 16)
        return 7;
    std::puts("resource_pack_probe: PASS");
    namespace fs = std::filesystem;
    const fs::path root = fs::temp_directory_path() /
        ("sm64ds-client-pack-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    fs::create_directories(root / "valid");
    fs::create_directories(root / "bad");
    std::ofstream(root / "valid" / "pack.lua") << "sm64ds.pack{id='valid'}; sm64ds.healthbar(); sm64ds.hitbox()";
    const char *rejected[] = {
        "sm64ds.character{base=0,hitbox={radius=999}}",
        "sm64ds.set_speed(1.5)",
        "sm64ds.hitbox{radius=999}",
        "sm64ds.healthbar{health=999}",
        "io.open('settings.json','w')",
        "os.execute('anything')",
        "require('anything')",
        "sm64ds.texture{target='0123456789abcdef',source='../escape.png'}",
        "while true do end",
        "local x=string.rep('x',32*1024*1024)",
        "pcall(function() while true do end end)",
    };
#ifdef _WIN32
    _putenv_s("SM64DS_COMMS_ROLE", "client");
#endif
    for (const char *script : rejected) {
        std::ofstream(root / "bad" / "pack.lua", std::ios::trunc) << script;
        if (sm64ds::packs::load_all(root.string(), error) ||
            sm64ds::packs::packs().size() != 2 ||
            !sm64ds::packs::characters().empty() ||
            !sm64ds::packs::packs()[1].loaded) {
            std::fprintf(stderr, "client boundary failure: %s\n", script);
            fs::remove_all(root);
            return 8;
        }
    }
    fs::remove_all(root);
    std::puts("client-only boundaries: PASS (11 rejected scripts, valid pack survives multiplayer mode)");
    return 0;
}

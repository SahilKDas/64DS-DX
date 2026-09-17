#include "ntr/host_mesh.h"

#include <cstdio>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: host_mesh_test mesh.obj\n");
        return 2;
    }
    ntr::HostMesh mesh;
    if (!mesh.load(argv[1])) {
        std::fprintf(stderr, "host_mesh_test: %s\n", mesh.error().c_str());
        return 1;
    }
    if (mesh.empty()) return 1;
    std::printf("host_mesh_test: loaded %s\n", argv[1]);
    return 0;
}

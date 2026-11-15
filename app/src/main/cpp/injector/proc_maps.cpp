#include "proc_maps.h"

#include <cstdio>
#include <cstring>

namespace injector {

bool find_module(int pid, const std::string& name_substr, ModuleInfo& out) {
    char path[64];
    if (pid == 0) {
        std::snprintf(path, sizeof(path), "/proc/self/maps");
    } else {
        std::snprintf(path, sizeof(path), "/proc/%d/maps", pid);
    }

    FILE* f = std::fopen(path, "r");
    if (!f) {
        return false;
    }

    bool found = false;
    uintptr_t lowest_base = 0;
    uintptr_t highest_end = 0;
    char line[512];

    while (std::fgets(line, sizeof(line), f)) {
        if (!std::strstr(line, name_substr.c_str())) {
            continue;
        }

        unsigned long long start = 0, end = 0;
        if (std::sscanf(line, "%llx-%llx", &start, &end) != 2) {
            continue;
        }

        if (!found || start < lowest_base) {
            lowest_base = start;
        }
        if (end > highest_end) {
            highest_end = end;
        }
        found = true;
    }

    std::fclose(f);

    if (!found) {
        return false;
    }

    out.base = lowest_base;
    out.end = highest_end;
    out.path = name_substr;
    return true;
}

}  // namespace injector

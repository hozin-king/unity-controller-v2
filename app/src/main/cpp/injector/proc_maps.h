#pragma once

#include <cstdint>
#include <string>

namespace injector {

struct ModuleInfo {
    uintptr_t base = 0;
    uintptr_t end = 0;
    // The substring that was matched, not necessarily the module's real
    // full path -- callers only need base/end for the offset math below.
    std::string path;
};

// Scans /proc/<pid>/maps for the first mapping whose path contains
// name_substr. Pass pid == 0 to read /proc/self/maps instead. base is the
// lowest mapped address for that name, which is what the offset arithmetic
// in injector_main.cpp assumes on both the local and remote side.
bool find_module(int pid, const std::string& name_substr, ModuleInfo& out);

}  // namespace injector

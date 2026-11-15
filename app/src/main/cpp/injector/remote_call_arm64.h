#pragma once

#include <cstdint>
#include <sys/types.h>
#include <vector>

namespace injector {

// Thin wrapper around a single ptrace-attached remote process. Everything
// here assumes arm64-v8a: register layout, calling convention (first eight
// integer args in x0-x7, return value in x0) and the breakpoint trampoline
// used to detect when a remote call has returned. Not safe to share across
// threads.
class RemoteProcess {
public:
    bool attach(pid_t pid);
    bool detach();

    bool read_mem(uintptr_t addr, void* buf, size_t len) const;
    bool write_mem(uintptr_t addr, const void* buf, size_t len) const;

    // Calls func_addr(args...) inside the attached process and returns the
    // value left in x0 through result. Returns false (result left
    // untouched) if the call could not be completed, e.g. the breakpoint
    // never hit within the retry budget.
    bool call(uintptr_t func_addr, const std::vector<uint64_t>& args, uint64_t& result);

private:
    pid_t pid_ = -1;
    int mem_fd_ = -1;
    bool attached_ = false;
};

}  // namespace injector

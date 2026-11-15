#include <android/log.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <sys/mman.h>
#include <vector>

#include "proc_maps.h"
#include "remote_call_arm64.h"

#define LOG_TAG "RootInjector"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

namespace {

// Offset of fn inside whatever currently maps lib_name in THIS process.
// Resolving the remote address the same way (lowest-base mapping whose path
// contains lib_name, plus this offset) only works because the library file
// on disk is identical for both processes -- ASLR only moves the base, not
// internal offsets.
bool local_offset(const char* lib_name, void* fn, uintptr_t& offset_out) {
    injector::ModuleInfo mod{};
    if (!injector::find_module(0, lib_name, mod)) {
        LOGE("could not find local module %s", lib_name);
        return false;
    }
    offset_out = reinterpret_cast<uintptr_t>(fn) - mod.base;
    return true;
}

bool resolve_remote(int pid, const char* lib_name, uintptr_t offset, uintptr_t& addr_out) {
    injector::ModuleInfo mod{};
    if (!injector::find_module(pid, lib_name, mod)) {
        LOGE("could not find remote module %s in pid %d", lib_name, pid);
        return false;
    }
    addr_out = mod.base + offset;
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::fprintf(stderr, "usage: injector <pid> <payload.so path>\n");
        return 1;
    }

    int pid = std::atoi(argv[1]);
    const char* payload_path = argv[2];
    if (pid <= 0 || std::strlen(payload_path) >= 0x1000) {
        LOGE("bad pid or payload path too long");
        return 1;
    }

    uintptr_t dlopen_offset = 0;
    uintptr_t mmap_offset = 0;
    if (!local_offset("libdl.so", reinterpret_cast<void*>(&dlopen), dlopen_offset)) {
        return 1;
    }
    if (!local_offset("libc.so", reinterpret_cast<void*>(&mmap), mmap_offset)) {
        return 1;
    }

    injector::RemoteProcess proc;
    if (!proc.attach(pid)) {
        LOGE("attach to pid %d failed -- confirm the shell running this is actually root "
             "(id -u == 0), and check getenforce if it still fails", pid);
        return 1;
    }

    uintptr_t remote_mmap = 0, remote_dlopen = 0;
    if (!resolve_remote(pid, "libc.so", mmap_offset, remote_mmap) ||
        !resolve_remote(pid, "libdl.so", dlopen_offset, remote_dlopen)) {
        LOGE("could not resolve remote dlopen/mmap, bionic layout may differ "
             "on this Android build; check /proc/%d/maps manually", pid);
        proc.detach();
        return 1;
    }
    LOGI("remote mmap=0x%lx remote dlopen=0x%lx", remote_mmap, remote_dlopen);

    // 1) allocate RW scratch in the target for the payload path string.
    uint64_t scratch = 0;
    std::vector<uint64_t> mmap_args = {
        0,                                     // addr
        0x1000,                                // length
        PROT_READ | PROT_WRITE,                // prot
        MAP_ANONYMOUS | MAP_PRIVATE,            // flags
        static_cast<uint64_t>(-1),              // fd
        0                                        // offset
    };
    if (!proc.call(remote_mmap, mmap_args, scratch) || scratch == 0 ||
        scratch == static_cast<uint64_t>(-1)) {
        LOGE("remote mmap failed");
        proc.detach();
        return 1;
    }

    // 2) write the payload path into that scratch region.
    size_t path_len = std::strlen(payload_path) + 1;
    if (!proc.write_mem(scratch, payload_path, path_len)) {
        LOGE("failed writing payload path into remote scratch at 0x%lx", scratch);
        proc.detach();
        return 1;
    }

    // 3) call dlopen(scratch, RTLD_NOW) in the target.
    uint64_t handle = 0;
    std::vector<uint64_t> dlopen_args = {scratch, RTLD_NOW};
    bool called = proc.call(remote_dlopen, dlopen_args, handle);
    proc.detach();

    if (!called) {
        LOGE("remote dlopen call did not complete");
        return 1;
    }
    if (handle == 0) {
        LOGE("remote dlopen returned NULL. On Android 8+ this is often a linker "
             "namespace rejection rather than a missing file -- see README "
             "'linker namespace' before assuming the path is wrong.");
        return 1;
    }

    LOGI("dlopen succeeded in pid %d, handle=0x%lx", pid, handle);
    std::printf("OK handle=0x%lx\n", handle);
    return 0;
}

#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <unordered_map>
#include "Includes/Logger.h"
#include "Includes/StatusLog.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Dobby/dobby.h"
#include "Color.h"
#include "Includes/Macros.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"
#include "UI/Debug.h"
#include "il2cpp/Il2Cpp.h"
#include "Dump/NepDumper.h"
#include "Dump/chams.h"
#include "Structs/include.h"
#include "Editor/Editor.h"
#include "UI/UI.h"
#include "ImGui/stuff.h"


DUMPER::Dumper *d;

#define targetLibName OBFUSCATE("libil2cpp.so")

int (*_OldWidth)(...);
int newWidth(...) {
    return 1600;
}
int (*_OldHeight)(...);
int newHeight(...) {
    return 720;
}

void *imgui_go(void *) {
    StatusLog::write("thread_start");
    // libil2cpp may not be mapped yet when the payload loads: poll the
    // game's own maps for up to 30s instead of sleeping a blind 10s.
    uintptr_t base = 0;
    for (int i = 0; i < 30 && base == 0; i++) {
        base = (uintptr_t)findLibrary("libil2cpp.so");
        if (base == 0) sleep(1);
    }
    StatusLog::writeHex("libil2cpp_base", base);
    if (base == 0) {
        StatusLog::write("abort: libil2cpp not found");
        pthread_exit(nullptr);
        return nullptr;
    }
    ImGui::address = base;
    StatusLog::write("attach_start");
    IL2Cpp::Il2CppAttach();
    StatusLog::write("attach_done");

    // RTLD_NEXT only finds symbols in objects loaded AFTER the caller.
    // Injected last, the payload sits at the end of the load order, so
    // RTLD_NEXT misses libEGL (loaded by the game long before us) and
    // returns NULL — v1.0 hooked that NULL and the game crashed. Fall back
    // to an explicit libEGL lookup and never hook a null address.
    void *egl_addr = dlsym(RTLD_NEXT, "eglSwapBuffers");
    if (!egl_addr) {
        void *egl = dlopen("libEGL.so", RTLD_NOW);
        if (egl) egl_addr = dlsym(egl, "eglSwapBuffers");
    }
    StatusLog::writeHex("egl_addr", (uintptr_t)egl_addr);
    if (!egl_addr) {
        StatusLog::write("abort: eglSwapBuffers not found");
        pthread_exit(nullptr);
        return nullptr;
    }
    int egl_ret = DobbyHook(egl_addr, (void *)ImGui::hook_eglSwapBuffers, (void **)&ImGui::old_eglSwapBuffers);
    StatusLog::writeInt("egl_hook", egl_ret);
    if (egl_ret != 0) {
        StatusLog::write("abort: DobbyHook eglSwapBuffers failed");
        pthread_exit(nullptr);
        return nullptr;
    }

    // 64-bit-only devices keep libinput in /system/lib64; the old
    // hardcoded /system/lib path is wrong there. Try lib64 first.
    void *sym_input = DobbySymbolResolver("/system/lib64/libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
    if (!sym_input) {
        sym_input = DobbySymbolResolver("/system/lib/libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
    }
    if (sym_input) {
        int in_ret = DobbyHook(sym_input, (void *)ImGui::myInput, (void **)&ImGui::origInput);
        StatusLog::writeInt("input_hook", in_ret);
    } else {
        StatusLog::write("input_hook=skipped (symbol not found)");
    }

    pthread_exit(nullptr);
    return nullptr;
}
__attribute__((constructor))
void lib_main() {
    StatusLog::write("ctor");
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, imgui_go, NULL);
}

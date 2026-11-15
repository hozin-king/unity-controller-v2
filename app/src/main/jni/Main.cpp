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
    sleep(10);
    IL2Cpp::Il2CppAttach();
    ImGui::address = findLibrary("libil2cpp.so");
    
    auto addr = (uintptr_t)dlsym(RTLD_NEXT, "eglSwapBuffers");
    DobbyHook((void *)addr, (void *)ImGui::hook_eglSwapBuffers, (void **)&ImGui::old_eglSwapBuffers);
    void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));
    if (NULL != sym_input) {
        DobbyHook((void *)sym_input, (void *)ImGui::myInput, (void **)&ImGui::origInput);
    }
    
    pthread_exit(nullptr);
    return nullptr;
}
__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, imgui_go, NULL);
}

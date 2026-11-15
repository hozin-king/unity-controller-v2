cmake_minimum_required(VERSION 3.22.1)
project(UnityControllerInjector CXX C)

# --- payload: the Unity ImGui menu library, injected into the target game.
# Built from the original ndkBuild sources (see app/src/main/jni/). It
# self-initializes on dlopen (constructor thread: sleep 10s, attach IL2CPP,
# hook eglSwapBuffers), so the injector only has to dlopen it in the target.
set(MENU_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../jni)

add_library(dobby STATIC IMPORTED)
set_target_properties(dobby PROPERTIES
    IMPORTED_LOCATION ${MENU_DIR}/libraries/arm64-v8a/libdobby.a
)

add_library(HarshitController SHARED
    ${MENU_DIR}/Main.cpp
    ${MENU_DIR}/il2cpp/Il2Cpp.cpp
    ${MENU_DIR}/Dump/NepDumper.cpp
    ${MENU_DIR}/Substrate/hde64.c
    ${MENU_DIR}/Substrate/SubstrateDebug.cpp
    ${MENU_DIR}/Substrate/SubstrateHook.cpp
    ${MENU_DIR}/Substrate/SubstratePosixMemory.cpp
    ${MENU_DIR}/Substrate/SymbolFinder.cpp
    ${MENU_DIR}/KittyMemory/KittyMemory.cpp
    ${MENU_DIR}/KittyMemory/MemoryPatch.cpp
    ${MENU_DIR}/KittyMemory/MemoryBackup.cpp
    ${MENU_DIR}/KittyMemory/KittyUtils.cpp
    ${MENU_DIR}/And64InlineHook/And64InlineHook.cpp
    ${MENU_DIR}/ImGui/imgui.cpp
    ${MENU_DIR}/ImGui/imgui_draw.cpp
    ${MENU_DIR}/ImGui/imgui_demo.cpp
    ${MENU_DIR}/ImGui/imgui_widgets.cpp
    ${MENU_DIR}/ImGui/imgui_tables.cpp
    ${MENU_DIR}/ImGui/backends/imgui_impl_opengl3.cpp
    ${MENU_DIR}/ImGui/backends/imgui_impl_android.cpp
)
# Menu code predates the current toolchain: keep its proven C++17, silence
# warnings (old ndkBuild used -w), do NOT use -Werror here.
set_target_properties(HarshitController PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_compile_options(HarshitController PRIVATE
    -w -fvisibility=hidden -fpermissive -fexceptions
    -Wno-error=c++11-narrowing
)
target_include_directories(HarshitController PRIVATE
    ${MENU_DIR}
    ${MENU_DIR}/ImGui
    ${MENU_DIR}/ImGui/backends
    ${MENU_DIR}/Includes
    ${MENU_DIR}/UI
    ${MENU_DIR}/Menu
    ${MENU_DIR}/Dump
    ${MENU_DIR}/Editor
    ${MENU_DIR}/Structs
    ${MENU_DIR}/il2cpp
    ${MENU_DIR}/KittyMemory
    ${MENU_DIR}/Substrate
    ${MENU_DIR}/And64InlineHook
    ${MENU_DIR}/Unity
)
target_link_libraries(HarshitController dobby log android EGL GLESv2)
target_link_options(HarshitController PRIVATE -Wl,--gc-sections)

# --- injector: standalone PIE binary, run as root via `su -c`. Same design
# as RootInjector: it is never dlopen'd, only executed, and renaming its
# output to .so is purely so AGP bundles it under lib/<ABI>/ next to the
# payload. Confirm after a build that lib/arm64-v8a/libinjector.so exists
# in the APK and is sized like a real binary, not 0 bytes.
add_executable(libinjector
    injector/injector_main.cpp
    injector/proc_maps.cpp
    injector/remote_call_arm64.cpp
)
set_target_properties(libinjector PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    SUFFIX ".so"
)
target_link_libraries(libinjector log dl)

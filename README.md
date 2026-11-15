# Unity Controller Injector

Generic Unity ImGui mod-menu framework, converted to **root injector mode**.

Original project: a NepMods/Aide-ImGui fork ("Unity Controller v2") — a
floating ImGui menu for Unity games (ESP / Aimbot / Chams testers, IL2CPP
dumper, object scanner). The menu library (`libHarshitController.so`)
self-initializes on `dlopen`: a constructor thread waits ~10s, attaches
IL2CPP, and hooks `eglSwapBuffers` via Dobby.

## What changed in this fork

- **No more repack.** The old flow (merge the menu APK into a game APK and
  `System.loadLibrary("HarshitController")` from its MainActivity) is gone.
  This app is now a standalone **root injector**: pick a running game's
  package name, find its PID, and ptrace-inject the menu library into the
  running process with `su`. The injector binary + technique are the same
  design as the generic RootInjector (ptrace, remote `mmap` + `dlopen`).
- **Login removed.** The original `UI/Login.h` phoned home to the author's
  key-auth server (`admin.nepmods.xyz`). That whole flow is deleted, along
  with the libcurl/libssl/libcrypto prebuilts (~52 MB). Nothing in this app
  talks to any server.
- **Build modernized.** AGP 8.5.2 / Gradle 8.7 / NDK 25 / CMake 3.22.1,
  `compileSdk 34`, `minSdk 24`, **arm64-v8a only**. The menu native code is
  built from the original sources via CMake (previously ndkBuild); the menu
  code itself is untouched except the login removal and one include-path
  case fix (`includes/` → `Includes/`, needed on case-sensitive Linux).
- **No game target.** The menu is generic: it hooks `libil2cpp.so` and its
  ESP/Aimbot/Chams tabs are testers that need per-game offsets to do
  anything. No game-specific target, offsets, or features are wired in.

## Usage (rooted phone, arm64)

1. Launch the target Unity game first.
2. Open Unity Controller Injector → **Check root** (must say available).
3. Enter the game's package/process name → **Find PID**.
4. Tap **Inject**. The menu library is dlopen'd inside the game; the ImGui
   overlay appears after ~10 seconds (floating icon / menu).

## Build

CI builds a debug APK on every push to `main`
(`.github/workflows/build.yml`). Locally: `./gradlew assembleDebug`
(needs Android SDK + NDK 25.2.9519653 + CMake 3.22.1).

## Packaging check

After a build, the APK must contain both, under `lib/arm64-v8a/`:

- `libHarshitController.so` — the menu payload
- `libinjector.so` — the standalone injector binary (real executable size,
  not 0 bytes)

## Original README (kept for reference)

**Change log (original author):**

```
-> Fixed for Aide
-> Added ByNameModding library
-> Touch Fixed [ Need One Offset ]
-> Made Code Cleaner
-> Made Easy
-> I Wont Work On This Anymore
-> Added Univarsal Touch
```

**Invoke code (old repack flow, no longer used):**

```
    const-string v0, "nepGUI"
    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```

**Start a window:**

```
   ImGui::Begin("Window Name", bool_isActive, ImGuiWindowFlags);
   ImGui::End();
```

Upstream: https://github.com/NepMods/Aide-Imgui

namespace ImGui {
    int glHeight, glWidth;
    bool setup;
    uintptr_t address;
    
    #define HOOKAF(ret, func, ...) \
        ret (*orig##func)(__VA_ARGS__); \
        ret my##func(__VA_ARGS__)

    HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
        origInput(thiz, ex_ab, ex_ac);
        ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
        return;
    }
    
    void SetupImgui() {
        IMGUI_CHECKVERSION();
        CreateContext();
        ImGuiIO& io = GetIO();
        io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
        ImGui_ImplOpenGL3_Init("#version 100");
        ImFontConfig font_cfg;
        font_cfg.SizePixels = 22.0f;
        io.Fonts->AddFontDefault(&font_cfg);
        GetStyle().ScaleAllSizes(3.0f);
    }


    EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
    EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
        // First call proves the hook is live; written before any Unity or
        // menu call so a crash below still leaves this stage in the log.
        static bool first_frame_logged = false;
        if (!first_frame_logged) {
            first_frame_logged = true;
            StatusLog::write("first_frame");
        }
        Structs::Display *d = Structs::Display::get_main();
        
        glWidth = d->get_renderingWidth();;//Structs::Screen::get_width();
        glHeight = d->get_renderingHeight();// Structs::Screen::get_height();
        
        if (!setup) {
            SetupImgui();
            setup = true;
        }
    
        ImGuiIO &io = GetIO();
        
        ImGuiStyle& menu = GetStyle();
        menu.WindowMinSize = ImVec2(glWidth/2 -150, glHeight/1.5);
            //PollUnicodeChars();
        OBJECT_SCANNER::glWidth = glWidth;
        OBJECT_SCANNER::glHeight = glHeight;
        
        static bool WantTextInputLast = false;
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        NewFrame();

        AIMBOT_TESTER::DoAimBot();
        ESP_TESTER::DrawESP(ImGui::GetBackgroundDrawList(), glWidth, glHeight);
        UI::RenderUI();
      //  DEBUG::window();
        EndFrame();
        Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
    
    
        return old_eglSwapBuffers(dpy, surface);
    }
    
}

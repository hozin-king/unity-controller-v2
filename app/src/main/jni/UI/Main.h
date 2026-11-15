namespace UI {
    void RenderUI()
    {
        ImGui::Begin("UnityController by Harshit", nullptr);
        if (ImGui::BeginTabBar("Controls"))
        {
            if (ImGui::BeginTabItem("Application"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                UI::GameInfo::UI();
                ImGui::PopStyleVar();
    
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Engine Info"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                UI::UnityInfo::UI();
                ImGui::PopStyleVar();
    
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Scenes"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                UI::Scene::UI();
                ImGui::PopStyleVar();
    
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("ESP"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                UI::EspTester::UI();
                ImGui::PopStyleVar();
    
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Aimbot"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                UI::AimbotTester::UI();
                ImGui::PopStyleVar();
    
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Camera"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                UI::Camera::UI();
                ImGui::PopStyleVar();
    
                ImGui::EndTabItem();
            }
             if (ImGui::BeginTabItem("Shader"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                UI::ChamsTester::UI();
                ImGui::PopStyleVar();
    
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Offsets"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                UI::Offset::UI();
                ImGui::PopStyleVar();
    
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    
        ImGui::End();
    }
    
}


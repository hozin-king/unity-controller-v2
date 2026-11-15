#include "Structs/include.h"
namespace UI {
    class GameInfo {
         public:
            static void UI() {
                static DUMPER::Dumper * d = NULL;
                
                
                ImGui::Checkbox("Scan", &OBJECT_SCANNER::scanning);
        if(OBJECT_SCANNER::scanning) {
            if(!OBJECT_SCANNER::HookStarted) {
                OBJECT_SCANNER::HookStarted = true;
                OBJECT_SCANNER::startScan();
            }
            ImGui::Text("Scanning (Transform)....");
         } 
         ImGui::Checkbox("Force Scan", &OBJECT_SCANNER::force);
         ImGui::SameLine();
         
         ImGui::Checkbox("Filter Classes", &OBJECT_SCANNER::FilterClass);
         ImGui::SliderInt("Threads", &OBJECT_SCANNER::workers, 0, 10);
        
         ImGui::SliderInt("Interval/Ms", &OBJECT_SCANNER::interval, 0, 100);
         ImGui::Text("Increase Interval if its crashing on Loading/Lobby");
         ImGui::Text("If You Decrease Interval, it will put more load, but Scanning will be fast,\n if You Increase it, Load will be less, and Scanning will be slow.");
        
                ImGui::Text("Game Info: ");
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                
                    //ImGui::Text("\tPlaying: %s", Structs::Application::get_isPlaying() ? "True" : "False");
                    ImGui::Text("\tVersion: %s", Structs::Application::get_version()->getString().c_str());
                    ImGui::Text("\tUntiy Version: %s", Structs::Application::get_unityVersion()->getString().c_str());
                 //   ImGui::Text("\tPackage: %s", DUMPER::GetPackageName());
                    ImGui::BeginChild("ScrollableRegion", ImVec2(ImGui::GetContentRegionAvail().x * 0.9f, ImGui::GetContentRegionAvail().y * 0.9), false, ImGuiWindowFlags_HorizontalScrollbar);
                        //ImGui::Text("\tApk Path: %s", Structs::Application::get_dataPath()->getString().c_str());
                        ImGui::Text("\tData Path: %s", (std::string("/storage/emulated/0/Android/data/")+d->GetPackageName()+std::string("/files")).c_str());
                    ImGui::EndChild();
                ImGui::PopStyleVar();
            }
    };
}

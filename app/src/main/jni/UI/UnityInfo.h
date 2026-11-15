#include "Structs/include.h"
namespace UI {
    bool isStarted = false;
    static int speed = 1;
     int savedSpeed = 1;
    DUMPER::Dumper *d = NULL;
    class UnityInfo {
       public:
            static void UI() {
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Text("Engine Info: (%s)", d->GetPackageName());
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                    ImGui::Text("\tName: Unity");
                    ImGui::Text("\tVersion: %s", Structs::Application::get_unityVersion()->getString().c_str());
                    ImGui::Text("\tCurrent FPS: %.1f FPS", io.Framerate);
                    ImGui::Text("\tTime Played: %f", Structs::Time::get_realtimeSinceStartup());
                    ImGui::Text("\tSpeed: %dx", savedSpeed);
                    ImGui::SliderInt("Set Speed", &speed, 0, 100);
                    if(savedSpeed != speed) {
                        Structs::Time::set_timeScale((float)speed);
                        savedSpeed = speed;
                    }
                    ImGui::Text("Engine Functions: (%s)", d->GetPackageName());
                    auto il2cpp_handle = dlopen("libil2cpp.so", 4);
                    ImGui::Text("\t");
                    ImGui::SameLine();
                    
                    if(d != NULL) {
                        ImGui::Text("Generated Dump Of %d Images", d->modules.size());
                        
                        ImGui::Text("\t");
                        ImGui::SameLine();
                    } else if(d == NULL) {
                        d = new DUMPER::Dumper(il2cpp_handle);
                    }
                    if(ImGui::Button("Save Dump")) {
                        d->GenDump();
                    }
                ImGui::PopStyleVar();
                
            }
    };
}

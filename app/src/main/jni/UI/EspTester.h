#include "Structs/include.h"
#include <cmath>
#include <algorithm>
#include <string>
    #define CREATE_COLOR(r, g, b, a) new float[4] {(float)r, (float)g, (float)b, (float)a};

namespace ESP_TESTER {
    static int itemsPerPage = 20;
    static int selectedPage = 0;
    static int checkedItem = -1;

    struct EspConfig {
        bool isEsp = false;
        bool line = false;
        bool box = false;
        bool name = false;
    } config;
    
    
    int32_t ToColor(float *col) {
        return ImGui::ColorConvertFloat4ToU32(*(ImVec4 *) (col));
    }
    
    float *EspAlive = CREATE_COLOR(255, 255, 255, 255);
    float espLineSize = 1.0;
    
    void DrawESP(ImDrawList *draw, int glWidth, int glHeight) {
        if(!OBJECT_SCANNER::force && Structs::SceneManager::get_sceneCount() > 1) {
            UI::SelectedCam = NULL;
            OBJECT_SCANNER::EspObjects.clear();
            OBJECT_SCANNER::AimbotObjects.clear();
            OBJECT_SCANNER::PlayerObjects.clear();
            OBJECT_SCANNER::ClassList.clear();
            OBJECT_SCANNER::AimBotMe = NULL;
            OBJECT_SCANNER::playerlate = NULL;
            OBJECT_SCANNER::ClassSet.clear();
            checkedItem = -1;
            selectedPage = 0;
            AIMBOT_TESTER::checkedItem = -1;
            AIMBOT_TESTER::checkedItem2 = -1;
            AIMBOT_TESTER::selectedPage = 0;
            AIMBOT_TESTER::selectedPage2 = 0;
            draw->AddText({glWidth/2, glHeight/1.2}, ToColor(EspAlive), "UnityGameController [Message] : Loading...");
            return;
        }
        void *player = OBJECT_SCANNER::playerlate;
        if (player != nullptr && config.isEsp) {
            Structs::Camera *camera = UI::getCam();
            if(!camera) {
                OBJECT_SCANNER::EspObjects.clear();
                 return;
            }
            if(camera != nullptr) {
                for(int i = 0; i < OBJECT_SCANNER::EspObjects.size(); i++) {
                    
                   Structs::Transform *enemy = OBJECT_SCANNER::EspObjects[i];
                       
                    try {
                        if(!enemy) {
                            OBJECT_SCANNER::EspObjects.erase(OBJECT_SCANNER::EspObjects.begin() + i);
                            continue;
                        }
                        if(!enemy->alive()) {
                           OBJECT_SCANNER::EspObjects.erase(OBJECT_SCANNER::EspObjects.begin() + i);
                           continue;
                       }          
                        Vector3 pos = enemy->get_position();
                    
                        if(pos == Vector3::Zero()) {
                            continue;
                         }                
                         Vector3 RealPos = camera->WorldToScreenPoint(pos);
                         if (RealPos.Z < 1.f) continue;
                         
                         Vector3 Origin = pos;
                         Origin.Y += 1.7f;
                         Vector3 NewPos = camera->WorldToScreenPoint(Origin);
                         float Hight = abs(NewPos.Y - RealPos.Y);
                         float Width = Hight * 0.6f;
                         
                         if(config.line) {
                             draw->AddLine({(float) glWidth / 2, 20},  { (glWidth- (glWidth- NewPos.X)), (glHeight - NewPos.Y - 8)}, ToColor(EspAlive),espLineSize);
                         }
                         if(config.box) {
                             ImVec2 vStart = {(glWidth- (glWidth- NewPos.X)) - (Width / 2), (glHeight - NewPos.Y - 8)};
                             ImVec2 vEnd = {vStart.x + Width, vStart.y + Hight};
                             draw->AddRect(vStart, vEnd, ToColor(EspAlive), 1.4f);
                         }
                         if(config.name) {
                            std::string t = "";
                            t += enemy->get_name()->getString();
                            draw->AddText({(glWidth - (glWidth - NewPos.X)), glHeight - NewPos.Y - 12.0f}, ToColor(EspAlive), t.c_str());
                         }
                         
                     } catch(std::string error) {
                         
                    }     
                }
            }    
        }    
    }
    
}
    
namespace UI {
    class EspTester {
    public:
         static void UI() {
            
            // Create tabs
            ImVec2 windowSize(800, 800);
            if (ImGui::BeginTabBar("Controls")) {
                if (ImGui::BeginTabItem("Classes")) {
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                    
                    if (ImGui::BeginCombo("Classes", OBJECT_SCANNER::espclass != "null" ? OBJECT_SCANNER::espclass : "-- Select --")) {
                        for (int i = 0; i < OBJECT_SCANNER::ClassList.size(); i++) {
                            bool selected = OBJECT_SCANNER::ClassList[i] == OBJECT_SCANNER::espclass;
                            if (ImGui::Selectable(OBJECT_SCANNER::ClassList[i].c_str(), selected)) {
                                 OBJECT_SCANNER::EspObjects.clear();
                                 OBJECT_SCANNER::playerlate = NULL;
                                 OBJECT_SCANNER::espclass = OBJECT_SCANNER::ClassList[i].c_str();
                            }
                            if (selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::Text("Objects: %d", OBJECT_SCANNER::EspObjects.size());
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                    ImGui::Checkbox("Enable Esp", &ESP_TESTER::config.isEsp);
                    ImGui::Checkbox("Esp Line", &ESP_TESTER::config.line);
                    ImGui::Checkbox("Esp Box", &ESP_TESTER::config.box);
                    ImGui::Checkbox("Esp Name", &ESP_TESTER::config.name);
                    ImGui::PopStyleVar();
                    ImGui::PopStyleVar();
                    ImGui::EndTabItem();
                }
                
                
                
                if (ImGui::BeginTabItem("ESP Objects")) {
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                    for (int i = 0; i < OBJECT_SCANNER::EspObjects.size(); i++) {
                        if(OBJECT_SCANNER::EspObjects[i] == nullptr) {
                            continue;
                        }
                        Editor::Transform(OBJECT_SCANNER::EspObjects[i]).UI(i);
                    }
                    
                    ImGui::PopStyleVar();
                    ImGui::EndTabItem();
                }
                
                
                ImGui::EndTabBar();
            }         
        }
    };
}


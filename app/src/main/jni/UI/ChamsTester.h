#include "Structs/include.h"
#include <cmath>
#include <algorithm>
#include <string>
#define CREATE_COLOR(r, g, b, a) new float[4] {(float)r, (float)g, (float)b, (float)a};

namespace CHAMS_TESTER {
    
    static int itemsPerPage = 20;
    static int selectedPage = 0;
    static int checkedItem = -1;
    float *ChamsColor = CREATE_COLOR(255, 0, 0, 255);


}
namespace UI {
    class ChamsTester {
    public:
         static void UI() {
            if(!CHAMS_SCANNER::isStarted) {
                if(ImGui::Button("Scan")) {
                    CHAMS_SCANNER::Start();
                }
            } else {
                ImGui::Text("Scanning Shaders");
            }
           
            ImVec2 windowSize(800, 800);
            if (ImGui::BeginTabBar("Controls")) {
                if (ImGui::BeginTabItem("Shader Names")) {
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                    ImVec2 windowSize(200, 300);
                    
                    if (ImGui::Button("Previous Page")) {
                        if (CHAMS_TESTER::selectedPage > 0) {
                            --CHAMS_TESTER::selectedPage;
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Next Page")) {
                        if ((CHAMS_TESTER::selectedPage + 1) * CHAMS_TESTER::itemsPerPage < CHAMS_SCANNER::chams.size()) {
                            ++CHAMS_TESTER::selectedPage;
                        }
                    }
                    ImGui::Text("Page: %d", CHAMS_TESTER::selectedPage + 1);
                    ImGui::BeginChild("ScrollableRegion", ImVec2(ImGui::GetContentRegionAvail().x * 0.9f, ImGui::GetContentRegionAvail().y * 0.9), false, ImGuiWindowFlags_HorizontalScrollbar);
                    int startIdx = CHAMS_TESTER::selectedPage * CHAMS_TESTER::itemsPerPage;
                    int endIdx = std::min(startIdx + CHAMS_TESTER::itemsPerPage, static_cast<int>(CHAMS_SCANNER::chams.size()));

                    for (int i = startIdx; i < endIdx; ++i) {
                        bool isChecked = (i == CHAMS_TESTER::checkedItem);
                        if (ImGui::Checkbox(CHAMS_SCANNER::chams[i].c_str(), &isChecked)) {
                            CHAMS_TESTER::checkedItem = isChecked ? i : -1;
                            CHAMS_SCANNER::config.shaderName = CHAMS_SCANNER::chams[i].c_str();
                        }
                    }
                    ImGui::EndChild();
                    
                    ImGui::PopStyleVar();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Functions")) {
                    ImGui::Text(CHAMS_SCANNER::config.shaderName);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                    ImGui::Checkbox("Chams", &CHAMS_SCANNER::config.enableWallhack);
                    ImGui::Checkbox("Chams Wire", &CHAMS_SCANNER::config.enableWallhackW);
                    ImGui::Checkbox("Chams Glow", &CHAMS_SCANNER::config.enableWallhackG);
                    ImGui::Checkbox("Chams Outline", &CHAMS_SCANNER::config.enableWallhackO);
                    ImGui::Checkbox("Chams Rainbow", &CHAMS_SCANNER::config.enableRainbow);
                    ImGui::Checkbox("Chams Shader", &CHAMS_SCANNER::config.enableWallhackS);
                    ImGui::Checkbox("Chams Visibility", &CHAMS_SCANNER::config.visibSh);
                    ImGui::Checkbox("Chams Flat Black", &CHAMS_SCANNER::config.BackFlat);
                    ImGui::Checkbox("Chams Invisible", &CHAMS_SCANNER::config.invis);
                    ImGui::SliderInt("Set Width", &CHAMS_SCANNER::config.w, 0, 120);
                    ImGui::ColorEdit4("Chams Color", CHAMS_TESTER::ChamsColor, ImGuiColorEditFlags_NoInputs);
                    CHAMS_SCANNER::config.r = CHAMS_TESTER::ChamsColor[0] * 255;
                    CHAMS_SCANNER::config.g = CHAMS_TESTER::ChamsColor[1] * 255;
                    CHAMS_SCANNER::config.b = CHAMS_TESTER::ChamsColor[2] * 255;
			        CHAMS_SCANNER::config.a = CHAMS_TESTER::ChamsColor[3] * 255;
                    
                    ImGui::PopStyleVar();
                    ImGui::EndTabItem();
                }
                
                ImGui::EndTabBar();
            }         
        }
    };
}


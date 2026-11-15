#include "Structs/include.h"
#include <cmath>
#include <algorithm>
#include <string>
#define CREATE_COLOR(r, g, b, a) new float[4] {(float)r, (float)g, (float)b, (float)a};

namespace AIMBOT_TESTER {
    static int itemsPerPage = 20;
    static int selectedPage = 0;
    static int checkedItem = -1;
    
    static int selectedPage2 = 0;
    static int checkedItem2 = -1;
    struct aim_conf {
        bool aimbot;
        bool teleport;
    } config;
    Vector3 getTop(Vector3 PosNew) {
        Vector3 Origin;
        Origin = PosNew;
        Origin.Y += 0.4f;
        float posnum = 1;
        Origin.Y += posnum;
        Vector3 BoxPosNew = {0.f, 0.f, 0.f};
        BoxPosNew = (Origin);
        return BoxPosNew;
    }
    
    Structs::Transform* GetClosestEnemy( std::vector<Structs::Transform*>& enemies, Structs::Transform* me) {
        Structs::Transform* closestEnemy = nullptr;
        float shortestDistance = std::numeric_limits<float>::max();
        float maxAngle = 180.0f;
        for (Structs::Transform* enemy : enemies) {
            if(!enemy) {
                
                continue;
            }
            if(!enemy->alive()) {
                continue;
            }
            Vector3 playerPos = enemy->get_position();
            Vector3 localPlayerPos = me->get_position();
            Vector3 targetDir = Vector3::Normalized(playerPos - localPlayerPos);
            float angle = std::acos(Vector3::Dot(targetDir, me->get_forward())) * (180.0f / M_PI);
    
            if (angle <= maxAngle){
                if (angle < shortestDistance){
                    shortestDistance = angle;
                    closestEnemy = enemy;
                }
            }
        }
    
        return closestEnemy;
    }
    
    void RotateCameraToTarget(){
        Structs::Transform* me = OBJECT_SCANNER::AimBotMe;
        std::vector<Structs::Transform*> enemies = OBJECT_SCANNER::AimbotObjects;
        
        Structs::Transform* closestEnemy = GetClosestEnemy(enemies, me);
        if (closestEnemy != nullptr && me != nullptr) {
            Vector3 direction = getTop(closestEnemy->get_position()) - (me->get_position());
            Quaternion rotation = Quaternion::LookRotation(direction);
            
            if(config.aimbot) {
                me->set_rotation(rotation);
            }
            if(config.teleport) {
                me->set_position(closestEnemy->get_position() + Vector3(3, 0, 3));
            }
            
        }
    }
    
    
    void DoAimBot() {
        if(OBJECT_SCANNER::AimBotMe != NULL) {
            RotateCameraToTarget();
        }
                         
    }
}
namespace UI {
    class AimbotTester {
    public:
         static void UI() {
                if (ImGui::BeginTabBar("Controls")) {
                    if (ImGui::BeginTabItem("Enemy Classes")) {
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                        
                        if (ImGui::BeginCombo("Enemy", OBJECT_SCANNER::aimbotclass != "null" ? OBJECT_SCANNER::aimbotclass : "-- Select --")) {
                            for (int i = 0; i < OBJECT_SCANNER::ClassList.size(); i++) {
                                bool selected = OBJECT_SCANNER::ClassList[i] == OBJECT_SCANNER::aimbotclass;
                                if (ImGui::Selectable(OBJECT_SCANNER::ClassList[i].c_str(), selected))
                                    OBJECT_SCANNER::aimbotclass = OBJECT_SCANNER::ClassList[i].c_str();
                                if (selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }
                        if (ImGui::BeginCombo("Player", OBJECT_SCANNER::playerclass != "null" ? OBJECT_SCANNER::playerclass : "-- Select --")) {
                            for (int i = 0; i < OBJECT_SCANNER::ClassList.size(); i++) {
                                bool selected = OBJECT_SCANNER::ClassList[i] == OBJECT_SCANNER::playerclass;
                                if (ImGui::Selectable(OBJECT_SCANNER::ClassList[i].c_str(), selected)) {
                                    OBJECT_SCANNER::AimbotObjects.clear();
                                    OBJECT_SCANNER::PlayerObjects.clear();
                                    OBJECT_SCANNER::AimBotMe = NULL;
                                    OBJECT_SCANNER::playerclass = OBJECT_SCANNER::ClassList[i].c_str();
                                }
                                if (selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                        ImGui::Checkbox("Aimbot", &AIMBOT_TESTER::config.aimbot);
                        ImGui::Checkbox("Teleport", &AIMBOT_TESTER::config.teleport);
                        ImGui::PopStyleVar();
                        
                        ImGui::PopStyleVar();
                        ImGui::EndTabItem();
                    }
                   
                    
                    if (ImGui::BeginTabItem("Player Objects")) {
                        ImGui::Text("If Player & Enemy Is Same Class, Select First option");
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                        for (int i = 0; i < OBJECT_SCANNER::PlayerObjects.size(); i++) {
                            ImGui::Checkbox(OBJECT_SCANNER::playerclass, new bool(OBJECT_SCANNER::PlayerObjects[i] == OBJECT_SCANNER::AimBotMe));
                            if(ImGui::Button("Select")) {
                                OBJECT_SCANNER::AimBotMe = OBJECT_SCANNER::PlayerObjects[i];
                            }
                        }
                        
                        ImGui::PopStyleVar();
                        ImGui::EndTabItem();
                    }
                    
                    ImGui::EndTabBar();
                }
            
            }
            
    };
}


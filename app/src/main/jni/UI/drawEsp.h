namespace ESP_TESTER {
        #define CREATE_COLOR(r, g, b, a) new float[4] {(float)r, (float)g, (float)b, (float)a};

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
    
    void DrawESP(ImDrawList *draw, int glWidth, int glHeight, Structs::Transform *playerlate, std::vector<Structs::Transform *> *EspObjects) {
        if(Structs::SceneManager::get_sceneCount() > 1) {
            
            draw->AddText({glWidth/2, glHeight/1.2}, ToColor(EspAlive), "UnityGameController [Message] : Loading...");
                         
            
            
            return;
        }
        void *player = playerlate;
        if (player != nullptr && config.isEsp) {
            Structs::Camera *camera = Structs::Camera::get_main();
            if(!camera->alive()) {
 
                 return;
            }
            if(EspObjects) {
                std::vector<Structs::Transform *> Obj = *EspObjects;
            
            if(camera != nullptr) {
                for(int i = 0; i < Obj.size(); i++) {
                    
                   Structs::Transform *enemy = Obj[i];
                       
                    try {
                        if(!enemy) {
                            Obj.erase(Obj.begin() + i);
                            continue;
                        }
                        if(!enemy->alive()) {
                           Obj.erase(Obj.begin() + i);
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
                         EspObjects->erase(EspObjects->begin() + i);
                            
                    }  
                }     
                }
            }    
        }    
    }
    
}

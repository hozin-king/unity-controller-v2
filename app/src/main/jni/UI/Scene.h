#include "Structs/include.h"
namespace UI {
    class Scene {
        
        public:
            static void UI() {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
                    ImGui::Text("Scenes: %d", Structs::SceneManager::get_sceneCount());
                    for (int i = 0; i <  Structs::SceneManager::get_sceneCount(); i++) {
                        std::string str = "Scene: ";
                        str += std::to_string((uintptr_t)Structs::SceneManager::get_sceneAt(i));
                        bool isChecked = Structs::SceneManager::get_sceneAt(i) == Structs::SceneManager::getActiveScene();
                        ImGui::Checkbox(str.c_str(), &isChecked);
                    }
                ImGui::PopStyleVar();
            }
    };
}

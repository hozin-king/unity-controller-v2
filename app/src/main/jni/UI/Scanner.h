#include "Structs/include.h"
namespace UI {
    class Scanner {  
        public:
            static void UI() {
            ImGui::Checkbox("Scan (Classes)", &OBJECT_SCANNER::scanning);
            if(OBJECT_SCANNER::scanning) {
                if(!OBJECT_SCANNER::HookStarted) {
                    OBJECT_SCANNER::HookStarted = true;   
                    OBJECT_SCANNER::startScan();
                }
                ImGui::Text("Scanning (Transform)....");
             } 
             
             
                ImGui::PopStyleVar();
            }
    };
}

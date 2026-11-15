#include "Structs/include.h"
namespace UI {
    Structs::Camera *SelectedCam = NULL;
    
    Structs::Camera * getCam() {
        if(!SelectedCam) {
            return Structs::Camera::get_main();
        } else {
            return SelectedCam;
        }
    }
    class Camera {
         public:
            static void UI() {
                
                Array<Structs::Camera *> *allCams = Structs::Camera::get_allCameras();
                if(allCams == NULL) {
                    goto chooser;
                }
                if(SelectedCam == NULL) {
                    SelectedCam = Structs::Camera::get_main();;
                }
                if((!SelectedCam) && (allCams->getLength() > 0)) {
                    SelectedCam =  *allCams[0].getPointer();
                }
                ImGui::Text("Total Camera: %d", Structs::Camera::get_allCamerasCount());
               
                if (ImGui::BeginCombo("Camera List",  "-- Select Camera --")) {
                    
                        for (int i = 0; i < allCams->getLength(); i++) {
                            Structs::Camera *thisCam = *allCams[i].getPointer();
                            bool selected =  thisCam == SelectedCam;
                            if (ImGui::Selectable(("Camera"+std::to_string(i)).c_str(), selected)) {
                                 SelectedCam = thisCam;
                             }
                            if (selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                 }
                chooser:
               Structs::Camera * cam = getCam();
                
                
               if(!cam) {
                   ImGui::Text("No Camera");
                   return;
               }
               Editor::Transform(cam->get_transform()).UI(0);
                
                ImGui::Text("Near Clip Plain");
                auto *EncF = new Editor::Float(cam->get_nearClipPlane(), "Near Clip Plain");
                if(EncF->UI()) {
                    cam->set_nearClipPlane(EncF->data());
                }
                ImGui::Text("Far Clip Plain");
                auto *EncF2 = new Editor::Float(cam->get_farClipPlane(), "Far Clip Plain");
                if(EncF2->UI()) {
                    cam->set_farClipPlane(EncF2->data());
                }
                
                ImGui::Text("FOV");
                auto *EncF3 = new Editor::Float(cam->get_fieldOfView(), "FOV");
                if(EncF3->UI()) {
                    cam->set_fieldOfView(EncF3->data());
                }
                
            }
    };
}

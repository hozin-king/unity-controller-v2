namespace Editor {
    class Vector_3 {
        Vector3 obj;
        std::string name;
        public:
            Vector_3(Vector3 data, std::string text) {
                obj = data;
                name = text;
            }
            bool UI(float power = 1) {
                float position[3] = {obj.X, obj.Y, obj.Z};
                if(ImGui::DragFloat3(name.c_str(), position, power)) {
                    obj = Vector3(position[0], position[1], position[2]);
                    return true;
                }
                return false;
            }
            Vector3 data() {
                return obj;
            }
    };
    class Int {
        int obj;
        std::string name;
        public:
            Int(int data, std::string text) {
                obj = data;
                name = text;
            }
            bool UI() {
                if(ImGui::DragInt(name.c_str(), &obj, 1)) {
                    return true;
                }
                return false;
            }
            int data() {
                return obj;
            }
    };
    class Bool {
        bool obj;
        std::string name;
        public:
            Bool(bool data, std::string text) {
                obj = data;
                name = text;
            }
            bool UI() {
                if(ImGui::Checkbox(name.c_str(), &obj)) {
                    return true;
                }
                return false;
            }
            bool data() {
                return obj;
            }
    };
    class Float {
        float obj;
        std::string name;
        public:
            Float(float data, std::string text) {
                obj = data;
                name = text;
            }
            bool UI() {
                if(ImGui::DragFloat(name.c_str(), &obj, 1)) {
                    return true;
                }
                return false;
            }
            float data() {
                return obj;
            }
    };
    class Transform {
        Structs::Transform *realData;
        Structs::Transform *obj;
          
        public:
            Transform(Structs::Transform *t) {
                realData = t;
                obj = t;
            }
            void reset() {
                obj = realData;
            }
            void UI(int i) {
                if(!obj) return;
                std::string t = obj->get_name()->getString();
                ImGui::PushID(i);
               
                ImGui::Text(t.c_str());
                
                
                ImGui::Text("Move");
                if(ImGui::Button("Left")) {
                    obj->set_position(obj->get_position() - obj->get_right());
                }
                ImGui::SameLine();
                if(ImGui::Button("Right")) {
                    obj->set_position(obj->get_position() + obj->get_right());
                }
                ImGui::SameLine();
                if(ImGui::Button("Back")) {
                    obj->set_position(obj->get_position() - obj->get_forward());
                }
                ImGui::SameLine();
                if(ImGui::Button("Forward")) {
                    obj->set_position(obj->get_position() + obj->get_forward());
                }
                ImGui::SameLine();
                if(ImGui::Button("Up")) {
                    obj->set_position(obj->get_position() + obj->get_up());
                }
                ImGui::SameLine();
                if(ImGui::Button("Down")) {
                    obj->set_position(obj->get_position() - obj->get_up());
                }
                
                Vector_3 *v3EditorRot = new Vector_3(Quaternion::ToEuler(obj->get_rotation()), "Rotation");
                if(v3EditorRot->UI(0.1)) {
                    obj->set_rotation(Quaternion::FromEuler(v3EditorRot->data()));
                }
                
                ImGui::Text("Scale");
                if(ImGui::Button("+")) {
                    obj->set_scale(obj->get_scale() + Vector3(1, 1, 1));
                }
                ImGui::SameLine();
                if(ImGui::Button("-")) {
                    obj->set_scale(obj->get_scale() - Vector3(1, 1, 1));
                }
                static std::unordered_map<int, bool> freeze;
                bool isFreeze = freeze[i];
                if(ImGui::Checkbox("Freeze", &isFreeze)) {
                    static Vector3 Fpos = obj->get_position();
                    obj->set_position(Fpos);
                    freeze[i] = isFreeze;
                }
                ImGui::PopID();
                
            }
    };
}

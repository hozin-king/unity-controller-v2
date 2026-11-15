#include "Structs/include.h"

namespace UI {
    class Offset {
    public:
        static void UI() {
            static DUMPER::Dumper* d = nullptr;
            if (d == nullptr) {
                auto il2cpp_handle = dlopen("libil2cpp.so", 4);
                d = new DUMPER::Dumper(il2cpp_handle);
            }
            
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

            // Display modules
            static int showDll = 0;
            static int dll = -1;
            static int namespase = -1;
           static int claxz = -1;
           static int meth = -1;
            static int fld = -1;
            static DUMPER::Module *objDll;
            static DUMPER::Namespace *objNamespase;
            static DUMPER::Class *objClass;
            static DUMPER::Field *objField;
            static DUMPER::Method *objMethod;
            
            if(OBJECT_SCANNER::playerlate != NULL) {
                
            
            /*
            if (ImGui::BeginCombo("Images", dll != -1 ? d->modules[dll]->name.c_str() : "-- Select --")) {
                for (int i = 0; i < d->modules.size(); i++) {
                    bool selected = i == dll;
                    if (ImGui::Selectable(d->modules[i]->name.c_str(), selected))
                        dll = i;
                    if (selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            
            if(dll != -1) {
                objDll = d->modules[dll];
                if (ImGui::BeginCombo("Namespaces", namespase != -1 ? objDll->ns[namespase]->name.c_str() : "-- Select --")){
                for (int i = 0; i < d->modules[dll]->ns.size(); i++) {
                    bool selected = i == namespase;
                    if (ImGui::Selectable(d->modules[dll]->ns[i]->name.c_str(), selected))
                        namespase = i;
                    if (selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
              }
            }
            if(namespase != -1) {
                objNamespase = objDll->ns[namespase];
                if (ImGui::BeginCombo("Classes",claxz != -1 ? objNamespase->Classes[claxz]->name.c_str() : "-- Select --")) {
                for (int i = 0; i < d->modules[dll]->ns[namespase]->Classes.size(); i++) {
                    bool selected = i == claxz;
                    if(d->modules[dll]->ns[namespase]->Classes[i]->isEnum || d->modules[dll]->ns[namespase]->Classes[i]->isinterface) {
                        continue;
                    }
                    if (ImGui::Selectable(d->modules[dll]->ns[namespase]->Classes[i]->name.c_str(), selected))
                        claxz = i;
                    if (selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
              }
            }
            */
            DUMPER::Class *test = new DUMPER::Class(*(void **)OBJECT_SCANNER::playerlate);
                
            if(test) {
                
                DUMPER::Class *clx = test;
                if(clx->methods.size() < meth) {
                    meth = -1;
                }
                if(clx->fields.size() < fld) {
                    fld = -1;
                }
                if(clx->methods.size() > 0) {
                    if (ImGui::BeginCombo("Methods", meth != -1 ? clx->methods[meth]->name.c_str() : "-- Select --")) {
                        for (int i = 0; i < clx->methods.size(); i++) {
                            bool selected = i == meth;
                            DUMPER::Method *tm = clx->methods[i];
                            if (ImGui::Selectable((tm->type + " : "+ tm->name + "(" + std::to_string(tm->params.size()) + ");").c_str(), selected))
                                meth = i;
                            if (selected)
                                ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
              }
              if(meth != -1) {
                  ImGui::Text("Type: %s" , clx->methods[meth]->type.c_str());
                  ImGui::Text("Modifier: %s" , clx->methods[meth]->modfier.c_str());
                  ImGui::Text("Name: %s" , clx->methods[meth]->name.c_str());
                  ImGui::Text("Params: %d" , clx->methods[meth]->params.size());
                  ImGui::Text("offset: %s" , clx->methods[meth]->offset.c_str());
                  
                  for(int i = 0; i < clx->methods[meth]->params.size(); i++) {
                      ImGui::PushID(i);
                      DUMPER::Param p = clx->methods[meth]->params[i];
                      if(strcmp(p.type.c_str(), "Int32") == 0){
                          static std::unordered_map<std::string, int> ints;
                          Editor::Int *i = new Editor::Int(ints[p.name], p.name);
                          if(i->UI()) {
                              ints[p.name] = i->data();
                          }
                      }
                      if(strcmp(p.type.c_str(), "Boolean") == 0) {
                          static std::unordered_map<std::string, bool> bools;
                          Editor::Bool *i = new Editor::Bool(bools[p.name], p.name);
                          if(i->UI()) {
                              bools[p.name] = i->data();
                          }
                      }
                      if(strcmp(p.type.c_str(), "Single") == 0) {
                          static std::unordered_map<std::string, float> floats;
                          Editor::Float *i = new Editor::Float(floats[p.name], p.name);
                          if(i->UI()) {
                              floats[p.name] = i->data();
                          }
                      }
                      ImGui::PopID();
                  }
                  
              }
          }
                if(clx->fields.size() > 0) {
                    if (ImGui::BeginCombo("Fields", (fld != -1 ? clx->fields[fld]->name : "-- Select --").c_str())) {
                        for (int i = 0; i < clx->fields.size(); i++) {
                            bool selected = i == fld;
                            DUMPER::Field *tm = clx->fields[i];
                            if (ImGui::Selectable((tm->type + " : " + tm->name + ";// " + tm->offset).c_str(), selected))
                                fld = i;
                            if (selected)
                                ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
                
              }
              if(fld != -1) {
                  ImGui::Text("Type: %s" , clx->fields[fld]->type.c_str());
                  ImGui::Text("Name: %s" , clx->fields[fld]->name.c_str());
                  ImGui::Text("offset: %s" , clx->fields[fld]->offset.c_str());
                  const char *type = clx->fields[fld]->type.c_str();
                  std::string name = clx->fields[fld]->name;
                  std::string offset = clx->fields[fld]->offset;
                  
                  if(strcmp(type, "Int32") == 0){
                       int value = *(int *)((uintptr_t) clx->static_address + string2Offset(offset.c_str()));
                       Editor::Int *i = new Editor::Int(value, name);
                          if(i->UI()) {
                              *(int *)((uintptr_t) clx->static_address + string2Offset(offset.c_str())) = i->data();
                          }
                      }
                  
                  if(strcmp(type, "Single") == 0){
                       float value = *(float *)((uintptr_t) clx->static_address + string2Offset(offset.c_str()));
                       Editor::Float *i = new Editor::Float(value, name);
                          if(i->UI()) {
                              *(float *)((uintptr_t) clx->static_address + string2Offset(offset.c_str())) = i->data();
                          }
                      }
                  
                  if(strcmp(type, "Vector3") == 0){
                       Vector3 value = *(Vector3 *)((uintptr_t) clx->static_address + string2Offset(offset.c_str()));
                       Editor::Vector_3 *i = new Editor::Vector_3(value, name);
                          if(i->UI()) {
                              *(Vector3 *)((uintptr_t) clx->static_address + string2Offset(offset.c_str())) = i->data();
                          }
                      }
                  
                  if(strcmp(type, "Boolean") == 0){
                       bool value = *(bool *)((uintptr_t) clx->static_address + string2Offset(offset.c_str()));
                       Editor::Bool *i = new Editor::Bool(value, name);
                          if(i->UI()) {
                              *(bool *)((uintptr_t) clx->static_address + string2Offset(offset.c_str())) = i->data();
                          }
                      }
                  
                  if(strcmp(type, "Transform") == 0){
                       Structs::Transform *value = *(Structs::Transform **)((uintptr_t) clx->static_address + string2Offset(offset.c_str()));
                       Editor::Transform *i = new Editor::Transform(value);
                       i->UI(0);
                    
                  }
                  if(strcmp(type, "String") == 0){
                       Structs::String *value = *(Structs::String **)((uintptr_t) clx->static_address + string2Offset(offset.c_str()));
                       ImGui::Text(value->getString().c_str());
                      }
                  }
                }
            }
            }
/*
            // Display fields
            if (!d->modules.empty() && dll < d->modules.size() &&
                !d->modules[dll]->ns.empty() && namespase < d->modules[dll]->ns.size() &&
                !d->modules[dll]->ns[namespase]->Classes.empty() && claxz < d->modules[dll]->ns[namespase]->Classes.size()) {
                auto fields = d->modules[dll]->ns[namespase]->Classes[claxz]->fields;
                ImGui::Text("Fields:");
                for (const auto& field : fields) {
                    ImGui::Text("- %s", field->name.c_str());
                }
            }

            // Display methods
            static int meth = 0;
            if (!d->modules.empty() && dll < d->modules.size() &&
                !d->modules[dll]->ns.empty() && namespase < d->modules[dll]->ns.size() &&
                !d->modules[dll]->ns[namespase]->Classes.empty() && claxz < d->modules[dll]->ns[namespase]->Classes.size()) {
                auto methods = d->modules[dll]->ns[namespase]->Classes[claxz]->methods;
                if (ImGui::BeginCombo("Methods", methods[meth]->name.c_str())) {
                    for (int i = 0; i < methods.size(); i++) {
                        bool selected = i == meth;
                        if (ImGui::Selectable(methods[i]->name.c_str(), selected))
                            meth = i;
                        if (selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }

            // Display selected field and method offset
            if (!d->modules.empty() && dll < d->modules.size() &&
                !d->modules[dll]->ns.empty() && namespase < d->modules[dll]->ns.size() &&
                !d->modules[dll]->ns[namespase]->Classes.empty() && claxz < d->modules[dll]->ns[namespase]->Classes.size() &&
                !d->modules[dll]->ns[namespase]->Classes[claxz]->fields.empty() && meth < d->modules[dll]->ns[namespase]->Classes[claxz]->fields.size()) {
                auto field = d->modules[dll]->ns[namespase]->Classes[claxz]->fields[meth];
                auto method = d->modules[dll]->ns[namespase]->Classes[claxz]->methods[meth];
                ImGui::Text("Selected Field: %s", field->name.c_str());
                ImGui::Text("Selected Method Offset: %s", method->offset.c_str());
            }
*/
            ImGui::PopStyleVar();
        }
    };
}


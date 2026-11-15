std::vector<std::string> namespaces_data = DUMPER::getNamespace() ;
std::unordered_map<std::string, std::vector<std::string>> classes_data = DUMPER::getClass();
std::unordered_map<std::string, std::vector<std::string>> functions_data =  DUMPER::getData();
std::unordered_map<std::string, bool> classChecked;
static int selectedNamespace = 0;
static int currentPage = 0;
static int itemsPerPage = 20;
static bool Ischecked = false;
static std::string checkdNS = "Nothing";
static std::string checkdClass = "Nothing";

void RefreshData() {
    namespaces_data = DUMPER::getNamespace() ;
    classes_data = DUMPER::getClass();
    functions_data=  DUMPER::getData();

}
void ShowUI() {
    RefreshData();
    std::vector<std::string> namespaces = namespaces_data;
    std::unordered_map<std::string, std::vector<std::string>> classes = classes_data;
    std::unordered_map<std::string, std::vector<std::string>> function =  functions_data;
    ImGui::Begin("Namespace and Class Selector");

    if (namespaces.empty()) {
        ImGui::Text("No namespaces available.");
    } else {
        // Choose namespace
        if (ImGui::BeginCombo("Namespaces", namespaces[selectedNamespace].c_str())) {
            for (int i = 0; i < namespaces.size(); i++) {
                bool isSelected = (selectedNamespace == i);
                if (ImGui::Selectable(namespaces[i].c_str(), isSelected)) {
                    selectedNamespace = i;
                    checkdNS = "";
                    checkdNS += namespaces[i].c_str();
                    // Reset class selection when namespace changes
                    currentPage = 0;
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Pagination
        if (ImGui::Button("Previous Page") && currentPage > 0) {
            currentPage--;
        }
        ImGui::SameLine();
        if (ImGui::Button("Next Page") && classes[namespaces[selectedNamespace]].size() > (currentPage + 1) * itemsPerPage) {
            currentPage++;
        }

        // Choose class with checkbox
        if (classes[namespaces[selectedNamespace]].empty()) {
            ImGui::Text("No classes available for this namespace.");
        } else {
            ImGui::Begin("Class Checkboxes");
            int start = currentPage * itemsPerPage;
            int end = start + itemsPerPage <= classes[namespaces[selectedNamespace]].size() ? start + itemsPerPage : classes[namespaces[selectedNamespace]].size();
            for (int i = start; i < end; i++) {
                const auto& cls = classes[namespaces[selectedNamespace]][i];
                // Checkbox for each class
                bool isChecked = classChecked[cls];
                if (ImGui::Checkbox(cls.c_str(), &isChecked)) {
                    // Uncheck all other classes
                    for (auto& pair : classChecked) {
                        pair.second = false;
                    }
                    // Check this class
                    Ischecked = true;
                    checkdClass = "";
                    checkdClass += cls;
                    classChecked[cls] = true;
                    
                }
            }
            ImGui::End();
        }
    }
    ImGui::End();
    if(Ischecked) {
        ImGui::Begin("Data");
        ImGui::Text(checkdClass.c_str());
        ImGui::Text(checkdNS.c_str());
        
        ImGui::End();
    }
}


ImGui::Text("Total: %d", );
    for (uintptr_t className : EspObjects)
    {
        try {
            if(!className) {
                throw "Invalid Object";
            }
            ImGui::Text("OBJECT OK");
            /*
            void *transform = get_transform((void *)className);
            if(transform == nullptr) {
                throw "Cant find Transform";
            }
            
            ImGui::Text("Transform OK");
            */
            Vector3 pos = get_position((void *)className);
            if(pos == Vector3::Zero()) {
                throw "No Position";
            }
            ImGui::Text("Position OK");
        
            ImGui::Text("POINTER: %p POSITION: x: %d y: %d z: %d", className, pos.X, pos.Y, pos.Z);
        } catch (std::string error) {
            ImGui::Text("Error: %s", error.c_str());
        
        }
    }
    auto androidDataPath = std::string("/storage/emulated/0/Android/data/").append(DUMPER::GetPackageName()).append("/").append(DUMPER::GetPackageName()).append(".cs");
    auto il2cpp_handle = dlopen("libil2cpp.so", 4);
    ImGui::Begin("Unity-Tool {TEST}", nullptr);
    ImGui::Text(DUMPER::GetPackageName());
    if(ImGui::Button("Dump")) {
        DUMPER::startDump(il2cpp_handle);
    }
    
    if(ImGui::Button("Reset")) {
        EspObjects.clear();
        EspClassesList.clear();
    }
    ImGui::Text("Total: %d", EspObjects.size());
    ShowUI();
    DrawESP(ImGui::GetBackgroundDrawList());
    
    ImGui::End();
    
void ShowUI() {
    

    ImGui::Begin("Class Chooser");

    // Previous page button
    
    

    
    // Checkbox list
    

    ImGui::End();
}

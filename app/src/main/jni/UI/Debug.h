namespace DEBUG {
    std::vector<std::string> logs;
    void window() {
        ImGui::Begin("Logs", nullptr);
        for (int i = 0; i < logs.size(); i++) {
            ImGui::Text(logs[i].c_str());
        }
    }
}

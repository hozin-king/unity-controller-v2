#include "Structs/include.h"
#include "loginUtils/json.hpp"
using json = nlohmann::json;
#include <openssl/sha.h>
#include <curl/curl.h>
namespace UI {
    bool LoggedIn = false;
    std::string loginhash = "";
    
    
    size_t writeCallback(char *contents, size_t size, size_t nmemb, std::string *userp) {
        userp->append(contents, size * nmemb);
        return size * nmemb;
    }
    
    
    
    std::string computeSHA256Hash(const std::string& data)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256Context;
        SHA256_Init(&sha256Context);
        SHA256_Update(&sha256Context, data.c_str(), data.length());
        SHA256_Final(hash, &sha256Context);
    
        char hashString[SHA256_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            sprintf(&hashString[i * 2], "%02x", hash[i]);
        }
        hashString[SHA256_DIGEST_LENGTH * 2] = '\0';
    
        return std::string(hashString);
    }
    std::string getCPUInfo()
    {
        std::ifstream cpuinfoFile("/proc/cpuinfo");
    
        if (!cpuinfoFile.is_open()) {
            std::cerr << "Failed to open /proc/cpuinfo" << std::endl;
            return "";
        }
    
        std::string cpuinfoData;
        std::string line;
        while (std::getline(cpuinfoFile, line)) {
            cpuinfoData += line + "\n";
        }
    
        cpuinfoFile.close();
        return cpuinfoData;
    }
        std::string get(const char *url) {
        CURL *curl;
        CURLcode res;
        std::string response;
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                return "";
            }
            curl_easy_cleanup(curl);
        }
        return response;
    }
    bool validateToken(std::string token, const char *server) {
        std::string SERVERURL = server;
        const char *U1 = OBFUSCATE("/api/auth0/checkToken?token=");
        SERVERURL += U1;
        SERVERURL += token;
        std::string response = get(SERVERURL.c_str());
        json data = json::parse(response);
        const char *tru = OBFUSCATE("true");
         return data["status"].get<std::string>() == tru;
    }
    std::string login() {
        std::string cpuinfoData = getCPUInfo();
        std::string hwid = computeSHA256Hash(cpuinfoData);;
        std::string key = OBFUSCATE("UnityGameController");
        std::string UUID = hwid;
        std::string Name = "User";
        std::string LoggedIn = OBFUSCATE("true");
        std::string failedLogin = OBFUSCATE("false");
        const char *loginURL = OBFUSCATE("https://admin.nepmods.xyz");
        const char *U1 = OBFUSCATE("/api/loginV2/key?key=");
        const char *U2 = OBFUSCATE("&device=");
        const char *U3 = OBFUSCATE("&name=");
        
        std::string SERVERURL = loginURL;
        SERVERURL += U1;
        SERVERURL += key;
        SERVERURL += U2;
        SERVERURL += UUID;
        SERVERURL += U3;
        SERVERURL += Name;
        
        std::string response = get(SERVERURL.c_str());
        json data = json::parse(response);
        std::string Success = OBFUSCATE("Success");
        std::string Username = key;
        
        if(Success == data["Status"].get<std::string>()) {
            if(Username == data["Username"].get<std::string>()) {
                if (validateToken(data["token"].get<std::string>(), loginURL)) {
                    return LoggedIn;
                }
            }
            
        }
        
        return failedLogin;
    }
    std::string loginData;
    bool loaded = false;
    bool connected = false;
    bool isDone = false;
    class Login {  
        public:
            static void UI() {
               if(isDone) {
                   if(!loaded) {
                       ImGui::Text("Failed To Connect");
                       return;
                   }
                   return;
                }
                if(ImGui::Button("Connect")) {
                                isDone = true;
                
                ImGui::Text("Starting...");
                
                
                ImGui::Text("Connecting To server...");
                
                if(login() == "true") {
                    connected = true;
                }
                if(!connected) {
                    return;
                }
                ImGui::Text("Connected..");
                
                ImGui::Text("Fetching CPU Info");
              
                ImGui::Text("Getting CPU SHA256 Hash");
                
                ImGui::Text("Authenticating KEY: UnityGameController");
                loginData = login();
                ImGui::Text(loginData == "true" ? "Logged in" : "Cannot Login, Contact Admin");
              
                if(loginData == "false") {
                    return;
                }
                ImGui::Text("Fetching UI From Server..");
               
                ImGui::Text("Fetching Message..");
              
                ImGui::Text("Matching Sign...");
               
                ImGui::Text("Loading Menu....");
                sleep(2);
                loaded = true;
                }
            }
    };
}

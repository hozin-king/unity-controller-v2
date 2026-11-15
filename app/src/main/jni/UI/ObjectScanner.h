#include "Structs/include.h"
#include <cmath>
#include <algorithm>
#include <string>
#include <thread>
#include <future>
#include <unordered_set>
#include "ThreadPool.h"

#include <vector>
#include <unordered_set>
#include <mutex>
#include <thread>
#include <memory>
#include <algorithm>
#include <functional>
#include <chrono>

namespace OBJECT_SCANNER {
    bool scanning = false;
    bool HookStarted = false;
    bool EspScanning = true;
    bool AimbotScanning = true;
    bool PlayersScanning = true;
    bool force = false;
    int interval = 1;
    int glHeight, glWidth;
    std::unordered_set<std::string> ClassSet;
    std::vector<std::string> ClassList = {"None"};
    std::vector<void *> CLists;
    std::vector<Structs::Transform *> EspObjects;
    std::vector<Structs::Transform *> AimbotObjects;
    std::vector<Structs::Transform *> PlayerObjects;
    const char *espclass = "null";
    const char *aimbotclass = "null";
    const char *playerclass = "null";
    Structs::Transform *AimBotMe = nullptr;
    Structs::Transform *playerlate = nullptr;
    Structs::Transform *Editor = nullptr;
    bool FilterClass = true;
    pthread_t scanner;
    il2cpp::Class *MonoBehaviour;
    std::mutex dataMutex;
    
    bool find(std::string inputString, std::string text) {
        size_t found = inputString.find(text);
        return (found != std::string::npos);
    }
    
    std::shared_ptr<std::thread> previousThread;
    int workers = 5;
    int savedWorkers = -1;
    ThreadPool *threadPool = NULL;
    Structs::Transform *(*Scanner_Get_Object)(...);
    
    void handleData(il2cpp::Class *thiz) {
        il2cpp::Class *clz = *(il2cpp::Class **)thiz;
        const char *ObjectClass = clz->get_name();
        
        if (scanning) {
            bool add = false;
            if (ClassSet.count(ObjectClass) == 0) {
                if(!FilterClass) {
                    add = true;
                    goto add;
                }
                 for (int i = 0; i < clz->get_extends().size(); i++) {
                 if(find(clz->get_extends()[i], "Behaviour")) {
                    add = true;
                    goto add;
                     break;
                }
              }
            }
            
            add:
            if (add) {
                 if (ClassSet.count(ObjectClass) == 0) {
                    ClassSet.insert(ObjectClass);
                    ClassList.push_back(ObjectClass);
                }
            }
        }
        
        Structs::Transform *obj = nullptr;
        
        if (EspScanning || AimbotScanning || PlayersScanning) {
            obj = Scanner_Get_Object(thiz);
        }
        
        if (EspScanning) {
            if (strcmp(ObjectClass, espclass) == 0) {
                auto checkObj = std::find(EspObjects.begin(), EspObjects.end(), obj);
                if (checkObj == EspObjects.end()) {
                    EspObjects.push_back(obj);
                }
            }
            if(EspObjects.size() > 0) {
                playerlate = EspObjects[0];
            }
         }
        
        if (AimbotScanning) {
            if (strcmp(ObjectClass, aimbotclass) == 0) {
                auto checkObj = std::find(AimbotObjects.begin(), AimbotObjects.end(), obj);
                if (checkObj == AimbotObjects.end()) {
                    AimbotObjects.push_back(obj);
                }
            }
        }
        
        if (PlayersScanning) {
            if (strcmp(ObjectClass, playerclass) == 0) {
                auto checkObj = std::find(PlayerObjects.begin(), PlayerObjects.end(), obj);
                if (checkObj == PlayerObjects.end()) {
                    PlayerObjects.push_back(obj);
                }
            }
        }
    }
    
    Structs::Transform *Scanner(il2cpp::Class *thiz) {
       static std::chrono::steady_clock::time_point lastScanTime;
        static const std::chrono::milliseconds minScanInterval(interval);
        
       std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        if (currentTime - lastScanTime < minScanInterval) {
            return Scanner_Get_Object(thiz);
        }
        if(workers != savedWorkers) {
            free(threadPool);
            threadPool = new ThreadPool(workers);
            savedWorkers = workers;
        }
        if (thiz) {
            threadPool->enqueue([thiz]() {
                handleData(thiz);
            });
        }
        lastScanTime = currentTime;
        
        
        
        return Scanner_Get_Object(thiz);
    }
    
    void (*Remove_Object)(...);
    
    void Remove(Structs::GameObject *thiz) {
        if(thiz != nullptr) {
            il2cpp::Class *c = *(il2cpp::Class **)thiz;
            std::string name = c->get_name();
            if(name == "GameObject") {
                Structs::Transform *t = thiz->get_transform();
               if(EspObjects.size() > 0 && EspScanning) {
                    EspObjects.erase(std::remove(EspObjects.begin(), EspObjects.end(), t), EspObjects.end());
                }
                if(AimbotObjects.size() > 0 && AimbotScanning) {
                    AimbotObjects.erase(std::remove(AimbotObjects.begin(), AimbotObjects.end(), t), AimbotObjects.end());
                }
                if(PlayerObjects.size() > 0 && PlayersScanning) {
                    PlayerObjects.erase(std::remove(PlayerObjects.begin(), PlayerObjects.end(), t), PlayerObjects.end());
                }
            }
        }
        Remove_Object(thiz);
    }
    
    void *scan_thread(void *) {
        auto offset = IL2Cpp::Il2CppGetMethodOffset("UnityEngine.CoreModule.dll", "UnityEngine", "Component", "get_transform", 0);
        MSHookFunction((void *)offset, (void *)Scanner, (void **)&Scanner_Get_Object);
        auto destroy = IL2Cpp::Il2CppGetMethodOffset("UnityEngine.CoreModule.dll", "UnityEngine", "Object", "Destroy", 1);
        MSHookFunction((void *)destroy, (void *)Remove, (void **)&Remove_Object);
        
        pthread_exit(nullptr);
        
        return nullptr;
    }
    
    void startScan() {
        pthread_create(&scanner, nullptr, scan_thread, nullptr);
        free(threadPool);
        
        scanning = true;
        ClassList.clear();
    }
}


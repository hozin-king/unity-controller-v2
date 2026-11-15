namespace Structs {
    
    class SceneManager {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine.SceneManagement";
            const char *klass = "SceneManager";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        public:
            
            static int get_sceneCount() {
                int (*_get_sceneCount)(...) = (int (*) (...))get_methodOffset("get_sceneCount", 0);
                return _get_sceneCount();
            }
            static void *get_sceneAt(int i) {
                void *(*_get_sceneAt)(...) = (void *(*) (...))get_methodOffset("GetSceneAt", 1);
                return _get_sceneAt(i);
            }
            static void *getActiveScene() {
                void *(*GetActiveScene)(...) = (void *(*) (...))get_methodOffset("GetActiveScene", 0);
                return GetActiveScene();
            }
            
            
    };
}

namespace Structs {
    class Application {
        
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Application";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);

        }
        public:
            static String *get_unityVersion() {
                String *(*_get_unityVersion)(...) = (String *(*)(...))Application::get_methodOffset("get_unityVersion");
                return _get_unityVersion();
            }
            static String *get_productName() {
                String *(*_get_productName)(...) = (String *(*)(...))Application::get_methodOffset("get_productName");
                return _get_productName();
            }
            static String *get_version() {
                String *(*_get_version)(...) = (String *(*)(...))Application::get_methodOffset("get_version");
                return _get_version();
            }
            //static String *get_dataPath() {
                //String *(*_get_dataPath)(...) = (String *(*)(...))Application::get_methodOffset("get_dataPath");
                //return _get_dataPath();
            //}
            static bool get_isPlaying() {
                bool (*_get_isPlaying)(...) = (bool (*)(...))Application::get_methodOffset("get_isPlaying");
                return _get_isPlaying();
            }
            
    };
}

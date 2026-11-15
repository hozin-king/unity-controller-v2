namespace Structs {
    class Time {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Time";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        public:
            
            static void set_timeScale(float scale) {
                void (*_set_timeScale)(...) = (void (*) (...))get_methodOffset("set_timeScale", 1);
                _set_timeScale(scale);
            }
            static float get_realtimeSinceStartup() {
                float (*_get_realtimeSinceStartup)(...) = (float (*) (...))get_methodOffset("get_realtimeSinceStartup");
                return _get_realtimeSinceStartup();
            }
    };
}

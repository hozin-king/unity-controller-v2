namespace Structs {
    class MonoBehaviour {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "MonoBehaviour";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        public:
            
            static bool IsObjectMonoBehaviour(Object *type) {
                bool (*_IsObjectMonoBehaviour)(...) = (bool (*) (...))get_methodOffset("IsObjectMonoBehaviour", 1);
                return _IsObjectMonoBehaviour(NULL, type);
            }
    };
}

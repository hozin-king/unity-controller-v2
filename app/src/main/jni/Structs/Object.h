namespace Structs {
    class Object {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Object";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        public:
            
            String *get_name() {
                String *(*_get_name)(...) = (String *(*) (...))get_methodOffset("get_name");
                return _get_name(this);
            }
            static bool IsNativeObjectAlive(Object *obj) {
                bool (*_IsNativeObjectAlive)(...) = (bool (*) (...))get_methodOffset("IsNativeObjectAlive", 1);
                return _IsNativeObjectAlive(obj);
            }
            bool alive() {
                return IsNativeObjectAlive(this);
            }
            static bool CompareBaseObjects(Object *lhs, Object *rhs) {
                bool (*_CompareBaseObjects)(...) = (bool (*) (...))get_methodOffset("CompareBaseObjects", 2);
                return _CompareBaseObjects(NULL, lhs, rhs);
            }
            bool isSame(Object *obj) {
                return CompareBaseObjects(this, obj);
            }
    };
}

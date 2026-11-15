
namespace Structs {
    class Component : public Object {
        Vector3 OldPos;
        uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Component";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        public:
            Transform *get_transform(){
                Transform *(*_get_transform)(...) = (Transform *(*)(...))get_methodOffset("get_transform");
                return _get_transform(this);
            }
            Component *get_component() {
                return this;
            } 
    };
}

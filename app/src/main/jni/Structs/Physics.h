namespace Structs {
    class Physics {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.PhysicsModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Physics";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        
        public:
            
            static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance, int layerMask) {
                bool (*_Raycast)(...) = (bool (*) (...))get_methodOffset("Raycast", 4);
                return _Raycast(NULL, origin, direction, maxDistance, layerMask);
            }
            
    };
}

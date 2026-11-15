namespace Structs {
    class Type {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "mscorlib.dll";
            const char *namespace_name = "System";
            const char *klass = "Type";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        public:
            
            static String *get_type(String *type) {
                String *(*_get_type)(...) = (String *(*) (...))get_methodOffset("GetType", 1);
                return _get_type(type);
            }
    };
}

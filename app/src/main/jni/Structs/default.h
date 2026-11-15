namespace Structs {
    class String {
        uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "mscorlib.dll";
            const char *namespace = "System";
            const char *klass = "String";
            IL2Cpp::Il2CppGetMethodOffset(dll, namespace, klass, name, param);

        }
        public:
            char get_char(int index) {
                char (*_get_Chars)(void* thiz, int i) = (char (*)(void*, int))get_methodOffset("get_Chars", 1);
                return _get_Chars(this, index);
            }
        
    };
}

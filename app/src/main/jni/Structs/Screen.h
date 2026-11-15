namespace Structs {
    class Screen {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Screen";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        public:
            
            static int get_width() {
                int (*_get_width)(...) = (int (*) (...))get_methodOffset("get_width");
                return _get_width();
            }
            static int get_height() {
                int (*_get_height)(...) = (int (*) (...))get_methodOffset("get_height");
                return _get_height();
            }
            static int get_dpi() {
                int (*_get_height)(...) = (int (*) (...))get_methodOffset("get_dpi");
                return _get_height();
            }
            static uintptr_t offset(std::string name, int params = 0) {
                uintptr_t offset = get_methodOffset(name.c_str(), params);
                return offset;
            } 
    };
}

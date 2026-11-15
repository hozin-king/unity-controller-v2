namespace Structs {
    class Display {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Display";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }
        public:
            
            static Display *get_main() {
                Display *(*_get_main)(...) = (Display *(*) (...))get_methodOffset("get_main");
                return _get_main();
            }
            int get_renderingWidth() {
                int (*_get_renderingWidth)(...) = (int (*) (...))get_methodOffset("get_renderingWidth");
                return _get_renderingWidth(this);
            }
            int get_renderingHeight() {
                int (*_get_renderingHeight)(...) = (int (*) (...))get_methodOffset("get_renderingHeight");
                return _get_renderingHeight(this);
            }
            int get_systemWidth() {
                int (*_get_systemWidth)(...) = (int (*) (...))get_methodOffset("get_systemWidth");
                return _get_systemWidth(this);
            }
            int get_systemHeight() {
                int (*_get_systemHeight)(...) = (int (*) (...))get_methodOffset("get_systemHeight");
                return _get_systemHeight(this);
            }
            
            static uintptr_t offset(std::string name, int params = 0) {
                uintptr_t offset = get_methodOffset(name.c_str(), params);
                return offset;
            } 
            
    };
}

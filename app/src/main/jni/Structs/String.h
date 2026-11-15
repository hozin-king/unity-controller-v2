namespace Structs {
    class String {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "mscorlib.dll";
            const char *namespace_name = "System";
            const char *klass = "String";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);

        }
        public:
            
            int get_Length(){
                int (*_get_Length)(void* thiz) = (int (*)(void*))get_methodOffset("get_Length");
                return _get_Length(this);
		    }
            char get_char(int index) {
                char (*_get_Chars)(void* thiz, int i) = (char (*)(void*, int))get_methodOffset("get_Chars", 1);
    		    return _get_Chars(this, index);
            }
            static String *CreateString(const char *str) {
                String *(*_CreateString)(void *thiz, const char *i) = (String *(*)(void*, const char *))get_methodOffset("CreateString", 1);
                return _CreateString(NULL, str);
            }
            std::string getString() {
                std::string ret = "";
                for(int i = 0; i < this->get_Length(); i++) {
                    ret += this->get_char(i);
                }
                return ret;
            }
    };
}

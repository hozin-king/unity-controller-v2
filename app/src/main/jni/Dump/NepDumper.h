#include <vector>
#include <string>
#include <iostream>
namespace DUMPER {
    struct Param {
        std::string byRef = "";
        std::string type = "";
        std::string name = "";
        void *klass = NULL;
    };
    class Field {
        public:
            std::vector<std::string> flagNames;
            std::string type = "";
            std::string name = "";
            int value = 0;
            std::string offset = "";
            bool isEnum = false;
            Field(void *clsz = nullptr, bool isEnum = false);
    };
    class Method {
        public:
            std::string offset = "";
            std::string name = "";
            std::string modfier = "";
            bool byRef = false;
            std::string type = "";
            std::vector<Param> params;
            
            Method(void *clasz = nullptr);
    };
    class Property {
        public:
            std::string prop = "";
            
            Property(void *p = nullptr);
    };
    class Class {
        public:
            void *klass = nullptr;
            uintptr_t static_address = 0;
            std::string name = "";
            std::string namespase = "";
            std::string image = "";
            int methodCount = 0;
            int fieldCount = 0;
            int propertyCount = 0;
            bool isStruct = false;
            bool isEnum = false;
            bool isinterface = false;
            bool isClass = false;
            
            std::vector<std::string> flagNames;
            std::vector<Field *> fields;
            std::vector<Method *> methods;
            std::vector<Property *> Properties;
            std::vector<std::string> Extends;
            
            Class(void *type = nullptr);
            
            int InitMethods();
            int InitFields();
            int InitProperty();
            void InitFlags();
            void InitExtends();
    };
    class Namespace {
        public:
            std::string name = "";
            std::vector<Class *> Classes;
            Namespace(std::string data = "");
            bool addClass(Class *data);
    };
    class Module {
        public:
            std::string name = "";
            int classCount = 0;
            std::vector<Namespace *> ns;
            Module(void *module = nullptr);
            bool InitModule(void *type = nullptr);
    };
    
    class Dumper {
        public:
            std::vector<Module *> modules;
            bool init = false;
            Dumper(void *handle = nullptr) ;
            void GenDump();
            const char *GetPackageName();
    };
}


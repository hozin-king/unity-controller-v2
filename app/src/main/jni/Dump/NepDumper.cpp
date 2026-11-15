#include "NepDumper.h"
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <jni.h>
#include "Dump/log.h"
#include "il2cpp-tabledefs.h"
#include "il2cpp-class.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <sys/stat.h>

static void *il2cpp_handle = nullptr;
static uint64_t il2cpp_base = 0;

#define DO_API(r, n, p) r (*n) p
#include "il2cpp-api-functions.h"
#include <Dump/BNMUtils.h>
#undef DO_API

void init_il2cpp_api() {
    #define DO_API(r, n, p) n = (r (*) p)dlsym(il2cpp_handle, #n)
    #include "il2cpp-api-functions.h"
    #undef DO_API
}
uint64_t get_module_base(const char *module_name) {
    uint64_t addr = 0;
    char line[1024];
    uint64_t start = 0;
    uint64_t end = 0;
    char flags[5];
    char path[PATH_MAX];

    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp != nullptr) {
        while (fgets(line, sizeof(line), fp)) {
            strcpy(path, "");
            sscanf(line, "%" PRIx64"-%" PRIx64" %s %*" PRIx64" %*x:%*x %*u %s\n", &start, &end, flags, path);
            #if defined(__aarch64__)
                if (strstr(flags, "x") == 0) //TODO
                    continue;
            #endif
            if (strstr(path, module_name)) {
                addr = start;
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}
std::string int2Hex(unsigned int intVal)
{
    std::string hexStr;
    std::stringstream sstream;
    sstream << std::hex << intVal;
    hexStr= sstream.str();
    sstream.clear(); 
    std::transform(hexStr.begin(), hexStr.end(), hexStr.begin(), ::toupper);
    return std::string("0x") + hexStr;
}
bool _il2cpp_type_is_byref(const Il2CppType *type) {
    auto byref = type->byref;
    if (il2cpp_type_is_byref) {
        byref = il2cpp_type_is_byref(type);
    }
    return byref;
}

std::string get_method_modifier(uint32_t flags) {
    std::stringstream outPut;
    auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;
    switch (access) {
        case METHOD_ATTRIBUTE_PRIVATE:
            outPut << "private ";
            break;
        case METHOD_ATTRIBUTE_PUBLIC:
            outPut << "public ";
            break;
        case METHOD_ATTRIBUTE_FAMILY:
            outPut << "protected ";
            break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
            outPut << "internal ";
            break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & METHOD_ATTRIBUTE_STATIC) {
        outPut << "static ";
    }
    if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_FINAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "sealed override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
            outPut << "virtual ";
        } else {
            outPut << "override ";
        }
    }
    if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
        outPut << "extern ";
    }
    return outPut.str();
}
namespace DUMPER {
    Method::Method(void *clasz) {
        MethodInfo *method = (MethodInfo *)clasz;
        
        if(method->methodPointer) {
            offset = int2Hex((uintptr_t)method->methodPointer - (uintptr_t)il2cpp_base);
        } else {
            offset = "0x0";
        }
        uint32_t iflags = 0;
        auto flags = il2cpp_method_get_flags(method, &iflags);
        modfier = get_method_modifier(flags);
        auto return_type = il2cpp_method_get_return_type(method);
        auto return_class = il2cpp_class_from_type(return_type);
        if (_il2cpp_type_is_byref(return_type)) {
            byRef = true;
        }
        type = il2cpp_class_get_name(return_class);
        name = il2cpp_method_get_name(method);
        auto param_count = il2cpp_method_get_param_count(method);
        
        for (int i = 0; i < param_count; ++i) {
            auto param = il2cpp_method_get_param(method, i);
            auto attrs = param->attrs;
            Param p;
            if (_il2cpp_type_is_byref(param)) {
                if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN)) {
                    p.byRef += "out ";
                } else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT)) {
                    p.byRef += "in ";
                } else {
                    p.byRef += "ref ";
                }
            } else {
                if (attrs & PARAM_ATTRIBUTE_IN) {
                    p.byRef += "[In] ";
                }
                if (attrs & PARAM_ATTRIBUTE_OUT) {
                    p.byRef += "[Out] ";
                }
            }
            
            auto parameter_class = il2cpp_class_from_type(param);
            
            p.type = il2cpp_class_get_name(parameter_class);
            p.name = il2cpp_method_get_param_name(method, i);
            p.klass = parameter_class;
            params.push_back(p);
        }
    }
    Property::Property(void *p) {
        PropertyInfo *props = (PropertyInfo *)p;
        auto get = il2cpp_property_get_get_method(props);
        auto set = il2cpp_property_get_set_method(props);
        auto prop_name = il2cpp_property_get_name(props);
        Il2CppClass *prop_class = nullptr;
        uint32_t iflags = 0;
        if (get) {
            prop+= get_method_modifier(il2cpp_method_get_flags(get, &iflags));
            prop_class = il2cpp_class_from_type(il2cpp_method_get_return_type(get));
        } else if (set) {
            prop += get_method_modifier(il2cpp_method_get_flags(set, &iflags));
            auto param = il2cpp_method_get_param(set, 0);
            prop_class = il2cpp_class_from_type(param);
        }
        if (prop_class) {
            prop += il2cpp_class_get_name(prop_class);
            prop += " ";
            prop += prop_name;
            prop +=" { ";
            if (get) {
                prop += "get; ";
            }
            if (set) {
                prop += "set; ";
            }
            prop += "}\n";
        } else {
            if (prop_name) {
                prop += " // unknown property ";
                prop += prop_name;
                prop += "\n";
            }
        }
    }
    
    Field::Field(void *clasz, bool is_enum) {
        FieldInfo *field = (FieldInfo *)clasz;
        isEnum = is_enum;
        auto field_type = il2cpp_field_get_type(field);
        auto field_class = il2cpp_class_from_type(field_type);
        
        type = il2cpp_class_get_name(field_class);
        name = il2cpp_field_get_name(field);
        
        auto attrs = il2cpp_field_get_flags(field);
        auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
            
        switch (access) {
            case FIELD_ATTRIBUTE_PRIVATE:
                flagNames.push_back("private ");
                break;
           case FIELD_ATTRIBUTE_PUBLIC:
                flagNames.push_back("public ");
                break;
            case FIELD_ATTRIBUTE_FAMILY:
                flagNames.push_back("protected ");
                break;
            case FIELD_ATTRIBUTE_ASSEMBLY:
            case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                flagNames.push_back("internal ");
                break;
            case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                flagNames.push_back("protected internal ");
                break;
        }
        if (attrs & FIELD_ATTRIBUTE_LITERAL) {
            flagNames.push_back("const ");
        } else {
            if (attrs & FIELD_ATTRIBUTE_STATIC) {
                flagNames.push_back("static ");
            }
            if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                flagNames.push_back("readonly ");
            }
        }
        if (attrs & FIELD_ATTRIBUTE_LITERAL && is_enum) {
            uint64_t val = 0;
            il2cpp_field_static_get_value(field, &val);
            value =  static_cast<int>(val);
        }
        std::string offs = int2Hex(il2cpp_field_get_offset(field));
        offset = offs;
    }
    Class::Class(void *data) {
        klass = data;
        Il2CppClass *klas = (Il2CppClass *) data;
        static_address = (uintptr_t)data;
        name = il2cpp_class_get_name(klas);
        isStruct = il2cpp_class_is_valuetype(klas);
        isEnum = il2cpp_class_is_enum(klas);
        image = il2cpp_image_get_name(il2cpp_class_get_image(klas));
        namespase = il2cpp_class_get_namespace(klas);
         InitFlags();
         InitExtends();
        fieldCount = InitFields();
        propertyCount = InitProperty();
        methodCount = InitMethods();
        
    }
    
    void Class::InitFlags() {
        Il2CppClass *klas = (Il2CppClass *) klass;
        
        auto flags = il2cpp_class_get_flags(klas);
        if (flags & TYPE_ATTRIBUTE_SERIALIZABLE) {
            flagNames.push_back("[Serializable]\n");
        }
        
        auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
        switch (visibility) {
            case TYPE_ATTRIBUTE_PUBLIC:
            case TYPE_ATTRIBUTE_NESTED_PUBLIC:
                flagNames.push_back("public ");
                break;
            case TYPE_ATTRIBUTE_NOT_PUBLIC:
            case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
            case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
                flagNames.push_back("internal ");
                break;
            case TYPE_ATTRIBUTE_NESTED_PRIVATE:
                flagNames.push_back("private ");
                break;
            case TYPE_ATTRIBUTE_NESTED_FAMILY:
                flagNames.push_back("protected ");
                break;
            case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
                flagNames.push_back("protected internal ");
                break;
        }
        
        
        if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED) {
            flagNames.push_back("static ");
        } else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT) {
            flagNames.push_back("abstract ");
        } else if (!isStruct && !isEnum && flags & TYPE_ATTRIBUTE_SEALED) {
            flagNames.push_back("sealed ");
        }
        
        
        if (flags & TYPE_ATTRIBUTE_INTERFACE) {
            isinterface = true;
            flagNames.push_back("interface ");
        } else if (isEnum) {
            flagNames.push_back("enum ");
        } else if (isStruct) {
            flagNames.push_back("struct ");
        } else {
            flagNames.push_back("class ");
        }     
    }
    void Class::InitExtends() {
        Il2CppClass *klas = (Il2CppClass *) klass;
        
        std::vector<std::string> extends;
        auto parent = il2cpp_class_get_parent(klas);
        if (!isStruct && !isEnum && parent) {
            auto parent_type = il2cpp_class_get_type(parent);
            if (parent_type->type != IL2CPP_TYPE_OBJECT) {
                extends.emplace_back(il2cpp_class_get_name(parent));
            }
        }
        void *iter = nullptr;
        while (auto itf = il2cpp_class_get_interfaces(klas, &iter)) {
            extends.emplace_back(il2cpp_class_get_name(itf));
        }
        Extends = extends;
    }
    int Class::InitFields() {
        Il2CppClass *klas = (Il2CppClass *) klass;
        
        int count = 0;
        void *iter = nullptr;
        while (auto field = il2cpp_class_get_fields(klas, &iter)) {
            Field *newField = new Field(field, isEnum);
            fields.push_back(newField);
            count += 1;
        }
        return count;
    }
    int Class::InitProperty() {
        Il2CppClass *klas = (Il2CppClass *) klass;
        
        int count = 0;
        void *iter = nullptr;
        while (auto prop = il2cpp_class_get_properties(klas, &iter)) {
            Property *newMethod = new Property((void *)prop);
            Properties.push_back(newMethod);
            count += 1;
        }
        return count;
    }
    int Class::InitMethods() {
        Il2CppClass *klas = (Il2CppClass *) klass;
        
        int count = 0;
        void *iter = nullptr;
        while (auto method = il2cpp_class_get_methods(klas, &iter)) {
            Method *newMethod = new Method((void *)method);
            methods.push_back(newMethod);
            count += 1;
        }
        return count;
    }
    Namespace::Namespace(std::string nam) {
        name = nam;
    }
    bool Namespace::addClass(Class *clsz) {
        Classes.push_back(clsz);
    }
    Module::Module(void *module) {
        if(il2cpp_image_get_class) {
            auto image = il2cpp_assembly_get_image((Il2CppAssembly *)module);
            name = il2cpp_image_get_name(image);
            classCount = il2cpp_image_get_class_count(image);
            
            for (int i = 0; i < classCount; i++) {
                auto klass = il2cpp_image_get_class(image, i);
                auto type = il2cpp_class_get_type(const_cast<Il2CppClass *>(klass));
                InitModule((void *)type);
            }
            
        } else {
            
            typedef void *(*Assembly_Load_ftn)(void *, Il2CppString *, void *);
            typedef Il2CppArray *(*Assembly_GetTypes_ftn)(void *, void *);
            auto corlib = il2cpp_get_corlib();
        auto assemblyClass = il2cpp_class_from_name(corlib, "System.Reflection", "Assembly");
        auto assemblyLoad = il2cpp_class_get_method_from_name(assemblyClass, "Load", 1);
        auto assemblyGetTypes = il2cpp_class_get_method_from_name(assemblyClass, "GetTypes", 0);
        if (assemblyLoad && assemblyLoad->methodPointer) {
            LOGI("Assembly::Load: %p", assemblyLoad->methodPointer);
        } else {
            LOGI("miss Assembly::Load");
            return;
        }
        if (assemblyGetTypes && assemblyGetTypes->methodPointer) {
            LOGI("Assembly::GetTypes: %p", assemblyGetTypes->methodPointer);
        } else {
            LOGI("miss Assembly::GetTypes");
            return;
        }
            auto image = il2cpp_assembly_get_image((Il2CppAssembly *)module);
            name = il2cpp_image_get_name(image);
            auto imageName = std::string(name);
            auto pos = imageName.rfind('.');
            auto imageNameNoExt = imageName.substr(0, pos);
            auto assemblyFileName = il2cpp_string_new(imageNameNoExt.c_str());
            auto reflectionAssembly = ((Assembly_Load_ftn) assemblyLoad->methodPointer)(nullptr,
                                                                                        assemblyFileName,
                                                                                        nullptr);
            auto reflectionTypes = ((Assembly_GetTypes_ftn) assemblyGetTypes->methodPointer)(
                    reflectionAssembly, nullptr);
            auto items = reflectionTypes->vector;
            classCount = reflectionTypes->max_length;
            
            for (int j = 0; j < reflectionTypes->max_length; ++j) {
                auto klass = il2cpp_class_from_system_type((Il2CppReflectionType *) items[j]);
                auto type = il2cpp_class_get_type(klass);
       
                InitModule((void *)type);
            }
            
        }
        
    }
    bool Module::InitModule(void *type) {
        Il2CppType * Type = (Il2CppType *)type;
        auto *klass = il2cpp_class_from_type(Type);
        
        Namespace *newNs = new Namespace(il2cpp_class_get_namespace(klass));
        Class *newClass = new Class(klass);
        
        
        for(int i = 0; i < ns.size(); i++) {
            if(ns[i]->name == newNs->name) {
                
                ns[i]->Classes.push_back(newClass);
                return true;
            }
        }
        
        newNs->Classes.push_back(newClass);
        ns.push_back(newNs);
        
        return true;
    }
    
    Dumper::Dumper(void *handle) {
        il2cpp_handle = handle;
        init_il2cpp_api();
        if (il2cpp_domain_get_assemblies) {
            Dl_info dlInfo;
            if (dladdr((void *) il2cpp_domain_get_assemblies, &dlInfo)) {
                il2cpp_base = reinterpret_cast<uint64_t>(dlInfo.dli_fbase);
            } else {
                il2cpp_base = get_module_base("libil2cpp.so");
            }
         } else {
             return;
         }
         auto domain = il2cpp_domain_get();
         il2cpp_thread_attach(domain);
         
         size_t size;
         auto assemblies = il2cpp_domain_get_assemblies(domain, &size);
          for(int i = 0; i < size; i++) {
             Module *newModule = new Module((void *)assemblies[i]);
             modules.push_back(newModule);
         }
         init = true;
     }
     const char *Dumper::GetPackageName() {
        char *application_id[256];
        FILE *fp = fopen("proc/self/cmdline", "r");
        if (fp) {
            fread(application_id, sizeof(application_id), 1, fp);
            fclose(fp);
        }
        return (const char *) application_id;
    }
    
     
     void Dumper::GenDump() {
       
         auto androidDataPath = std::string("/storage/emulated/0/Android/data/").append(GetPackageName());
         
         androidDataPath += "/Dump.cs";
         
         std::ofstream outStream(androidDataPath);
         
         for(int i = 0; i < modules.size(); i++) {
             
             outStream << "// Image: " << modules[i]->name << "\n";
             
         }
         for(int i = 0; i < modules.size(); i++) {
             Module *mod = modules[i];
             for(int j = 0; j < mod->ns.size(); j++) {
                 Namespace *name = mod->ns[j];
                 for (int k = 0; k < name->Classes.size(); k++) {
                     Class *cls = name->Classes[k];
                 
                     outStream << "\nDll: " << mod->name;
                     outStream << "\nNamespace: " << name->name << "\n";
                     for(int l = 0; l < cls->flagNames.size(); l++) {
                         std::string flag = cls->flagNames[l];
                         outStream << flag;
                     }
                     outStream << cls->name;
                     std::vector<std::string> extends = cls->Extends;
                     if (!extends.empty()) {
                        outStream << " : " << extends[0];
                        for (int i = 1; i < extends.size(); ++i) {
                            outStream << ", " << extends[i];
                        }
                    }
                    std::string data = "";
                    
                    data += "\n\tFields (" + std::to_string(cls->fields.size()) + "): \n";
                    
                    for(int f = 0; f < cls->fields.size(); f++) {
                        Field *fil = cls->fields[f];
                        std::stringstream field;
                        field << "\t";
                        for(int flg = 0; flg < fil->flagNames.size(); flg++) {
                            field << fil->flagNames[flg];
                        }
                        field << fil->type << " " << fil->name;
                        if(fil->isEnum) {
                            field << " = " << fil->value;
                        }
                        field << "; // " << fil->offset;
                        field << "\n";
                        data += field.str();
                    }
                    data += "\n";
                    data += "\tProperties (" + std::to_string(cls->Properties.size()) + "): \n";
                    
                    for(int p = 0; p < cls->Properties.size(); p++) {
                        data += "\t";
                        data += cls->Properties[p]->prop;
                    }
                    
                    data += "\n";
                    data += "\tMethods (" + std::to_string(cls->methods.size()) + "): \n";
                    
                    for(int m = 0; m < cls->methods.size(); m++) {
                        data += "\t";
                        Method *meth = cls->methods[m];
                        data += "OFFSET: ";
                        data += meth->offset;
                        data += "\n\t";
                        data += meth->modfier;
                        data += meth->type + " ";
                        data += meth->name + "(";
                        for (int p = 0; p < meth->params.size(); p++) {
                            Param param = meth->params[p];
                            data += param.byRef;
                            data += param.type + " ";
                            data += param.name;
                            if(p != (meth->params.size() -1)) {
                                data += ", ";
                            }
                        }
                        data += ") {} \n\n";
                    }
                    
                    outStream << "\n{";
                    outStream << data;
                    outStream << "}\n";
                    
                 }
                 
             }
         }
         outStream.close();
     }
    
}


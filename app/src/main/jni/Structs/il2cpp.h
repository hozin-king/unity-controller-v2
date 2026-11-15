#include <dlfcn.h>

namespace il2cpp {
    #include "Dump/il2cpp-class.h"
     uintptr_t get_methodOffset(const char *name) {
            void *lib = dlopen("libil2cpp.so", 4);
            if(lib) {
                uintptr_t address = (uintptr_t)dlsym(lib, name);
                if(address) {
                    return address;
                }
            }
            return NULL;
        }
    class image {
         public:
             const char *get_name() {
                const char *(*get_domain)(...) = (const char *(*)(...))get_methodOffset("il2cpp_image_get_name");
                return get_domain(this);
             } 
     };
    class assembly {
        public:
            image *get_image() {
                image *(*get_domain)(...) = (image *(*)(...))get_methodOffset("il2cpp_assembly_get_image");
                return get_domain(this);
            }
    };
    class domain {
        public:
            static domain *get() {
                domain *(*get_domain)(...) = (domain *(*)(...))get_methodOffset("il2cpp_domain_get");
                return get_domain();
            }
            assembly **get_assemblies(size_t *size) {
                assembly **(*_get_assemblies)(...) = (assembly **(*)(...))get_methodOffset("il2cpp_domain_get_assemblies");
                return _get_assemblies(this, size);
            }
    };
    image *getImage(const char *imae) {
                domain *global = domain::get();
                size_t size;
                assembly **assemblies = global->get_assemblies(&size);
                for(int i = 0; i < size; ++i) {
                    image *img = assemblies[i]->get_image();
                    
                    const char *img_name = img->get_name();
        
                    if (strcmp(img_name, imae) == 0) {
                        return img;
                    }
                    
                }
                return nullptr;
            }
            
    class Class {
        public:
         const char *get_name(){
                const char *(*_get_classNameFromInstance)(Class* clas) = (const char *(*)(Class*))get_methodOffset("il2cpp_class_get_name");
                return _get_classNameFromInstance(this);
          }
          Class *get_parent() {
              Class *(*_il2cpp_class_get_parent)(...) = (Class *(*)(...))get_methodOffset("il2cpp_class_get_parent");
              return _il2cpp_class_get_parent(this);
          }
          Class *get_interfaces(void* *iter) {
              Class *(*il2cpp_class_get_interfaces)(...) = (Class *(*)(...))get_methodOffset("il2cpp_class_get_interfaces");
              return il2cpp_class_get_interfaces(this);
          }
          bool is_abstract() {
              bool (*_il2cpp_class_is_abstract)(...) = (bool (*)(...))get_methodOffset("il2cpp_class_is_abstract");
              return _il2cpp_class_is_abstract(this);
          }
          bool is_enum() {
              bool (*_il2cpp_class_is_abstract)(...) = (bool (*)(...))get_methodOffset("il2cpp_class_is_abstract");
              return _il2cpp_class_is_abstract(this);
          }
          void *get_type() {
              void *(*_il2cpp_class_get_type)(...) = (void *(*)(...))get_methodOffset("il2cpp_class_get_type");
              return _il2cpp_class_get_type(this);
          }
          
          std::vector<std::string> get_extends() {
              
              std::vector<std::string> extends;
              auto parent = this->get_parent();
              
              
              if (!this->is_abstract() && !this->is_enum() && parent) {
                 extends.emplace_back(parent->get_name());
              }
              
              
              void *iter = nullptr;
              while (auto itf = this->get_interfaces(&iter)) {
                  extends.emplace_back(itf->get_name());
                }
                
                return extends;
          }
          bool hasExtend(const char *extend) {
              std::vector<std::string> extends = this->get_extends();
              for(int i = 0; i < extends.size(); i++) {
                  if(strcmp(extend, extends[i].c_str()) == 0) {
                      return true;
                  }
              }
              return false;
          }
          static Class *from_name(const char *imge, const char *namespase, const char *name) {
              image *img=  getImage(imge);
              if(!img) {
                  return NULL;
              }
              Class *(*_get_assemblies)(...) = (Class *(*)(...))get_methodOffset("il2cpp_class_from_name");
              return _get_assemblies(img, namespase, name);
          }
          
          
    };
    
      
           
}

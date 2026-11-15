namespace Structs {
    class Camera : public Component {
        static uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Camera";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);

        }
        public:
            static Camera * get_main(){
                Camera * (*_get_main)(...) = (Camera * (*)(...))get_methodOffset("get_main");
    		    return _get_main();
            }
            
            Vector3 WorldToScreenPoint(Vector3 position){
                Vector3 (*_WorldToScreenPoint)(void* thiz, Vector3) = (Vector3 (*)(void*, Vector3))get_methodOffset("WorldToScreenPoint", 1);
                return _WorldToScreenPoint(this, position);
		    }
            float get_nearClipPlane() {
                float (*_get_nearClipPlane)(...) = (float (*)(...))get_methodOffset("get_nearClipPlane");
                return _get_nearClipPlane(this);
            }
            void set_nearClipPlane(float i) {
                void (*_set_nearClipPlane)(...) = (void (*)(...))get_methodOffset("set_nearClipPlane", 1);
                _set_nearClipPlane(this, i);
            }
            float get_farClipPlane() {
                float (*_get_farClipPlane)(...) = (float (*)(...))get_methodOffset("get_farClipPlane");
                return _get_farClipPlane(this);
            }
            void set_farClipPlane(float i) {
                void (*_set_farClipPlane)(...) = (void (*)(...))get_methodOffset("set_farClipPlane", 1);
                _set_farClipPlane(this, i);
            }
            static int get_allCamerasCount() {
                int (*_get_allCamerasCount)(...) = (int (*)(...))get_methodOffset("get_allCamerasCount");
                return _get_allCamerasCount();
            }
            static Array<Camera *> *get_allCameras() {
                uintptr_t off = get_methodOffset("get_allCameras");
                if(off == 0) {
                    return NULL;
                }
                Array<Camera *> *(*_get_allCameras)(...) = (Array<Camera *> *(*)(...))off;
                return _get_allCameras();
            }
            float get_fieldOfView() {
                float (*_get_fieldOfView)(...) = (float (*)(...))get_methodOffset("get_fieldOfView");
                return _get_fieldOfView(this);
            }
            void set_fieldOfView(float i) {
                void (*_set_fieldOfView)(...) = (void (*)(...))get_methodOffset("set_fieldOfView", 1);
                _set_fieldOfView(this, i);
            }
            
            
    };
}

namespace Structs {
    enum Space {
        World = 0,
        Self = 1
    };

    class Transform : public Object {
        Vector3 OldPos;
        uintptr_t get_methodOffset(const char *name, int param = 0) {
            const char *dll = "UnityEngine.CoreModule.dll";
            const char *namespace_name = "UnityEngine";
            const char *klass = "Transform";
            return (uintptr_t)IL2Cpp::Il2CppGetMethodOffset(dll, namespace_name, klass, name, param);
        }

    public:
        Vector3 get_position() {
            if (this == nullptr) {
                return Vector3(0, 0, 0);
            }
            Vector3 (*_get_position)(void *) = (Vector3 (*)(void *))get_methodOffset("get_position");
            return _get_position(this);
        }

        void set_position(Vector3 position) {
            if (this == nullptr) {
                return;
            }
            void (*_set_position)(void *, Vector3) = (void (*)(void *, Vector3))get_methodOffset("set_position", 1);
            _set_position(this, position);
        }

        Vector3 get_scale() {
            if (this == nullptr) {
                return Vector3(0, 0, 0);
            }
            Vector3 (*_get_scale)(void *) = (Vector3 (*)(void *))get_methodOffset("get_localScale");
            return _get_scale(this);
        }

        void set_scale(Vector3 position) {
            if (this == nullptr) {
                return;
            }
            void (*_set_scale)(void *, Vector3) = (void (*)(void *, Vector3))get_methodOffset("set_localScale", 1);
            _set_scale(this, position);
        }

        Vector3 get_right() {
            if (this == nullptr) {
                return Vector3(0, 0, 0);
            }
            Vector3 (*_get_right)(void *) = (Vector3 (*)(void *))get_methodOffset("get_right");
            return _get_right(this);
        }

        Vector3 get_forward() {
            if (this == nullptr) {
                return Vector3(0, 0, 0);
            }
            Vector3 (*_get_forward)(void *) = (Vector3 (*)(void *))get_methodOffset("get_forward");
            return _get_forward(this);
        }

        Vector3 get_up() {
            if (this == nullptr) {
                return Vector3(0, 0, 0);
            }
            Vector3 (*_get_up)(void *) = (Vector3 (*)(void *))get_methodOffset("get_up");
            return _get_up(this);
        }

        Quaternion get_rotation() {
            if (this == nullptr) {
                return Quaternion(0, 0, 0, 0);
            }
            Quaternion (*_get_rotation)(void *) = (Quaternion (*)(void *))get_methodOffset("get_rotation");
            return _get_rotation(this);
        }

        void set_rotation(Quaternion rotation) {
            if (this == nullptr) {
                return;
            }
            void (*_set_rotation)(void *, Quaternion) = (void (*)(void *, Quaternion))get_methodOffset("set_rotation", 1);
            _set_rotation(this, rotation);
        }

        void Rotate(Vector3 Axis, int relative) {
            if (this == nullptr) {
                return;
            }
            void (*_set_Rotate)(...) = (void (*)(...))get_methodOffset("Rotate", 2);
            _set_Rotate(this, Axis, relative);
        }

        void showEditor() {
            if (this == nullptr) {
                return;
            }
            if (ImGui::Button("Back")) {
                this->Rotate(Vector3(0, 1, 0), 0);
            }
        }
    };
}


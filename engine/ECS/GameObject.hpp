#pragma once
#include "Component.hpp"
#include "Base/Asset.hpp"
#include "Math/EngineMath.hpp"

namespace wfe {
    struct Transform {
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;

        Matrix4x4 GetTranslationMatrix()    const {
            return Matrix4x4::Translation(position);
        }
        Matrix4x4 GetInvTranslationMatrix() const {
            return Matrix4x4::Translation(-position);
        }
        Matrix4x4 GetRotationMatrix()       const {
            return Matrix4x4::Rotation(rotation);
        }
        Matrix4x4 GetInvRotationMatrix()    const {
            return Matrix4x4::Rotation(rotation.Inverted());
        }
        Matrix4x4 GetScalingMatrix()        const {
            return Matrix4x4::Scaling(scale);
        }
        Matrix4x4 GetInvScalingMatrix()     const {
            return Matrix4x4::Scaling({ 1.f / scale.x, 1.f / scale.y, 1.f / scale.z });
        }
        Matrix4x4 GetTransformMatrix()      const {
            return GetScalingMatrix() * GetRotationMatrix() * GetTranslationMatrix();
        }
        Matrix4x4 GetInvTransformMatrix()   const {
            return GetInvTranslationMatrix() * GetInvRotationMatrix() * GetInvScalingMatrix();
        }
        Matrix4x4 GetViewMatrix()           const {
            return GetRotationMatrix() * GetTranslationMatrix();
        }
        Matrix4x4 GetInvViewMatrix()        const {
            return GetInvTranslationMatrix() * GetInvRotationMatrix();
        }
    };

    class GameObject : public Asset {
    public:
        static GameObject* scene;

        string name;
        Transform transform;

        GameObject() = default;
        GameObject(const GameObject&) = delete;
        GameObject(GameObject&&) noexcept = delete;
        explicit GameObject(GameObject* parent);

        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&) noexcept = delete;

        GameObject* GetParent() {
            return parent;
        }
        void SetParent(GameObject* newParent);
        vector<GameObject*> GetChildren() {
            return { children.begin(), children.end() };
        }

        set<Component*> GetComponents() {
            return components;
        }
        template<class T>
        T* AddComponent() {
            return dynamic_cast<T*>(AddComponent(WFE_TYPE_ID(T).Hash64()));
        }
        Component* AddComponent(uint64_t hashCode) {
            ComponentType* componentType = Component::GetComponentType(hashCode);
            assert(componentType && "Invalid type provided!");

            Component* component = componentType->create(this);

            return component;
        }

        ~GameObject();
    protected:
        void LoadFromFile(const string& fileLocation) override;
        void SaveToFile  (const string& fileLocation) override;
    private:
        void LoadFromStream(FileInput&  input );
        void SaveToStream  (FileOutput& output);

        set<Component*> components{};

        GameObject* parent = nullptr;
        set<GameObject*> children{};

        friend Component;
    };
}
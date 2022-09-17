#pragma once
#include "Component.hpp"
#include "Base/Asset.hpp"
#include "Math/EngineMath.hpp"

namespace wfe {
    struct Transform {
        Vector3 position = { 0.f, 0.f, 0.f };
        Quaternion rotation = { 0.f, 0.f, 0.f, 1.f };
        Vector3 scale = { 1.f, 1.f, 1.f };

        /// @brief Returns the transform's translation matrix.
        Matrix4x4 GetTranslationMatrix()    const {
            return Matrix4x4::Translation(position);
        }
        /// @brief Returns the transform's inverse translation matrix.
        Matrix4x4 GetInvTranslationMatrix() const {
            return Matrix4x4::Translation(-position);
        }
        /// @brief Returns the transform's rotation matrix of the transform.
        Matrix4x4 GetRotationMatrix()       const {
            return Matrix4x4::Rotation(rotation);
        }
        /// @brief Returns the transform's inverse rotation matrix.
        Matrix4x4 GetInvRotationMatrix()    const {
            return Matrix4x4::Rotation(rotation.Inverted());
        }
        /// @brief Returns the transform's scaling matrix.
        Matrix4x4 GetScalingMatrix()        const {
            return Matrix4x4::Scaling(scale);
        }
        /// @brief Returns the transform's inverse scaling matrix.
        Matrix4x4 GetInvScalingMatrix()     const {
            return Matrix4x4::Scaling({ 1.f / scale.x, 1.f / scale.y, 1.f / scale.z });
        }
        /// @brief Returns the transform's matrix.
        Matrix4x4 GetTransformMatrix()      const {
            return GetScalingMatrix() * GetRotationMatrix() * GetTranslationMatrix();
        }
        /// @brief Returns the transform's inverse matrix.
        Matrix4x4 GetInvTransformMatrix()   const {
            return GetInvTranslationMatrix() * GetInvRotationMatrix() * GetInvScalingMatrix();
        }
        /// @brief Returns the transform's view matrix.
        Matrix4x4 GetViewMatrix()           const {
            return GetRotationMatrix() * GetTranslationMatrix();
        }
        /// @brief Returns the 
        Matrix4x4 GetInvViewMatrix()        const {
            return GetInvTranslationMatrix() * GetInvRotationMatrix();
        }
    };

    class GameObject : public Asset {
    public:
        static GameObject* scene;

        string name = "";
        Transform transform;

        GameObject() = default;
        GameObject(const GameObject&) = delete;
        GameObject(GameObject&&) noexcept = delete;
        explicit GameObject(GameObject* parent);

        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&) noexcept = delete;

        /// @brief Returns the game object's parent.
        GameObject* GetParent() {
            return parent;
        }
        /// @brief Sets the game objet's parent.
        void SetParent(GameObject* newParent);
        /// @brief Returns a vector with the game object's children.
        vector<GameObject*> GetChildren() {
            return { children.begin(), children.end() };
        }

        /// @brief Returns a set with the game object's components.
        set<Component*> GetComponents() {
            return components;
        }
        /// @brief Adds a component with the specified type to the game object.
        /// @tparam The type of the new component.
        /// @return A pointer to the new component.
        template<class T>
        T* AddComponent() {
            return dynamic_cast<T*>(AddComponent(WFE_TYPE_ID(T).Hash64()));
        }
        /// @brief Returns the first component it finds with the specified type.
        /// @tparam The type of the wanted component.
        /// @return A pointer to the component if it exists, otherwise a nullptr.
        template<class T>
        T* GetComponent() {
            return dynamic_cast<T*>(GetComponent(WFE_TYPE_ID(T).Hash64()));
        }
        /// @brief Adds a component with the specified type hash code to the game object.
        /// @param hashCode The component type's hash code.
        /// @return A pointer to the newly created component.
        Component* AddComponent(uint64_t hashCode) {
            // Find the component type with the specified hash code
            auto* componentType = Component::componentTypes.find(hashCode);
            assert((componentType != Component::componentTypes.end()) && "Invalid type provided!");

            // Create a component with the specified type
            Component* component = componentType->val2.create(this);

            return component;
        }
        /// @brief Returns the first component it finds with the specified type hash code.
        /// @param hashCode The component type's hash code.
        /// @return A pointer to the component if it exists, otherwise a nullptr.
        Component* GetComponent(uint64_t hashCode) {
            // Return the first component with the specified hash code
            for(auto* component : components)
                if(component->componentType->hashCode == hashCode)
                    return component;
            
            // No component was found Return a nullptr
            return nullptr;
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
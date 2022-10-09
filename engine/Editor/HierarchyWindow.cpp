#include "Editor.hpp"
#include "ECS/EngineECS.hpp"
#include "imgui_internal.hpp"

namespace wfe {
    // Variables
    Asset* selectedAsset;
    bool8_t renamingObject;

    // Internal helper functions
    static void RenderGameObject(GameObject* gameObject) {
        // Check if the current object should be deleted
        if(!renamingObject && ImGui::GetIO().KeysData[ImGuiKey_Delete].Down && ImGui::GetIO().KeysData[ImGuiKey_Delete].DownDuration == 0.f && selectedAsset == (Asset*)gameObject && gameObject != GameObject::scene) {
            delete gameObject;
            selectedAsset = nullptr;
            return;
        }

        // Select the current tree node name
        string treeNodeName;
        if(renamingObject && selectedAsset == (Asset*)gameObject)
            treeNodeName = "";
        else
            treeNodeName = gameObject->name;

        // Select the current tree node flags
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
        if(selectedAsset == (Asset*)gameObject)
            flags |= ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_AllowItemOverlap;
        
        if(!renamingObject || selectedAsset != (Asset*)gameObject)
            flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        // Display the tree node
        bool8_t open = ImGui::TreeNodeEx((void*)gameObject, flags, "%s", treeNodeName.c_str());

        // Select the object if it is clicked
        if(ImGui::IsItemClicked())
            selectedAsset = (Asset*)gameObject;

        // Add the renaming input box if needed
        if(renamingObject && selectedAsset == (Asset*)gameObject) {
            ImGui::SameLine();

            char_t buffer[256];
            strcpy(buffer, gameObject->name.c_str());

            ImGui::PushID(0);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));

            if(ImGui::InputText("", buffer, 256, ImGuiInputTextFlags_AutoSelectAll))
                gameObject->name = buffer;

            if(ImGui::IsItemDeactivated() || ImGui::IsItemDeactivatedAfterEdit())
                renamingObject = false;

            ImGui::PopStyleVar();
            ImGui::PopID();
        }

        if(open) {
            for(auto* child : gameObject->GetChildren())
                RenderGameObject(child);
            
            ImGui::TreePop();
        }
    }

    static void RenderWindow() {
        auto& windowType = editor::WindowType::windowTypes->at("Scene Hierarchy");

        ImGui::SetNextWindowSize(ImVec2(300.f, 600.f), ImGuiCond_FirstUseEver);
        if(ImGui::Begin("Scene Hierarchy", &windowType.open)) {
            ImVec2 windowRectMin = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 windowRectMax = ImVec2(windowRectMin.x + windowSize.x, windowRectMin.y + windowSize.y);

            // Check if the current object should be renamed
            if(!renamingObject && ImGui::GetIO().KeysData[ImGuiKey_F2].Down && ImGui::GetIO().KeysData[ImGuiKey_F2].DownDuration == 0.f && selectedAsset)
                renamingObject = true;
            else if(renamingObject && ImGui::GetIO().KeysData[ImGuiKey_Enter].Down && ImGui::GetIO().KeysData[ImGuiKey_Enter].DownDuration == 0.f)
                renamingObject = false;

            // Open the options popup
            if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsMouseHoveringRect(windowRectMin, windowRectMax)) {
                ImGui::OpenPopup("Options");
            }

            // Begin the options popup
            if(ImGui::BeginPopup("Options")) {
                if(ImGui::MenuItem("New game object")) {
                    GameObject* gameObject = new GameObject();
                    if(dynamic_cast<GameObject*>(selectedAsset))
                        gameObject->SetParent((GameObject*)selectedAsset);
                    else
                        gameObject->SetParent(GameObject::scene);

                    gameObject->name = "New game object";
                }
                if(ImGui::MenuItem("Rename game object", nullptr, false, dynamic_cast<GameObject*>(selectedAsset)))
                    renamingObject = true;
                if(ImGui::MenuItem("Delete game object", nullptr, false, dynamic_cast<GameObject*>(selectedAsset))) {
                    delete selectedAsset;
                    selectedAsset = nullptr;
                }

                ImGui::EndPopup();
            }

            RenderGameObject(GameObject::scene);
        }
    }

    // Public functions
    Asset* editor::GetSelectedAsset() {
        return selectedAsset;
    }
    void editor::SetSelectedAsset(Asset* newAsset) {
        selectedAsset = newAsset;
    }

    WFE_EDITOR_WINDOW_TYPE("Scene Hierarchy", RenderWindow)
}
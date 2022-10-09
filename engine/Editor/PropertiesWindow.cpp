#include "Editor.hpp"
#include "Base/Asset.hpp"

namespace wfe {
    // Internal helper functions
    static void RenderWindow() {
        auto& windowType = editor::WindowType::windowTypes->at("Properties");

        if(ImGui::Begin("Properties", &windowType.open)) {
            Asset* selectedAsset = editor::GetSelectedAsset();
            if(selectedAsset)
                selectedAsset->DrawEditorWindow();

            ImGui::End();
        }
    }

    WFE_EDITOR_WINDOW_TYPE("Properties", RenderWindow)
}
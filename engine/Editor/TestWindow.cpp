#include "WireframeEngineEditor.hpp"

namespace wfe {
    static void RenderWindow() {
        editor::WindowType& windowType = editor::WindowType::windowTypes->at("Test Window");

        if(ImGui::Begin("Test Window", &windowType.open)) {
            ImGui::Text("epic win!!!!1!!");
            ImGui::End();
        }
    }

    WFE_EDITOR_WINDOW_TYPE("Test Window", RenderWindow)
}
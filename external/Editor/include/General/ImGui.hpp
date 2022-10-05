#pragma once

#include "Core.hpp"
#include "imgui.hpp"

namespace wfe::editor {
    /// @brief Creates the ImGui context. Internal use only.
    void CreateImGui();
    /// @brief Deletes the ImGui context. Internal use only.
    void DeleteImGui();
    /// @brief Processes any ImGui relevant events. Meant for internal use.
    void ProcessImGuiEvents();

    /// @brief Gets the ImGui context.
    /// @return A pointer to the ImGui context.
    ImGuiContext* GetImGuiContext();
    /// @brief Gets the ImGui IO.
    /// @return A pointer to the ImGui IO.
    ImGuiIO* GetImGuiIO();
    /// @brief Gets the ImGui style.
    /// @return A pointer to the ImGui style.
    ImGuiStyle* GetImGuiStyle();

    /// @brief Gets the ImGui normal font.
    /// @return A pointer to the ImGui normal font.
    ImFont* GetImGuiNormalFont();
    /// @brief Gets the ImGui bold font.
    /// @return A pointer to the ImGui bold font.
    ImFont* GetImGuiBoldFont();
    /// @brief Gets the ImGui italic font.
    /// @return A pointer to the ImGui italic font.
    ImFont* GetImGuiItalicFont();
}
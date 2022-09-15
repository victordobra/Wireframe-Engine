#pragma once

#include "VulkanInclude.hpp"

namespace wfe::editor {
    /// @brief Creates the ImGui pipeline. Internal use only.
    void CreateImGuiPipeline();
    /// @brief Deletes the ImGui pipeline. Internal use only.
    void DeleteImGuiPipeline();
    /// @brief Draws the ImGui objects. Internal use only.
    void DrawImGui();

    /// @brief Returns the ImGui pipeline.
    VkPipeline GetImGuiPipeline();
    /// @brief Returns the ImGui pipeline layout.
    VkPipelineLayout GetImGuiPipelineLayout();
    /// @brief Returns the ImGui command buffer.
    VkCommandBuffer GetImGuiCommandBuffer();
}
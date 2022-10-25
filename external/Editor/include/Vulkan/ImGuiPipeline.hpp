#pragma once

#include "Core.hpp"
#include "VulkanInclude.hpp"
#include "imgui.hpp"

namespace wfe::editor {
    typedef enum : uint8_t {
        IMAGE_ID_FILE,
        IMAGE_ID_FOLDER_EMPTY,
        IMAGE_ID_FOLDER_FULL,
    } ImageID;

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

    /// @brief Returns the coresponding ImGui texture ID for the given image ID.
    /// @param imageId The image ID.
    ImTextureID GetImGuiTexture(ImageID imageId);
}
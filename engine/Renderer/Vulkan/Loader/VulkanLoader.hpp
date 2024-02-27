#pragma once

#include <Core.hpp>
#include "Renderer/Renderer.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#if defined(WFE_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(WFE_PLATFORM_LINUX)
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>
#endif

namespace wfe {
	/// @brief A loader containing function pointers for all Vulkan functions.
	class VulkanLoader {
	public:
		/// @brief Creates the loader and loads all Vulkan core functions
		VulkanLoader();
		VulkanLoader(const VulkanLoader&) = delete;
		VulkanLoader(VulkanLoader&&) noexcept = delete;

		VulkanLoader& operator=(const VulkanLoader&) = delete;
		VulkanLoader& operator=(VulkanLoader&&) = delete;

		/// @brief Load Vulkan's instance function pointers.
		/// @param instance The instance to load from.
		void LoadInstanceFunctions(VkInstance instance);
		/// @brief Load Vulkan's device function pointers.
		/// @param device The device to load from.
		void LoadDeviceFunctions(VkDevice device);

		/// @brief Destroys the loader.
		~VulkanLoader();

#if defined(VK_VERSION_1_0)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const;
		VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName) const;
		VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName) const;
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) const;
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) const;
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) const;
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) const;
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceVersion(uint32_t* pApiVersion) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const;
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkQueueWaitIdle(VkQueue queue) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkDeviceWaitIdle(VkDevice device) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) const;
		VKAPI_ATTR void VKAPI_CALL vkFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) const;
		VKAPI_ATTR void VKAPI_CALL vkUnmapMemory(VkDevice device, VkDeviceMemory memory) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) const;
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) const;
		VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) const;
		VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) const;
		VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) const;
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceStatus(VkDevice device, VkFence fence) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetEventStatus(VkDevice device, VkEvent event) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkSetEvent(VkDevice device, VkEvent event) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkResetEvent(VkDevice device, VkEvent event) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR void VKAPI_CALL vkResetQueryPool(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const;
#endif
#if defined(VK_EXT_host_query_reset) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkResetQueryPoolEXT(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) const;
#endif
#if defined(VK_HUAWEI_subpass_shading) && (defined(VK_KHR_create_renderpass2) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI(VkDevice device, VkRenderPass renderpass, VkExtent2D* pMaxWorkgroupSize) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR void VKAPI_CALL vkDestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) const;
		VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) const;
#endif
#if defined(VK_KHR_maintenance5) && (defined(VK_VERSION_1_1) && defined(VK_KHR_dynamic_rendering))
		VKAPI_ATTR void VKAPI_CALL vkGetRenderingAreaGranularityKHR(VkDevice device, const VkRenderingAreaInfoKHR* pRenderingAreaInfo, VkExtent2D* pGranularity) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) const;
		VKAPI_ATTR void VKAPI_CALL vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkEndCommandBuffer(VkCommandBuffer commandBuffer) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const;
#endif
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_EXT_attachment_feedback_loop_layout))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetAttachmentFeedbackLoopEnableEXT(VkCommandBuffer commandBuffer, VkImageAspectFlags aspectMask) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const;
#endif
#if defined(VK_EXT_multi_draw) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawMultiEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawInfoEXT* pVertexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawMultiIndexedEXT(VkCommandBuffer commandBuffer, uint32_t drawCount, const VkMultiDrawIndexedInfoEXT* pIndexInfo, uint32_t instanceCount, uint32_t firstInstance, uint32_t stride, const int32_t* pVertexOffset) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) const;
#endif
#if defined(VK_HUAWEI_subpass_shading) && (defined(VK_KHR_create_renderpass2) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR void VKAPI_CALL vkCmdSubpassShadingHUAWEI(VkCommandBuffer commandBuffer) const;
#endif
#if defined(VK_HUAWEI_cluster_culling_shader) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawClusterHUAWEI(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawClusterIndirectHUAWEI(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) const;
#endif
#if defined(VK_NV_device_generated_commands_compute) && (defined(VK_NV_device_generated_commands))
		VKAPI_ATTR void VKAPI_CALL vkCmdUpdatePipelineIndirectBufferNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const;
#endif
#if defined(VK_NV_copy_memory_indirect) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_buffer_device_address))
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryToImageIndirectNV(VkCommandBuffer commandBuffer, VkDeviceAddress copyBufferAddress, uint32_t copyCount, uint32_t stride, VkImage dstImage, VkImageLayout dstImageLayout, const VkImageSubresourceLayers* pImageSubresources) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR void VKAPI_CALL vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) const;
#endif
#if defined(VK_EXT_conditional_rendering) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) const;
#endif
#if defined(VK_VERSION_1_0)
		VKAPI_ATTR void VKAPI_CALL vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass(VkCommandBuffer commandBuffer) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const;
#endif
#if defined(VK_KHR_android_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_KHR_display) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_KHR_display_swapchain) && (defined(VK_KHR_swapchain) && defined(VK_KHR_display))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateSharedSwapchainsKHR(VkDevice device, uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchains) const;
#endif
#if defined(VK_KHR_surface)
		VKAPI_ATTR void VKAPI_CALL vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const;
#endif
#if defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) const;
#endif
#if defined(VK_NN_vi_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateViSurfaceNN(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_KHR_wayland_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
		VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceWaylandPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display) const;
#endif
#if defined(VK_KHR_win32_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
		VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;
#endif
#if defined(VK_KHR_xlib_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
		VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) const;
#endif
#if defined(VK_KHR_xcb_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateXcbSurfaceKHR(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
		VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceXcbPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id) const;
#endif
#if defined(VK_EXT_directfb_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDirectFBSurfaceEXT(VkInstance instance, const VkDirectFBSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
		VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceDirectFBPresentationSupportEXT(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, IDirectFB* dfb) const;
#endif
#if defined(VK_FUCHSIA_imagepipe_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateImagePipeSurfaceFUCHSIA(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_GGP_stream_descriptor_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateStreamDescriptorSurfaceGGP(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_QNX_screen_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateScreenSurfaceQNX(VkInstance instance, const VkScreenSurfaceCreateInfoQNX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
		VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceScreenPresentationSupportQNX(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct _screen_window* window) const;
#endif
#if defined(VK_EXT_debug_report)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage) const;
#endif
#if defined(VK_EXT_debug_marker) && (defined(VK_EXT_debug_report))
		VKAPI_ATTR VkResult VKAPI_CALL vkDebugMarkerSetObjectNameEXT(VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkDebugMarkerSetObjectTagEXT(VkDevice device, const VkDebugMarkerObjectTagInfoEXT* pTagInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const;
#endif
#if defined(VK_NV_external_memory_capabilities)
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceExternalImageFormatPropertiesNV(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) const;
#endif
#if defined(VK_NV_external_memory_win32) && (defined(VK_NV_external_memory))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) const;
#endif
#if defined(VK_NV_device_generated_commands) && (defined(VK_VERSION_1_1) && defined(VK_KHR_buffer_device_address))
		VKAPI_ATTR void VKAPI_CALL vkCmdExecuteGeneratedCommandsNV(VkCommandBuffer commandBuffer, VkBool32 isPreprocessed, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdPreprocessGeneratedCommandsNV(VkCommandBuffer commandBuffer, const VkGeneratedCommandsInfoNV* pGeneratedCommandsInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindPipelineShaderGroupNV(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline, uint32_t groupIndex) const;
		VKAPI_ATTR void VKAPI_CALL vkGetGeneratedCommandsMemoryRequirementsNV(VkDevice device, const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2* pMemoryRequirements) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutNV(VkDevice device, const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNV* pIndirectCommandsLayout) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyIndirectCommandsLayoutNV(VkDevice device, VkIndirectCommandsLayoutNV indirectCommandsLayout, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2KHR* pFeatures) const;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2KHR* pFeatures) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2KHR* pProperties) const;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2KHR* pProperties) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2KHR* pFormatProperties) const;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2KHR(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2KHR* pFormatProperties) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2KHR* pImageFormatInfo, VkImageFormatProperties2KHR* pImageFormatProperties) const;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2KHR* pImageFormatInfo, VkImageFormatProperties2KHR* pImageFormatProperties) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2KHR* pQueueFamilyProperties) const;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2KHR* pQueueFamilyProperties) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2KHR* pMemoryProperties) const;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2KHR* pMemoryProperties) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2KHR* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2KHR* pProperties) const;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2KHR* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2KHR* pProperties) const;
#endif
#if defined(VK_KHR_push_descriptor) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkTrimCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlagsKHR flags) const;
#endif
#if defined(VK_KHR_maintenance1)
		VKAPI_ATTR void VKAPI_CALL vkTrimCommandPoolKHR(VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlagsKHR flags) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfoKHR* pExternalBufferInfo, VkExternalBufferPropertiesKHR* pExternalBufferProperties) const;
#endif
#if defined(VK_KHR_external_memory_capabilities) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalBufferPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfoKHR* pExternalBufferInfo, VkExternalBufferPropertiesKHR* pExternalBufferProperties) const;
#endif
#if defined(VK_KHR_external_memory_win32) && (defined(VK_KHR_external_memory))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryWin32HandleKHR(VkDevice device, const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryWin32HandlePropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) const;
#endif
#if defined(VK_KHR_external_memory_fd) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryFdKHR(VkDevice device, const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryFdPropertiesKHR(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) const;
#endif
#if defined(VK_FUCHSIA_external_memory) && (defined(VK_KHR_external_memory_capabilities) && defined(VK_KHR_external_memory))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryZirconHandleFUCHSIA(VkDevice device, const VkMemoryGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryZirconHandlePropertiesFUCHSIA(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, zx_handle_t zirconHandle, VkMemoryZirconHandlePropertiesFUCHSIA* pMemoryZirconHandleProperties) const;
#endif
#if defined(VK_NV_external_memory_rdma) && (defined(VK_KHR_external_memory))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryRemoteAddressNV(VkDevice device, const VkMemoryGetRemoteAddressInfoNV* pMemoryGetRemoteAddressInfo, VkRemoteAddressNV* pAddress) const;
#endif
#if defined(VK_NV_external_memory_sci_buf) && (defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemorySciBufNV(VkDevice device, const VkMemoryGetSciBufInfoNV* pGetSciBufInfo, NvSciBufObj* pHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV(VkPhysicalDevice physicalDevice, VkExternalMemoryHandleTypeFlagBits handleType, NvSciBufObj handle, VkMemorySciBufPropertiesNV* pMemorySciBufProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSciBufAttributesNV(VkPhysicalDevice physicalDevice, NvSciBufAttrList pAttributes) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfoKHR* pExternalSemaphoreInfo, VkExternalSemaphorePropertiesKHR* pExternalSemaphoreProperties) const;
#endif
#if defined(VK_KHR_external_semaphore_capabilities) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfoKHR* pExternalSemaphoreInfo, VkExternalSemaphorePropertiesKHR* pExternalSemaphoreProperties) const;
#endif
#if defined(VK_KHR_external_semaphore_win32) && (defined(VK_KHR_external_semaphore))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreWin32HandleKHR(VkDevice device, const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkImportSemaphoreWin32HandleKHR(VkDevice device, const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) const;
#endif
#if defined(VK_KHR_external_semaphore_fd) && (defined(VK_KHR_external_semaphore) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreFdKHR(VkDevice device, const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkImportSemaphoreFdKHR(VkDevice device, const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) const;
#endif
#if defined(VK_FUCHSIA_external_semaphore) && (defined(VK_KHR_external_semaphore_capabilities) && defined(VK_KHR_external_semaphore))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkSemaphoreGetZirconHandleInfoFUCHSIA* pGetZirconHandleInfo, zx_handle_t* pZirconHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkImportSemaphoreZirconHandleFUCHSIA(VkDevice device, const VkImportSemaphoreZirconHandleInfoFUCHSIA* pImportSemaphoreZirconHandleInfo) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfoKHR* pExternalFenceInfo, VkExternalFencePropertiesKHR* pExternalFenceProperties) const;
#endif
#if defined(VK_KHR_external_fence_capabilities) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalFencePropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfoKHR* pExternalFenceInfo, VkExternalFencePropertiesKHR* pExternalFenceProperties) const;
#endif
#if defined(VK_KHR_external_fence_win32) && (defined(VK_KHR_external_fence))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceWin32HandleKHR(VkDevice device, const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkImportFenceWin32HandleKHR(VkDevice device, const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) const;
#endif
#if defined(VK_KHR_external_fence_fd) && (defined(VK_KHR_external_fence) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceFdKHR(VkDevice device, const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkImportFenceFdKHR(VkDevice device, const VkImportFenceFdInfoKHR* pImportFenceFdInfo) const;
#endif
#if (defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1))) || (defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1)))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceSciSyncFenceNV(VkDevice device, const VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceSciSyncObjNV(VkDevice device, const VkFenceGetSciSyncInfoNV* pGetSciSyncHandleInfo, void* pHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkImportFenceSciSyncFenceNV(VkDevice device, const VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkImportFenceSciSyncObjNV(VkDevice device, const VkImportFenceSciSyncInfoNV* pImportFenceSciSyncInfo) const;
#endif
#if defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreSciSyncObjNV(VkDevice device, const VkSemaphoreGetSciSyncInfoNV* pGetSciSyncInfo, void* pHandle) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkImportSemaphoreSciSyncObjNV(VkDevice device, const VkImportSemaphoreSciSyncInfoNV* pImportSemaphoreSciSyncInfo) const;
#endif
#if (defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1))) || (defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1)))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSciSyncAttributesNV(VkPhysicalDevice physicalDevice, const VkSciSyncAttributesInfoNV* pSciSyncAttributesInfo, NvSciSyncAttrList pAttributes) const;
#endif
#if defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateSemaphoreSciSyncPoolNV(VkDevice device, const VkSemaphoreSciSyncPoolCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphoreSciSyncPoolNV* pSemaphorePool) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroySemaphoreSciSyncPoolNV(VkDevice device, VkSemaphoreSciSyncPoolNV semaphorePool, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_EXT_direct_mode_display) && (defined(VK_KHR_display))
		VKAPI_ATTR VkResult VKAPI_CALL vkReleaseDisplayEXT(VkPhysicalDevice physicalDevice, VkDisplayKHR display) const;
#endif
#if defined(VK_EXT_acquire_xlib_display) && (defined(VK_EXT_direct_mode_display))
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquireXlibDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR display) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetRandROutputDisplayEXT(VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay) const;
#endif
#if defined(VK_NV_acquire_winrt_display) && (defined(VK_EXT_direct_mode_display))
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquireWinrtDisplayNV(VkPhysicalDevice physicalDevice, VkDisplayKHR display) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetWinrtDisplayNV(VkPhysicalDevice physicalDevice, uint32_t deviceRelativeId, VkDisplayKHR* pDisplay) const;
#endif
#if defined(VK_EXT_display_control) && (defined(VK_EXT_display_surface_counter) && defined(VK_KHR_swapchain))
		VKAPI_ATTR VkResult VKAPI_CALL vkDisplayPowerControlEXT(VkDevice device, VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkRegisterDeviceEventEXT(VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkRegisterDisplayEventEXT(VkDevice device, VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainCounterEXT(VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) const;
#endif
#if defined(VK_EXT_display_surface_counter) && (defined(VK_KHR_display))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2EXT(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupPropertiesKHR* pPhysicalDeviceGroupProperties) const;
#endif
#if defined(VK_KHR_device_group_creation)
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR(VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupPropertiesKHR* pPhysicalDeviceGroupProperties) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlagsKHR* pPeerMemoryFeatures) const;
#endif
#if defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeaturesKHR(VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlagsKHR* pPeerMemoryFeatures) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfoKHR* pBindInfos) const;
#endif
#if defined(VK_KHR_bind_memory2)
		VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindBufferMemoryInfoKHR* pBindInfos) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory2(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfoKHR* pBindInfos) const;
#endif
#if defined(VK_KHR_bind_memory2)
		VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory2KHR(VkDevice device, uint32_t bindInfoCount, const VkBindImageMemoryInfoKHR* pBindInfos) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) const;
#endif
#if defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) const;
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_surface)))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR(VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR(VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) const;
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_swapchain)))
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImage2KHR(VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkCmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
#endif
#if defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))
		VKAPI_ATTR void VKAPI_CALL vkCmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_surface)))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate(VkDevice device, const VkDescriptorUpdateTemplateCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplateKHR* pDescriptorUpdateTemplate) const;
#endif
#if defined(VK_KHR_descriptor_update_template)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplateKHR(VkDevice device, const VkDescriptorUpdateTemplateCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplateKHR* pDescriptorUpdateTemplate) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorUpdateTemplate(VkDevice device, VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_KHR_descriptor_update_template)
		VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorUpdateTemplateKHR(VkDevice device, VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSetWithTemplate(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, const void* pData) const;
#endif
#if defined(VK_KHR_descriptor_update_template)
		VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSetWithTemplateKHR(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, const void* pData) const;
#endif
#if (((defined(VK_KHR_push_descriptor) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_push_descriptor) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_KHR_descriptor_update_template)))) || ((defined(VK_KHR_descriptor_update_template)) && (defined(VK_KHR_push_descriptor)))
		VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) const;
#endif
#if defined(VK_EXT_hdr_metadata) && (defined(VK_KHR_swapchain))
		VKAPI_ATTR void VKAPI_CALL vkSetHdrMetadataEXT(VkDevice device, uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) const;
#endif
#if defined(VK_KHR_shared_presentable_image) && (defined(VK_KHR_swapchain) && defined(VK_KHR_get_surface_capabilities2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainStatusKHR(VkDevice device, VkSwapchainKHR swapchain) const;
#endif
#if defined(VK_GOOGLE_display_timing) && (defined(VK_KHR_swapchain))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetRefreshCycleDurationGOOGLE(VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPastPresentationTimingGOOGLE(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) const;
#endif
#if defined(VK_MVK_ios_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateIOSSurfaceMVK(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_MVK_macos_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_EXT_metal_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateMetalSurfaceEXT(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_NV_clip_space_w_scaling)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) const;
#endif
#if defined(VK_EXT_discard_rectangles) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDiscardRectangleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 discardRectangleEnable) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDiscardRectangleModeEXT(VkCommandBuffer commandBuffer, VkDiscardRectangleModeEXT discardRectangleMode) const;
#endif
#if defined(VK_EXT_sample_locations) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMultisamplePropertiesEXT(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) const;
#endif
#if defined(VK_KHR_get_surface_capabilities2) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormats2KHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) const;
#endif
#if defined(VK_KHR_get_display_properties2) && (defined(VK_KHR_display))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlaneProperties2KHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayModeProperties2KHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDisplayPlaneCapabilities2KHR(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements2(VkDevice device, const VkBufferMemoryRequirementsInfo2KHR* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
#endif
#if defined(VK_KHR_get_memory_requirements2)
		VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements2KHR(VkDevice device, const VkBufferMemoryRequirementsInfo2KHR* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements2(VkDevice device, const VkImageMemoryRequirementsInfo2KHR* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
#endif
#if defined(VK_KHR_get_memory_requirements2)
		VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements2KHR(VkDevice device, const VkImageMemoryRequirementsInfo2KHR* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements2(VkDevice device, const VkImageSparseMemoryRequirementsInfo2KHR* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2KHR* pSparseMemoryRequirements) const;
#endif
#if defined(VK_KHR_get_memory_requirements2)
		VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements2KHR(VkDevice device, const VkImageSparseMemoryRequirementsInfo2KHR* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2KHR* pSparseMemoryRequirements) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceBufferMemoryRequirements(VkDevice device, const VkDeviceBufferMemoryRequirementsKHR* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
#endif
#if defined(VK_KHR_maintenance4) && (defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceBufferMemoryRequirementsKHR(VkDevice device, const VkDeviceBufferMemoryRequirementsKHR* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirementsKHR* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
#endif
#if defined(VK_KHR_maintenance4) && (defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirementsKHR* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageSparseMemoryRequirements(VkDevice device, const VkDeviceImageMemoryRequirementsKHR* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2KHR* pSparseMemoryRequirements) const;
#endif
#if defined(VK_KHR_maintenance4) && (defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageSparseMemoryRequirementsKHR(VkDevice device, const VkDeviceImageMemoryRequirementsKHR* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2KHR* pSparseMemoryRequirements) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion(VkDevice device, const VkSamplerYcbcrConversionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversionKHR* pYcbcrConversion) const;
#endif
#if defined(VK_KHR_sampler_ycbcr_conversion) && (defined(VK_KHR_maintenance1) && defined(VK_KHR_bind_memory2) && defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateSamplerYcbcrConversionKHR(VkDevice device, const VkSamplerYcbcrConversionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversionKHR* pYcbcrConversion) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkDestroySamplerYcbcrConversion(VkDevice device, VkSamplerYcbcrConversionKHR ycbcrConversion, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_KHR_sampler_ycbcr_conversion) && (defined(VK_KHR_maintenance1) && defined(VK_KHR_bind_memory2) && defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkDestroySamplerYcbcrConversionKHR(VkDevice device, VkSamplerYcbcrConversionKHR ycbcrConversion, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue2(VkDevice device, const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) const;
#endif
#if defined(VK_EXT_validation_cache)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateValidationCacheEXT(VkDevice device, const VkValidationCacheCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyValidationCacheEXT(VkDevice device, VkValidationCacheEXT validationCache, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetValidationCacheDataEXT(VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkMergeValidationCachesEXT(VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) const;
#endif
#if defined(VK_VERSION_1_1)
		VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSupport(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupportKHR* pSupport) const;
#endif
#if defined(VK_KHR_maintenance3) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSupportKHR(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupportKHR* pSupport) const;
#endif
#if defined(VK_ANDROID_native_buffer)
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainGrallocUsageANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, int* grallocUsage) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainGrallocUsage2ANDROID(VkDevice device, VkFormat format, VkImageUsageFlags imageUsage, VkSwapchainImageUsageFlagsANDROID swapchainImageUsage, uint64_t* grallocConsumerUsage, uint64_t* grallocProducerUsage) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquireImageANDROID(VkDevice device, VkImage image, int nativeFenceFd, VkSemaphore semaphore, VkFence fence) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkQueueSignalReleaseImageANDROID(VkQueue queue, uint32_t waitSemaphoreCount, const VkSemaphore* pWaitSemaphores, VkImage image, int* pNativeFenceFd) const;
#endif
#if defined(VK_AMD_shader_info)
		VKAPI_ATTR VkResult VKAPI_CALL vkGetShaderInfoAMD(VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) const;
#endif
#if defined(VK_AMD_display_native_hdr) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))
		VKAPI_ATTR void VKAPI_CALL vkSetLocalDimmingAMD(VkDevice device, VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) const;
#endif
#if defined(VK_KHR_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsKHR(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains) const;
#endif
#if defined(VK_EXT_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains) const;
#endif
#if defined(VK_KHR_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetCalibratedTimestampsKHR(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) const;
#endif
#if defined(VK_EXT_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetCalibratedTimestampsEXT(VkDevice device, uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) const;
#endif
#if defined(VK_EXT_debug_utils)
		VKAPI_ATTR VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkSetDebugUtilsObjectTagEXT(VkDevice device, const VkDebugUtilsObjectTagInfoEXT* pTagInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkQueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkQueueEndDebugUtilsLabelEXT(VkQueue queue) const;
		VKAPI_ATTR void VKAPI_CALL vkQueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) const;
#endif
#if defined(VK_EXT_external_memory_host) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryHostPointerPropertiesEXT(VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) const;
#endif
#if defined(VK_AMD_buffer_marker)
		VKAPI_ATTR void VKAPI_CALL vkCmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass2(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) const;
#endif
#if defined(VK_KHR_create_renderpass2) && (defined(VK_KHR_multiview) && defined(VK_KHR_maintenance2))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass2KHR(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfoKHR* pSubpassBeginInfo) const;
#endif
#if defined(VK_KHR_create_renderpass2) && (defined(VK_KHR_multiview) && defined(VK_KHR_maintenance2))
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfoKHR* pSubpassBeginInfo) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass2(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR* pSubpassBeginInfo, const VkSubpassEndInfoKHR* pSubpassEndInfo) const;
#endif
#if defined(VK_KHR_create_renderpass2) && (defined(VK_KHR_multiview) && defined(VK_KHR_maintenance2))
		VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR* pSubpassBeginInfo, const VkSubpassEndInfoKHR* pSubpassEndInfo) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass2(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR* pSubpassEndInfo) const;
#endif
#if defined(VK_KHR_create_renderpass2) && (defined(VK_KHR_multiview) && defined(VK_KHR_maintenance2))
		VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR* pSubpassEndInfo) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreCounterValue(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) const;
#endif
#if defined(VK_KHR_timeline_semaphore) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreCounterValueKHR(VkDevice device, VkSemaphore semaphore, uint64_t* pValue) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR VkResult VKAPI_CALL vkWaitSemaphores(VkDevice device, const VkSemaphoreWaitInfoKHR* pWaitInfo, uint64_t timeout) const;
#endif
#if defined(VK_KHR_timeline_semaphore) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkWaitSemaphoresKHR(VkDevice device, const VkSemaphoreWaitInfoKHR* pWaitInfo, uint64_t timeout) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR VkResult VKAPI_CALL vkSignalSemaphore(VkDevice device, const VkSemaphoreSignalInfoKHR* pSignalInfo) const;
#endif
#if defined(VK_KHR_timeline_semaphore) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkSignalSemaphoreKHR(VkDevice device, const VkSemaphoreSignalInfoKHR* pSignalInfo) const;
#endif
#if defined(VK_ANDROID_external_memory_android_hardware_buffer) && (defined(VK_KHR_sampler_ycbcr_conversion) && defined(VK_KHR_external_memory) && defined(VK_EXT_queue_family_foreign) && defined(VK_KHR_dedicated_allocation))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetAndroidHardwareBufferPropertiesANDROID(VkDevice device, const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryAndroidHardwareBufferANDROID(VkDevice device, const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_KHR_draw_indirect_count)
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_AMD_draw_indirect_count)
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCount(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_KHR_draw_indirect_count)
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_AMD_draw_indirect_count)
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) const;
		VKAPI_ATTR void VKAPI_CALL vkGetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) const;
#endif
#if defined(VK_EXT_transform_feedback) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) const;
#endif
#if defined(VK_NV_scissor_exclusive) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetExclusiveScissorEnableNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkBool32* pExclusiveScissorEnables) const;
#endif
#if defined(VK_NV_shading_rate_image) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) const;
#endif
#if defined(VK_NV_mesh_shader) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_EXT_mesh_shader) && (defined(VK_KHR_spirv_1_4))
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksEXT(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountEXT(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		VKAPI_ATTR VkResult VKAPI_CALL vkCompileDeferredNV(VkDevice device, VkPipeline pipeline, uint32_t shader) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateAccelerationStructureNV(VkDevice device, const VkAccelerationStructureCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure) const;
#endif
#if defined(VK_HUAWEI_invocation_mask) && (defined(VK_KHR_ray_tracing_pipeline) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR void VKAPI_CALL vkCmdBindInvocationMaskHUAWEI(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) const;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		VKAPI_ATTR void VKAPI_CALL vkDestroyAccelerationStructureKHR(VkDevice device, VkAccelerationStructureKHR accelerationStructure, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		VKAPI_ATTR void VKAPI_CALL vkDestroyAccelerationStructureNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV(VkDevice device, const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkBindAccelerationStructureMemoryNV(VkDevice device, uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeKHR mode) const;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureInfoKHR* pInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureInfoKHR* pInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyAccelerationStructureToMemoryKHR(VkCommandBuffer commandBuffer, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyAccelerationStructureToMemoryKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyAccelerationStructureToMemoryInfoKHR* pInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryToAccelerationStructureKHR(VkCommandBuffer commandBuffer, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyMemoryToAccelerationStructureKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToAccelerationStructureInfoKHR* pInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesKHR(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		VKAPI_ATTR void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) const;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		VKAPI_ATTR VkResult VKAPI_CALL vkWriteAccelerationStructuresPropertiesKHR(VkDevice device, uint32_t accelerationStructureCount, const VkAccelerationStructureKHR* pAccelerationStructures, VkQueryType queryType, size_t dataSize, void* pData, size_t stride) const;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, uint32_t width, uint32_t height, uint32_t depth) const;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) const;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) const;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) const;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetAccelerationStructureHandleNV(VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateRayTracingPipelinesNV(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) const;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateRayTracingPipelinesKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) const;
#endif
#if defined(VK_NV_cooperative_matrix) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties) const;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysIndirectKHR(VkCommandBuffer commandBuffer, const VkStridedDeviceAddressRegionKHR* pRaygenShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pMissShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pHitShaderBindingTable, const VkStridedDeviceAddressRegionKHR* pCallableShaderBindingTable, VkDeviceAddress indirectDeviceAddress) const;
#endif
#if (defined(VK_KHR_ray_tracing_maintenance1) && (defined(VK_KHR_acceleration_structure))) && (defined(VK_KHR_ray_tracing_pipeline))
		VKAPI_ATTR void VKAPI_CALL vkCmdTraceRaysIndirect2KHR(VkCommandBuffer commandBuffer, VkDeviceAddress indirectDeviceAddress) const;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceAccelerationStructureCompatibilityKHR(VkDevice device, const VkAccelerationStructureVersionInfoKHR* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) const;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		VKAPI_ATTR VkDeviceSize VKAPI_CALL vkGetRayTracingShaderGroupStackSizeKHR(VkDevice device, VkPipeline pipeline, uint32_t group, VkShaderGroupShaderKHR groupShader) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetRayTracingPipelineStackSizeKHR(VkCommandBuffer commandBuffer, uint32_t pipelineStackSize) const;
#endif
#if defined(VK_NVX_image_view_handle)
		VKAPI_ATTR uint32_t VKAPI_CALL vkGetImageViewHandleNVX(VkDevice device, const VkImageViewHandleInfoNVX* pInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetImageViewAddressNVX(VkDevice device, VkImageView imageView, VkImageViewAddressPropertiesNVX* pProperties) const;
#endif
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModes2EXT(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const;
#endif
#if ((defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))) && (defined(VK_KHR_device_group))) || ((defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))) && (defined(VK_VERSION_1_1)))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModes2EXT(VkDevice device, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) const;
#endif
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquireFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkReleaseFullScreenExclusiveModeEXT(VkDevice device, VkSwapchainKHR swapchain) const;
#endif
#if defined(VK_KHR_performance_query) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, uint32_t* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions) const;
		VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, uint32_t* pNumPasses) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquireProfilingLockKHR(VkDevice device, const VkAcquireProfilingLockInfoKHR* pInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkReleaseProfilingLockKHR(VkDevice device) const;
#endif
#if defined(VK_EXT_image_drm_format_modifier) && (((defined(VK_KHR_bind_memory2) && defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_sampler_ycbcr_conversion)) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_image_format_list) || defined(VK_VERSION_1_2)))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetImageDrmFormatModifierPropertiesEXT(VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR uint64_t VKAPI_CALL vkGetBufferOpaqueCaptureAddress(VkDevice device, const VkBufferDeviceAddressInfoKHR* pInfo) const;
#endif
#if defined(VK_KHR_buffer_device_address) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_device_group)) || defined(VK_VERSION_1_1))
		VKAPI_ATTR uint64_t VKAPI_CALL vkGetBufferOpaqueCaptureAddressKHR(VkDevice device, const VkBufferDeviceAddressInfoKHR* pInfo) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddress(VkDevice device, const VkBufferDeviceAddressInfoKHR* pInfo) const;
#endif
#if defined(VK_KHR_buffer_device_address) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_device_group)) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressKHR(VkDevice device, const VkBufferDeviceAddressInfoKHR* pInfo) const;
#endif
#if defined(VK_EXT_buffer_device_address) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressEXT(VkDevice device, const VkBufferDeviceAddressInfoKHR* pInfo) const;
#endif
#if defined(VK_EXT_headless_surface) && (defined(VK_KHR_surface))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateHeadlessSurfaceEXT(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_NV_coverage_reduction_mode) && (defined(VK_NV_framebuffer_mixed_samples) && defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(VkPhysicalDevice physicalDevice, uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations) const;
#endif
#if defined(VK_INTEL_performance_query)
		VKAPI_ATTR VkResult VKAPI_CALL vkInitializePerformanceApiINTEL(VkDevice device, const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkUninitializePerformanceApiINTEL(VkDevice device) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquirePerformanceConfigurationINTEL(VkDevice device, const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkReleasePerformanceConfigurationINTEL(VkDevice device, VkPerformanceConfigurationINTEL configuration) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkQueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPerformanceParameterINTEL(VkDevice device, VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) const;
#endif
#if defined(VK_VERSION_1_2)
		VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceMemoryOpaqueCaptureAddress(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfoKHR* pInfo) const;
#endif
#if defined(VK_KHR_buffer_device_address) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_device_group)) || defined(VK_VERSION_1_1))
		VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceMemoryOpaqueCaptureAddressKHR(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfoKHR* pInfo) const;
#endif
#if defined(VK_KHR_pipeline_executable_properties) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutablePropertiesKHR(VkDevice device, const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutableStatisticsKHR(VkDevice device, const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineExecutableInternalRepresentationsKHR(VkDevice device, const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) const;
#endif
#if defined(VK_KHR_line_rasterization) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetLineStippleKHR(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) const;
#endif
#if defined(VK_EXT_line_rasterization) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceToolProperties(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolPropertiesEXT* pToolProperties) const;
#endif
#if defined(VK_EXT_tooling_info)
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceToolPropertiesEXT(VkPhysicalDevice physicalDevice, uint32_t* pToolCount, VkPhysicalDeviceToolPropertiesEXT* pToolProperties) const;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateAccelerationStructureKHR(VkDevice device, const VkAccelerationStructureCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkAccelerationStructureKHR* pAccelerationStructure) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBuildAccelerationStructuresKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBuildAccelerationStructuresIndirectKHR(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkDeviceAddress* pIndirectDeviceAddresses, const uint32_t* pIndirectStrides, const uint32_t* const* ppMaxPrimitiveCounts) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkBuildAccelerationStructuresKHR(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkAccelerationStructureBuildGeometryInfoKHR* pInfos, const VkAccelerationStructureBuildRangeInfoKHR* const* ppBuildRangeInfos) const;
		VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetAccelerationStructureDeviceAddressKHR(VkDevice device, const VkAccelerationStructureDeviceAddressInfoKHR* pInfo) const;
#endif
#if defined(VK_KHR_deferred_host_operations)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateDeferredOperationKHR(VkDevice device, const VkAllocationCallbacks* pAllocator, VkDeferredOperationKHR* pDeferredOperation) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyDeferredOperationKHR(VkDevice device, VkDeferredOperationKHR operation, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR uint32_t VKAPI_CALL vkGetDeferredOperationMaxConcurrencyKHR(VkDevice device, VkDeferredOperationKHR operation) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDeferredOperationResultKHR(VkDevice device, VkDeferredOperationKHR operation) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkDeferredOperationJoinKHR(VkDevice device, VkDeferredOperationKHR operation) const;
#endif
#if defined(VK_NV_device_generated_commands_compute) && (defined(VK_NV_device_generated_commands))
		VKAPI_ATTR void VKAPI_CALL vkGetPipelineIndirectMemoryRequirementsNV(VkDevice device, const VkComputePipelineCreateInfo* pCreateInfo, VkMemoryRequirements2* pMemoryRequirements) const;
		VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetPipelineIndirectDeviceAddressNV(VkDevice device, const VkPipelineIndirectDeviceAddressInfoNV* pInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCullMode(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCullModeEXT(VkCommandBuffer commandBuffer, VkCullModeFlags cullMode) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetFrontFace(VkCommandBuffer commandBuffer, VkFrontFace frontFace) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetFrontFaceEXT(VkCommandBuffer commandBuffer, VkFrontFace frontFace) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveTopology(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveTopologyEXT(VkCommandBuffer commandBuffer, VkPrimitiveTopology primitiveTopology) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWithCount(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWithCountEXT(VkCommandBuffer commandBuffer, uint32_t viewportCount, const VkViewport* pViewports) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetScissorWithCount(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetScissorWithCountEXT(VkCommandBuffer commandBuffer, uint32_t scissorCount, const VkRect2D* pScissors) const;
#endif
#if defined(VK_KHR_maintenance5) && (defined(VK_VERSION_1_1) && defined(VK_KHR_dynamic_rendering))
		VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer2KHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size, VkIndexType indexType) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers2(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers2EXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes, const VkDeviceSize* pStrides) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthTestEnable) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthWriteEnable(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthWriteEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthWriteEnable) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthCompareOp(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthCompareOpEXT(VkCommandBuffer commandBuffer, VkCompareOp depthCompareOp) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBoundsTestEnable(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBoundsTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBoundsTestEnable) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilTestEnable(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilTestEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stencilTestEnable) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilOp(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilOpEXT(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp compareOp) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetPatchControlPointsEXT(VkCommandBuffer commandBuffer, uint32_t patchControlPoints) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetRasterizerDiscardEnable(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetRasterizerDiscardEnableEXT(VkCommandBuffer commandBuffer, VkBool32 rasterizerDiscardEnable) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBiasEnable(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBiasEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthBiasEnable) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetLogicOpEXT(VkCommandBuffer commandBuffer, VkLogicOp logicOp) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveRestartEnable(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetPrimitiveRestartEnableEXT(VkCommandBuffer commandBuffer, VkBool32 primitiveRestartEnable) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetTessellationDomainOriginEXT(VkCommandBuffer commandBuffer, VkTessellationDomainOrigin domainOrigin) const;
#endif
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthClampEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClampEnable) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetPolygonModeEXT(VkCommandBuffer commandBuffer, VkPolygonMode polygonMode) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetRasterizationSamplesEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits rasterizationSamples) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetSampleMaskEXT(VkCommandBuffer commandBuffer, VkSampleCountFlagBits samples, const VkSampleMask* pSampleMask) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetAlphaToCoverageEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToCoverageEnable) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetAlphaToOneEnableEXT(VkCommandBuffer commandBuffer, VkBool32 alphaToOneEnable) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetLogicOpEnableEXT(VkCommandBuffer commandBuffer, VkBool32 logicOpEnable) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetColorBlendEnableEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkBool32* pColorBlendEnables) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetColorBlendEquationEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendEquationEXT* pColorBlendEquations) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetColorWriteMaskEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorComponentFlags* pColorWriteMasks) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_transform_feedback))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_transform_feedback)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetRasterizationStreamEXT(VkCommandBuffer commandBuffer, uint32_t rasterizationStream) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_conservative_rasterization))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_conservative_rasterization)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetConservativeRasterizationModeEXT(VkCommandBuffer commandBuffer, VkConservativeRasterizationModeEXT conservativeRasterizationMode) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetExtraPrimitiveOverestimationSizeEXT(VkCommandBuffer commandBuffer, float extraPrimitiveOverestimationSize) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_depth_clip_enable))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_depth_clip_enable)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthClipEnableEXT(VkCommandBuffer commandBuffer, VkBool32 depthClipEnable) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_sample_locations))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_sample_locations)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetSampleLocationsEnableEXT(VkCommandBuffer commandBuffer, VkBool32 sampleLocationsEnable) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_blend_operation_advanced))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_blend_operation_advanced)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetColorBlendAdvancedEXT(VkCommandBuffer commandBuffer, uint32_t firstAttachment, uint32_t attachmentCount, const VkColorBlendAdvancedEXT* pColorBlendAdvanced) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_provoking_vertex))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_provoking_vertex)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetProvokingVertexModeEXT(VkCommandBuffer commandBuffer, VkProvokingVertexModeEXT provokingVertexMode) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_line_rasterization))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_line_rasterization)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetLineRasterizationModeEXT(VkCommandBuffer commandBuffer, VkLineRasterizationModeEXT lineRasterizationMode) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetLineStippleEnableEXT(VkCommandBuffer commandBuffer, VkBool32 stippledLineEnable) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_depth_clip_control))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_depth_clip_control)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthClipNegativeOneToOneEXT(VkCommandBuffer commandBuffer, VkBool32 negativeOneToOne) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_clip_space_w_scaling))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_clip_space_w_scaling)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportWScalingEnableNV(VkCommandBuffer commandBuffer, VkBool32 viewportWScalingEnable) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_viewport_swizzle))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_viewport_swizzle)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetViewportSwizzleNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportSwizzleNV* pViewportSwizzles) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_fragment_coverage_to_color))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_fragment_coverage_to_color)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageToColorEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageToColorEnable) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageToColorLocationNV(VkCommandBuffer commandBuffer, uint32_t coverageToColorLocation) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_framebuffer_mixed_samples))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_framebuffer_mixed_samples)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageModulationModeNV(VkCommandBuffer commandBuffer, VkCoverageModulationModeNV coverageModulationMode) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageModulationTableEnableNV(VkCommandBuffer commandBuffer, VkBool32 coverageModulationTableEnable) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageModulationTableNV(VkCommandBuffer commandBuffer, uint32_t coverageModulationTableCount, const float* pCoverageModulationTable) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_shading_rate_image))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_shading_rate_image)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetShadingRateImageEnableNV(VkCommandBuffer commandBuffer, VkBool32 shadingRateImageEnable) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_coverage_reduction_mode))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_coverage_reduction_mode)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetCoverageReductionModeNV(VkCommandBuffer commandBuffer, VkCoverageReductionModeNV coverageReductionMode) const;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_representative_fragment_test))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_representative_fragment_test)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetRepresentativeFragmentTestEnableNV(VkCommandBuffer commandBuffer, VkBool32 representativeFragmentTestEnable) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreatePrivateDataSlot(VkDevice device, const VkPrivateDataSlotCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlotEXT* pPrivateDataSlot) const;
#endif
#if defined(VK_EXT_private_data) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreatePrivateDataSlotEXT(VkDevice device, const VkPrivateDataSlotCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPrivateDataSlotEXT* pPrivateDataSlot) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkDestroyPrivateDataSlot(VkDevice device, VkPrivateDataSlotEXT privateDataSlot, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_EXT_private_data) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkDestroyPrivateDataSlotEXT(VkDevice device, VkPrivateDataSlotEXT privateDataSlot, const VkAllocationCallbacks* pAllocator) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR VkResult VKAPI_CALL vkSetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlotEXT privateDataSlot, uint64_t data) const;
#endif
#if defined(VK_EXT_private_data) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkSetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlotEXT privateDataSlot, uint64_t data) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkGetPrivateData(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlotEXT privateDataSlot, uint64_t* pData) const;
#endif
#if defined(VK_EXT_private_data) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkGetPrivateDataEXT(VkDevice device, VkObjectType objectType, uint64_t objectHandle, VkPrivateDataSlotEXT privateDataSlot, uint64_t* pData) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer2(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2KHR* pCopyBufferInfo) const;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferInfo2KHR* pCopyBufferInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage2(VkCommandBuffer commandBuffer, const VkCopyImageInfo2KHR* pCopyImageInfo) const;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage2KHR(VkCommandBuffer commandBuffer, const VkCopyImageInfo2KHR* pCopyImageInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage2(VkCommandBuffer commandBuffer, const VkBlitImageInfo2KHR* pBlitImageInfo) const;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage2KHR(VkCommandBuffer commandBuffer, const VkBlitImageInfo2KHR* pBlitImageInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage2(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo) const;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage2KHR(VkCommandBuffer commandBuffer, const VkCopyBufferToImageInfo2KHR* pCopyBufferToImageInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer2(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo) const;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer2KHR(VkCommandBuffer commandBuffer, const VkCopyImageToBufferInfo2KHR* pCopyImageToBufferInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage2(VkCommandBuffer commandBuffer, const VkResolveImageInfo2KHR* pResolveImageInfo) const;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage2KHR(VkCommandBuffer commandBuffer, const VkResolveImageInfo2KHR* pResolveImageInfo) const;
#endif
#if defined(VK_KHR_object_refresh)
		VKAPI_ATTR void VKAPI_CALL vkCmdRefreshObjectsKHR(VkCommandBuffer commandBuffer, const VkRefreshObjectListKHR* pRefreshObjects) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceRefreshableObjectTypesKHR(VkPhysicalDevice physicalDevice, uint32_t* pRefreshableObjectTypeCount, VkObjectType* pRefreshableObjectTypes) const;
#endif
#if defined(VK_KHR_fragment_shading_rate) && ((defined(VK_KHR_create_renderpass2) || defined(VK_VERSION_1_2)) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetFragmentShadingRateKHR(VkCommandBuffer commandBuffer, const VkExtent2D* pFragmentSize, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceFragmentShadingRatesKHR(VkPhysicalDevice physicalDevice, uint32_t* pFragmentShadingRateCount, VkPhysicalDeviceFragmentShadingRateKHR* pFragmentShadingRates) const;
#endif
#if defined(VK_NV_fragment_shading_rate_enums) && (defined(VK_KHR_fragment_shading_rate))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetFragmentShadingRateEnumNV(VkCommandBuffer commandBuffer, VkFragmentShadingRateNV shadingRate, const VkFragmentShadingRateCombinerOpKHR combinerOps[2]) const;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		VKAPI_ATTR void VKAPI_CALL vkGetAccelerationStructureBuildSizesKHR(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, const VkAccelerationStructureBuildGeometryInfoKHR* pBuildInfo, const uint32_t* pMaxPrimitiveCounts, VkAccelerationStructureBuildSizesInfoKHR* pSizeInfo) const;
#endif
#if (defined(VK_EXT_vertex_input_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetVertexInputEXT(VkCommandBuffer commandBuffer, uint32_t vertexBindingDescriptionCount, const VkVertexInputBindingDescription2EXT* pVertexBindingDescriptions, uint32_t vertexAttributeDescriptionCount, const VkVertexInputAttributeDescription2EXT* pVertexAttributeDescriptions) const;
#endif
#if defined(VK_EXT_color_write_enable) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetColorWriteEnableEXT(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkBool32* pColorWriteEnables) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent2(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfoKHR* pDependencyInfo) const;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, const VkDependencyInfoKHR* pDependencyInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent2(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2KHR stageMask) const;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent2KHR(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags2KHR stageMask) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents2(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfoKHR* pDependencyInfos) const;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents2KHR(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, const VkDependencyInfoKHR* pDependencyInfos) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier2(VkCommandBuffer commandBuffer, const VkDependencyInfoKHR* pDependencyInfo) const;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier2KHR(VkCommandBuffer commandBuffer, const VkDependencyInfoKHR* pDependencyInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit2(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2KHR* pSubmits, VkFence fence) const;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit2KHR(VkQueue queue, uint32_t submitCount, const VkSubmitInfo2KHR* pSubmits, VkFence fence) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp2(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkQueryPool queryPool, uint32_t query) const;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp2KHR(VkCommandBuffer commandBuffer, VkPipelineStageFlags2KHR stage, VkQueryPool queryPool, uint32_t query) const;
#endif
#if (defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) && (defined(VK_AMD_buffer_marker))
		VKAPI_ATTR void VKAPI_CALL vkCmdWriteBufferMarker2AMD(VkCommandBuffer commandBuffer, VkPipelineStageFlags2 stage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const;
#endif
#if (defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) && (defined(VK_NV_device_diagnostic_checkpoints))
		VKAPI_ATTR void VKAPI_CALL vkGetQueueCheckpointData2NV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointData2NV* pCheckpointData) const;
#endif
#if defined(VK_EXT_host_image_copy) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_copy_commands2) && defined(VK_KHR_format_feature_flags2))
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyMemoryToImageEXT(VkDevice device, const VkCopyMemoryToImageInfoEXT* pCopyMemoryToImageInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyImageToMemoryEXT(VkDevice device, const VkCopyImageToMemoryInfoEXT* pCopyImageToMemoryInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyImageToImageEXT(VkDevice device, const VkCopyImageToImageInfoEXT* pCopyImageToImageInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkTransitionImageLayoutEXT(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfoEXT* pTransitions) const;
#endif
#if defined(VK_KHR_video_queue) && (defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceVideoCapabilitiesKHR(VkPhysicalDevice physicalDevice, const VkVideoProfileInfoKHR* pVideoProfile, VkVideoCapabilitiesKHR* pCapabilities) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceVideoFormatPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoFormatInfoKHR* pVideoFormatInfo, uint32_t* pVideoFormatPropertyCount, VkVideoFormatPropertiesKHR* pVideoFormatProperties) const;
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR* pQualityLevelInfo, VkVideoEncodeQualityLevelPropertiesKHR* pQualityLevelProperties) const;
#endif
#if defined(VK_KHR_video_queue) && (defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateVideoSessionKHR(VkDevice device, const VkVideoSessionCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionKHR* pVideoSession) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyVideoSessionKHR(VkDevice device, VkVideoSessionKHR videoSession, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateVideoSessionParametersKHR(VkDevice device, const VkVideoSessionParametersCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkVideoSessionParametersKHR* pVideoSessionParameters) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkUpdateVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkVideoSessionParametersUpdateInfoKHR* pUpdateInfo) const;
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetEncodedVideoSessionParametersKHR(VkDevice device, const VkVideoEncodeSessionParametersGetInfoKHR* pVideoSessionParametersInfo, VkVideoEncodeSessionParametersFeedbackInfoKHR* pFeedbackInfo, size_t* pDataSize, void* pData) const;
#endif
#if defined(VK_KHR_video_queue) && (defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR void VKAPI_CALL vkDestroyVideoSessionParametersKHR(VkDevice device, VkVideoSessionParametersKHR videoSessionParameters, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetVideoSessionMemoryRequirementsKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t* pMemoryRequirementsCount, VkVideoSessionMemoryRequirementsKHR* pMemoryRequirements) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkBindVideoSessionMemoryKHR(VkDevice device, VkVideoSessionKHR videoSession, uint32_t bindSessionMemoryInfoCount, const VkBindVideoSessionMemoryInfoKHR* pBindSessionMemoryInfos) const;
#endif
#if defined(VK_KHR_video_decode_queue) && (defined(VK_KHR_video_queue) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR void VKAPI_CALL vkCmdDecodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoDecodeInfoKHR* pDecodeInfo) const;
#endif
#if defined(VK_KHR_video_queue) && (defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoBeginCodingInfoKHR* pBeginInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdControlVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoCodingControlInfoKHR* pCodingControlInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdEndVideoCodingKHR(VkCommandBuffer commandBuffer, const VkVideoEndCodingInfoKHR* pEndCodingInfo) const;
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR void VKAPI_CALL vkCmdEncodeVideoKHR(VkCommandBuffer commandBuffer, const VkVideoEncodeInfoKHR* pEncodeInfo) const;
#endif
#if defined(VK_NV_memory_decompression) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_buffer_device_address))
		VKAPI_ATTR void VKAPI_CALL vkCmdDecompressMemoryNV(VkCommandBuffer commandBuffer, uint32_t decompressRegionCount, const VkDecompressMemoryRegionNV* pDecompressMemoryRegions) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDecompressMemoryIndirectCountNV(VkCommandBuffer commandBuffer, VkDeviceAddress indirectCommandsAddress, VkDeviceAddress indirectCommandsCountAddress, uint32_t stride) const;
#endif
#if defined(VK_NVX_binary_import)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateCuModuleNVX(VkDevice device, const VkCuModuleCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuModuleNVX* pModule) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateCuFunctionNVX(VkDevice device, const VkCuFunctionCreateInfoNVX* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCuFunctionNVX* pFunction) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyCuModuleNVX(VkDevice device, VkCuModuleNVX module, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyCuFunctionNVX(VkDevice device, VkCuFunctionNVX function, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCuLaunchKernelNVX(VkCommandBuffer commandBuffer, const VkCuLaunchInfoNVX* pLaunchInfo) const;
#endif
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_synchronization2) && defined(VK_EXT_descriptor_indexing))
		VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSizeEXT(VkDevice device, VkDescriptorSetLayout layout, VkDeviceSize* pLayoutSizeInBytes) const;
		VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutBindingOffsetEXT(VkDevice device, VkDescriptorSetLayout layout, uint32_t binding, VkDeviceSize* pOffset) const;
		VKAPI_ATTR void VKAPI_CALL vkGetDescriptorEXT(VkDevice device, const VkDescriptorGetInfoEXT* pDescriptorInfo, size_t dataSize, void* pDescriptor) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorBuffersEXT(VkCommandBuffer commandBuffer, uint32_t bufferCount, const VkDescriptorBufferBindingInfoEXT* pBindingInfos) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDescriptorBufferOffsetsEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t setCount, const uint32_t* pBufferIndices, const VkDeviceSize* pOffsets) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorBufferEmbeddedSamplersEXT(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetBufferOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkBufferCaptureDescriptorDataInfoEXT* pInfo, void* pData) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetImageOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageCaptureDescriptorDataInfoEXT* pInfo, void* pData) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetImageViewOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkImageViewCaptureDescriptorDataInfoEXT* pInfo, void* pData) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetSamplerOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkSamplerCaptureDescriptorDataInfoEXT* pInfo, void* pData) const;
#endif
#if (defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_synchronization2) && defined(VK_EXT_descriptor_indexing))) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT(VkDevice device, const VkAccelerationStructureCaptureDescriptorDataInfoEXT* pInfo, void* pData) const;
#endif
#if defined(VK_EXT_pageable_device_local_memory) && (defined(VK_EXT_memory_priority))
		VKAPI_ATTR void VKAPI_CALL vkSetDeviceMemoryPriorityEXT(VkDevice device, VkDeviceMemory memory, float priority) const;
#endif
#if defined(VK_EXT_acquire_drm_display) && (defined(VK_EXT_direct_mode_display))
		VKAPI_ATTR VkResult VKAPI_CALL vkAcquireDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, VkDisplayKHR display) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDrmDisplayEXT(VkPhysicalDevice physicalDevice, int32_t drmFd, uint32_t connectorId, VkDisplayKHR* display) const;
#endif
#if defined(VK_KHR_present_wait) && (defined(VK_KHR_swapchain) && defined(VK_KHR_present_id))
		VKAPI_ATTR VkResult VKAPI_CALL vkWaitForPresentKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t presentId, uint64_t timeout) const;
#endif
#if defined(VK_FUCHSIA_buffer_collection) && (defined(VK_FUCHSIA_external_memory) && defined(VK_KHR_sampler_ycbcr_conversion))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateBufferCollectionFUCHSIA(VkDevice device, const VkBufferCollectionCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferCollectionFUCHSIA* pCollection) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkSetBufferCollectionBufferConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkBufferConstraintsInfoFUCHSIA* pBufferConstraintsInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkSetBufferCollectionImageConstraintsFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkImageConstraintsInfoFUCHSIA* pImageConstraintsInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyBufferCollectionFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetBufferCollectionPropertiesFUCHSIA(VkDevice device, VkBufferCollectionFUCHSIA collection, VkBufferCollectionPropertiesFUCHSIA* pProperties) const;
#endif
#if defined(VK_NV_cuda_kernel_launch)
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateCudaModuleNV(VkDevice device, const VkCudaModuleCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaModuleNV* pModule) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetCudaModuleCacheNV(VkDevice device, VkCudaModuleNV module, size_t* pCacheSize, void* pCacheData) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateCudaFunctionNV(VkDevice device, const VkCudaFunctionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCudaFunctionNV* pFunction) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyCudaModuleNV(VkDevice device, VkCudaModuleNV module, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyCudaFunctionNV(VkDevice device, VkCudaFunctionNV function, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCudaLaunchKernelNV(VkCommandBuffer commandBuffer, const VkCudaLaunchInfoNV* pLaunchInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginRendering(VkCommandBuffer commandBuffer, const VkRenderingInfoKHR* pRenderingInfo) const;
#endif
#if defined(VK_KHR_dynamic_rendering) && (defined(VK_KHR_depth_stencil_resolve) && defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderingKHR(VkCommandBuffer commandBuffer, const VkRenderingInfoKHR* pRenderingInfo) const;
#endif
#if defined(VK_VERSION_1_3)
		VKAPI_ATTR void VKAPI_CALL vkCmdEndRendering(VkCommandBuffer commandBuffer) const;
#endif
#if defined(VK_KHR_dynamic_rendering) && (defined(VK_KHR_depth_stencil_resolve) && defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderingKHR(VkCommandBuffer commandBuffer) const;
#endif
#if defined(VK_VALVE_descriptor_set_host_mapping) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutHostMappingInfoVALVE(VkDevice device, const VkDescriptorSetBindingReferenceVALVE* pBindingReference, VkDescriptorSetLayoutHostMappingInfoVALVE* pHostMapping) const;
		VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetHostMappingVALVE(VkDevice device, VkDescriptorSet descriptorSet, void** ppData) const;
#endif
#if defined(VK_EXT_opacity_micromap) && (defined(VK_KHR_acceleration_structure) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateMicromapEXT(VkDevice device, const VkMicromapCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkMicromapEXT* pMicromap) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBuildMicromapsEXT(VkCommandBuffer commandBuffer, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkBuildMicromapsEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, uint32_t infoCount, const VkMicromapBuildInfoEXT* pInfos) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyMicromapEXT(VkDevice device, VkMicromapEXT micromap, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapInfoEXT* pInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapInfoEXT* pInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyMicromapToMemoryEXT(VkCommandBuffer commandBuffer, const VkCopyMicromapToMemoryInfoEXT* pInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyMicromapToMemoryEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMicromapToMemoryInfoEXT* pInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdCopyMemoryToMicromapEXT(VkCommandBuffer commandBuffer, const VkCopyMemoryToMicromapInfoEXT* pInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCopyMemoryToMicromapEXT(VkDevice device, VkDeferredOperationKHR deferredOperation, const VkCopyMemoryToMicromapInfoEXT* pInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdWriteMicromapsPropertiesEXT(VkCommandBuffer commandBuffer, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkWriteMicromapsPropertiesEXT(VkDevice device, uint32_t micromapCount, const VkMicromapEXT* pMicromaps, VkQueryType queryType, size_t dataSize, void* pData, size_t stride) const;
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceMicromapCompatibilityEXT(VkDevice device, const VkMicromapVersionInfoEXT* pVersionInfo, VkAccelerationStructureCompatibilityKHR* pCompatibility) const;
		VKAPI_ATTR void VKAPI_CALL vkGetMicromapBuildSizesEXT(VkDevice device, VkAccelerationStructureBuildTypeKHR buildType, const VkMicromapBuildInfoEXT* pBuildInfo, VkMicromapBuildSizesInfoEXT* pSizeInfo) const;
#endif
#if defined(VK_EXT_shader_module_identifier) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_EXT_pipeline_creation_cache_control))
		VKAPI_ATTR void VKAPI_CALL vkGetShaderModuleIdentifierEXT(VkDevice device, VkShaderModule shaderModule, VkShaderModuleIdentifierEXT* pIdentifier) const;
		VKAPI_ATTR void VKAPI_CALL vkGetShaderModuleCreateInfoIdentifierEXT(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModuleIdentifierEXT* pIdentifier) const;
#endif
#if defined(VK_KHR_maintenance5) && (defined(VK_VERSION_1_1) && defined(VK_KHR_dynamic_rendering))
		VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout2KHR(VkDevice device, VkImage image, const VkImageSubresource2EXT* pSubresource, VkSubresourceLayout2EXT* pLayout) const;
#endif
#if (defined(VK_EXT_host_image_copy) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_copy_commands2) && defined(VK_KHR_format_feature_flags2))) || (defined(VK_EXT_image_compression_control) && (defined(VK_KHR_get_physical_device_properties2)))
		VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout2EXT(VkDevice device, VkImage image, const VkImageSubresource2EXT* pSubresource, VkSubresourceLayout2EXT* pLayout) const;
#endif
#if defined(VK_EXT_pipeline_properties) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelinePropertiesEXT(VkDevice device, const VkPipelineInfoEXT* pPipelineInfo, VkBaseOutStructure* pPipelineProperties) const;
#endif
#if defined(VK_EXT_metal_objects)
		VKAPI_ATTR void VKAPI_CALL vkExportMetalObjectsEXT(VkDevice device, VkExportMetalObjectsInfoEXT* pMetalObjectsInfo) const;
#endif
#if defined(VK_QCOM_tile_properties) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetFramebufferTilePropertiesQCOM(VkDevice device, VkFramebuffer framebuffer, uint32_t* pPropertiesCount, VkTilePropertiesQCOM* pProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDynamicRenderingTilePropertiesQCOM(VkDevice device, const VkRenderingInfo* pRenderingInfo, VkTilePropertiesQCOM* pProperties) const;
#endif
#if defined(VK_NV_optical_flow) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_format_feature_flags2) && defined(VK_KHR_synchronization2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceOpticalFlowImageFormatsNV(VkPhysicalDevice physicalDevice, const VkOpticalFlowImageFormatInfoNV* pOpticalFlowImageFormatInfo, uint32_t* pFormatCount, VkOpticalFlowImageFormatPropertiesNV* pImageFormatProperties) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateOpticalFlowSessionNV(VkDevice device, const VkOpticalFlowSessionCreateInfoNV* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkOpticalFlowSessionNV* pSession) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyOpticalFlowSessionNV(VkDevice device, VkOpticalFlowSessionNV session, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkBindOpticalFlowSessionImageNV(VkDevice device, VkOpticalFlowSessionNV session, VkOpticalFlowSessionBindingPointNV bindingPoint, VkImageView view, VkImageLayout layout) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdOpticalFlowExecuteNV(VkCommandBuffer commandBuffer, VkOpticalFlowSessionNV session, const VkOpticalFlowExecuteInfoNV* pExecuteInfo) const;
#endif
#if defined(VK_EXT_device_fault) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetDeviceFaultInfoEXT(VkDevice device, VkDeviceFaultCountsEXT* pFaultCounts, VkDeviceFaultInfoEXT* pFaultInfo) const;
#endif
#if defined(VK_EXT_depth_bias_control) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBias2EXT(VkCommandBuffer commandBuffer, const VkDepthBiasInfoEXT* pDepthBiasInfo) const;
#endif
#if defined(VK_EXT_swapchain_maintenance1) && (defined(VK_KHR_swapchain) && defined(VK_EXT_surface_maintenance1) && defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkReleaseSwapchainImagesEXT(VkDevice device, const VkReleaseSwapchainImagesInfoEXT* pReleaseInfo) const;
#endif
#if defined(VK_KHR_maintenance5) && (defined(VK_VERSION_1_1) && defined(VK_KHR_dynamic_rendering))
		VKAPI_ATTR void VKAPI_CALL vkGetDeviceImageSubresourceLayoutKHR(VkDevice device, const VkDeviceImageSubresourceInfoKHR* pInfo, VkSubresourceLayout2KHR* pLayout) const;
#endif
#if defined(VK_KHR_map_memory2)
		VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory2KHR(VkDevice device, const VkMemoryMapInfoKHR* pMemoryMapInfo, void** ppData) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkUnmapMemory2KHR(VkDevice device, const VkMemoryUnmapInfoKHR* pMemoryUnmapInfo) const;
#endif
#if defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateShadersEXT(VkDevice device, uint32_t createInfoCount, const VkShaderCreateInfoEXT* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkShaderEXT* pShaders) const;
		VKAPI_ATTR void VKAPI_CALL vkDestroyShaderEXT(VkDevice device, VkShaderEXT shader, const VkAllocationCallbacks* pAllocator) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetShaderBinaryDataEXT(VkDevice device, VkShaderEXT shader, size_t* pDataSize, void* pData) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindShadersEXT(VkCommandBuffer commandBuffer, uint32_t stageCount, const VkShaderStageFlagBits* pStages, const VkShaderEXT* pShaders) const;
#endif
#if defined(VK_QNX_external_memory_screen_buffer) && (((defined(VK_KHR_sampler_ycbcr_conversion) && defined(VK_KHR_external_memory) && defined(VK_KHR_dedicated_allocation)) || defined(VK_VERSION_1_1)) && defined(VK_EXT_queue_family_foreign))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetScreenBufferPropertiesQNX(VkDevice device, const struct _screen_buffer* buffer, VkScreenBufferPropertiesQNX* pProperties) const;
#endif
#if defined(VK_KHR_cooperative_matrix) && (defined(VK_KHR_get_physical_device_properties2))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesKHR* pProperties) const;
#endif
#if defined(VK_AMDX_shader_enqueue) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_synchronization2) && defined(VK_KHR_pipeline_library) && defined(VK_KHR_spirv_1_4))
		VKAPI_ATTR VkResult VKAPI_CALL vkGetExecutionGraphPipelineScratchSizeAMDX(VkDevice device, VkPipeline executionGraph, VkExecutionGraphPipelineScratchSizeAMDX* pSizeInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkGetExecutionGraphPipelineNodeIndexAMDX(VkDevice device, VkPipeline executionGraph, const VkPipelineShaderStageNodeCreateInfoAMDX* pNodeInfo, uint32_t* pNodeIndex) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkCreateExecutionGraphPipelinesAMDX(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkExecutionGraphPipelineCreateInfoAMDX* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdInitializeGraphScratchMemoryAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDispatchGraphAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, const VkDispatchGraphCountInfoAMDX* pCountInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDispatchGraphIndirectAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, const VkDispatchGraphCountInfoAMDX* pCountInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdDispatchGraphIndirectCountAMDX(VkCommandBuffer commandBuffer, VkDeviceAddress scratch, VkDeviceAddress countInfo) const;
#endif
#if defined(VK_KHR_maintenance6) && (defined(VK_VERSION_1_1))
		VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets2KHR(VkCommandBuffer commandBuffer, const VkBindDescriptorSetsInfoKHR* pBindDescriptorSetsInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants2KHR(VkCommandBuffer commandBuffer, const VkPushConstantsInfoKHR* pPushConstantsInfo) const;
#endif
#if (defined(VK_KHR_maintenance6) && (defined(VK_VERSION_1_1))) && (defined(VK_KHR_push_descriptor))
		VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSet2KHR(VkCommandBuffer commandBuffer, const VkPushDescriptorSetInfoKHR* pPushDescriptorSetInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdPushDescriptorSetWithTemplate2KHR(VkCommandBuffer commandBuffer, const VkPushDescriptorSetWithTemplateInfoKHR* pPushDescriptorSetWithTemplateInfo) const;
#endif
#if (defined(VK_KHR_maintenance6) && (defined(VK_VERSION_1_1))) && (defined(VK_EXT_descriptor_buffer))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetDescriptorBufferOffsets2EXT(VkCommandBuffer commandBuffer, const VkSetDescriptorBufferOffsetsInfoEXT* pSetDescriptorBufferOffsetsInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorBufferEmbeddedSamplers2EXT(VkCommandBuffer commandBuffer, const VkBindDescriptorBufferEmbeddedSamplersInfoEXT* pBindDescriptorBufferEmbeddedSamplersInfo) const;
#endif
#if defined(VK_NV_low_latency2) && (defined(VK_VERSION_1_2) || defined(VK_KHR_timeline_semaphore))
		VKAPI_ATTR VkResult VKAPI_CALL vkSetLatencySleepModeNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepModeInfoNV* pSleepModeInfo) const;
		VKAPI_ATTR VkResult VKAPI_CALL vkLatencySleepNV(VkDevice device, VkSwapchainKHR swapchain, const VkLatencySleepInfoNV* pSleepInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkSetLatencyMarkerNV(VkDevice device, VkSwapchainKHR swapchain, const VkSetLatencyMarkerInfoNV* pLatencyMarkerInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkGetLatencyTimingsNV(VkDevice device, VkSwapchainKHR swapchain, VkGetLatencyMarkerInfoNV* pLatencyMarkerInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkQueueNotifyOutOfBandNV(VkQueue queue, const VkOutOfBandQueueTypeInfoNV* pQueueTypeInfo) const;
#endif
#if defined(VK_KHR_dynamic_rendering_local_read) && (defined(VK_KHR_dynamic_rendering))
		VKAPI_ATTR void VKAPI_CALL vkCmdSetRenderingAttachmentLocationsKHR(VkCommandBuffer commandBuffer, const VkRenderingAttachmentLocationInfoKHR* pLocationInfo) const;
		VKAPI_ATTR void VKAPI_CALL vkCmdSetRenderingInputAttachmentIndicesKHR(VkCommandBuffer commandBuffer, const VkRenderingInputAttachmentIndexInfoKHR* pLocationInfo) const;
#endif
	private:
		// Vulkan function pointers
#if defined(VK_VERSION_1_0)
		PFN_vkCreateInstance pfn_vkCreateInstance;
		PFN_vkDestroyInstance pfn_vkDestroyInstance;
		PFN_vkEnumeratePhysicalDevices pfn_vkEnumeratePhysicalDevices;
		PFN_vkGetDeviceProcAddr pfn_vkGetDeviceProcAddr;
		PFN_vkGetInstanceProcAddr pfn_vkGetInstanceProcAddr;
		PFN_vkGetPhysicalDeviceProperties pfn_vkGetPhysicalDeviceProperties;
		PFN_vkGetPhysicalDeviceQueueFamilyProperties pfn_vkGetPhysicalDeviceQueueFamilyProperties;
		PFN_vkGetPhysicalDeviceMemoryProperties pfn_vkGetPhysicalDeviceMemoryProperties;
		PFN_vkGetPhysicalDeviceFeatures pfn_vkGetPhysicalDeviceFeatures;
		PFN_vkGetPhysicalDeviceFormatProperties pfn_vkGetPhysicalDeviceFormatProperties;
		PFN_vkGetPhysicalDeviceImageFormatProperties pfn_vkGetPhysicalDeviceImageFormatProperties;
		PFN_vkCreateDevice pfn_vkCreateDevice;
		PFN_vkDestroyDevice pfn_vkDestroyDevice;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkEnumerateInstanceVersion pfn_vkEnumerateInstanceVersion;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkEnumerateInstanceLayerProperties pfn_vkEnumerateInstanceLayerProperties;
		PFN_vkEnumerateInstanceExtensionProperties pfn_vkEnumerateInstanceExtensionProperties;
		PFN_vkEnumerateDeviceLayerProperties pfn_vkEnumerateDeviceLayerProperties;
		PFN_vkEnumerateDeviceExtensionProperties pfn_vkEnumerateDeviceExtensionProperties;
		PFN_vkGetDeviceQueue pfn_vkGetDeviceQueue;
		PFN_vkQueueSubmit pfn_vkQueueSubmit;
		PFN_vkQueueWaitIdle pfn_vkQueueWaitIdle;
		PFN_vkDeviceWaitIdle pfn_vkDeviceWaitIdle;
		PFN_vkAllocateMemory pfn_vkAllocateMemory;
		PFN_vkFreeMemory pfn_vkFreeMemory;
		PFN_vkMapMemory pfn_vkMapMemory;
		PFN_vkUnmapMemory pfn_vkUnmapMemory;
		PFN_vkFlushMappedMemoryRanges pfn_vkFlushMappedMemoryRanges;
		PFN_vkInvalidateMappedMemoryRanges pfn_vkInvalidateMappedMemoryRanges;
		PFN_vkGetDeviceMemoryCommitment pfn_vkGetDeviceMemoryCommitment;
		PFN_vkGetBufferMemoryRequirements pfn_vkGetBufferMemoryRequirements;
		PFN_vkBindBufferMemory pfn_vkBindBufferMemory;
		PFN_vkGetImageMemoryRequirements pfn_vkGetImageMemoryRequirements;
		PFN_vkBindImageMemory pfn_vkBindImageMemory;
		PFN_vkGetImageSparseMemoryRequirements pfn_vkGetImageSparseMemoryRequirements;
		PFN_vkGetPhysicalDeviceSparseImageFormatProperties pfn_vkGetPhysicalDeviceSparseImageFormatProperties;
		PFN_vkQueueBindSparse pfn_vkQueueBindSparse;
		PFN_vkCreateFence pfn_vkCreateFence;
		PFN_vkDestroyFence pfn_vkDestroyFence;
		PFN_vkResetFences pfn_vkResetFences;
		PFN_vkGetFenceStatus pfn_vkGetFenceStatus;
		PFN_vkWaitForFences pfn_vkWaitForFences;
		PFN_vkCreateSemaphore pfn_vkCreateSemaphore;
		PFN_vkDestroySemaphore pfn_vkDestroySemaphore;
		PFN_vkCreateEvent pfn_vkCreateEvent;
		PFN_vkDestroyEvent pfn_vkDestroyEvent;
		PFN_vkGetEventStatus pfn_vkGetEventStatus;
		PFN_vkSetEvent pfn_vkSetEvent;
		PFN_vkResetEvent pfn_vkResetEvent;
		PFN_vkCreateQueryPool pfn_vkCreateQueryPool;
		PFN_vkDestroyQueryPool pfn_vkDestroyQueryPool;
		PFN_vkGetQueryPoolResults pfn_vkGetQueryPoolResults;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkResetQueryPool pfn_vkResetQueryPool;
#endif
#if defined(VK_EXT_host_query_reset) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkResetQueryPoolEXT pfn_vkResetQueryPoolEXT;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkCreateBuffer pfn_vkCreateBuffer;
		PFN_vkDestroyBuffer pfn_vkDestroyBuffer;
		PFN_vkCreateBufferView pfn_vkCreateBufferView;
		PFN_vkDestroyBufferView pfn_vkDestroyBufferView;
		PFN_vkCreateImage pfn_vkCreateImage;
		PFN_vkDestroyImage pfn_vkDestroyImage;
		PFN_vkGetImageSubresourceLayout pfn_vkGetImageSubresourceLayout;
		PFN_vkCreateImageView pfn_vkCreateImageView;
		PFN_vkDestroyImageView pfn_vkDestroyImageView;
		PFN_vkCreateShaderModule pfn_vkCreateShaderModule;
		PFN_vkDestroyShaderModule pfn_vkDestroyShaderModule;
		PFN_vkCreatePipelineCache pfn_vkCreatePipelineCache;
		PFN_vkDestroyPipelineCache pfn_vkDestroyPipelineCache;
		PFN_vkGetPipelineCacheData pfn_vkGetPipelineCacheData;
		PFN_vkMergePipelineCaches pfn_vkMergePipelineCaches;
		PFN_vkCreateGraphicsPipelines pfn_vkCreateGraphicsPipelines;
		PFN_vkCreateComputePipelines pfn_vkCreateComputePipelines;
#endif
#if defined(VK_HUAWEI_subpass_shading) && (defined(VK_KHR_create_renderpass2) && defined(VK_KHR_synchronization2))
		PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI pfn_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkDestroyPipeline pfn_vkDestroyPipeline;
		PFN_vkCreatePipelineLayout pfn_vkCreatePipelineLayout;
		PFN_vkDestroyPipelineLayout pfn_vkDestroyPipelineLayout;
		PFN_vkCreateSampler pfn_vkCreateSampler;
		PFN_vkDestroySampler pfn_vkDestroySampler;
		PFN_vkCreateDescriptorSetLayout pfn_vkCreateDescriptorSetLayout;
		PFN_vkDestroyDescriptorSetLayout pfn_vkDestroyDescriptorSetLayout;
		PFN_vkCreateDescriptorPool pfn_vkCreateDescriptorPool;
		PFN_vkDestroyDescriptorPool pfn_vkDestroyDescriptorPool;
		PFN_vkResetDescriptorPool pfn_vkResetDescriptorPool;
		PFN_vkAllocateDescriptorSets pfn_vkAllocateDescriptorSets;
		PFN_vkFreeDescriptorSets pfn_vkFreeDescriptorSets;
		PFN_vkUpdateDescriptorSets pfn_vkUpdateDescriptorSets;
		PFN_vkCreateFramebuffer pfn_vkCreateFramebuffer;
		PFN_vkDestroyFramebuffer pfn_vkDestroyFramebuffer;
		PFN_vkCreateRenderPass pfn_vkCreateRenderPass;
		PFN_vkDestroyRenderPass pfn_vkDestroyRenderPass;
		PFN_vkGetRenderAreaGranularity pfn_vkGetRenderAreaGranularity;
#endif
#if defined(VK_KHR_maintenance5) && (defined(VK_VERSION_1_1) && defined(VK_KHR_dynamic_rendering))
		PFN_vkGetRenderingAreaGranularityKHR pfn_vkGetRenderingAreaGranularityKHR;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkCreateCommandPool pfn_vkCreateCommandPool;
		PFN_vkDestroyCommandPool pfn_vkDestroyCommandPool;
		PFN_vkResetCommandPool pfn_vkResetCommandPool;
		PFN_vkAllocateCommandBuffers pfn_vkAllocateCommandBuffers;
		PFN_vkFreeCommandBuffers pfn_vkFreeCommandBuffers;
		PFN_vkBeginCommandBuffer pfn_vkBeginCommandBuffer;
		PFN_vkEndCommandBuffer pfn_vkEndCommandBuffer;
		PFN_vkResetCommandBuffer pfn_vkResetCommandBuffer;
		PFN_vkCmdBindPipeline pfn_vkCmdBindPipeline;
#endif
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_EXT_attachment_feedback_loop_layout))
		PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT pfn_vkCmdSetAttachmentFeedbackLoopEnableEXT;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkCmdSetViewport pfn_vkCmdSetViewport;
		PFN_vkCmdSetScissor pfn_vkCmdSetScissor;
		PFN_vkCmdSetLineWidth pfn_vkCmdSetLineWidth;
		PFN_vkCmdSetDepthBias pfn_vkCmdSetDepthBias;
		PFN_vkCmdSetBlendConstants pfn_vkCmdSetBlendConstants;
		PFN_vkCmdSetDepthBounds pfn_vkCmdSetDepthBounds;
		PFN_vkCmdSetStencilCompareMask pfn_vkCmdSetStencilCompareMask;
		PFN_vkCmdSetStencilWriteMask pfn_vkCmdSetStencilWriteMask;
		PFN_vkCmdSetStencilReference pfn_vkCmdSetStencilReference;
		PFN_vkCmdBindDescriptorSets pfn_vkCmdBindDescriptorSets;
		PFN_vkCmdBindIndexBuffer pfn_vkCmdBindIndexBuffer;
		PFN_vkCmdBindVertexBuffers pfn_vkCmdBindVertexBuffers;
		PFN_vkCmdDraw pfn_vkCmdDraw;
		PFN_vkCmdDrawIndexed pfn_vkCmdDrawIndexed;
#endif
#if defined(VK_EXT_multi_draw) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdDrawMultiEXT pfn_vkCmdDrawMultiEXT;
		PFN_vkCmdDrawMultiIndexedEXT pfn_vkCmdDrawMultiIndexedEXT;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkCmdDrawIndirect pfn_vkCmdDrawIndirect;
		PFN_vkCmdDrawIndexedIndirect pfn_vkCmdDrawIndexedIndirect;
		PFN_vkCmdDispatch pfn_vkCmdDispatch;
		PFN_vkCmdDispatchIndirect pfn_vkCmdDispatchIndirect;
#endif
#if defined(VK_HUAWEI_subpass_shading) && (defined(VK_KHR_create_renderpass2) && defined(VK_KHR_synchronization2))
		PFN_vkCmdSubpassShadingHUAWEI pfn_vkCmdSubpassShadingHUAWEI;
#endif
#if defined(VK_HUAWEI_cluster_culling_shader) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdDrawClusterHUAWEI pfn_vkCmdDrawClusterHUAWEI;
		PFN_vkCmdDrawClusterIndirectHUAWEI pfn_vkCmdDrawClusterIndirectHUAWEI;
#endif
#if defined(VK_NV_device_generated_commands_compute) && (defined(VK_NV_device_generated_commands))
		PFN_vkCmdUpdatePipelineIndirectBufferNV pfn_vkCmdUpdatePipelineIndirectBufferNV;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkCmdCopyBuffer pfn_vkCmdCopyBuffer;
		PFN_vkCmdCopyImage pfn_vkCmdCopyImage;
		PFN_vkCmdBlitImage pfn_vkCmdBlitImage;
		PFN_vkCmdCopyBufferToImage pfn_vkCmdCopyBufferToImage;
		PFN_vkCmdCopyImageToBuffer pfn_vkCmdCopyImageToBuffer;
#endif
#if defined(VK_NV_copy_memory_indirect) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_buffer_device_address))
		PFN_vkCmdCopyMemoryIndirectNV pfn_vkCmdCopyMemoryIndirectNV;
		PFN_vkCmdCopyMemoryToImageIndirectNV pfn_vkCmdCopyMemoryToImageIndirectNV;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkCmdUpdateBuffer pfn_vkCmdUpdateBuffer;
		PFN_vkCmdFillBuffer pfn_vkCmdFillBuffer;
		PFN_vkCmdClearColorImage pfn_vkCmdClearColorImage;
		PFN_vkCmdClearDepthStencilImage pfn_vkCmdClearDepthStencilImage;
		PFN_vkCmdClearAttachments pfn_vkCmdClearAttachments;
		PFN_vkCmdResolveImage pfn_vkCmdResolveImage;
		PFN_vkCmdSetEvent pfn_vkCmdSetEvent;
		PFN_vkCmdResetEvent pfn_vkCmdResetEvent;
		PFN_vkCmdWaitEvents pfn_vkCmdWaitEvents;
		PFN_vkCmdPipelineBarrier pfn_vkCmdPipelineBarrier;
		PFN_vkCmdBeginQuery pfn_vkCmdBeginQuery;
		PFN_vkCmdEndQuery pfn_vkCmdEndQuery;
#endif
#if defined(VK_EXT_conditional_rendering) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdBeginConditionalRenderingEXT pfn_vkCmdBeginConditionalRenderingEXT;
		PFN_vkCmdEndConditionalRenderingEXT pfn_vkCmdEndConditionalRenderingEXT;
#endif
#if defined(VK_VERSION_1_0)
		PFN_vkCmdResetQueryPool pfn_vkCmdResetQueryPool;
		PFN_vkCmdWriteTimestamp pfn_vkCmdWriteTimestamp;
		PFN_vkCmdCopyQueryPoolResults pfn_vkCmdCopyQueryPoolResults;
		PFN_vkCmdPushConstants pfn_vkCmdPushConstants;
		PFN_vkCmdBeginRenderPass pfn_vkCmdBeginRenderPass;
		PFN_vkCmdNextSubpass pfn_vkCmdNextSubpass;
		PFN_vkCmdEndRenderPass pfn_vkCmdEndRenderPass;
		PFN_vkCmdExecuteCommands pfn_vkCmdExecuteCommands;
#endif
#if defined(VK_KHR_android_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateAndroidSurfaceKHR pfn_vkCreateAndroidSurfaceKHR;
#endif
#if defined(VK_KHR_display) && (defined(VK_KHR_surface))
		PFN_vkGetPhysicalDeviceDisplayPropertiesKHR pfn_vkGetPhysicalDeviceDisplayPropertiesKHR;
		PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR pfn_vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
		PFN_vkGetDisplayPlaneSupportedDisplaysKHR pfn_vkGetDisplayPlaneSupportedDisplaysKHR;
		PFN_vkGetDisplayModePropertiesKHR pfn_vkGetDisplayModePropertiesKHR;
		PFN_vkCreateDisplayModeKHR pfn_vkCreateDisplayModeKHR;
		PFN_vkGetDisplayPlaneCapabilitiesKHR pfn_vkGetDisplayPlaneCapabilitiesKHR;
		PFN_vkCreateDisplayPlaneSurfaceKHR pfn_vkCreateDisplayPlaneSurfaceKHR;
#endif
#if defined(VK_KHR_display_swapchain) && (defined(VK_KHR_swapchain) && defined(VK_KHR_display))
		PFN_vkCreateSharedSwapchainsKHR pfn_vkCreateSharedSwapchainsKHR;
#endif
#if defined(VK_KHR_surface)
		PFN_vkDestroySurfaceKHR pfn_vkDestroySurfaceKHR;
		PFN_vkGetPhysicalDeviceSurfaceSupportKHR pfn_vkGetPhysicalDeviceSurfaceSupportKHR;
		PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR pfn_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
		PFN_vkGetPhysicalDeviceSurfaceFormatsKHR pfn_vkGetPhysicalDeviceSurfaceFormatsKHR;
		PFN_vkGetPhysicalDeviceSurfacePresentModesKHR pfn_vkGetPhysicalDeviceSurfacePresentModesKHR;
#endif
#if defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))
		PFN_vkCreateSwapchainKHR pfn_vkCreateSwapchainKHR;
		PFN_vkDestroySwapchainKHR pfn_vkDestroySwapchainKHR;
		PFN_vkGetSwapchainImagesKHR pfn_vkGetSwapchainImagesKHR;
		PFN_vkAcquireNextImageKHR pfn_vkAcquireNextImageKHR;
		PFN_vkQueuePresentKHR pfn_vkQueuePresentKHR;
#endif
#if defined(VK_NN_vi_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateViSurfaceNN pfn_vkCreateViSurfaceNN;
#endif
#if defined(VK_KHR_wayland_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateWaylandSurfaceKHR pfn_vkCreateWaylandSurfaceKHR;
		PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR pfn_vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif
#if defined(VK_KHR_win32_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateWin32SurfaceKHR pfn_vkCreateWin32SurfaceKHR;
		PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR pfn_vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif
#if defined(VK_KHR_xlib_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateXlibSurfaceKHR pfn_vkCreateXlibSurfaceKHR;
		PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR pfn_vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif
#if defined(VK_KHR_xcb_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateXcbSurfaceKHR pfn_vkCreateXcbSurfaceKHR;
		PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR pfn_vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif
#if defined(VK_EXT_directfb_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateDirectFBSurfaceEXT pfn_vkCreateDirectFBSurfaceEXT;
		PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT pfn_vkGetPhysicalDeviceDirectFBPresentationSupportEXT;
#endif
#if defined(VK_FUCHSIA_imagepipe_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateImagePipeSurfaceFUCHSIA pfn_vkCreateImagePipeSurfaceFUCHSIA;
#endif
#if defined(VK_GGP_stream_descriptor_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateStreamDescriptorSurfaceGGP pfn_vkCreateStreamDescriptorSurfaceGGP;
#endif
#if defined(VK_QNX_screen_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateScreenSurfaceQNX pfn_vkCreateScreenSurfaceQNX;
		PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX pfn_vkGetPhysicalDeviceScreenPresentationSupportQNX;
#endif
#if defined(VK_EXT_debug_report)
		PFN_vkCreateDebugReportCallbackEXT pfn_vkCreateDebugReportCallbackEXT;
		PFN_vkDestroyDebugReportCallbackEXT pfn_vkDestroyDebugReportCallbackEXT;
		PFN_vkDebugReportMessageEXT pfn_vkDebugReportMessageEXT;
#endif
#if defined(VK_EXT_debug_marker) && (defined(VK_EXT_debug_report))
		PFN_vkDebugMarkerSetObjectNameEXT pfn_vkDebugMarkerSetObjectNameEXT;
		PFN_vkDebugMarkerSetObjectTagEXT pfn_vkDebugMarkerSetObjectTagEXT;
		PFN_vkCmdDebugMarkerBeginEXT pfn_vkCmdDebugMarkerBeginEXT;
		PFN_vkCmdDebugMarkerEndEXT pfn_vkCmdDebugMarkerEndEXT;
		PFN_vkCmdDebugMarkerInsertEXT pfn_vkCmdDebugMarkerInsertEXT;
#endif
#if defined(VK_NV_external_memory_capabilities)
		PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV pfn_vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
#endif
#if defined(VK_NV_external_memory_win32) && (defined(VK_NV_external_memory))
		PFN_vkGetMemoryWin32HandleNV pfn_vkGetMemoryWin32HandleNV;
#endif
#if defined(VK_NV_device_generated_commands) && (defined(VK_VERSION_1_1) && defined(VK_KHR_buffer_device_address))
		PFN_vkCmdExecuteGeneratedCommandsNV pfn_vkCmdExecuteGeneratedCommandsNV;
		PFN_vkCmdPreprocessGeneratedCommandsNV pfn_vkCmdPreprocessGeneratedCommandsNV;
		PFN_vkCmdBindPipelineShaderGroupNV pfn_vkCmdBindPipelineShaderGroupNV;
		PFN_vkGetGeneratedCommandsMemoryRequirementsNV pfn_vkGetGeneratedCommandsMemoryRequirementsNV;
		PFN_vkCreateIndirectCommandsLayoutNV pfn_vkCreateIndirectCommandsLayoutNV;
		PFN_vkDestroyIndirectCommandsLayoutNV pfn_vkDestroyIndirectCommandsLayoutNV;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceFeatures2 pfn_vkGetPhysicalDeviceFeatures2;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		PFN_vkGetPhysicalDeviceFeatures2KHR pfn_vkGetPhysicalDeviceFeatures2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceProperties2 pfn_vkGetPhysicalDeviceProperties2;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		PFN_vkGetPhysicalDeviceProperties2KHR pfn_vkGetPhysicalDeviceProperties2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceFormatProperties2 pfn_vkGetPhysicalDeviceFormatProperties2;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		PFN_vkGetPhysicalDeviceFormatProperties2KHR pfn_vkGetPhysicalDeviceFormatProperties2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceImageFormatProperties2 pfn_vkGetPhysicalDeviceImageFormatProperties2;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		PFN_vkGetPhysicalDeviceImageFormatProperties2KHR pfn_vkGetPhysicalDeviceImageFormatProperties2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceQueueFamilyProperties2 pfn_vkGetPhysicalDeviceQueueFamilyProperties2;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR pfn_vkGetPhysicalDeviceQueueFamilyProperties2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceMemoryProperties2 pfn_vkGetPhysicalDeviceMemoryProperties2;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		PFN_vkGetPhysicalDeviceMemoryProperties2KHR pfn_vkGetPhysicalDeviceMemoryProperties2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 pfn_vkGetPhysicalDeviceSparseImageFormatProperties2;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
		PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR pfn_vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
#endif
#if defined(VK_KHR_push_descriptor) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdPushDescriptorSetKHR pfn_vkCmdPushDescriptorSetKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkTrimCommandPool pfn_vkTrimCommandPool;
#endif
#if defined(VK_KHR_maintenance1)
		PFN_vkTrimCommandPoolKHR pfn_vkTrimCommandPoolKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceExternalBufferProperties pfn_vkGetPhysicalDeviceExternalBufferProperties;
#endif
#if defined(VK_KHR_external_memory_capabilities) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR pfn_vkGetPhysicalDeviceExternalBufferPropertiesKHR;
#endif
#if defined(VK_KHR_external_memory_win32) && (defined(VK_KHR_external_memory))
		PFN_vkGetMemoryWin32HandleKHR pfn_vkGetMemoryWin32HandleKHR;
		PFN_vkGetMemoryWin32HandlePropertiesKHR pfn_vkGetMemoryWin32HandlePropertiesKHR;
#endif
#if defined(VK_KHR_external_memory_fd) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		PFN_vkGetMemoryFdKHR pfn_vkGetMemoryFdKHR;
		PFN_vkGetMemoryFdPropertiesKHR pfn_vkGetMemoryFdPropertiesKHR;
#endif
#if defined(VK_FUCHSIA_external_memory) && (defined(VK_KHR_external_memory_capabilities) && defined(VK_KHR_external_memory))
		PFN_vkGetMemoryZirconHandleFUCHSIA pfn_vkGetMemoryZirconHandleFUCHSIA;
		PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA pfn_vkGetMemoryZirconHandlePropertiesFUCHSIA;
#endif
#if defined(VK_NV_external_memory_rdma) && (defined(VK_KHR_external_memory))
		PFN_vkGetMemoryRemoteAddressNV pfn_vkGetMemoryRemoteAddressNV;
#endif
#if defined(VK_NV_external_memory_sci_buf) && (defined(VK_VERSION_1_1))
		PFN_vkGetMemorySciBufNV pfn_vkGetMemorySciBufNV;
		PFN_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV pfn_vkGetPhysicalDeviceExternalMemorySciBufPropertiesNV;
		PFN_vkGetPhysicalDeviceSciBufAttributesNV pfn_vkGetPhysicalDeviceSciBufAttributesNV;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceExternalSemaphoreProperties pfn_vkGetPhysicalDeviceExternalSemaphoreProperties;
#endif
#if defined(VK_KHR_external_semaphore_capabilities) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR pfn_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
#endif
#if defined(VK_KHR_external_semaphore_win32) && (defined(VK_KHR_external_semaphore))
		PFN_vkGetSemaphoreWin32HandleKHR pfn_vkGetSemaphoreWin32HandleKHR;
		PFN_vkImportSemaphoreWin32HandleKHR pfn_vkImportSemaphoreWin32HandleKHR;
#endif
#if defined(VK_KHR_external_semaphore_fd) && (defined(VK_KHR_external_semaphore) || defined(VK_VERSION_1_1))
		PFN_vkGetSemaphoreFdKHR pfn_vkGetSemaphoreFdKHR;
		PFN_vkImportSemaphoreFdKHR pfn_vkImportSemaphoreFdKHR;
#endif
#if defined(VK_FUCHSIA_external_semaphore) && (defined(VK_KHR_external_semaphore_capabilities) && defined(VK_KHR_external_semaphore))
		PFN_vkGetSemaphoreZirconHandleFUCHSIA pfn_vkGetSemaphoreZirconHandleFUCHSIA;
		PFN_vkImportSemaphoreZirconHandleFUCHSIA pfn_vkImportSemaphoreZirconHandleFUCHSIA;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetPhysicalDeviceExternalFenceProperties pfn_vkGetPhysicalDeviceExternalFenceProperties;
#endif
#if defined(VK_KHR_external_fence_capabilities) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR pfn_vkGetPhysicalDeviceExternalFencePropertiesKHR;
#endif
#if defined(VK_KHR_external_fence_win32) && (defined(VK_KHR_external_fence))
		PFN_vkGetFenceWin32HandleKHR pfn_vkGetFenceWin32HandleKHR;
		PFN_vkImportFenceWin32HandleKHR pfn_vkImportFenceWin32HandleKHR;
#endif
#if defined(VK_KHR_external_fence_fd) && (defined(VK_KHR_external_fence) || defined(VK_VERSION_1_1))
		PFN_vkGetFenceFdKHR pfn_vkGetFenceFdKHR;
		PFN_vkImportFenceFdKHR pfn_vkImportFenceFdKHR;
#endif
#if (defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1))) || (defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1)))
		PFN_vkGetFenceSciSyncFenceNV pfn_vkGetFenceSciSyncFenceNV;
		PFN_vkGetFenceSciSyncObjNV pfn_vkGetFenceSciSyncObjNV;
		PFN_vkImportFenceSciSyncFenceNV pfn_vkImportFenceSciSyncFenceNV;
		PFN_vkImportFenceSciSyncObjNV pfn_vkImportFenceSciSyncObjNV;
#endif
#if defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1))
		PFN_vkGetSemaphoreSciSyncObjNV pfn_vkGetSemaphoreSciSyncObjNV;
		PFN_vkImportSemaphoreSciSyncObjNV pfn_vkImportSemaphoreSciSyncObjNV;
#endif
#if (defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1))) || (defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1)))
		PFN_vkGetPhysicalDeviceSciSyncAttributesNV pfn_vkGetPhysicalDeviceSciSyncAttributesNV;
#endif
#if defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1))
		PFN_vkCreateSemaphoreSciSyncPoolNV pfn_vkCreateSemaphoreSciSyncPoolNV;
		PFN_vkDestroySemaphoreSciSyncPoolNV pfn_vkDestroySemaphoreSciSyncPoolNV;
#endif
#if defined(VK_EXT_direct_mode_display) && (defined(VK_KHR_display))
		PFN_vkReleaseDisplayEXT pfn_vkReleaseDisplayEXT;
#endif
#if defined(VK_EXT_acquire_xlib_display) && (defined(VK_EXT_direct_mode_display))
		PFN_vkAcquireXlibDisplayEXT pfn_vkAcquireXlibDisplayEXT;
		PFN_vkGetRandROutputDisplayEXT pfn_vkGetRandROutputDisplayEXT;
#endif
#if defined(VK_NV_acquire_winrt_display) && (defined(VK_EXT_direct_mode_display))
		PFN_vkAcquireWinrtDisplayNV pfn_vkAcquireWinrtDisplayNV;
		PFN_vkGetWinrtDisplayNV pfn_vkGetWinrtDisplayNV;
#endif
#if defined(VK_EXT_display_control) && (defined(VK_EXT_display_surface_counter) && defined(VK_KHR_swapchain))
		PFN_vkDisplayPowerControlEXT pfn_vkDisplayPowerControlEXT;
		PFN_vkRegisterDeviceEventEXT pfn_vkRegisterDeviceEventEXT;
		PFN_vkRegisterDisplayEventEXT pfn_vkRegisterDisplayEventEXT;
		PFN_vkGetSwapchainCounterEXT pfn_vkGetSwapchainCounterEXT;
#endif
#if defined(VK_EXT_display_surface_counter) && (defined(VK_KHR_display))
		PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT pfn_vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkEnumeratePhysicalDeviceGroups pfn_vkEnumeratePhysicalDeviceGroups;
#endif
#if defined(VK_KHR_device_group_creation)
		PFN_vkEnumeratePhysicalDeviceGroupsKHR pfn_vkEnumeratePhysicalDeviceGroupsKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetDeviceGroupPeerMemoryFeatures pfn_vkGetDeviceGroupPeerMemoryFeatures;
#endif
#if defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))
		PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR pfn_vkGetDeviceGroupPeerMemoryFeaturesKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkBindBufferMemory2 pfn_vkBindBufferMemory2;
#endif
#if defined(VK_KHR_bind_memory2)
		PFN_vkBindBufferMemory2KHR pfn_vkBindBufferMemory2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkBindImageMemory2 pfn_vkBindImageMemory2;
#endif
#if defined(VK_KHR_bind_memory2)
		PFN_vkBindImageMemory2KHR pfn_vkBindImageMemory2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkCmdSetDeviceMask pfn_vkCmdSetDeviceMask;
#endif
#if defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))
		PFN_vkCmdSetDeviceMaskKHR pfn_vkCmdSetDeviceMaskKHR;
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_surface)))
		PFN_vkGetDeviceGroupPresentCapabilitiesKHR pfn_vkGetDeviceGroupPresentCapabilitiesKHR;
		PFN_vkGetDeviceGroupSurfacePresentModesKHR pfn_vkGetDeviceGroupSurfacePresentModesKHR;
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_swapchain)))
		PFN_vkAcquireNextImage2KHR pfn_vkAcquireNextImage2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkCmdDispatchBase pfn_vkCmdDispatchBase;
#endif
#if defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))
		PFN_vkCmdDispatchBaseKHR pfn_vkCmdDispatchBaseKHR;
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_surface)))
		PFN_vkGetPhysicalDevicePresentRectanglesKHR pfn_vkGetPhysicalDevicePresentRectanglesKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkCreateDescriptorUpdateTemplate pfn_vkCreateDescriptorUpdateTemplate;
#endif
#if defined(VK_KHR_descriptor_update_template)
		PFN_vkCreateDescriptorUpdateTemplateKHR pfn_vkCreateDescriptorUpdateTemplateKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkDestroyDescriptorUpdateTemplate pfn_vkDestroyDescriptorUpdateTemplate;
#endif
#if defined(VK_KHR_descriptor_update_template)
		PFN_vkDestroyDescriptorUpdateTemplateKHR pfn_vkDestroyDescriptorUpdateTemplateKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkUpdateDescriptorSetWithTemplate pfn_vkUpdateDescriptorSetWithTemplate;
#endif
#if defined(VK_KHR_descriptor_update_template)
		PFN_vkUpdateDescriptorSetWithTemplateKHR pfn_vkUpdateDescriptorSetWithTemplateKHR;
#endif
#if (((defined(VK_KHR_push_descriptor) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_push_descriptor) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_KHR_descriptor_update_template)))) || ((defined(VK_KHR_descriptor_update_template)) && (defined(VK_KHR_push_descriptor)))
		PFN_vkCmdPushDescriptorSetWithTemplateKHR pfn_vkCmdPushDescriptorSetWithTemplateKHR;
#endif
#if defined(VK_EXT_hdr_metadata) && (defined(VK_KHR_swapchain))
		PFN_vkSetHdrMetadataEXT pfn_vkSetHdrMetadataEXT;
#endif
#if defined(VK_KHR_shared_presentable_image) && (defined(VK_KHR_swapchain) && defined(VK_KHR_get_surface_capabilities2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)))
		PFN_vkGetSwapchainStatusKHR pfn_vkGetSwapchainStatusKHR;
#endif
#if defined(VK_GOOGLE_display_timing) && (defined(VK_KHR_swapchain))
		PFN_vkGetRefreshCycleDurationGOOGLE pfn_vkGetRefreshCycleDurationGOOGLE;
		PFN_vkGetPastPresentationTimingGOOGLE pfn_vkGetPastPresentationTimingGOOGLE;
#endif
#if defined(VK_MVK_ios_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateIOSSurfaceMVK pfn_vkCreateIOSSurfaceMVK;
#endif
#if defined(VK_MVK_macos_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateMacOSSurfaceMVK pfn_vkCreateMacOSSurfaceMVK;
#endif
#if defined(VK_EXT_metal_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateMetalSurfaceEXT pfn_vkCreateMetalSurfaceEXT;
#endif
#if defined(VK_NV_clip_space_w_scaling)
		PFN_vkCmdSetViewportWScalingNV pfn_vkCmdSetViewportWScalingNV;
#endif
#if defined(VK_EXT_discard_rectangles) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdSetDiscardRectangleEXT pfn_vkCmdSetDiscardRectangleEXT;
		PFN_vkCmdSetDiscardRectangleEnableEXT pfn_vkCmdSetDiscardRectangleEnableEXT;
		PFN_vkCmdSetDiscardRectangleModeEXT pfn_vkCmdSetDiscardRectangleModeEXT;
#endif
#if defined(VK_EXT_sample_locations) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdSetSampleLocationsEXT pfn_vkCmdSetSampleLocationsEXT;
		PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT pfn_vkGetPhysicalDeviceMultisamplePropertiesEXT;
#endif
#if defined(VK_KHR_get_surface_capabilities2) && (defined(VK_KHR_surface))
		PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR pfn_vkGetPhysicalDeviceSurfaceCapabilities2KHR;
		PFN_vkGetPhysicalDeviceSurfaceFormats2KHR pfn_vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif
#if defined(VK_KHR_get_display_properties2) && (defined(VK_KHR_display))
		PFN_vkGetPhysicalDeviceDisplayProperties2KHR pfn_vkGetPhysicalDeviceDisplayProperties2KHR;
		PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR pfn_vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
		PFN_vkGetDisplayModeProperties2KHR pfn_vkGetDisplayModeProperties2KHR;
		PFN_vkGetDisplayPlaneCapabilities2KHR pfn_vkGetDisplayPlaneCapabilities2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetBufferMemoryRequirements2 pfn_vkGetBufferMemoryRequirements2;
#endif
#if defined(VK_KHR_get_memory_requirements2)
		PFN_vkGetBufferMemoryRequirements2KHR pfn_vkGetBufferMemoryRequirements2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetImageMemoryRequirements2 pfn_vkGetImageMemoryRequirements2;
#endif
#if defined(VK_KHR_get_memory_requirements2)
		PFN_vkGetImageMemoryRequirements2KHR pfn_vkGetImageMemoryRequirements2KHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetImageSparseMemoryRequirements2 pfn_vkGetImageSparseMemoryRequirements2;
#endif
#if defined(VK_KHR_get_memory_requirements2)
		PFN_vkGetImageSparseMemoryRequirements2KHR pfn_vkGetImageSparseMemoryRequirements2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkGetDeviceBufferMemoryRequirements pfn_vkGetDeviceBufferMemoryRequirements;
#endif
#if defined(VK_KHR_maintenance4) && (defined(VK_VERSION_1_1))
		PFN_vkGetDeviceBufferMemoryRequirementsKHR pfn_vkGetDeviceBufferMemoryRequirementsKHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkGetDeviceImageMemoryRequirements pfn_vkGetDeviceImageMemoryRequirements;
#endif
#if defined(VK_KHR_maintenance4) && (defined(VK_VERSION_1_1))
		PFN_vkGetDeviceImageMemoryRequirementsKHR pfn_vkGetDeviceImageMemoryRequirementsKHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkGetDeviceImageSparseMemoryRequirements pfn_vkGetDeviceImageSparseMemoryRequirements;
#endif
#if defined(VK_KHR_maintenance4) && (defined(VK_VERSION_1_1))
		PFN_vkGetDeviceImageSparseMemoryRequirementsKHR pfn_vkGetDeviceImageSparseMemoryRequirementsKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkCreateSamplerYcbcrConversion pfn_vkCreateSamplerYcbcrConversion;
#endif
#if defined(VK_KHR_sampler_ycbcr_conversion) && (defined(VK_KHR_maintenance1) && defined(VK_KHR_bind_memory2) && defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCreateSamplerYcbcrConversionKHR pfn_vkCreateSamplerYcbcrConversionKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkDestroySamplerYcbcrConversion pfn_vkDestroySamplerYcbcrConversion;
#endif
#if defined(VK_KHR_sampler_ycbcr_conversion) && (defined(VK_KHR_maintenance1) && defined(VK_KHR_bind_memory2) && defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_get_physical_device_properties2))
		PFN_vkDestroySamplerYcbcrConversionKHR pfn_vkDestroySamplerYcbcrConversionKHR;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetDeviceQueue2 pfn_vkGetDeviceQueue2;
#endif
#if defined(VK_EXT_validation_cache)
		PFN_vkCreateValidationCacheEXT pfn_vkCreateValidationCacheEXT;
		PFN_vkDestroyValidationCacheEXT pfn_vkDestroyValidationCacheEXT;
		PFN_vkGetValidationCacheDataEXT pfn_vkGetValidationCacheDataEXT;
		PFN_vkMergeValidationCachesEXT pfn_vkMergeValidationCachesEXT;
#endif
#if defined(VK_VERSION_1_1)
		PFN_vkGetDescriptorSetLayoutSupport pfn_vkGetDescriptorSetLayoutSupport;
#endif
#if defined(VK_KHR_maintenance3) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetDescriptorSetLayoutSupportKHR pfn_vkGetDescriptorSetLayoutSupportKHR;
#endif
#if defined(VK_ANDROID_native_buffer)
		PFN_vkGetSwapchainGrallocUsageANDROID pfn_vkGetSwapchainGrallocUsageANDROID;
		PFN_vkGetSwapchainGrallocUsage2ANDROID pfn_vkGetSwapchainGrallocUsage2ANDROID;
		PFN_vkAcquireImageANDROID pfn_vkAcquireImageANDROID;
		PFN_vkQueueSignalReleaseImageANDROID pfn_vkQueueSignalReleaseImageANDROID;
#endif
#if defined(VK_AMD_shader_info)
		PFN_vkGetShaderInfoAMD pfn_vkGetShaderInfoAMD;
#endif
#if defined(VK_AMD_display_native_hdr) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))
		PFN_vkSetLocalDimmingAMD pfn_vkSetLocalDimmingAMD;
#endif
#if defined(VK_KHR_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR pfn_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR;
#endif
#if defined(VK_EXT_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT pfn_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
#endif
#if defined(VK_KHR_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkGetCalibratedTimestampsKHR pfn_vkGetCalibratedTimestampsKHR;
#endif
#if defined(VK_EXT_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkGetCalibratedTimestampsEXT pfn_vkGetCalibratedTimestampsEXT;
#endif
#if defined(VK_EXT_debug_utils)
		PFN_vkSetDebugUtilsObjectNameEXT pfn_vkSetDebugUtilsObjectNameEXT;
		PFN_vkSetDebugUtilsObjectTagEXT pfn_vkSetDebugUtilsObjectTagEXT;
		PFN_vkQueueBeginDebugUtilsLabelEXT pfn_vkQueueBeginDebugUtilsLabelEXT;
		PFN_vkQueueEndDebugUtilsLabelEXT pfn_vkQueueEndDebugUtilsLabelEXT;
		PFN_vkQueueInsertDebugUtilsLabelEXT pfn_vkQueueInsertDebugUtilsLabelEXT;
		PFN_vkCmdBeginDebugUtilsLabelEXT pfn_vkCmdBeginDebugUtilsLabelEXT;
		PFN_vkCmdEndDebugUtilsLabelEXT pfn_vkCmdEndDebugUtilsLabelEXT;
		PFN_vkCmdInsertDebugUtilsLabelEXT pfn_vkCmdInsertDebugUtilsLabelEXT;
		PFN_vkCreateDebugUtilsMessengerEXT pfn_vkCreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT pfn_vkDestroyDebugUtilsMessengerEXT;
		PFN_vkSubmitDebugUtilsMessageEXT pfn_vkSubmitDebugUtilsMessageEXT;
#endif
#if defined(VK_EXT_external_memory_host) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		PFN_vkGetMemoryHostPointerPropertiesEXT pfn_vkGetMemoryHostPointerPropertiesEXT;
#endif
#if defined(VK_AMD_buffer_marker)
		PFN_vkCmdWriteBufferMarkerAMD pfn_vkCmdWriteBufferMarkerAMD;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkCreateRenderPass2 pfn_vkCreateRenderPass2;
#endif
#if defined(VK_KHR_create_renderpass2) && (defined(VK_KHR_multiview) && defined(VK_KHR_maintenance2))
		PFN_vkCreateRenderPass2KHR pfn_vkCreateRenderPass2KHR;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkCmdBeginRenderPass2 pfn_vkCmdBeginRenderPass2;
#endif
#if defined(VK_KHR_create_renderpass2) && (defined(VK_KHR_multiview) && defined(VK_KHR_maintenance2))
		PFN_vkCmdBeginRenderPass2KHR pfn_vkCmdBeginRenderPass2KHR;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkCmdNextSubpass2 pfn_vkCmdNextSubpass2;
#endif
#if defined(VK_KHR_create_renderpass2) && (defined(VK_KHR_multiview) && defined(VK_KHR_maintenance2))
		PFN_vkCmdNextSubpass2KHR pfn_vkCmdNextSubpass2KHR;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkCmdEndRenderPass2 pfn_vkCmdEndRenderPass2;
#endif
#if defined(VK_KHR_create_renderpass2) && (defined(VK_KHR_multiview) && defined(VK_KHR_maintenance2))
		PFN_vkCmdEndRenderPass2KHR pfn_vkCmdEndRenderPass2KHR;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkGetSemaphoreCounterValue pfn_vkGetSemaphoreCounterValue;
#endif
#if defined(VK_KHR_timeline_semaphore) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetSemaphoreCounterValueKHR pfn_vkGetSemaphoreCounterValueKHR;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkWaitSemaphores pfn_vkWaitSemaphores;
#endif
#if defined(VK_KHR_timeline_semaphore) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkWaitSemaphoresKHR pfn_vkWaitSemaphoresKHR;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkSignalSemaphore pfn_vkSignalSemaphore;
#endif
#if defined(VK_KHR_timeline_semaphore) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkSignalSemaphoreKHR pfn_vkSignalSemaphoreKHR;
#endif
#if defined(VK_ANDROID_external_memory_android_hardware_buffer) && (defined(VK_KHR_sampler_ycbcr_conversion) && defined(VK_KHR_external_memory) && defined(VK_EXT_queue_family_foreign) && defined(VK_KHR_dedicated_allocation))
		PFN_vkGetAndroidHardwareBufferPropertiesANDROID pfn_vkGetAndroidHardwareBufferPropertiesANDROID;
		PFN_vkGetMemoryAndroidHardwareBufferANDROID pfn_vkGetMemoryAndroidHardwareBufferANDROID;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkCmdDrawIndirectCount pfn_vkCmdDrawIndirectCount;
#endif
#if defined(VK_KHR_draw_indirect_count)
		PFN_vkCmdDrawIndirectCountKHR pfn_vkCmdDrawIndirectCountKHR;
#endif
#if defined(VK_AMD_draw_indirect_count)
		PFN_vkCmdDrawIndirectCountAMD pfn_vkCmdDrawIndirectCountAMD;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkCmdDrawIndexedIndirectCount pfn_vkCmdDrawIndexedIndirectCount;
#endif
#if defined(VK_KHR_draw_indirect_count)
		PFN_vkCmdDrawIndexedIndirectCountKHR pfn_vkCmdDrawIndexedIndirectCountKHR;
#endif
#if defined(VK_AMD_draw_indirect_count)
		PFN_vkCmdDrawIndexedIndirectCountAMD pfn_vkCmdDrawIndexedIndirectCountAMD;
#endif
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdSetCheckpointNV pfn_vkCmdSetCheckpointNV;
		PFN_vkGetQueueCheckpointDataNV pfn_vkGetQueueCheckpointDataNV;
#endif
#if defined(VK_EXT_transform_feedback) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdBindTransformFeedbackBuffersEXT pfn_vkCmdBindTransformFeedbackBuffersEXT;
		PFN_vkCmdBeginTransformFeedbackEXT pfn_vkCmdBeginTransformFeedbackEXT;
		PFN_vkCmdEndTransformFeedbackEXT pfn_vkCmdEndTransformFeedbackEXT;
		PFN_vkCmdBeginQueryIndexedEXT pfn_vkCmdBeginQueryIndexedEXT;
		PFN_vkCmdEndQueryIndexedEXT pfn_vkCmdEndQueryIndexedEXT;
		PFN_vkCmdDrawIndirectByteCountEXT pfn_vkCmdDrawIndirectByteCountEXT;
#endif
#if defined(VK_NV_scissor_exclusive) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdSetExclusiveScissorNV pfn_vkCmdSetExclusiveScissorNV;
		PFN_vkCmdSetExclusiveScissorEnableNV pfn_vkCmdSetExclusiveScissorEnableNV;
#endif
#if defined(VK_NV_shading_rate_image) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdBindShadingRateImageNV pfn_vkCmdBindShadingRateImageNV;
		PFN_vkCmdSetViewportShadingRatePaletteNV pfn_vkCmdSetViewportShadingRatePaletteNV;
		PFN_vkCmdSetCoarseSampleOrderNV pfn_vkCmdSetCoarseSampleOrderNV;
#endif
#if defined(VK_NV_mesh_shader) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdDrawMeshTasksNV pfn_vkCmdDrawMeshTasksNV;
		PFN_vkCmdDrawMeshTasksIndirectNV pfn_vkCmdDrawMeshTasksIndirectNV;
		PFN_vkCmdDrawMeshTasksIndirectCountNV pfn_vkCmdDrawMeshTasksIndirectCountNV;
#endif
#if defined(VK_EXT_mesh_shader) && (defined(VK_KHR_spirv_1_4))
		PFN_vkCmdDrawMeshTasksEXT pfn_vkCmdDrawMeshTasksEXT;
		PFN_vkCmdDrawMeshTasksIndirectEXT pfn_vkCmdDrawMeshTasksIndirectEXT;
		PFN_vkCmdDrawMeshTasksIndirectCountEXT pfn_vkCmdDrawMeshTasksIndirectCountEXT;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		PFN_vkCompileDeferredNV pfn_vkCompileDeferredNV;
		PFN_vkCreateAccelerationStructureNV pfn_vkCreateAccelerationStructureNV;
#endif
#if defined(VK_HUAWEI_invocation_mask) && (defined(VK_KHR_ray_tracing_pipeline) && defined(VK_KHR_synchronization2))
		PFN_vkCmdBindInvocationMaskHUAWEI pfn_vkCmdBindInvocationMaskHUAWEI;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		PFN_vkDestroyAccelerationStructureKHR pfn_vkDestroyAccelerationStructureKHR;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		PFN_vkDestroyAccelerationStructureNV pfn_vkDestroyAccelerationStructureNV;
		PFN_vkGetAccelerationStructureMemoryRequirementsNV pfn_vkGetAccelerationStructureMemoryRequirementsNV;
		PFN_vkBindAccelerationStructureMemoryNV pfn_vkBindAccelerationStructureMemoryNV;
		PFN_vkCmdCopyAccelerationStructureNV pfn_vkCmdCopyAccelerationStructureNV;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		PFN_vkCmdCopyAccelerationStructureKHR pfn_vkCmdCopyAccelerationStructureKHR;
		PFN_vkCopyAccelerationStructureKHR pfn_vkCopyAccelerationStructureKHR;
		PFN_vkCmdCopyAccelerationStructureToMemoryKHR pfn_vkCmdCopyAccelerationStructureToMemoryKHR;
		PFN_vkCopyAccelerationStructureToMemoryKHR pfn_vkCopyAccelerationStructureToMemoryKHR;
		PFN_vkCmdCopyMemoryToAccelerationStructureKHR pfn_vkCmdCopyMemoryToAccelerationStructureKHR;
		PFN_vkCopyMemoryToAccelerationStructureKHR pfn_vkCopyMemoryToAccelerationStructureKHR;
		PFN_vkCmdWriteAccelerationStructuresPropertiesKHR pfn_vkCmdWriteAccelerationStructuresPropertiesKHR;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		PFN_vkCmdWriteAccelerationStructuresPropertiesNV pfn_vkCmdWriteAccelerationStructuresPropertiesNV;
		PFN_vkCmdBuildAccelerationStructureNV pfn_vkCmdBuildAccelerationStructureNV;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		PFN_vkWriteAccelerationStructuresPropertiesKHR pfn_vkWriteAccelerationStructuresPropertiesKHR;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		PFN_vkCmdTraceRaysKHR pfn_vkCmdTraceRaysKHR;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		PFN_vkCmdTraceRaysNV pfn_vkCmdTraceRaysNV;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		PFN_vkGetRayTracingShaderGroupHandlesKHR pfn_vkGetRayTracingShaderGroupHandlesKHR;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		PFN_vkGetRayTracingShaderGroupHandlesNV pfn_vkGetRayTracingShaderGroupHandlesNV;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR pfn_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
#endif
#if defined(VK_NV_ray_tracing) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2))
		PFN_vkGetAccelerationStructureHandleNV pfn_vkGetAccelerationStructureHandleNV;
		PFN_vkCreateRayTracingPipelinesNV pfn_vkCreateRayTracingPipelinesNV;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		PFN_vkCreateRayTracingPipelinesKHR pfn_vkCreateRayTracingPipelinesKHR;
#endif
#if defined(VK_NV_cooperative_matrix) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV pfn_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		PFN_vkCmdTraceRaysIndirectKHR pfn_vkCmdTraceRaysIndirectKHR;
#endif
#if (defined(VK_KHR_ray_tracing_maintenance1) && (defined(VK_KHR_acceleration_structure))) && (defined(VK_KHR_ray_tracing_pipeline))
		PFN_vkCmdTraceRaysIndirect2KHR pfn_vkCmdTraceRaysIndirect2KHR;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		PFN_vkGetDeviceAccelerationStructureCompatibilityKHR pfn_vkGetDeviceAccelerationStructureCompatibilityKHR;
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_spirv_1_4) && defined(VK_KHR_acceleration_structure))
		PFN_vkGetRayTracingShaderGroupStackSizeKHR pfn_vkGetRayTracingShaderGroupStackSizeKHR;
		PFN_vkCmdSetRayTracingPipelineStackSizeKHR pfn_vkCmdSetRayTracingPipelineStackSizeKHR;
#endif
#if defined(VK_NVX_image_view_handle)
		PFN_vkGetImageViewHandleNVX pfn_vkGetImageViewHandleNVX;
		PFN_vkGetImageViewAddressNVX pfn_vkGetImageViewAddressNVX;
#endif
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))
		PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT pfn_vkGetPhysicalDeviceSurfacePresentModes2EXT;
#endif
#if ((defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))) && (defined(VK_KHR_device_group))) || ((defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))) && (defined(VK_VERSION_1_1)))
		PFN_vkGetDeviceGroupSurfacePresentModes2EXT pfn_vkGetDeviceGroupSurfacePresentModes2EXT;
#endif
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))
		PFN_vkAcquireFullScreenExclusiveModeEXT pfn_vkAcquireFullScreenExclusiveModeEXT;
		PFN_vkReleaseFullScreenExclusiveModeEXT pfn_vkReleaseFullScreenExclusiveModeEXT;
#endif
#if defined(VK_KHR_performance_query) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR pfn_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
		PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR pfn_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
		PFN_vkAcquireProfilingLockKHR pfn_vkAcquireProfilingLockKHR;
		PFN_vkReleaseProfilingLockKHR pfn_vkReleaseProfilingLockKHR;
#endif
#if defined(VK_EXT_image_drm_format_modifier) && (((defined(VK_KHR_bind_memory2) && defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_sampler_ycbcr_conversion)) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_image_format_list) || defined(VK_VERSION_1_2)))
		PFN_vkGetImageDrmFormatModifierPropertiesEXT pfn_vkGetImageDrmFormatModifierPropertiesEXT;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkGetBufferOpaqueCaptureAddress pfn_vkGetBufferOpaqueCaptureAddress;
#endif
#if defined(VK_KHR_buffer_device_address) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_device_group)) || defined(VK_VERSION_1_1))
		PFN_vkGetBufferOpaqueCaptureAddressKHR pfn_vkGetBufferOpaqueCaptureAddressKHR;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkGetBufferDeviceAddress pfn_vkGetBufferDeviceAddress;
#endif
#if defined(VK_KHR_buffer_device_address) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_device_group)) || defined(VK_VERSION_1_1))
		PFN_vkGetBufferDeviceAddressKHR pfn_vkGetBufferDeviceAddressKHR;
#endif
#if defined(VK_EXT_buffer_device_address) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetBufferDeviceAddressEXT pfn_vkGetBufferDeviceAddressEXT;
#endif
#if defined(VK_EXT_headless_surface) && (defined(VK_KHR_surface))
		PFN_vkCreateHeadlessSurfaceEXT pfn_vkCreateHeadlessSurfaceEXT;
#endif
#if defined(VK_NV_coverage_reduction_mode) && (defined(VK_NV_framebuffer_mixed_samples) && defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV pfn_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
#endif
#if defined(VK_INTEL_performance_query)
		PFN_vkInitializePerformanceApiINTEL pfn_vkInitializePerformanceApiINTEL;
		PFN_vkUninitializePerformanceApiINTEL pfn_vkUninitializePerformanceApiINTEL;
		PFN_vkCmdSetPerformanceMarkerINTEL pfn_vkCmdSetPerformanceMarkerINTEL;
		PFN_vkCmdSetPerformanceStreamMarkerINTEL pfn_vkCmdSetPerformanceStreamMarkerINTEL;
		PFN_vkCmdSetPerformanceOverrideINTEL pfn_vkCmdSetPerformanceOverrideINTEL;
		PFN_vkAcquirePerformanceConfigurationINTEL pfn_vkAcquirePerformanceConfigurationINTEL;
		PFN_vkReleasePerformanceConfigurationINTEL pfn_vkReleasePerformanceConfigurationINTEL;
		PFN_vkQueueSetPerformanceConfigurationINTEL pfn_vkQueueSetPerformanceConfigurationINTEL;
		PFN_vkGetPerformanceParameterINTEL pfn_vkGetPerformanceParameterINTEL;
#endif
#if defined(VK_VERSION_1_2)
		PFN_vkGetDeviceMemoryOpaqueCaptureAddress pfn_vkGetDeviceMemoryOpaqueCaptureAddress;
#endif
#if defined(VK_KHR_buffer_device_address) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_device_group)) || defined(VK_VERSION_1_1))
		PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR pfn_vkGetDeviceMemoryOpaqueCaptureAddressKHR;
#endif
#if defined(VK_KHR_pipeline_executable_properties) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPipelineExecutablePropertiesKHR pfn_vkGetPipelineExecutablePropertiesKHR;
		PFN_vkGetPipelineExecutableStatisticsKHR pfn_vkGetPipelineExecutableStatisticsKHR;
		PFN_vkGetPipelineExecutableInternalRepresentationsKHR pfn_vkGetPipelineExecutableInternalRepresentationsKHR;
#endif
#if defined(VK_KHR_line_rasterization) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdSetLineStippleKHR pfn_vkCmdSetLineStippleKHR;
#endif
#if defined(VK_EXT_line_rasterization) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdSetLineStippleEXT pfn_vkCmdSetLineStippleEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkGetPhysicalDeviceToolProperties pfn_vkGetPhysicalDeviceToolProperties;
#endif
#if defined(VK_EXT_tooling_info)
		PFN_vkGetPhysicalDeviceToolPropertiesEXT pfn_vkGetPhysicalDeviceToolPropertiesEXT;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		PFN_vkCreateAccelerationStructureKHR pfn_vkCreateAccelerationStructureKHR;
		PFN_vkCmdBuildAccelerationStructuresKHR pfn_vkCmdBuildAccelerationStructuresKHR;
		PFN_vkCmdBuildAccelerationStructuresIndirectKHR pfn_vkCmdBuildAccelerationStructuresIndirectKHR;
		PFN_vkBuildAccelerationStructuresKHR pfn_vkBuildAccelerationStructuresKHR;
		PFN_vkGetAccelerationStructureDeviceAddressKHR pfn_vkGetAccelerationStructureDeviceAddressKHR;
#endif
#if defined(VK_KHR_deferred_host_operations)
		PFN_vkCreateDeferredOperationKHR pfn_vkCreateDeferredOperationKHR;
		PFN_vkDestroyDeferredOperationKHR pfn_vkDestroyDeferredOperationKHR;
		PFN_vkGetDeferredOperationMaxConcurrencyKHR pfn_vkGetDeferredOperationMaxConcurrencyKHR;
		PFN_vkGetDeferredOperationResultKHR pfn_vkGetDeferredOperationResultKHR;
		PFN_vkDeferredOperationJoinKHR pfn_vkDeferredOperationJoinKHR;
#endif
#if defined(VK_NV_device_generated_commands_compute) && (defined(VK_NV_device_generated_commands))
		PFN_vkGetPipelineIndirectMemoryRequirementsNV pfn_vkGetPipelineIndirectMemoryRequirementsNV;
		PFN_vkGetPipelineIndirectDeviceAddressNV pfn_vkGetPipelineIndirectDeviceAddressNV;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetCullMode pfn_vkCmdSetCullMode;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetCullModeEXT pfn_vkCmdSetCullModeEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetFrontFace pfn_vkCmdSetFrontFace;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetFrontFaceEXT pfn_vkCmdSetFrontFaceEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetPrimitiveTopology pfn_vkCmdSetPrimitiveTopology;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetPrimitiveTopologyEXT pfn_vkCmdSetPrimitiveTopologyEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetViewportWithCount pfn_vkCmdSetViewportWithCount;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetViewportWithCountEXT pfn_vkCmdSetViewportWithCountEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetScissorWithCount pfn_vkCmdSetScissorWithCount;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetScissorWithCountEXT pfn_vkCmdSetScissorWithCountEXT;
#endif
#if defined(VK_KHR_maintenance5) && (defined(VK_VERSION_1_1) && defined(VK_KHR_dynamic_rendering))
		PFN_vkCmdBindIndexBuffer2KHR pfn_vkCmdBindIndexBuffer2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdBindVertexBuffers2 pfn_vkCmdBindVertexBuffers2;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdBindVertexBuffers2EXT pfn_vkCmdBindVertexBuffers2EXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetDepthTestEnable pfn_vkCmdSetDepthTestEnable;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetDepthTestEnableEXT pfn_vkCmdSetDepthTestEnableEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetDepthWriteEnable pfn_vkCmdSetDepthWriteEnable;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetDepthWriteEnableEXT pfn_vkCmdSetDepthWriteEnableEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetDepthCompareOp pfn_vkCmdSetDepthCompareOp;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetDepthCompareOpEXT pfn_vkCmdSetDepthCompareOpEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetDepthBoundsTestEnable pfn_vkCmdSetDepthBoundsTestEnable;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetDepthBoundsTestEnableEXT pfn_vkCmdSetDepthBoundsTestEnableEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetStencilTestEnable pfn_vkCmdSetStencilTestEnable;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetStencilTestEnableEXT pfn_vkCmdSetStencilTestEnableEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetStencilOp pfn_vkCmdSetStencilOp;
#endif
#if (defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetStencilOpEXT pfn_vkCmdSetStencilOpEXT;
#endif
#if (defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetPatchControlPointsEXT pfn_vkCmdSetPatchControlPointsEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetRasterizerDiscardEnable pfn_vkCmdSetRasterizerDiscardEnable;
#endif
#if (defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetRasterizerDiscardEnableEXT pfn_vkCmdSetRasterizerDiscardEnableEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetDepthBiasEnable pfn_vkCmdSetDepthBiasEnable;
#endif
#if (defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetDepthBiasEnableEXT pfn_vkCmdSetDepthBiasEnableEXT;
		PFN_vkCmdSetLogicOpEXT pfn_vkCmdSetLogicOpEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetPrimitiveRestartEnable pfn_vkCmdSetPrimitiveRestartEnable;
#endif
#if (defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetPrimitiveRestartEnableEXT pfn_vkCmdSetPrimitiveRestartEnableEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetTessellationDomainOriginEXT pfn_vkCmdSetTessellationDomainOriginEXT;
#endif
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetDepthClampEnableEXT pfn_vkCmdSetDepthClampEnableEXT;
		PFN_vkCmdSetPolygonModeEXT pfn_vkCmdSetPolygonModeEXT;
		PFN_vkCmdSetRasterizationSamplesEXT pfn_vkCmdSetRasterizationSamplesEXT;
		PFN_vkCmdSetSampleMaskEXT pfn_vkCmdSetSampleMaskEXT;
		PFN_vkCmdSetAlphaToCoverageEnableEXT pfn_vkCmdSetAlphaToCoverageEnableEXT;
		PFN_vkCmdSetAlphaToOneEnableEXT pfn_vkCmdSetAlphaToOneEnableEXT;
		PFN_vkCmdSetLogicOpEnableEXT pfn_vkCmdSetLogicOpEnableEXT;
		PFN_vkCmdSetColorBlendEnableEXT pfn_vkCmdSetColorBlendEnableEXT;
		PFN_vkCmdSetColorBlendEquationEXT pfn_vkCmdSetColorBlendEquationEXT;
		PFN_vkCmdSetColorWriteMaskEXT pfn_vkCmdSetColorWriteMaskEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_transform_feedback))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_transform_feedback)))
		PFN_vkCmdSetRasterizationStreamEXT pfn_vkCmdSetRasterizationStreamEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_conservative_rasterization))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_conservative_rasterization)))
		PFN_vkCmdSetConservativeRasterizationModeEXT pfn_vkCmdSetConservativeRasterizationModeEXT;
		PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT pfn_vkCmdSetExtraPrimitiveOverestimationSizeEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_depth_clip_enable))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_depth_clip_enable)))
		PFN_vkCmdSetDepthClipEnableEXT pfn_vkCmdSetDepthClipEnableEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_sample_locations))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_sample_locations)))
		PFN_vkCmdSetSampleLocationsEnableEXT pfn_vkCmdSetSampleLocationsEnableEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_blend_operation_advanced))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_blend_operation_advanced)))
		PFN_vkCmdSetColorBlendAdvancedEXT pfn_vkCmdSetColorBlendAdvancedEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_provoking_vertex))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_provoking_vertex)))
		PFN_vkCmdSetProvokingVertexModeEXT pfn_vkCmdSetProvokingVertexModeEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_line_rasterization))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_line_rasterization)))
		PFN_vkCmdSetLineRasterizationModeEXT pfn_vkCmdSetLineRasterizationModeEXT;
		PFN_vkCmdSetLineStippleEnableEXT pfn_vkCmdSetLineStippleEnableEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_EXT_depth_clip_control))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_EXT_depth_clip_control)))
		PFN_vkCmdSetDepthClipNegativeOneToOneEXT pfn_vkCmdSetDepthClipNegativeOneToOneEXT;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_clip_space_w_scaling))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_clip_space_w_scaling)))
		PFN_vkCmdSetViewportWScalingEnableNV pfn_vkCmdSetViewportWScalingEnableNV;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_viewport_swizzle))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_viewport_swizzle)))
		PFN_vkCmdSetViewportSwizzleNV pfn_vkCmdSetViewportSwizzleNV;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_fragment_coverage_to_color))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_fragment_coverage_to_color)))
		PFN_vkCmdSetCoverageToColorEnableNV pfn_vkCmdSetCoverageToColorEnableNV;
		PFN_vkCmdSetCoverageToColorLocationNV pfn_vkCmdSetCoverageToColorLocationNV;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_framebuffer_mixed_samples))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_framebuffer_mixed_samples)))
		PFN_vkCmdSetCoverageModulationModeNV pfn_vkCmdSetCoverageModulationModeNV;
		PFN_vkCmdSetCoverageModulationTableEnableNV pfn_vkCmdSetCoverageModulationTableEnableNV;
		PFN_vkCmdSetCoverageModulationTableNV pfn_vkCmdSetCoverageModulationTableNV;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_shading_rate_image))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_shading_rate_image)))
		PFN_vkCmdSetShadingRateImageEnableNV pfn_vkCmdSetShadingRateImageEnableNV;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_coverage_reduction_mode))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_coverage_reduction_mode)))
		PFN_vkCmdSetCoverageReductionModeNV pfn_vkCmdSetCoverageReductionModeNV;
#endif
#if ((defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2))) && (defined(VK_NV_representative_fragment_test))) || ((defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))) && (defined(VK_NV_representative_fragment_test)))
		PFN_vkCmdSetRepresentativeFragmentTestEnableNV pfn_vkCmdSetRepresentativeFragmentTestEnableNV;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCreatePrivateDataSlot pfn_vkCreatePrivateDataSlot;
#endif
#if defined(VK_EXT_private_data) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCreatePrivateDataSlotEXT pfn_vkCreatePrivateDataSlotEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkDestroyPrivateDataSlot pfn_vkDestroyPrivateDataSlot;
#endif
#if defined(VK_EXT_private_data) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkDestroyPrivateDataSlotEXT pfn_vkDestroyPrivateDataSlotEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkSetPrivateData pfn_vkSetPrivateData;
#endif
#if defined(VK_EXT_private_data) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkSetPrivateDataEXT pfn_vkSetPrivateDataEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkGetPrivateData pfn_vkGetPrivateData;
#endif
#if defined(VK_EXT_private_data) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPrivateDataEXT pfn_vkGetPrivateDataEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdCopyBuffer2 pfn_vkCmdCopyBuffer2;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdCopyBuffer2KHR pfn_vkCmdCopyBuffer2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdCopyImage2 pfn_vkCmdCopyImage2;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdCopyImage2KHR pfn_vkCmdCopyImage2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdBlitImage2 pfn_vkCmdBlitImage2;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdBlitImage2KHR pfn_vkCmdBlitImage2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdCopyBufferToImage2 pfn_vkCmdCopyBufferToImage2;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdCopyBufferToImage2KHR pfn_vkCmdCopyBufferToImage2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdCopyImageToBuffer2 pfn_vkCmdCopyImageToBuffer2;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdCopyImageToBuffer2KHR pfn_vkCmdCopyImageToBuffer2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdResolveImage2 pfn_vkCmdResolveImage2;
#endif
#if defined(VK_KHR_copy_commands2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdResolveImage2KHR pfn_vkCmdResolveImage2KHR;
#endif
#if defined(VK_KHR_object_refresh)
		PFN_vkCmdRefreshObjectsKHR pfn_vkCmdRefreshObjectsKHR;
		PFN_vkGetPhysicalDeviceRefreshableObjectTypesKHR pfn_vkGetPhysicalDeviceRefreshableObjectTypesKHR;
#endif
#if defined(VK_KHR_fragment_shading_rate) && ((defined(VK_KHR_create_renderpass2) || defined(VK_VERSION_1_2)) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)))
		PFN_vkCmdSetFragmentShadingRateKHR pfn_vkCmdSetFragmentShadingRateKHR;
		PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR pfn_vkGetPhysicalDeviceFragmentShadingRatesKHR;
#endif
#if defined(VK_NV_fragment_shading_rate_enums) && (defined(VK_KHR_fragment_shading_rate))
		PFN_vkCmdSetFragmentShadingRateEnumNV pfn_vkCmdSetFragmentShadingRateEnumNV;
#endif
#if defined(VK_KHR_acceleration_structure) && (defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_deferred_host_operations))
		PFN_vkGetAccelerationStructureBuildSizesKHR pfn_vkGetAccelerationStructureBuildSizesKHR;
#endif
#if (defined(VK_EXT_vertex_input_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))))
		PFN_vkCmdSetVertexInputEXT pfn_vkCmdSetVertexInputEXT;
#endif
#if defined(VK_EXT_color_write_enable) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdSetColorWriteEnableEXT pfn_vkCmdSetColorWriteEnableEXT;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdSetEvent2 pfn_vkCmdSetEvent2;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdSetEvent2KHR pfn_vkCmdSetEvent2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdResetEvent2 pfn_vkCmdResetEvent2;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdResetEvent2KHR pfn_vkCmdResetEvent2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdWaitEvents2 pfn_vkCmdWaitEvents2;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdWaitEvents2KHR pfn_vkCmdWaitEvents2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdPipelineBarrier2 pfn_vkCmdPipelineBarrier2;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdPipelineBarrier2KHR pfn_vkCmdPipelineBarrier2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkQueueSubmit2 pfn_vkQueueSubmit2;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkQueueSubmit2KHR pfn_vkQueueSubmit2KHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdWriteTimestamp2 pfn_vkCmdWriteTimestamp2;
#endif
#if defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		PFN_vkCmdWriteTimestamp2KHR pfn_vkCmdWriteTimestamp2KHR;
#endif
#if (defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) && (defined(VK_AMD_buffer_marker))
		PFN_vkCmdWriteBufferMarker2AMD pfn_vkCmdWriteBufferMarker2AMD;
#endif
#if (defined(VK_KHR_synchronization2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) && (defined(VK_NV_device_diagnostic_checkpoints))
		PFN_vkGetQueueCheckpointData2NV pfn_vkGetQueueCheckpointData2NV;
#endif
#if defined(VK_EXT_host_image_copy) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_copy_commands2) && defined(VK_KHR_format_feature_flags2))
		PFN_vkCopyMemoryToImageEXT pfn_vkCopyMemoryToImageEXT;
		PFN_vkCopyImageToMemoryEXT pfn_vkCopyImageToMemoryEXT;
		PFN_vkCopyImageToImageEXT pfn_vkCopyImageToImageEXT;
		PFN_vkTransitionImageLayoutEXT pfn_vkTransitionImageLayoutEXT;
#endif
#if defined(VK_KHR_video_queue) && (defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2))
		PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR pfn_vkGetPhysicalDeviceVideoCapabilitiesKHR;
		PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR pfn_vkGetPhysicalDeviceVideoFormatPropertiesKHR;
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && defined(VK_KHR_synchronization2))
		PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR pfn_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR;
#endif
#if defined(VK_KHR_video_queue) && (defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2))
		PFN_vkCreateVideoSessionKHR pfn_vkCreateVideoSessionKHR;
		PFN_vkDestroyVideoSessionKHR pfn_vkDestroyVideoSessionKHR;
		PFN_vkCreateVideoSessionParametersKHR pfn_vkCreateVideoSessionParametersKHR;
		PFN_vkUpdateVideoSessionParametersKHR pfn_vkUpdateVideoSessionParametersKHR;
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && defined(VK_KHR_synchronization2))
		PFN_vkGetEncodedVideoSessionParametersKHR pfn_vkGetEncodedVideoSessionParametersKHR;
#endif
#if defined(VK_KHR_video_queue) && (defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2))
		PFN_vkDestroyVideoSessionParametersKHR pfn_vkDestroyVideoSessionParametersKHR;
		PFN_vkGetVideoSessionMemoryRequirementsKHR pfn_vkGetVideoSessionMemoryRequirementsKHR;
		PFN_vkBindVideoSessionMemoryKHR pfn_vkBindVideoSessionMemoryKHR;
#endif
#if defined(VK_KHR_video_decode_queue) && (defined(VK_KHR_video_queue) && defined(VK_KHR_synchronization2))
		PFN_vkCmdDecodeVideoKHR pfn_vkCmdDecodeVideoKHR;
#endif
#if defined(VK_KHR_video_queue) && (defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2))
		PFN_vkCmdBeginVideoCodingKHR pfn_vkCmdBeginVideoCodingKHR;
		PFN_vkCmdControlVideoCodingKHR pfn_vkCmdControlVideoCodingKHR;
		PFN_vkCmdEndVideoCodingKHR pfn_vkCmdEndVideoCodingKHR;
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && defined(VK_KHR_synchronization2))
		PFN_vkCmdEncodeVideoKHR pfn_vkCmdEncodeVideoKHR;
#endif
#if defined(VK_NV_memory_decompression) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_buffer_device_address))
		PFN_vkCmdDecompressMemoryNV pfn_vkCmdDecompressMemoryNV;
		PFN_vkCmdDecompressMemoryIndirectCountNV pfn_vkCmdDecompressMemoryIndirectCountNV;
#endif
#if defined(VK_NVX_binary_import)
		PFN_vkCreateCuModuleNVX pfn_vkCreateCuModuleNVX;
		PFN_vkCreateCuFunctionNVX pfn_vkCreateCuFunctionNVX;
		PFN_vkDestroyCuModuleNVX pfn_vkDestroyCuModuleNVX;
		PFN_vkDestroyCuFunctionNVX pfn_vkDestroyCuFunctionNVX;
		PFN_vkCmdCuLaunchKernelNVX pfn_vkCmdCuLaunchKernelNVX;
#endif
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_synchronization2) && defined(VK_EXT_descriptor_indexing))
		PFN_vkGetDescriptorSetLayoutSizeEXT pfn_vkGetDescriptorSetLayoutSizeEXT;
		PFN_vkGetDescriptorSetLayoutBindingOffsetEXT pfn_vkGetDescriptorSetLayoutBindingOffsetEXT;
		PFN_vkGetDescriptorEXT pfn_vkGetDescriptorEXT;
		PFN_vkCmdBindDescriptorBuffersEXT pfn_vkCmdBindDescriptorBuffersEXT;
		PFN_vkCmdSetDescriptorBufferOffsetsEXT pfn_vkCmdSetDescriptorBufferOffsetsEXT;
		PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT pfn_vkCmdBindDescriptorBufferEmbeddedSamplersEXT;
		PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT pfn_vkGetBufferOpaqueCaptureDescriptorDataEXT;
		PFN_vkGetImageOpaqueCaptureDescriptorDataEXT pfn_vkGetImageOpaqueCaptureDescriptorDataEXT;
		PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT pfn_vkGetImageViewOpaqueCaptureDescriptorDataEXT;
		PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT pfn_vkGetSamplerOpaqueCaptureDescriptorDataEXT;
#endif
#if (defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_buffer_device_address) && defined(VK_KHR_synchronization2) && defined(VK_EXT_descriptor_indexing))) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
		PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT pfn_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT;
#endif
#if defined(VK_EXT_pageable_device_local_memory) && (defined(VK_EXT_memory_priority))
		PFN_vkSetDeviceMemoryPriorityEXT pfn_vkSetDeviceMemoryPriorityEXT;
#endif
#if defined(VK_EXT_acquire_drm_display) && (defined(VK_EXT_direct_mode_display))
		PFN_vkAcquireDrmDisplayEXT pfn_vkAcquireDrmDisplayEXT;
		PFN_vkGetDrmDisplayEXT pfn_vkGetDrmDisplayEXT;
#endif
#if defined(VK_KHR_present_wait) && (defined(VK_KHR_swapchain) && defined(VK_KHR_present_id))
		PFN_vkWaitForPresentKHR pfn_vkWaitForPresentKHR;
#endif
#if defined(VK_FUCHSIA_buffer_collection) && (defined(VK_FUCHSIA_external_memory) && defined(VK_KHR_sampler_ycbcr_conversion))
		PFN_vkCreateBufferCollectionFUCHSIA pfn_vkCreateBufferCollectionFUCHSIA;
		PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA pfn_vkSetBufferCollectionBufferConstraintsFUCHSIA;
		PFN_vkSetBufferCollectionImageConstraintsFUCHSIA pfn_vkSetBufferCollectionImageConstraintsFUCHSIA;
		PFN_vkDestroyBufferCollectionFUCHSIA pfn_vkDestroyBufferCollectionFUCHSIA;
		PFN_vkGetBufferCollectionPropertiesFUCHSIA pfn_vkGetBufferCollectionPropertiesFUCHSIA;
#endif
#if defined(VK_NV_cuda_kernel_launch)
		PFN_vkCreateCudaModuleNV pfn_vkCreateCudaModuleNV;
		PFN_vkGetCudaModuleCacheNV pfn_vkGetCudaModuleCacheNV;
		PFN_vkCreateCudaFunctionNV pfn_vkCreateCudaFunctionNV;
		PFN_vkDestroyCudaModuleNV pfn_vkDestroyCudaModuleNV;
		PFN_vkDestroyCudaFunctionNV pfn_vkDestroyCudaFunctionNV;
		PFN_vkCmdCudaLaunchKernelNV pfn_vkCmdCudaLaunchKernelNV;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdBeginRendering pfn_vkCmdBeginRendering;
#endif
#if defined(VK_KHR_dynamic_rendering) && (defined(VK_KHR_depth_stencil_resolve) && defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdBeginRenderingKHR pfn_vkCmdBeginRenderingKHR;
#endif
#if defined(VK_VERSION_1_3)
		PFN_vkCmdEndRendering pfn_vkCmdEndRendering;
#endif
#if defined(VK_KHR_dynamic_rendering) && (defined(VK_KHR_depth_stencil_resolve) && defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdEndRenderingKHR pfn_vkCmdEndRenderingKHR;
#endif
#if defined(VK_VALVE_descriptor_set_host_mapping) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE pfn_vkGetDescriptorSetLayoutHostMappingInfoVALVE;
		PFN_vkGetDescriptorSetHostMappingVALVE pfn_vkGetDescriptorSetHostMappingVALVE;
#endif
#if defined(VK_EXT_opacity_micromap) && (defined(VK_KHR_acceleration_structure) && defined(VK_KHR_synchronization2))
		PFN_vkCreateMicromapEXT pfn_vkCreateMicromapEXT;
		PFN_vkCmdBuildMicromapsEXT pfn_vkCmdBuildMicromapsEXT;
		PFN_vkBuildMicromapsEXT pfn_vkBuildMicromapsEXT;
		PFN_vkDestroyMicromapEXT pfn_vkDestroyMicromapEXT;
		PFN_vkCmdCopyMicromapEXT pfn_vkCmdCopyMicromapEXT;
		PFN_vkCopyMicromapEXT pfn_vkCopyMicromapEXT;
		PFN_vkCmdCopyMicromapToMemoryEXT pfn_vkCmdCopyMicromapToMemoryEXT;
		PFN_vkCopyMicromapToMemoryEXT pfn_vkCopyMicromapToMemoryEXT;
		PFN_vkCmdCopyMemoryToMicromapEXT pfn_vkCmdCopyMemoryToMicromapEXT;
		PFN_vkCopyMemoryToMicromapEXT pfn_vkCopyMemoryToMicromapEXT;
		PFN_vkCmdWriteMicromapsPropertiesEXT pfn_vkCmdWriteMicromapsPropertiesEXT;
		PFN_vkWriteMicromapsPropertiesEXT pfn_vkWriteMicromapsPropertiesEXT;
		PFN_vkGetDeviceMicromapCompatibilityEXT pfn_vkGetDeviceMicromapCompatibilityEXT;
		PFN_vkGetMicromapBuildSizesEXT pfn_vkGetMicromapBuildSizesEXT;
#endif
#if defined(VK_EXT_shader_module_identifier) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_EXT_pipeline_creation_cache_control))
		PFN_vkGetShaderModuleIdentifierEXT pfn_vkGetShaderModuleIdentifierEXT;
		PFN_vkGetShaderModuleCreateInfoIdentifierEXT pfn_vkGetShaderModuleCreateInfoIdentifierEXT;
#endif
#if defined(VK_KHR_maintenance5) && (defined(VK_VERSION_1_1) && defined(VK_KHR_dynamic_rendering))
		PFN_vkGetImageSubresourceLayout2KHR pfn_vkGetImageSubresourceLayout2KHR;
#endif
#if (defined(VK_EXT_host_image_copy) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_copy_commands2) && defined(VK_KHR_format_feature_flags2))) || (defined(VK_EXT_image_compression_control) && (defined(VK_KHR_get_physical_device_properties2)))
		PFN_vkGetImageSubresourceLayout2EXT pfn_vkGetImageSubresourceLayout2EXT;
#endif
#if defined(VK_EXT_pipeline_properties) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPipelinePropertiesEXT pfn_vkGetPipelinePropertiesEXT;
#endif
#if defined(VK_EXT_metal_objects)
		PFN_vkExportMetalObjectsEXT pfn_vkExportMetalObjectsEXT;
#endif
#if defined(VK_QCOM_tile_properties) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetFramebufferTilePropertiesQCOM pfn_vkGetFramebufferTilePropertiesQCOM;
		PFN_vkGetDynamicRenderingTilePropertiesQCOM pfn_vkGetDynamicRenderingTilePropertiesQCOM;
#endif
#if defined(VK_NV_optical_flow) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_format_feature_flags2) && defined(VK_KHR_synchronization2))
		PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV pfn_vkGetPhysicalDeviceOpticalFlowImageFormatsNV;
		PFN_vkCreateOpticalFlowSessionNV pfn_vkCreateOpticalFlowSessionNV;
		PFN_vkDestroyOpticalFlowSessionNV pfn_vkDestroyOpticalFlowSessionNV;
		PFN_vkBindOpticalFlowSessionImageNV pfn_vkBindOpticalFlowSessionImageNV;
		PFN_vkCmdOpticalFlowExecuteNV pfn_vkCmdOpticalFlowExecuteNV;
#endif
#if defined(VK_EXT_device_fault) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetDeviceFaultInfoEXT pfn_vkGetDeviceFaultInfoEXT;
#endif
#if defined(VK_EXT_depth_bias_control) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkCmdSetDepthBias2EXT pfn_vkCmdSetDepthBias2EXT;
#endif
#if defined(VK_EXT_swapchain_maintenance1) && (defined(VK_KHR_swapchain) && defined(VK_EXT_surface_maintenance1) && defined(VK_KHR_get_physical_device_properties2))
		PFN_vkReleaseSwapchainImagesEXT pfn_vkReleaseSwapchainImagesEXT;
#endif
#if defined(VK_KHR_maintenance5) && (defined(VK_VERSION_1_1) && defined(VK_KHR_dynamic_rendering))
		PFN_vkGetDeviceImageSubresourceLayoutKHR pfn_vkGetDeviceImageSubresourceLayoutKHR;
#endif
#if defined(VK_KHR_map_memory2)
		PFN_vkMapMemory2KHR pfn_vkMapMemory2KHR;
		PFN_vkUnmapMemory2KHR pfn_vkUnmapMemory2KHR;
#endif
#if defined(VK_EXT_shader_object) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)))
		PFN_vkCreateShadersEXT pfn_vkCreateShadersEXT;
		PFN_vkDestroyShaderEXT pfn_vkDestroyShaderEXT;
		PFN_vkGetShaderBinaryDataEXT pfn_vkGetShaderBinaryDataEXT;
		PFN_vkCmdBindShadersEXT pfn_vkCmdBindShadersEXT;
#endif
#if defined(VK_QNX_external_memory_screen_buffer) && (((defined(VK_KHR_sampler_ycbcr_conversion) && defined(VK_KHR_external_memory) && defined(VK_KHR_dedicated_allocation)) || defined(VK_VERSION_1_1)) && defined(VK_EXT_queue_family_foreign))
		PFN_vkGetScreenBufferPropertiesQNX pfn_vkGetScreenBufferPropertiesQNX;
#endif
#if defined(VK_KHR_cooperative_matrix) && (defined(VK_KHR_get_physical_device_properties2))
		PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR pfn_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR;
#endif
#if defined(VK_AMDX_shader_enqueue) && (defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_synchronization2) && defined(VK_KHR_pipeline_library) && defined(VK_KHR_spirv_1_4))
		PFN_vkGetExecutionGraphPipelineScratchSizeAMDX pfn_vkGetExecutionGraphPipelineScratchSizeAMDX;
		PFN_vkGetExecutionGraphPipelineNodeIndexAMDX pfn_vkGetExecutionGraphPipelineNodeIndexAMDX;
		PFN_vkCreateExecutionGraphPipelinesAMDX pfn_vkCreateExecutionGraphPipelinesAMDX;
		PFN_vkCmdInitializeGraphScratchMemoryAMDX pfn_vkCmdInitializeGraphScratchMemoryAMDX;
		PFN_vkCmdDispatchGraphAMDX pfn_vkCmdDispatchGraphAMDX;
		PFN_vkCmdDispatchGraphIndirectAMDX pfn_vkCmdDispatchGraphIndirectAMDX;
		PFN_vkCmdDispatchGraphIndirectCountAMDX pfn_vkCmdDispatchGraphIndirectCountAMDX;
#endif
#if defined(VK_KHR_maintenance6) && (defined(VK_VERSION_1_1))
		PFN_vkCmdBindDescriptorSets2KHR pfn_vkCmdBindDescriptorSets2KHR;
		PFN_vkCmdPushConstants2KHR pfn_vkCmdPushConstants2KHR;
#endif
#if (defined(VK_KHR_maintenance6) && (defined(VK_VERSION_1_1))) && (defined(VK_KHR_push_descriptor))
		PFN_vkCmdPushDescriptorSet2KHR pfn_vkCmdPushDescriptorSet2KHR;
		PFN_vkCmdPushDescriptorSetWithTemplate2KHR pfn_vkCmdPushDescriptorSetWithTemplate2KHR;
#endif
#if (defined(VK_KHR_maintenance6) && (defined(VK_VERSION_1_1))) && (defined(VK_EXT_descriptor_buffer))
		PFN_vkCmdSetDescriptorBufferOffsets2EXT pfn_vkCmdSetDescriptorBufferOffsets2EXT;
		PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT pfn_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT;
#endif
#if defined(VK_NV_low_latency2) && (defined(VK_VERSION_1_2) || defined(VK_KHR_timeline_semaphore))
		PFN_vkSetLatencySleepModeNV pfn_vkSetLatencySleepModeNV;
		PFN_vkLatencySleepNV pfn_vkLatencySleepNV;
		PFN_vkSetLatencyMarkerNV pfn_vkSetLatencyMarkerNV;
		PFN_vkGetLatencyTimingsNV pfn_vkGetLatencyTimingsNV;
		PFN_vkQueueNotifyOutOfBandNV pfn_vkQueueNotifyOutOfBandNV;
#endif
#if defined(VK_KHR_dynamic_rendering_local_read) && (defined(VK_KHR_dynamic_rendering))
		PFN_vkCmdSetRenderingAttachmentLocationsKHR pfn_vkCmdSetRenderingAttachmentLocationsKHR;
		PFN_vkCmdSetRenderingInputAttachmentIndicesKHR pfn_vkCmdSetRenderingInputAttachmentIndicesKHR;
#endif
	};
}

#include "Loader.hpp"

#if defined(WFE_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(WFE_PLATFORM_LINUX)
#define VK_USE_PLATFORM_XLIB_KHR
#include <X11/Xlib.h>
#endif

#include <vulkan/vulkan.h>

static PFN_vkCreateInstance pfn_vkCreateInstance;
static PFN_vkDestroyInstance pfn_vkDestroyInstance;
static PFN_vkEnumeratePhysicalDevices pfn_vkEnumeratePhysicalDevices;
static PFN_vkGetDeviceProcAddr pfn_vkGetDeviceProcAddr;
static PFN_vkGetInstanceProcAddr pfn_vkGetInstanceProcAddr;
static PFN_vkGetPhysicalDeviceProperties pfn_vkGetPhysicalDeviceProperties;
static PFN_vkGetPhysicalDeviceQueueFamilyProperties pfn_vkGetPhysicalDeviceQueueFamilyProperties;
static PFN_vkGetPhysicalDeviceMemoryProperties pfn_vkGetPhysicalDeviceMemoryProperties;
static PFN_vkGetPhysicalDeviceFeatures pfn_vkGetPhysicalDeviceFeatures;
static PFN_vkGetPhysicalDeviceFormatProperties pfn_vkGetPhysicalDeviceFormatProperties;
static PFN_vkGetPhysicalDeviceImageFormatProperties pfn_vkGetPhysicalDeviceImageFormatProperties;
static PFN_vkCreateDevice pfn_vkCreateDevice;
static PFN_vkDestroyDevice pfn_vkDestroyDevice;
static PFN_vkEnumerateInstanceLayerProperties pfn_vkEnumerateInstanceLayerProperties;
static PFN_vkEnumerateInstanceExtensionProperties pfn_vkEnumerateInstanceExtensionProperties;
static PFN_vkEnumerateDeviceLayerProperties pfn_vkEnumerateDeviceLayerProperties;
static PFN_vkEnumerateDeviceExtensionProperties pfn_vkEnumerateDeviceExtensionProperties;
static PFN_vkGetDeviceQueue pfn_vkGetDeviceQueue;
static PFN_vkQueueSubmit pfn_vkQueueSubmit;
static PFN_vkQueueWaitIdle pfn_vkQueueWaitIdle;
static PFN_vkDeviceWaitIdle pfn_vkDeviceWaitIdle;
static PFN_vkAllocateMemory pfn_vkAllocateMemory;
static PFN_vkFreeMemory pfn_vkFreeMemory;
static PFN_vkMapMemory pfn_vkMapMemory;
static PFN_vkUnmapMemory pfn_vkUnmapMemory;
static PFN_vkFlushMappedMemoryRanges pfn_vkFlushMappedMemoryRanges;
static PFN_vkInvalidateMappedMemoryRanges pfn_vkInvalidateMappedMemoryRanges;
static PFN_vkGetDeviceMemoryCommitment pfn_vkGetDeviceMemoryCommitment;
static PFN_vkGetBufferMemoryRequirements pfn_vkGetBufferMemoryRequirements;
static PFN_vkBindBufferMemory pfn_vkBindBufferMemory;
static PFN_vkGetImageMemoryRequirements pfn_vkGetImageMemoryRequirements;
static PFN_vkBindImageMemory pfn_vkBindImageMemory;
static PFN_vkGetImageSparseMemoryRequirements pfn_vkGetImageSparseMemoryRequirements;
static PFN_vkGetPhysicalDeviceSparseImageFormatProperties pfn_vkGetPhysicalDeviceSparseImageFormatProperties;
static PFN_vkQueueBindSparse pfn_vkQueueBindSparse;
static PFN_vkCreateFence pfn_vkCreateFence;
static PFN_vkDestroyFence pfn_vkDestroyFence;
static PFN_vkResetFences pfn_vkResetFences;
static PFN_vkGetFenceStatus pfn_vkGetFenceStatus;
static PFN_vkWaitForFences pfn_vkWaitForFences;
static PFN_vkCreateSemaphore pfn_vkCreateSemaphore;
static PFN_vkDestroySemaphore pfn_vkDestroySemaphore;
static PFN_vkCreateEvent pfn_vkCreateEvent;
static PFN_vkDestroyEvent pfn_vkDestroyEvent;
static PFN_vkGetEventStatus pfn_vkGetEventStatus;
static PFN_vkSetEvent pfn_vkSetEvent;
static PFN_vkResetEvent pfn_vkResetEvent;
static PFN_vkCreateQueryPool pfn_vkCreateQueryPool;
static PFN_vkDestroyQueryPool pfn_vkDestroyQueryPool;
static PFN_vkGetQueryPoolResults pfn_vkGetQueryPoolResults;
static PFN_vkCreateBuffer pfn_vkCreateBuffer;
static PFN_vkDestroyBuffer pfn_vkDestroyBuffer;
static PFN_vkCreateBufferView pfn_vkCreateBufferView;
static PFN_vkDestroyBufferView pfn_vkDestroyBufferView;
static PFN_vkCreateImage pfn_vkCreateImage;
static PFN_vkDestroyImage pfn_vkDestroyImage;
static PFN_vkGetImageSubresourceLayout pfn_vkGetImageSubresourceLayout;
static PFN_vkCreateImageView pfn_vkCreateImageView;
static PFN_vkDestroyImageView pfn_vkDestroyImageView;
static PFN_vkCreateShaderModule pfn_vkCreateShaderModule;
static PFN_vkDestroyShaderModule pfn_vkDestroyShaderModule;
static PFN_vkCreatePipelineCache pfn_vkCreatePipelineCache;
static PFN_vkDestroyPipelineCache pfn_vkDestroyPipelineCache;
static PFN_vkGetPipelineCacheData pfn_vkGetPipelineCacheData;
static PFN_vkMergePipelineCaches pfn_vkMergePipelineCaches;
static PFN_vkCreateGraphicsPipelines pfn_vkCreateGraphicsPipelines;
static PFN_vkCreateComputePipelines pfn_vkCreateComputePipelines;
static PFN_vkDestroyPipeline pfn_vkDestroyPipeline;
static PFN_vkCreatePipelineLayout pfn_vkCreatePipelineLayout;
static PFN_vkDestroyPipelineLayout pfn_vkDestroyPipelineLayout;
static PFN_vkCreateSampler pfn_vkCreateSampler;
static PFN_vkDestroySampler pfn_vkDestroySampler;
static PFN_vkCreateDescriptorSetLayout pfn_vkCreateDescriptorSetLayout;
static PFN_vkDestroyDescriptorSetLayout pfn_vkDestroyDescriptorSetLayout;
static PFN_vkCreateDescriptorPool pfn_vkCreateDescriptorPool;
static PFN_vkDestroyDescriptorPool pfn_vkDestroyDescriptorPool;
static PFN_vkResetDescriptorPool pfn_vkResetDescriptorPool;
static PFN_vkAllocateDescriptorSets pfn_vkAllocateDescriptorSets;
static PFN_vkFreeDescriptorSets pfn_vkFreeDescriptorSets;
static PFN_vkUpdateDescriptorSets pfn_vkUpdateDescriptorSets;
static PFN_vkCreateFramebuffer pfn_vkCreateFramebuffer;
static PFN_vkDestroyFramebuffer pfn_vkDestroyFramebuffer;
static PFN_vkCreateRenderPass pfn_vkCreateRenderPass;
static PFN_vkDestroyRenderPass pfn_vkDestroyRenderPass;
static PFN_vkGetRenderAreaGranularity pfn_vkGetRenderAreaGranularity;
static PFN_vkCreateCommandPool pfn_vkCreateCommandPool;
static PFN_vkDestroyCommandPool pfn_vkDestroyCommandPool;
static PFN_vkResetCommandPool pfn_vkResetCommandPool;
static PFN_vkAllocateCommandBuffers pfn_vkAllocateCommandBuffers;
static PFN_vkFreeCommandBuffers pfn_vkFreeCommandBuffers;
static PFN_vkBeginCommandBuffer pfn_vkBeginCommandBuffer;
static PFN_vkEndCommandBuffer pfn_vkEndCommandBuffer;
static PFN_vkResetCommandBuffer pfn_vkResetCommandBuffer;
static PFN_vkCmdBindPipeline pfn_vkCmdBindPipeline;
static PFN_vkCmdSetViewport pfn_vkCmdSetViewport;
static PFN_vkCmdSetScissor pfn_vkCmdSetScissor;
static PFN_vkCmdSetLineWidth pfn_vkCmdSetLineWidth;
static PFN_vkCmdSetDepthBias pfn_vkCmdSetDepthBias;
static PFN_vkCmdSetBlendConstants pfn_vkCmdSetBlendConstants;
static PFN_vkCmdSetDepthBounds pfn_vkCmdSetDepthBounds;
static PFN_vkCmdSetStencilCompareMask pfn_vkCmdSetStencilCompareMask;
static PFN_vkCmdSetStencilWriteMask pfn_vkCmdSetStencilWriteMask;
static PFN_vkCmdSetStencilReference pfn_vkCmdSetStencilReference;
static PFN_vkCmdBindDescriptorSets pfn_vkCmdBindDescriptorSets;
static PFN_vkCmdBindIndexBuffer pfn_vkCmdBindIndexBuffer;
static PFN_vkCmdBindVertexBuffers pfn_vkCmdBindVertexBuffers;
static PFN_vkCmdDraw pfn_vkCmdDraw;
static PFN_vkCmdDrawIndexed pfn_vkCmdDrawIndexed;
static PFN_vkCmdDrawIndirect pfn_vkCmdDrawIndirect;
static PFN_vkCmdDrawIndexedIndirect pfn_vkCmdDrawIndexedIndirect;
static PFN_vkCmdDispatch pfn_vkCmdDispatch;
static PFN_vkCmdDispatchIndirect pfn_vkCmdDispatchIndirect;
static PFN_vkCmdCopyBuffer pfn_vkCmdCopyBuffer;
static PFN_vkCmdCopyImage pfn_vkCmdCopyImage;
static PFN_vkCmdBlitImage pfn_vkCmdBlitImage;
static PFN_vkCmdCopyBufferToImage pfn_vkCmdCopyBufferToImage;
static PFN_vkCmdCopyImageToBuffer pfn_vkCmdCopyImageToBuffer;
static PFN_vkCmdUpdateBuffer pfn_vkCmdUpdateBuffer;
static PFN_vkCmdFillBuffer pfn_vkCmdFillBuffer;
static PFN_vkCmdClearColorImage pfn_vkCmdClearColorImage;
static PFN_vkCmdClearDepthStencilImage pfn_vkCmdClearDepthStencilImage;
static PFN_vkCmdClearAttachments pfn_vkCmdClearAttachments;
static PFN_vkCmdResolveImage pfn_vkCmdResolveImage;
static PFN_vkCmdSetEvent pfn_vkCmdSetEvent;
static PFN_vkCmdResetEvent pfn_vkCmdResetEvent;
static PFN_vkCmdWaitEvents pfn_vkCmdWaitEvents;
static PFN_vkCmdPipelineBarrier pfn_vkCmdPipelineBarrier;
static PFN_vkCmdBeginQuery pfn_vkCmdBeginQuery;
static PFN_vkCmdEndQuery pfn_vkCmdEndQuery;
static PFN_vkCmdResetQueryPool pfn_vkCmdResetQueryPool;
static PFN_vkCmdWriteTimestamp pfn_vkCmdWriteTimestamp;
static PFN_vkCmdCopyQueryPoolResults pfn_vkCmdCopyQueryPoolResults;
static PFN_vkCmdPushConstants pfn_vkCmdPushConstants;
static PFN_vkCmdBeginRenderPass pfn_vkCmdBeginRenderPass;
static PFN_vkCmdNextSubpass pfn_vkCmdNextSubpass;
static PFN_vkCmdEndRenderPass pfn_vkCmdEndRenderPass;
static PFN_vkCmdExecuteCommands pfn_vkCmdExecuteCommands;
static PFN_vkDestroySurfaceKHR pfn_vkDestroySurfaceKHR;
static PFN_vkGetPhysicalDeviceSurfaceSupportKHR pfn_vkGetPhysicalDeviceSurfaceSupportKHR;
static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR pfn_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR pfn_vkGetPhysicalDeviceSurfaceFormatsKHR;
static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR pfn_vkGetPhysicalDeviceSurfacePresentModesKHR;
static PFN_vkCreateSwapchainKHR pfn_vkCreateSwapchainKHR;
static PFN_vkDestroySwapchainKHR pfn_vkDestroySwapchainKHR;
static PFN_vkGetSwapchainImagesKHR pfn_vkGetSwapchainImagesKHR;
static PFN_vkAcquireNextImageKHR pfn_vkAcquireNextImageKHR;
static PFN_vkQueuePresentKHR pfn_vkQueuePresentKHR;

#if defined(WFE_PLATFORM_WINDOWS)
static PFN_vkCreateWin32SurfaceKHR pfn_vkCreateWin32SurfaceKHR;
static PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR pfn_vkGetPhysicalDeviceWin32PresentationSupportKHR;
#elif defined(WFE_PLATFORM_LINUX)
static PFN_vkCreateXlibSurfaceKHR pfn_vkCreateXlibSurfaceKHR;
static PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR pfn_vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif

extern "C" {

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {
	return pfn_vkCreateInstance(pCreateInfo, pAllocator, pInstance);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyInstance(instance, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) {
	return pfn_vkEnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
}
VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName) {
	return pfn_vkGetDeviceProcAddr(device, pName);
}
VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName) {
	return pfn_vkGetInstanceProcAddr(instance, pName);
}
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) {
	pfn_vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) {
	pfn_vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) {
	pfn_vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) {
	pfn_vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) {
	pfn_vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) {
	return pfn_vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
	return pfn_vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyDevice(device, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
	return pfn_vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
	return pfn_vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
}
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
	return pfn_vkEnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
}
VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
	return pfn_vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
}
VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) {
	pfn_vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
}
VKAPI_ATTR VkResult VKAPI_CALL vkQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) {
	return pfn_vkQueueSubmit(queue, submitCount, pSubmits, fence);
}
VKAPI_ATTR VkResult VKAPI_CALL vkQueueWaitIdle(VkQueue queue) {
	return pfn_vkQueueWaitIdle(queue);
}
VKAPI_ATTR VkResult VKAPI_CALL vkDeviceWaitIdle(VkDevice device) {
	return pfn_vkDeviceWaitIdle(device);
}
VKAPI_ATTR VkResult VKAPI_CALL vkAllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) {
	return pfn_vkAllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
}
VKAPI_ATTR void VKAPI_CALL vkFreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator) {
	pfn_vkFreeMemory(device, memory, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkMapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) {
	return pfn_vkMapMemory(device, memory, offset, size, flags, ppData);
}
VKAPI_ATTR void VKAPI_CALL vkUnmapMemory(VkDevice device, VkDeviceMemory memory) {
	pfn_vkUnmapMemory(device, memory);
}
VKAPI_ATTR VkResult VKAPI_CALL vkFlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) {
	return pfn_vkFlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}
VKAPI_ATTR VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) {
	return pfn_vkInvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}
VKAPI_ATTR void VKAPI_CALL vkGetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) {
	pfn_vkGetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
}
VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) {
	pfn_vkGetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
}
VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
	return pfn_vkBindBufferMemory(device, buffer, memory, memoryOffset);
}
VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements) {
	pfn_vkGetImageMemoryRequirements(device, image, pMemoryRequirements);
}
VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) {
	return pfn_vkBindImageMemory(device, image, memory, memoryOffset);
}
VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) {
	pfn_vkGetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) {
	pfn_vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}
VKAPI_ATTR VkResult VKAPI_CALL vkQueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) {
	return pfn_vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence) {
	return pfn_vkCreateFence(device, pCreateInfo, pAllocator, pFence);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyFence(device, fence, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences) {
	return pfn_vkResetFences(device, fenceCount, pFences);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetFenceStatus(VkDevice device, VkFence fence) {
	return pfn_vkGetFenceStatus(device, fence);
}
VKAPI_ATTR VkResult VKAPI_CALL vkWaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) {
	return pfn_vkWaitForFences(device, fenceCount, pFences, waitAll, timeout);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) {
	return pfn_vkCreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
}
VKAPI_ATTR void VKAPI_CALL vkDestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroySemaphore(device, semaphore, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) {
	return pfn_vkCreateEvent(device, pCreateInfo, pAllocator, pEvent);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyEvent(device, event, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetEventStatus(VkDevice device, VkEvent event) {
	return pfn_vkGetEventStatus(device, event);
}
VKAPI_ATTR VkResult VKAPI_CALL vkSetEvent(VkDevice device, VkEvent event) {
	return pfn_vkSetEvent(device, event);
}
VKAPI_ATTR VkResult VKAPI_CALL vkResetEvent(VkDevice device, VkEvent event) {
	return pfn_vkResetEvent(device, event);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) {
	return pfn_vkCreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyQueryPool(device, queryPool, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) {
	return pfn_vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) {
	return pfn_vkCreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyBuffer(device, buffer, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView) {
	return pfn_vkCreateBufferView(device, pCreateInfo, pAllocator, pView);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyBufferView(device, bufferView, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage) {
	return pfn_vkCreateImage(device, pCreateInfo, pAllocator, pImage);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyImage(device, image, pAllocator);
}
VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) {
	pfn_vkGetImageSubresourceLayout(device, image, pSubresource, pLayout);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView) {
	return pfn_vkCreateImageView(device, pCreateInfo, pAllocator, pView);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyImageView(device, imageView, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) {
	return pfn_vkCreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyShaderModule(device, shaderModule, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) {
	return pfn_vkCreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyPipelineCache(device, pipelineCache, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) {
	return pfn_vkGetPipelineCacheData(device, pipelineCache, pDataSize, pData);
}
VKAPI_ATTR VkResult VKAPI_CALL vkMergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) {
	return pfn_vkMergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
	return pfn_vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
	return pfn_vkCreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyPipeline(device, pipeline, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) {
	return pfn_vkCreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyPipelineLayout(device, pipelineLayout, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) {
	return pfn_vkCreateSampler(device, pCreateInfo, pAllocator, pSampler);
}
VKAPI_ATTR void VKAPI_CALL vkDestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroySampler(device, sampler, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout) {
	return pfn_vkCreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) {
	return pfn_vkCreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyDescriptorPool(device, descriptorPool, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) {
	return pfn_vkResetDescriptorPool(device, descriptorPool, flags);
}
VKAPI_ATTR VkResult VKAPI_CALL vkAllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) {
	return pfn_vkAllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
}
VKAPI_ATTR VkResult VKAPI_CALL vkFreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) {
	return pfn_vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
}
VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) {
	pfn_vkUpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) {
	return pfn_vkCreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyFramebuffer(device, framebuffer, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
	return pfn_vkCreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyRenderPass(device, renderPass, pAllocator);
}
VKAPI_ATTR void VKAPI_CALL vkGetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity) {
	pfn_vkGetRenderAreaGranularity(device, renderPass, pGranularity);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) {
	return pfn_vkCreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
}
VKAPI_ATTR void VKAPI_CALL vkDestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroyCommandPool(device, commandPool, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags) {
	return pfn_vkResetCommandPool(device, commandPool, flags);
}
VKAPI_ATTR VkResult VKAPI_CALL vkAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) {
	return pfn_vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
}
VKAPI_ATTR void VKAPI_CALL vkFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
	pfn_vkFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
}
VKAPI_ATTR VkResult VKAPI_CALL vkBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) {
	return pfn_vkBeginCommandBuffer(commandBuffer, pBeginInfo);
}
VKAPI_ATTR VkResult VKAPI_CALL vkEndCommandBuffer(VkCommandBuffer commandBuffer) {
	return pfn_vkEndCommandBuffer(commandBuffer);
}
VKAPI_ATTR VkResult VKAPI_CALL vkResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {
	return pfn_vkResetCommandBuffer(commandBuffer, flags);
}
VKAPI_ATTR void VKAPI_CALL vkCmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) {
	pfn_vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) {
	pfn_vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) {
	pfn_vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) {
	pfn_vkCmdSetLineWidth(commandBuffer, lineWidth);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) {
	pfn_vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) {
	pfn_vkCmdSetBlendConstants(commandBuffer, blendConstants);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) {
	pfn_vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) {
	pfn_vkCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) {
	pfn_vkCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) {
	pfn_vkCmdSetStencilReference(commandBuffer, faceMask, reference);
}
VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) {
	pfn_vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
VKAPI_ATTR void VKAPI_CALL vkCmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) {
	pfn_vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}
VKAPI_ATTR void VKAPI_CALL vkCmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
	pfn_vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}
VKAPI_ATTR void VKAPI_CALL vkCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
	pfn_vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
	pfn_vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
	pfn_vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
	pfn_vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
VKAPI_ATTR void VKAPI_CALL vkCmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
	pfn_vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}
VKAPI_ATTR void VKAPI_CALL vkCmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) {
	pfn_vkCmdDispatchIndirect(commandBuffer, buffer, offset);
}
VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) {
	pfn_vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) {
	pfn_vkCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) {
	pfn_vkCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}
VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
	pfn_vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}
VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) {
	pfn_vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}
VKAPI_ATTR void VKAPI_CALL vkCmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
	pfn_vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}
VKAPI_ATTR void VKAPI_CALL vkCmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
	pfn_vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}
VKAPI_ATTR void VKAPI_CALL vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
	pfn_vkCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}
VKAPI_ATTR void VKAPI_CALL vkCmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
	pfn_vkCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}
VKAPI_ATTR void VKAPI_CALL vkCmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
	pfn_vkCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}
VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) {
	pfn_vkCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
	pfn_vkCmdSetEvent(commandBuffer, event, stageMask);
}
VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) {
	pfn_vkCmdResetEvent(commandBuffer, event, stageMask);
}
VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
	pfn_vkCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
VKAPI_ATTR void VKAPI_CALL vkCmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) {
	pfn_vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
VKAPI_ATTR void VKAPI_CALL vkCmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) {
	pfn_vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
}
VKAPI_ATTR void VKAPI_CALL vkCmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
	pfn_vkCmdEndQuery(commandBuffer, queryPool, query);
}
VKAPI_ATTR void VKAPI_CALL vkCmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) {
	pfn_vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}
VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) {
	pfn_vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}
VKAPI_ATTR void VKAPI_CALL vkCmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) {
	pfn_vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}
VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
	pfn_vkCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}
VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
	pfn_vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}
VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) {
	pfn_vkCmdNextSubpass(commandBuffer, contents);
}
VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass(VkCommandBuffer commandBuffer) {
	pfn_vkCmdEndRenderPass(commandBuffer);
}
VKAPI_ATTR void VKAPI_CALL vkCmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {
	pfn_vkCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}
VKAPI_ATTR void VKAPI_CALL vkDestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroySurfaceKHR(instance, surface, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) {
	return pfn_vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {
	return pfn_vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) {
	return pfn_vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) {
	return pfn_vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
}
VKAPI_ATTR VkResult VKAPI_CALL vkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
	return pfn_vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
}
VKAPI_ATTR void VKAPI_CALL vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) {
	pfn_vkDestroySwapchainKHR(device, swapchain, pAllocator);
}
VKAPI_ATTR VkResult VKAPI_CALL vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) {
	return pfn_vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
}
VKAPI_ATTR VkResult VKAPI_CALL vkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {
	return pfn_vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
}
VKAPI_ATTR VkResult VKAPI_CALL vkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
	return pfn_vkQueuePresentKHR(queue, pPresentInfo);
}

#if defined(WFE_PLATFORM_WINDOWS)
VKAPI_ATTR VkResult VKAPI_CALL vkCreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
	return pfn_vkCreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceWin32PresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) {
	return pfn_vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
}
#elif defined(WFE_PLATFORM_LINUX)
VKAPI_ATTR VkResult VKAPI_CALL vkCreateXlibSurfaceKHR(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
	return pfn_vkCreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VKAPI_ATTR VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) {
	return pfn_vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
}
#endif

}

namespace wfe {
	// Internal variables
	DynamicLib vulkanLib;
	
	// Public functions
	bool8_t LoadVulkanFunctions() {
		// Load the Vulkan dynamic lib
#if defined(WFE_PLATFORM_WINDOWS)
		bool8_t libLoaded = vulkanLib.LoadLib("vulkan-1.dll");
#elif defined(WFE_PLATFORM_LINUX)
		bool8_t libLoaded = vulkanLib.LoadLib("libvulkan.so");
#endif

		if(!libLoaded)
			return false;

		pfn_vkCreateInstance = (PFN_vkCreateInstance)vulkanLib.LoadFunc("vkCreateInstance");
		if(!pfn_vkCreateInstance) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyInstance = (PFN_vkDestroyInstance)vulkanLib.LoadFunc("vkDestroyInstance");
		if(!pfn_vkDestroyInstance) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)vulkanLib.LoadFunc("vkEnumeratePhysicalDevices");
		if(!pfn_vkEnumeratePhysicalDevices) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vulkanLib.LoadFunc("vkGetDeviceProcAddr");
		if(!pfn_vkGetDeviceProcAddr) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)vulkanLib.LoadFunc("vkGetInstanceProcAddr");
		if(!pfn_vkGetInstanceProcAddr) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)vulkanLib.LoadFunc("vkGetPhysicalDeviceProperties");
		if(!pfn_vkGetPhysicalDeviceProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)vulkanLib.LoadFunc("vkGetPhysicalDeviceQueueFamilyProperties");
		if(!pfn_vkGetPhysicalDeviceQueueFamilyProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)vulkanLib.LoadFunc("vkGetPhysicalDeviceMemoryProperties");
		if(!pfn_vkGetPhysicalDeviceMemoryProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)vulkanLib.LoadFunc("vkGetPhysicalDeviceFeatures");
		if(!pfn_vkGetPhysicalDeviceFeatures) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)vulkanLib.LoadFunc("vkGetPhysicalDeviceFormatProperties");
		if(!pfn_vkGetPhysicalDeviceFormatProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)vulkanLib.LoadFunc("vkGetPhysicalDeviceImageFormatProperties");
		if(!pfn_vkGetPhysicalDeviceImageFormatProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateDevice = (PFN_vkCreateDevice)vulkanLib.LoadFunc("vkCreateDevice");
		if(!pfn_vkCreateDevice) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyDevice = (PFN_vkDestroyDevice)vulkanLib.LoadFunc("vkDestroyDevice");
		if(!pfn_vkDestroyDevice) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)vulkanLib.LoadFunc("vkEnumerateInstanceLayerProperties");
		if(!pfn_vkEnumerateInstanceLayerProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)vulkanLib.LoadFunc("vkEnumerateInstanceExtensionProperties");
		if(!pfn_vkEnumerateInstanceExtensionProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)vulkanLib.LoadFunc("vkEnumerateDeviceLayerProperties");
		if(!pfn_vkEnumerateDeviceLayerProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)vulkanLib.LoadFunc("vkEnumerateDeviceExtensionProperties");
		if(!pfn_vkEnumerateDeviceExtensionProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetDeviceQueue = (PFN_vkGetDeviceQueue)vulkanLib.LoadFunc("vkGetDeviceQueue");
		if(!pfn_vkGetDeviceQueue) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkQueueSubmit = (PFN_vkQueueSubmit)vulkanLib.LoadFunc("vkQueueSubmit");
		if(!pfn_vkQueueSubmit) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkQueueWaitIdle = (PFN_vkQueueWaitIdle)vulkanLib.LoadFunc("vkQueueWaitIdle");
		if(!pfn_vkQueueWaitIdle) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)vulkanLib.LoadFunc("vkDeviceWaitIdle");
		if(!pfn_vkDeviceWaitIdle) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkAllocateMemory = (PFN_vkAllocateMemory)vulkanLib.LoadFunc("vkAllocateMemory");
		if(!pfn_vkAllocateMemory) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkFreeMemory = (PFN_vkFreeMemory)vulkanLib.LoadFunc("vkFreeMemory");
		if(!pfn_vkFreeMemory) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkMapMemory = (PFN_vkMapMemory)vulkanLib.LoadFunc("vkMapMemory");
		if(!pfn_vkMapMemory) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkUnmapMemory = (PFN_vkUnmapMemory)vulkanLib.LoadFunc("vkUnmapMemory");
		if(!pfn_vkUnmapMemory) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)vulkanLib.LoadFunc("vkFlushMappedMemoryRanges");
		if(!pfn_vkFlushMappedMemoryRanges) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkInvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)vulkanLib.LoadFunc("vkInvalidateMappedMemoryRanges");
		if(!pfn_vkInvalidateMappedMemoryRanges) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment)vulkanLib.LoadFunc("vkGetDeviceMemoryCommitment");
		if(!pfn_vkGetDeviceMemoryCommitment) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)vulkanLib.LoadFunc("vkGetBufferMemoryRequirements");
		if(!pfn_vkGetBufferMemoryRequirements) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkBindBufferMemory = (PFN_vkBindBufferMemory)vulkanLib.LoadFunc("vkBindBufferMemory");
		if(!pfn_vkBindBufferMemory) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)vulkanLib.LoadFunc("vkGetImageMemoryRequirements");
		if(!pfn_vkGetImageMemoryRequirements) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkBindImageMemory = (PFN_vkBindImageMemory)vulkanLib.LoadFunc("vkBindImageMemory");
		if(!pfn_vkBindImageMemory) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements)vulkanLib.LoadFunc("vkGetImageSparseMemoryRequirements");
		if(!pfn_vkGetImageSparseMemoryRequirements) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)vulkanLib.LoadFunc("vkGetPhysicalDeviceSparseImageFormatProperties");
		if(!pfn_vkGetPhysicalDeviceSparseImageFormatProperties) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkQueueBindSparse = (PFN_vkQueueBindSparse)vulkanLib.LoadFunc("vkQueueBindSparse");
		if(!pfn_vkQueueBindSparse) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateFence = (PFN_vkCreateFence)vulkanLib.LoadFunc("vkCreateFence");
		if(!pfn_vkCreateFence) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyFence = (PFN_vkDestroyFence)vulkanLib.LoadFunc("vkDestroyFence");
		if(!pfn_vkDestroyFence) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkResetFences = (PFN_vkResetFences)vulkanLib.LoadFunc("vkResetFences");
		if(!pfn_vkResetFences) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetFenceStatus = (PFN_vkGetFenceStatus)vulkanLib.LoadFunc("vkGetFenceStatus");
		if(!pfn_vkGetFenceStatus) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkWaitForFences = (PFN_vkWaitForFences)vulkanLib.LoadFunc("vkWaitForFences");
		if(!pfn_vkWaitForFences) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateSemaphore = (PFN_vkCreateSemaphore)vulkanLib.LoadFunc("vkCreateSemaphore");
		if(!pfn_vkCreateSemaphore) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroySemaphore = (PFN_vkDestroySemaphore)vulkanLib.LoadFunc("vkDestroySemaphore");
		if(!pfn_vkDestroySemaphore) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateEvent = (PFN_vkCreateEvent)vulkanLib.LoadFunc("vkCreateEvent");
		if(!pfn_vkCreateEvent) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyEvent = (PFN_vkDestroyEvent)vulkanLib.LoadFunc("vkDestroyEvent");
		if(!pfn_vkDestroyEvent) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetEventStatus = (PFN_vkGetEventStatus)vulkanLib.LoadFunc("vkGetEventStatus");
		if(!pfn_vkGetEventStatus) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkSetEvent = (PFN_vkSetEvent)vulkanLib.LoadFunc("vkSetEvent");
		if(!pfn_vkSetEvent) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkResetEvent = (PFN_vkResetEvent)vulkanLib.LoadFunc("vkResetEvent");
		if(!pfn_vkResetEvent) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateQueryPool = (PFN_vkCreateQueryPool)vulkanLib.LoadFunc("vkCreateQueryPool");
		if(!pfn_vkCreateQueryPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyQueryPool = (PFN_vkDestroyQueryPool)vulkanLib.LoadFunc("vkDestroyQueryPool");
		if(!pfn_vkDestroyQueryPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetQueryPoolResults = (PFN_vkGetQueryPoolResults)vulkanLib.LoadFunc("vkGetQueryPoolResults");
		if(!pfn_vkGetQueryPoolResults) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateBuffer = (PFN_vkCreateBuffer)vulkanLib.LoadFunc("vkCreateBuffer");
		if(!pfn_vkCreateBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyBuffer = (PFN_vkDestroyBuffer)vulkanLib.LoadFunc("vkDestroyBuffer");
		if(!pfn_vkDestroyBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateBufferView = (PFN_vkCreateBufferView)vulkanLib.LoadFunc("vkCreateBufferView");
		if(!pfn_vkCreateBufferView) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyBufferView = (PFN_vkDestroyBufferView)vulkanLib.LoadFunc("vkDestroyBufferView");
		if(!pfn_vkDestroyBufferView) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateImage = (PFN_vkCreateImage)vulkanLib.LoadFunc("vkCreateImage");
		if(!pfn_vkCreateImage) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyImage = (PFN_vkDestroyImage)vulkanLib.LoadFunc("vkDestroyImage");
		if(!pfn_vkDestroyImage) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout)vulkanLib.LoadFunc("vkGetImageSubresourceLayout");
		if(!pfn_vkGetImageSubresourceLayout) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateImageView = (PFN_vkCreateImageView)vulkanLib.LoadFunc("vkCreateImageView");
		if(!pfn_vkCreateImageView) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyImageView = (PFN_vkDestroyImageView)vulkanLib.LoadFunc("vkDestroyImageView");
		if(!pfn_vkDestroyImageView) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateShaderModule = (PFN_vkCreateShaderModule)vulkanLib.LoadFunc("vkCreateShaderModule");
		if(!pfn_vkCreateShaderModule) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyShaderModule = (PFN_vkDestroyShaderModule)vulkanLib.LoadFunc("vkDestroyShaderModule");
		if(!pfn_vkDestroyShaderModule) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreatePipelineCache = (PFN_vkCreatePipelineCache)vulkanLib.LoadFunc("vkCreatePipelineCache");
		if(!pfn_vkCreatePipelineCache) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyPipelineCache = (PFN_vkDestroyPipelineCache)vulkanLib.LoadFunc("vkDestroyPipelineCache");
		if(!pfn_vkDestroyPipelineCache) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPipelineCacheData = (PFN_vkGetPipelineCacheData)vulkanLib.LoadFunc("vkGetPipelineCacheData");
		if(!pfn_vkGetPipelineCacheData) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkMergePipelineCaches = (PFN_vkMergePipelineCaches)vulkanLib.LoadFunc("vkMergePipelineCaches");
		if(!pfn_vkMergePipelineCaches) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)vulkanLib.LoadFunc("vkCreateGraphicsPipelines");
		if(!pfn_vkCreateGraphicsPipelines) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateComputePipelines = (PFN_vkCreateComputePipelines)vulkanLib.LoadFunc("vkCreateComputePipelines");
		if(!pfn_vkCreateComputePipelines) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyPipeline = (PFN_vkDestroyPipeline)vulkanLib.LoadFunc("vkDestroyPipeline");
		if(!pfn_vkDestroyPipeline) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)vulkanLib.LoadFunc("vkCreatePipelineLayout");
		if(!pfn_vkCreatePipelineLayout) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)vulkanLib.LoadFunc("vkDestroyPipelineLayout");
		if(!pfn_vkDestroyPipelineLayout) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateSampler = (PFN_vkCreateSampler)vulkanLib.LoadFunc("vkCreateSampler");
		if(!pfn_vkCreateSampler) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroySampler = (PFN_vkDestroySampler)vulkanLib.LoadFunc("vkDestroySampler");
		if(!pfn_vkDestroySampler) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)vulkanLib.LoadFunc("vkCreateDescriptorSetLayout");
		if(!pfn_vkCreateDescriptorSetLayout) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)vulkanLib.LoadFunc("vkDestroyDescriptorSetLayout");
		if(!pfn_vkDestroyDescriptorSetLayout) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool)vulkanLib.LoadFunc("vkCreateDescriptorPool");
		if(!pfn_vkCreateDescriptorPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)vulkanLib.LoadFunc("vkDestroyDescriptorPool");
		if(!pfn_vkDestroyDescriptorPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkResetDescriptorPool = (PFN_vkResetDescriptorPool)vulkanLib.LoadFunc("vkResetDescriptorPool");
		if(!pfn_vkResetDescriptorPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)vulkanLib.LoadFunc("vkAllocateDescriptorSets");
		if(!pfn_vkAllocateDescriptorSets) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkFreeDescriptorSets = (PFN_vkFreeDescriptorSets)vulkanLib.LoadFunc("vkFreeDescriptorSets");
		if(!pfn_vkFreeDescriptorSets) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)vulkanLib.LoadFunc("vkUpdateDescriptorSets");
		if(!pfn_vkUpdateDescriptorSets) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateFramebuffer = (PFN_vkCreateFramebuffer)vulkanLib.LoadFunc("vkCreateFramebuffer");
		if(!pfn_vkCreateFramebuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyFramebuffer = (PFN_vkDestroyFramebuffer)vulkanLib.LoadFunc("vkDestroyFramebuffer");
		if(!pfn_vkDestroyFramebuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateRenderPass = (PFN_vkCreateRenderPass)vulkanLib.LoadFunc("vkCreateRenderPass");
		if(!pfn_vkCreateRenderPass) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyRenderPass = (PFN_vkDestroyRenderPass)vulkanLib.LoadFunc("vkDestroyRenderPass");
		if(!pfn_vkDestroyRenderPass) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity)vulkanLib.LoadFunc("vkGetRenderAreaGranularity");
		if(!pfn_vkGetRenderAreaGranularity) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateCommandPool = (PFN_vkCreateCommandPool)vulkanLib.LoadFunc("vkCreateCommandPool");
		if(!pfn_vkCreateCommandPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroyCommandPool = (PFN_vkDestroyCommandPool)vulkanLib.LoadFunc("vkDestroyCommandPool");
		if(!pfn_vkDestroyCommandPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkResetCommandPool = (PFN_vkResetCommandPool)vulkanLib.LoadFunc("vkResetCommandPool");
		if(!pfn_vkResetCommandPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)vulkanLib.LoadFunc("vkAllocateCommandBuffers");
		if(!pfn_vkAllocateCommandBuffers) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers)vulkanLib.LoadFunc("vkFreeCommandBuffers");
		if(!pfn_vkFreeCommandBuffers) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)vulkanLib.LoadFunc("vkBeginCommandBuffer");
		if(!pfn_vkBeginCommandBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkEndCommandBuffer = (PFN_vkEndCommandBuffer)vulkanLib.LoadFunc("vkEndCommandBuffer");
		if(!pfn_vkEndCommandBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkResetCommandBuffer = (PFN_vkResetCommandBuffer)vulkanLib.LoadFunc("vkResetCommandBuffer");
		if(!pfn_vkResetCommandBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdBindPipeline = (PFN_vkCmdBindPipeline)vulkanLib.LoadFunc("vkCmdBindPipeline");
		if(!pfn_vkCmdBindPipeline) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetViewport = (PFN_vkCmdSetViewport)vulkanLib.LoadFunc("vkCmdSetViewport");
		if(!pfn_vkCmdSetViewport) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetScissor = (PFN_vkCmdSetScissor)vulkanLib.LoadFunc("vkCmdSetScissor");
		if(!pfn_vkCmdSetScissor) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetLineWidth = (PFN_vkCmdSetLineWidth)vulkanLib.LoadFunc("vkCmdSetLineWidth");
		if(!pfn_vkCmdSetLineWidth) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetDepthBias = (PFN_vkCmdSetDepthBias)vulkanLib.LoadFunc("vkCmdSetDepthBias");
		if(!pfn_vkCmdSetDepthBias) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetBlendConstants = (PFN_vkCmdSetBlendConstants)vulkanLib.LoadFunc("vkCmdSetBlendConstants");
		if(!pfn_vkCmdSetBlendConstants) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetDepthBounds = (PFN_vkCmdSetDepthBounds)vulkanLib.LoadFunc("vkCmdSetDepthBounds");
		if(!pfn_vkCmdSetDepthBounds) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask)vulkanLib.LoadFunc("vkCmdSetStencilCompareMask");
		if(!pfn_vkCmdSetStencilCompareMask) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask)vulkanLib.LoadFunc("vkCmdSetStencilWriteMask");
		if(!pfn_vkCmdSetStencilWriteMask) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetStencilReference = (PFN_vkCmdSetStencilReference)vulkanLib.LoadFunc("vkCmdSetStencilReference");
		if(!pfn_vkCmdSetStencilReference) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)vulkanLib.LoadFunc("vkCmdBindDescriptorSets");
		if(!pfn_vkCmdBindDescriptorSets) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)vulkanLib.LoadFunc("vkCmdBindIndexBuffer");
		if(!pfn_vkCmdBindIndexBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)vulkanLib.LoadFunc("vkCmdBindVertexBuffers");
		if(!pfn_vkCmdBindVertexBuffers) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdDraw = (PFN_vkCmdDraw)vulkanLib.LoadFunc("vkCmdDraw");
		if(!pfn_vkCmdDraw) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)vulkanLib.LoadFunc("vkCmdDrawIndexed");
		if(!pfn_vkCmdDrawIndexed) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdDrawIndirect = (PFN_vkCmdDrawIndirect)vulkanLib.LoadFunc("vkCmdDrawIndirect");
		if(!pfn_vkCmdDrawIndirect) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect)vulkanLib.LoadFunc("vkCmdDrawIndexedIndirect");
		if(!pfn_vkCmdDrawIndexedIndirect) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdDispatch = (PFN_vkCmdDispatch)vulkanLib.LoadFunc("vkCmdDispatch");
		if(!pfn_vkCmdDispatch) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdDispatchIndirect = (PFN_vkCmdDispatchIndirect)vulkanLib.LoadFunc("vkCmdDispatchIndirect");
		if(!pfn_vkCmdDispatchIndirect) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer)vulkanLib.LoadFunc("vkCmdCopyBuffer");
		if(!pfn_vkCmdCopyBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdCopyImage = (PFN_vkCmdCopyImage)vulkanLib.LoadFunc("vkCmdCopyImage");
		if(!pfn_vkCmdCopyImage) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdBlitImage = (PFN_vkCmdBlitImage)vulkanLib.LoadFunc("vkCmdBlitImage");
		if(!pfn_vkCmdBlitImage) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)vulkanLib.LoadFunc("vkCmdCopyBufferToImage");
		if(!pfn_vkCmdCopyBufferToImage) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer)vulkanLib.LoadFunc("vkCmdCopyImageToBuffer");
		if(!pfn_vkCmdCopyImageToBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdUpdateBuffer = (PFN_vkCmdUpdateBuffer)vulkanLib.LoadFunc("vkCmdUpdateBuffer");
		if(!pfn_vkCmdUpdateBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdFillBuffer = (PFN_vkCmdFillBuffer)vulkanLib.LoadFunc("vkCmdFillBuffer");
		if(!pfn_vkCmdFillBuffer) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdClearColorImage = (PFN_vkCmdClearColorImage)vulkanLib.LoadFunc("vkCmdClearColorImage");
		if(!pfn_vkCmdClearColorImage) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage)vulkanLib.LoadFunc("vkCmdClearDepthStencilImage");
		if(!pfn_vkCmdClearDepthStencilImage) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdClearAttachments = (PFN_vkCmdClearAttachments)vulkanLib.LoadFunc("vkCmdClearAttachments");
		if(!pfn_vkCmdClearAttachments) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdResolveImage = (PFN_vkCmdResolveImage)vulkanLib.LoadFunc("vkCmdResolveImage");
		if(!pfn_vkCmdResolveImage) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdSetEvent = (PFN_vkCmdSetEvent)vulkanLib.LoadFunc("vkCmdSetEvent");
		if(!pfn_vkCmdSetEvent) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdResetEvent = (PFN_vkCmdResetEvent)vulkanLib.LoadFunc("vkCmdResetEvent");
		if(!pfn_vkCmdResetEvent) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdWaitEvents = (PFN_vkCmdWaitEvents)vulkanLib.LoadFunc("vkCmdWaitEvents");
		if(!pfn_vkCmdWaitEvents) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)vulkanLib.LoadFunc("vkCmdPipelineBarrier");
		if(!pfn_vkCmdPipelineBarrier) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdBeginQuery = (PFN_vkCmdBeginQuery)vulkanLib.LoadFunc("vkCmdBeginQuery");
		if(!pfn_vkCmdBeginQuery) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdEndQuery = (PFN_vkCmdEndQuery)vulkanLib.LoadFunc("vkCmdEndQuery");
		if(!pfn_vkCmdEndQuery) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdResetQueryPool = (PFN_vkCmdResetQueryPool)vulkanLib.LoadFunc("vkCmdResetQueryPool");
		if(!pfn_vkCmdResetQueryPool) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdWriteTimestamp = (PFN_vkCmdWriteTimestamp)vulkanLib.LoadFunc("vkCmdWriteTimestamp");
		if(!pfn_vkCmdWriteTimestamp) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults)vulkanLib.LoadFunc("vkCmdCopyQueryPoolResults");
		if(!pfn_vkCmdCopyQueryPoolResults) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdPushConstants = (PFN_vkCmdPushConstants)vulkanLib.LoadFunc("vkCmdPushConstants");
		if(!pfn_vkCmdPushConstants) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)vulkanLib.LoadFunc("vkCmdBeginRenderPass");
		if(!pfn_vkCmdBeginRenderPass) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdNextSubpass = (PFN_vkCmdNextSubpass)vulkanLib.LoadFunc("vkCmdNextSubpass");
		if(!pfn_vkCmdNextSubpass) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass)vulkanLib.LoadFunc("vkCmdEndRenderPass");
		if(!pfn_vkCmdEndRenderPass) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCmdExecuteCommands = (PFN_vkCmdExecuteCommands)vulkanLib.LoadFunc("vkCmdExecuteCommands");
		if(!pfn_vkCmdExecuteCommands) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vulkanLib.LoadFunc("vkDestroySurfaceKHR");
		if(!pfn_vkDestroySurfaceKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)vulkanLib.LoadFunc("vkGetPhysicalDeviceSurfaceSupportKHR");
		if(!pfn_vkGetPhysicalDeviceSurfaceSupportKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)vulkanLib.LoadFunc("vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
		if(!pfn_vkGetPhysicalDeviceSurfaceCapabilitiesKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)vulkanLib.LoadFunc("vkGetPhysicalDeviceSurfaceFormatsKHR");
		if(!pfn_vkGetPhysicalDeviceSurfaceFormatsKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)vulkanLib.LoadFunc("vkGetPhysicalDeviceSurfacePresentModesKHR");
		if(!pfn_vkGetPhysicalDeviceSurfacePresentModesKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)vulkanLib.LoadFunc("vkCreateSwapchainKHR");
		if(!pfn_vkCreateSwapchainKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vulkanLib.LoadFunc("vkDestroySwapchainKHR");
		if(!pfn_vkDestroySwapchainKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vulkanLib.LoadFunc("vkGetSwapchainImagesKHR");
		if(!pfn_vkGetSwapchainImagesKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)vulkanLib.LoadFunc("vkAcquireNextImageKHR");
		if(!pfn_vkAcquireNextImageKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkQueuePresentKHR = (PFN_vkQueuePresentKHR)vulkanLib.LoadFunc("vkQueuePresentKHR");
		if(!pfn_vkQueuePresentKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

#if defined(WFE_PLATFORM_WINDOWS)
		pfn_vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vulkanLib.LoadFunc("vkCreateWin32SurfaceKHR");
		if(!pfn_vkCreateWin32SurfaceKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)vulkanLib.LoadFunc("vkGetPhysicalDeviceWin32PresentationSupportKHR");
		if(!pfn_vkGetPhysicalDeviceWin32PresentationSupportKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}
#elif defined(WFE_PLATFORM_LINUX)
		pfn_vkCreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)vulkanLib.LoadFunc("vkCreateXlibSurfaceKHR");
		if(!pfn_vkCreateXlibSurfaceKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}

		pfn_vkGetPhysicalDeviceXlibPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)vulkanLib.LoadFunc("vkGetPhysicalDeviceXlibPresentationSupportKHR");
		if(!pfn_vkGetPhysicalDeviceXlibPresentationSupportKHR) {
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}
#endif

		WFE_LOG_INFO("Loaded Vulkan functions.");

		return true;
	}
	void FreeVulkanFunctions() {
		// Free the Vulkan dynamic library
		vulkanLib.FreeLib();
	}
}

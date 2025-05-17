#pragma once

#include "Core/Types/Defines.hpp"
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#if defined(WFE_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(WFE_PLATFORM_LINUX)
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>
#endif

namespace wfe {
	/// @brief Gets the Vulkan struct's size, based on its sType.
	/// @param sType The Vulkan structure's type.
	/// @return The size, in bytes, of the Vulkan struct, or 0 if the type was not recoognised.
	inline size_t VulkanGetStructSize(VkStructureType sType) {
		switch(sType) {
#if defined(VK_VERSION_1_0)
		case VK_STRUCTURE_TYPE_APPLICATION_INFO:
			return sizeof(VkApplicationInfo);
		case VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO:
			return sizeof(VkDeviceQueueCreateInfo);
		case VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO:
			return sizeof(VkDeviceCreateInfo);
		case VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO:
			return sizeof(VkInstanceCreateInfo);
		case VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO:
			return sizeof(VkMemoryAllocateInfo);
		case VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE:
			return sizeof(VkMappedMemoryRange);
		case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET:
			return sizeof(VkWriteDescriptorSet);
		case VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET:
			return sizeof(VkCopyDescriptorSet);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_BUFFER_USAGE_FLAGS_2_CREATE_INFO:
			return sizeof(VkBufferUsageFlags2CreateInfo);
#endif
#if defined(VK_VERSION_1_0)
		case VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO:
			return sizeof(VkBufferCreateInfo);
		case VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO:
			return sizeof(VkBufferViewCreateInfo);
		case VK_STRUCTURE_TYPE_MEMORY_BARRIER:
			return sizeof(VkMemoryBarrier);
		case VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER:
			return sizeof(VkBufferMemoryBarrier);
		case VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER:
			return sizeof(VkImageMemoryBarrier);
		case VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO:
			return sizeof(VkImageCreateInfo);
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO:
			return sizeof(VkImageViewCreateInfo);
		case VK_STRUCTURE_TYPE_BIND_SPARSE_INFO:
			return sizeof(VkBindSparseInfo);
		case VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO:
			return sizeof(VkShaderModuleCreateInfo);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO:
			return sizeof(VkDescriptorSetLayoutCreateInfo);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO:
			return sizeof(VkDescriptorPoolCreateInfo);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO:
			return sizeof(VkDescriptorSetAllocateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO:
			return sizeof(VkPipelineShaderStageCreateInfo);
		case VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO:
			return sizeof(VkComputePipelineCreateInfo);
#endif
#if defined(VK_NV_device_generated_commands_compute) && (defined(VK_NV_device_generated_commands))
		case VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_INDIRECT_BUFFER_INFO_NV:
			return sizeof(VkComputePipelineIndirectBufferInfoNV);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PIPELINE_CREATE_FLAGS_2_CREATE_INFO:
			return sizeof(VkPipelineCreateFlags2CreateInfo);
#endif
#if defined(VK_VERSION_1_0)
		case VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO:
			return sizeof(VkPipelineVertexInputStateCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO:
			return sizeof(VkPipelineInputAssemblyStateCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO:
			return sizeof(VkPipelineTessellationStateCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO:
			return sizeof(VkPipelineViewportStateCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO:
			return sizeof(VkPipelineRasterizationStateCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO:
			return sizeof(VkPipelineMultisampleStateCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO:
			return sizeof(VkPipelineColorBlendStateCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO:
			return sizeof(VkPipelineDynamicStateCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO:
			return sizeof(VkPipelineDepthStencilStateCreateInfo);
		case VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO:
			return sizeof(VkGraphicsPipelineCreateInfo);
		case VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO:
			return sizeof(VkPipelineCacheCreateInfo);
#endif
#if defined(VK_KHR_pipeline_binary) && (defined(VK_KHR_maintenance5) || defined(VK_VERSION_1_4))
		case VK_STRUCTURE_TYPE_PIPELINE_BINARY_CREATE_INFO_KHR:
			return sizeof(VkPipelineBinaryCreateInfoKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_BINARY_HANDLES_INFO_KHR:
			return sizeof(VkPipelineBinaryHandlesInfoKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_BINARY_KEY_KHR:
			return sizeof(VkPipelineBinaryKeyKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_BINARY_INFO_KHR:
			return sizeof(VkPipelineBinaryInfoKHR);
		case VK_STRUCTURE_TYPE_RELEASE_CAPTURED_PIPELINE_DATA_INFO_KHR:
			return sizeof(VkReleaseCapturedPipelineDataInfoKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_BINARY_DATA_INFO_KHR:
			return sizeof(VkPipelineBinaryDataInfoKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_CREATE_INFO_KHR:
			return sizeof(VkPipelineCreateInfoKHR);
#endif
#if defined(VK_VERSION_1_0)
		case VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO:
			return sizeof(VkPipelineLayoutCreateInfo);
		case VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO:
			return sizeof(VkSamplerCreateInfo);
		case VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO:
			return sizeof(VkCommandPoolCreateInfo);
		case VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO:
			return sizeof(VkCommandBufferAllocateInfo);
		case VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO:
			return sizeof(VkCommandBufferInheritanceInfo);
		case VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO:
			return sizeof(VkCommandBufferBeginInfo);
		case VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO:
			return sizeof(VkRenderPassBeginInfo);
		case VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO:
			return sizeof(VkRenderPassCreateInfo);
		case VK_STRUCTURE_TYPE_EVENT_CREATE_INFO:
			return sizeof(VkEventCreateInfo);
		case VK_STRUCTURE_TYPE_FENCE_CREATE_INFO:
			return sizeof(VkFenceCreateInfo);
		case VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO:
			return sizeof(VkSemaphoreCreateInfo);
		case VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO:
			return sizeof(VkQueryPoolCreateInfo);
		case VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO:
			return sizeof(VkFramebufferCreateInfo);
		case VK_STRUCTURE_TYPE_SUBMIT_INFO:
			return sizeof(VkSubmitInfo);
#endif
#if defined(VK_KHR_display) && (defined(VK_KHR_surface))
		case VK_STRUCTURE_TYPE_DISPLAY_MODE_CREATE_INFO_KHR:
			return sizeof(VkDisplayModeCreateInfoKHR);
		case VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR:
			return sizeof(VkDisplaySurfaceCreateInfoKHR);
#endif
#if defined(VK_NV_display_stereo) && (defined(VK_KHR_display) && defined(VK_KHR_get_display_properties2))
		case VK_STRUCTURE_TYPE_DISPLAY_SURFACE_STEREO_CREATE_INFO_NV:
			return sizeof(VkDisplaySurfaceStereoCreateInfoNV);
#endif
#if defined(VK_KHR_display_swapchain) && (defined(VK_KHR_swapchain) && defined(VK_KHR_display))
		case VK_STRUCTURE_TYPE_DISPLAY_PRESENT_INFO_KHR:
			return sizeof(VkDisplayPresentInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR) && (defined(VK_KHR_android_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR:
			return sizeof(VkAndroidSurfaceCreateInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_VI_NN) && (defined(VK_NN_vi_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_VI_SURFACE_CREATE_INFO_NN:
			return sizeof(VkViSurfaceCreateInfoNN);
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR) && (defined(VK_KHR_wayland_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR:
			return sizeof(VkWaylandSurfaceCreateInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_KHR_win32_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR:
			return sizeof(VkWin32SurfaceCreateInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR) && (defined(VK_KHR_xlib_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR:
			return sizeof(VkXlibSurfaceCreateInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR) && (defined(VK_KHR_xcb_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR:
			return sizeof(VkXcbSurfaceCreateInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_DIRECTFB_EXT) && (defined(VK_EXT_directfb_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_DIRECTFB_SURFACE_CREATE_INFO_EXT:
			return sizeof(VkDirectFBSurfaceCreateInfoEXT);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA) && (defined(VK_FUCHSIA_imagepipe_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_IMAGEPIPE_SURFACE_CREATE_INFO_FUCHSIA:
			return sizeof(VkImagePipeSurfaceCreateInfoFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_GGP) && (defined(VK_GGP_stream_descriptor_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_STREAM_DESCRIPTOR_SURFACE_CREATE_INFO_GGP:
			return sizeof(VkStreamDescriptorSurfaceCreateInfoGGP);
#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX) && (defined(VK_QNX_screen_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_SCREEN_SURFACE_CREATE_INFO_QNX:
			return sizeof(VkScreenSurfaceCreateInfoQNX);
#endif
#if defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))
		case VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR:
			return sizeof(VkSwapchainCreateInfoKHR);
		case VK_STRUCTURE_TYPE_PRESENT_INFO_KHR:
			return sizeof(VkPresentInfoKHR);
#endif
#if defined(VK_EXT_debug_report)
		case VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT:
			return sizeof(VkDebugReportCallbackCreateInfoEXT);
#endif
#if defined(VK_EXT_validation_flags)
		case VK_STRUCTURE_TYPE_VALIDATION_FLAGS_EXT:
			return sizeof(VkValidationFlagsEXT);
#endif
#if defined(VK_EXT_validation_features)
		case VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT:
			return sizeof(VkValidationFeaturesEXT);
#endif
#if defined(VK_EXT_layer_settings)
		case VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT:
			return sizeof(VkLayerSettingsCreateInfoEXT);
#endif
#if defined(VK_EXT_application_parameters)
		case VK_STRUCTURE_TYPE_APPLICATION_PARAMETERS_EXT:
			return sizeof(VkApplicationParametersEXT);
#endif
#if defined(VK_AMD_rasterization_order)
		case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD:
			return sizeof(VkPipelineRasterizationStateRasterizationOrderAMD);
#endif
#if defined(VK_EXT_debug_marker) && (defined(VK_EXT_debug_report))
		case VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT:
			return sizeof(VkDebugMarkerObjectNameInfoEXT);
		case VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT:
			return sizeof(VkDebugMarkerObjectTagInfoEXT);
		case VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT:
			return sizeof(VkDebugMarkerMarkerInfoEXT);
#endif
#if defined(VK_NV_dedicated_allocation)
		case VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_IMAGE_CREATE_INFO_NV:
			return sizeof(VkDedicatedAllocationImageCreateInfoNV);
		case VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_BUFFER_CREATE_INFO_NV:
			return sizeof(VkDedicatedAllocationBufferCreateInfoNV);
		case VK_STRUCTURE_TYPE_DEDICATED_ALLOCATION_MEMORY_ALLOCATE_INFO_NV:
			return sizeof(VkDedicatedAllocationMemoryAllocateInfoNV);
#endif
#if defined(VK_NV_external_memory) && (defined(VK_NV_external_memory_capabilities))
		case VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_NV:
			return sizeof(VkExternalMemoryImageCreateInfoNV);
		case VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_NV:
			return sizeof(VkExportMemoryAllocateInfoNV);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_NV_external_memory_win32) && (defined(VK_NV_external_memory)))
		case VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_NV:
			return sizeof(VkImportMemoryWin32HandleInfoNV);
		case VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_NV:
			return sizeof(VkExportMemoryWin32HandleInfoNV);
#endif
#if defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_memory_sci_buf) && (defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_EXPORT_MEMORY_SCI_BUF_INFO_NV:
			return sizeof(VkExportMemorySciBufInfoNV);
		case VK_STRUCTURE_TYPE_IMPORT_MEMORY_SCI_BUF_INFO_NV:
			return sizeof(VkImportMemorySciBufInfoNV);
		case VK_STRUCTURE_TYPE_MEMORY_GET_SCI_BUF_INFO_NV:
			return sizeof(VkMemoryGetSciBufInfoNV);
		case VK_STRUCTURE_TYPE_MEMORY_SCI_BUF_PROPERTIES_NV:
			return sizeof(VkMemorySciBufPropertiesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_SCI_BUF_FEATURES_NV:
			return sizeof(VkPhysicalDeviceExternalMemorySciBufFeaturesNV);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_NV_win32_keyed_mutex) && (defined(VK_NV_external_memory_win32)))
		case VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_NV:
			return sizeof(VkWin32KeyedMutexAcquireReleaseInfoNV);
#endif
#if defined(VK_NV_device_generated_commands) && ((defined(VK_VERSION_1_1) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_NV:
			return sizeof(VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV);
#endif
#if defined(VK_NV_device_generated_commands_compute) && (defined(VK_NV_device_generated_commands))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_COMPUTE_FEATURES_NV:
			return sizeof(VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_DEVICE_PRIVATE_DATA_CREATE_INFO:
			return sizeof(VkDevicePrivateDataCreateInfo);
		case VK_STRUCTURE_TYPE_PRIVATE_DATA_SLOT_CREATE_INFO:
			return sizeof(VkPrivateDataSlotCreateInfo);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIVATE_DATA_FEATURES:
			return sizeof(VkPhysicalDevicePrivateDataFeatures);
#endif
#if defined(VK_NV_device_generated_commands) && ((defined(VK_VERSION_1_1) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceDeviceGeneratedCommandsPropertiesNV);
#endif
#if defined(VK_NV_cluster_acceleration_structure) && (defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_ACCELERATION_STRUCTURE_FEATURES_NV:
			return sizeof(VkPhysicalDeviceClusterAccelerationStructureFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_ACCELERATION_STRUCTURE_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceClusterAccelerationStructurePropertiesNV);
#endif
#if (defined(VK_NV_cluster_acceleration_structure) && (defined(VK_KHR_acceleration_structure))) && (defined(VK_KHR_ray_tracing_pipeline))
		case VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CLUSTER_ACCELERATION_STRUCTURE_CREATE_INFO_NV:
			return sizeof(VkRayTracingPipelineClusterAccelerationStructureCreateInfoNV);
#endif
#if defined(VK_NV_cluster_acceleration_structure) && (defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_CLUSTERS_BOTTOM_LEVEL_INPUT_NV:
			return sizeof(VkClusterAccelerationStructureClustersBottomLevelInputNV);
		case VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_TRIANGLE_CLUSTER_INPUT_NV:
			return sizeof(VkClusterAccelerationStructureTriangleClusterInputNV);
		case VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_MOVE_OBJECTS_INPUT_NV:
			return sizeof(VkClusterAccelerationStructureMoveObjectsInputNV);
		case VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_INPUT_INFO_NV:
			return sizeof(VkClusterAccelerationStructureInputInfoNV);
		case VK_STRUCTURE_TYPE_CLUSTER_ACCELERATION_STRUCTURE_COMMANDS_INFO_NV:
			return sizeof(VkClusterAccelerationStructureCommandsInfoNV);
#endif
#if defined(VK_EXT_multi_draw) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceMultiDrawPropertiesEXT);
#endif
#if defined(VK_NV_device_generated_commands) && ((defined(VK_VERSION_1_1) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_GRAPHICS_SHADER_GROUP_CREATE_INFO_NV:
			return sizeof(VkGraphicsShaderGroupCreateInfoNV);
		case VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_SHADER_GROUPS_CREATE_INFO_NV:
			return sizeof(VkGraphicsPipelineShaderGroupsCreateInfoNV);
		case VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_NV:
			return sizeof(VkIndirectCommandsLayoutTokenNV);
		case VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_CREATE_INFO_NV:
			return sizeof(VkIndirectCommandsLayoutCreateInfoNV);
		case VK_STRUCTURE_TYPE_GENERATED_COMMANDS_INFO_NV:
			return sizeof(VkGeneratedCommandsInfoNV);
		case VK_STRUCTURE_TYPE_GENERATED_COMMANDS_MEMORY_REQUIREMENTS_INFO_NV:
			return sizeof(VkGeneratedCommandsMemoryRequirementsInfoNV);
#endif
#if defined(VK_NV_device_generated_commands_compute) && (defined(VK_NV_device_generated_commands))
		case VK_STRUCTURE_TYPE_PIPELINE_INDIRECT_DEVICE_ADDRESS_INFO_NV:
			return sizeof(VkPipelineIndirectDeviceAddressInfoNV);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2:
			return sizeof(VkPhysicalDeviceFeatures2);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2:
			return sizeof(VkPhysicalDeviceProperties2);
		case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2:
			return sizeof(VkFormatProperties2);
		case VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2:
			return sizeof(VkImageFormatProperties2);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2:
			return sizeof(VkPhysicalDeviceImageFormatInfo2);
		case VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2:
			return sizeof(VkQueueFamilyProperties2);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2:
			return sizeof(VkPhysicalDeviceMemoryProperties2);
		case VK_STRUCTURE_TYPE_SPARSE_IMAGE_FORMAT_PROPERTIES_2:
			return sizeof(VkSparseImageFormatProperties2);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SPARSE_IMAGE_FORMAT_INFO_2:
			return sizeof(VkPhysicalDeviceSparseImageFormatInfo2);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES:
			return sizeof(VkPhysicalDevicePushDescriptorProperties);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES:
			return sizeof(VkPhysicalDeviceDriverProperties);
#endif
#if defined(VK_KHR_incremental_present) && (defined(VK_KHR_swapchain))
		case VK_STRUCTURE_TYPE_PRESENT_REGIONS_KHR:
			return sizeof(VkPresentRegionsKHR);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VARIABLE_POINTERS_FEATURES:
			return sizeof(VkPhysicalDeviceVariablePointersFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_IMAGE_FORMAT_INFO:
			return sizeof(VkPhysicalDeviceExternalImageFormatInfo);
		case VK_STRUCTURE_TYPE_EXTERNAL_IMAGE_FORMAT_PROPERTIES:
			return sizeof(VkExternalImageFormatProperties);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_BUFFER_INFO:
			return sizeof(VkPhysicalDeviceExternalBufferInfo);
		case VK_STRUCTURE_TYPE_EXTERNAL_BUFFER_PROPERTIES:
			return sizeof(VkExternalBufferProperties);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES:
			return sizeof(VkPhysicalDeviceIDProperties);
		case VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO:
			return sizeof(VkExternalMemoryImageCreateInfo);
		case VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_BUFFER_CREATE_INFO:
			return sizeof(VkExternalMemoryBufferCreateInfo);
		case VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO:
			return sizeof(VkExportMemoryAllocateInfo);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_KHR_external_memory_win32) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkImportMemoryWin32HandleInfoKHR);
		case VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkExportMemoryWin32HandleInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA) && (defined(VK_FUCHSIA_external_memory) && ((defined(VK_KHR_external_memory_capabilities) && defined(VK_KHR_external_memory)) || defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_IMPORT_MEMORY_ZIRCON_HANDLE_INFO_FUCHSIA:
			return sizeof(VkImportMemoryZirconHandleInfoFUCHSIA);
		case VK_STRUCTURE_TYPE_MEMORY_ZIRCON_HANDLE_PROPERTIES_FUCHSIA:
			return sizeof(VkMemoryZirconHandlePropertiesFUCHSIA);
		case VK_STRUCTURE_TYPE_MEMORY_GET_ZIRCON_HANDLE_INFO_FUCHSIA:
			return sizeof(VkMemoryGetZirconHandleInfoFUCHSIA);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_KHR_external_memory_win32) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_MEMORY_WIN32_HANDLE_PROPERTIES_KHR:
			return sizeof(VkMemoryWin32HandlePropertiesKHR);
		case VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkMemoryGetWin32HandleInfoKHR);
#endif
#if defined(VK_KHR_external_memory_fd) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_IMPORT_MEMORY_FD_INFO_KHR:
			return sizeof(VkImportMemoryFdInfoKHR);
		case VK_STRUCTURE_TYPE_MEMORY_FD_PROPERTIES_KHR:
			return sizeof(VkMemoryFdPropertiesKHR);
		case VK_STRUCTURE_TYPE_MEMORY_GET_FD_INFO_KHR:
			return sizeof(VkMemoryGetFdInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_KHR_win32_keyed_mutex) && (defined(VK_KHR_external_memory_win32)))
		case VK_STRUCTURE_TYPE_WIN32_KEYED_MUTEX_ACQUIRE_RELEASE_INFO_KHR:
			return sizeof(VkWin32KeyedMutexAcquireReleaseInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_METAL_EXT) && (defined(VK_EXT_external_memory_metal) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_IMPORT_MEMORY_METAL_HANDLE_INFO_EXT:
			return sizeof(VkImportMemoryMetalHandleInfoEXT);
		case VK_STRUCTURE_TYPE_MEMORY_METAL_HANDLE_PROPERTIES_EXT:
			return sizeof(VkMemoryMetalHandlePropertiesEXT);
		case VK_STRUCTURE_TYPE_MEMORY_GET_METAL_HANDLE_INFO_EXT:
			return sizeof(VkMemoryGetMetalHandleInfoEXT);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SEMAPHORE_INFO:
			return sizeof(VkPhysicalDeviceExternalSemaphoreInfo);
		case VK_STRUCTURE_TYPE_EXTERNAL_SEMAPHORE_PROPERTIES:
			return sizeof(VkExternalSemaphoreProperties);
		case VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_CREATE_INFO:
			return sizeof(VkExportSemaphoreCreateInfo);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_KHR_external_semaphore_win32) && (defined(VK_KHR_external_semaphore)))
		case VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkImportSemaphoreWin32HandleInfoKHR);
		case VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkExportSemaphoreWin32HandleInfoKHR);
		case VK_STRUCTURE_TYPE_D3D12_FENCE_SUBMIT_INFO_KHR:
			return sizeof(VkD3D12FenceSubmitInfoKHR);
		case VK_STRUCTURE_TYPE_SEMAPHORE_GET_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkSemaphoreGetWin32HandleInfoKHR);
#endif
#if defined(VK_KHR_external_semaphore_fd) && (defined(VK_KHR_external_semaphore) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_FD_INFO_KHR:
			return sizeof(VkImportSemaphoreFdInfoKHR);
		case VK_STRUCTURE_TYPE_SEMAPHORE_GET_FD_INFO_KHR:
			return sizeof(VkSemaphoreGetFdInfoKHR);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA) && (defined(VK_FUCHSIA_external_semaphore) && (defined(VK_KHR_external_semaphore_capabilities) && defined(VK_KHR_external_semaphore)))
		case VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_ZIRCON_HANDLE_INFO_FUCHSIA:
			return sizeof(VkImportSemaphoreZirconHandleInfoFUCHSIA);
		case VK_STRUCTURE_TYPE_SEMAPHORE_GET_ZIRCON_HANDLE_INFO_FUCHSIA:
			return sizeof(VkSemaphoreGetZirconHandleInfoFUCHSIA);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FENCE_INFO:
			return sizeof(VkPhysicalDeviceExternalFenceInfo);
		case VK_STRUCTURE_TYPE_EXTERNAL_FENCE_PROPERTIES:
			return sizeof(VkExternalFenceProperties);
		case VK_STRUCTURE_TYPE_EXPORT_FENCE_CREATE_INFO:
			return sizeof(VkExportFenceCreateInfo);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_KHR_external_fence_win32) && (defined(VK_KHR_external_fence)))
		case VK_STRUCTURE_TYPE_IMPORT_FENCE_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkImportFenceWin32HandleInfoKHR);
		case VK_STRUCTURE_TYPE_EXPORT_FENCE_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkExportFenceWin32HandleInfoKHR);
		case VK_STRUCTURE_TYPE_FENCE_GET_WIN32_HANDLE_INFO_KHR:
			return sizeof(VkFenceGetWin32HandleInfoKHR);
#endif
#if defined(VK_KHR_external_fence_fd) && (defined(VK_KHR_external_fence) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_IMPORT_FENCE_FD_INFO_KHR:
			return sizeof(VkImportFenceFdInfoKHR);
		case VK_STRUCTURE_TYPE_FENCE_GET_FD_INFO_KHR:
			return sizeof(VkFenceGetFdInfoKHR);
#endif
#if (defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1)))) || (defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1))))
		case VK_STRUCTURE_TYPE_EXPORT_FENCE_SCI_SYNC_INFO_NV:
			return sizeof(VkExportFenceSciSyncInfoNV);
		case VK_STRUCTURE_TYPE_IMPORT_FENCE_SCI_SYNC_INFO_NV:
			return sizeof(VkImportFenceSciSyncInfoNV);
		case VK_STRUCTURE_TYPE_FENCE_GET_SCI_SYNC_INFO_NV:
			return sizeof(VkFenceGetSciSyncInfoNV);
#endif
#if defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_EXPORT_SEMAPHORE_SCI_SYNC_INFO_NV:
			return sizeof(VkExportSemaphoreSciSyncInfoNV);
		case VK_STRUCTURE_TYPE_IMPORT_SEMAPHORE_SCI_SYNC_INFO_NV:
			return sizeof(VkImportSemaphoreSciSyncInfoNV);
		case VK_STRUCTURE_TYPE_SEMAPHORE_GET_SCI_SYNC_INFO_NV:
			return sizeof(VkSemaphoreGetSciSyncInfoNV);
#endif
#if (defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1)))) || (defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1))))
		case VK_STRUCTURE_TYPE_SCI_SYNC_ATTRIBUTES_INFO_NV:
			return sizeof(VkSciSyncAttributesInfoNV);
#endif
#if defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_sci_sync) && (defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SCI_SYNC_FEATURES_NV:
			return sizeof(VkPhysicalDeviceExternalSciSyncFeaturesNV);
#endif
#if defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_SCI_SYNC_2_FEATURES_NV:
			return sizeof(VkPhysicalDeviceExternalSciSync2FeaturesNV);
		case VK_STRUCTURE_TYPE_SEMAPHORE_SCI_SYNC_POOL_CREATE_INFO_NV:
			return sizeof(VkSemaphoreSciSyncPoolCreateInfoNV);
		case VK_STRUCTURE_TYPE_SEMAPHORE_SCI_SYNC_CREATE_INFO_NV:
			return sizeof(VkSemaphoreSciSyncCreateInfoNV);
#endif
#if (defined(VK_USE_PLATFORM_SCI) && (defined(VK_NV_external_sci_sync2) && (defined(VK_VERSION_1_1)))) && (defined(VKSC_VERSION_1_0))
		case VK_STRUCTURE_TYPE_DEVICE_SEMAPHORE_SCI_SYNC_POOL_RESERVATION_CREATE_INFO_NV:
			return sizeof(VkDeviceSemaphoreSciSyncPoolReservationCreateInfoNV);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES:
			return sizeof(VkPhysicalDeviceMultiviewFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES:
			return sizeof(VkPhysicalDeviceMultiviewProperties);
		case VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO:
			return sizeof(VkRenderPassMultiviewCreateInfo);
#endif
#if defined(VK_EXT_display_surface_counter) && (defined(VK_KHR_display))
		case VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_EXT:
			return sizeof(VkSurfaceCapabilities2EXT);
#endif
#if defined(VK_EXT_display_control) && (defined(VK_EXT_display_surface_counter) && defined(VK_KHR_swapchain))
		case VK_STRUCTURE_TYPE_DISPLAY_POWER_INFO_EXT:
			return sizeof(VkDisplayPowerInfoEXT);
		case VK_STRUCTURE_TYPE_DEVICE_EVENT_INFO_EXT:
			return sizeof(VkDeviceEventInfoEXT);
		case VK_STRUCTURE_TYPE_DISPLAY_EVENT_INFO_EXT:
			return sizeof(VkDisplayEventInfoEXT);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_COUNTER_CREATE_INFO_EXT:
			return sizeof(VkSwapchainCounterCreateInfoEXT);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GROUP_PROPERTIES:
			return sizeof(VkPhysicalDeviceGroupProperties);
		case VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO:
			return sizeof(VkMemoryAllocateFlagsInfo);
		case VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO:
			return sizeof(VkBindBufferMemoryInfo);
		case VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_DEVICE_GROUP_INFO:
			return sizeof(VkBindBufferMemoryDeviceGroupInfo);
		case VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO:
			return sizeof(VkBindImageMemoryInfo);
		case VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO:
			return sizeof(VkBindImageMemoryDeviceGroupInfo);
		case VK_STRUCTURE_TYPE_DEVICE_GROUP_RENDER_PASS_BEGIN_INFO:
			return sizeof(VkDeviceGroupRenderPassBeginInfo);
		case VK_STRUCTURE_TYPE_DEVICE_GROUP_COMMAND_BUFFER_BEGIN_INFO:
			return sizeof(VkDeviceGroupCommandBufferBeginInfo);
		case VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO:
			return sizeof(VkDeviceGroupSubmitInfo);
		case VK_STRUCTURE_TYPE_DEVICE_GROUP_BIND_SPARSE_INFO:
			return sizeof(VkDeviceGroupBindSparseInfo);
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR:
			return sizeof(VkDeviceGroupPresentCapabilitiesKHR);
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_swapchain)))
		case VK_STRUCTURE_TYPE_IMAGE_SWAPCHAIN_CREATE_INFO_KHR:
			return sizeof(VkImageSwapchainCreateInfoKHR);
		case VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR:
			return sizeof(VkBindImageMemorySwapchainInfoKHR);
		case VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR:
			return sizeof(VkAcquireNextImageInfoKHR);
		case VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_INFO_KHR:
			return sizeof(VkDeviceGroupPresentInfoKHR);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_DEVICE_GROUP_DEVICE_CREATE_INFO:
			return sizeof(VkDeviceGroupDeviceCreateInfo);
#endif
#if ((defined(VK_KHR_swapchain) && (defined(VK_KHR_surface))) && (defined(VK_VERSION_1_1))) || ((defined(VK_KHR_device_group) && (defined(VK_KHR_device_group_creation))) && (defined(VK_KHR_swapchain)))
		case VK_STRUCTURE_TYPE_DEVICE_GROUP_SWAPCHAIN_CREATE_INFO_KHR:
			return sizeof(VkDeviceGroupSwapchainCreateInfoKHR);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO:
			return sizeof(VkDescriptorUpdateTemplateCreateInfo);
#endif
#if defined(VK_KHR_present_id) && (defined(VK_KHR_swapchain) && defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_ID_FEATURES_KHR:
			return sizeof(VkPhysicalDevicePresentIdFeaturesKHR);
		case VK_STRUCTURE_TYPE_PRESENT_ID_KHR:
			return sizeof(VkPresentIdKHR);
#endif
#if defined(VK_KHR_present_wait) && (defined(VK_KHR_swapchain) && defined(VK_KHR_present_id))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_WAIT_FEATURES_KHR:
			return sizeof(VkPhysicalDevicePresentWaitFeaturesKHR);
#endif
#if defined(VK_EXT_hdr_metadata) && (defined(VK_KHR_swapchain))
		case VK_STRUCTURE_TYPE_HDR_METADATA_EXT:
			return sizeof(VkHdrMetadataEXT);
#endif
#if defined(VK_HUAWEI_hdr_vivid) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_swapchain) && defined(VK_EXT_hdr_metadata))
		case VK_STRUCTURE_TYPE_HDR_VIVID_DYNAMIC_METADATA_HUAWEI:
			return sizeof(VkHdrVividDynamicMetadataHUAWEI);
#endif
#if defined(VK_AMD_display_native_hdr) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))
		case VK_STRUCTURE_TYPE_DISPLAY_NATIVE_HDR_SURFACE_CAPABILITIES_AMD:
			return sizeof(VkDisplayNativeHdrSurfaceCapabilitiesAMD);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_DISPLAY_NATIVE_HDR_CREATE_INFO_AMD:
			return sizeof(VkSwapchainDisplayNativeHdrCreateInfoAMD);
#endif
#if defined(VK_GOOGLE_display_timing) && (defined(VK_KHR_swapchain))
		case VK_STRUCTURE_TYPE_PRESENT_TIMES_INFO_GOOGLE:
			return sizeof(VkPresentTimesInfoGOOGLE);
#endif
#if defined(VK_USE_PLATFORM_IOS_MVK) && (defined(VK_MVK_ios_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK:
			return sizeof(VkIOSSurfaceCreateInfoMVK);
#endif
#if defined(VK_USE_PLATFORM_MACOS_MVK) && (defined(VK_MVK_macos_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK:
			return sizeof(VkMacOSSurfaceCreateInfoMVK);
#endif
#if defined(VK_USE_PLATFORM_METAL_EXT) && (defined(VK_EXT_metal_surface) && (defined(VK_KHR_surface)))
		case VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT:
			return sizeof(VkMetalSurfaceCreateInfoEXT);
#endif
#if defined(VK_NV_clip_space_w_scaling)
		case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_W_SCALING_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineViewportWScalingStateCreateInfoNV);
#endif
#if defined(VK_NV_viewport_swizzle)
		case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_SWIZZLE_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineViewportSwizzleStateCreateInfoNV);
#endif
#if defined(VK_EXT_discard_rectangles) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceDiscardRectanglePropertiesEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_DISCARD_RECTANGLE_STATE_CREATE_INFO_EXT:
			return sizeof(VkPipelineDiscardRectangleStateCreateInfoEXT);
#endif
#if defined(VK_NVX_multiview_per_view_attributes) && (defined(VK_KHR_multiview) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_ATTRIBUTES_PROPERTIES_NVX:
			return sizeof(VkPhysicalDeviceMultiviewPerViewAttributesPropertiesNVX);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_RENDER_PASS_INPUT_ATTACHMENT_ASPECT_CREATE_INFO:
			return sizeof(VkRenderPassInputAttachmentAspectCreateInfo);
#endif
#if defined(VK_KHR_get_surface_capabilities2) && (defined(VK_KHR_surface))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR:
			return sizeof(VkPhysicalDeviceSurfaceInfo2KHR);
		case VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR:
			return sizeof(VkSurfaceCapabilities2KHR);
		case VK_STRUCTURE_TYPE_SURFACE_FORMAT_2_KHR:
			return sizeof(VkSurfaceFormat2KHR);
#endif
#if defined(VK_KHR_get_display_properties2) && (defined(VK_KHR_display))
		case VK_STRUCTURE_TYPE_DISPLAY_PROPERTIES_2_KHR:
			return sizeof(VkDisplayProperties2KHR);
		case VK_STRUCTURE_TYPE_DISPLAY_PLANE_PROPERTIES_2_KHR:
			return sizeof(VkDisplayPlaneProperties2KHR);
		case VK_STRUCTURE_TYPE_DISPLAY_MODE_PROPERTIES_2_KHR:
			return sizeof(VkDisplayModeProperties2KHR);
#endif
#if defined(VK_NV_display_stereo) && (defined(VK_KHR_display) && defined(VK_KHR_get_display_properties2))
		case VK_STRUCTURE_TYPE_DISPLAY_MODE_STEREO_PROPERTIES_NV:
			return sizeof(VkDisplayModeStereoPropertiesNV);
#endif
#if defined(VK_KHR_get_display_properties2) && (defined(VK_KHR_display))
		case VK_STRUCTURE_TYPE_DISPLAY_PLANE_INFO_2_KHR:
			return sizeof(VkDisplayPlaneInfo2KHR);
		case VK_STRUCTURE_TYPE_DISPLAY_PLANE_CAPABILITIES_2_KHR:
			return sizeof(VkDisplayPlaneCapabilities2KHR);
#endif
#if defined(VK_KHR_shared_presentable_image) && (defined(VK_KHR_swapchain) && defined(VK_KHR_get_surface_capabilities2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_SHARED_PRESENT_SURFACE_CAPABILITIES_KHR:
			return sizeof(VkSharedPresentSurfaceCapabilitiesKHR);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES:
			return sizeof(VkPhysicalDevice16BitStorageFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES:
			return sizeof(VkPhysicalDeviceSubgroupProperties);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_EXTENDED_TYPES_FEATURES:
			return sizeof(VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2:
			return sizeof(VkBufferMemoryRequirementsInfo2);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_DEVICE_BUFFER_MEMORY_REQUIREMENTS:
			return sizeof(VkDeviceBufferMemoryRequirements);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2:
			return sizeof(VkImageMemoryRequirementsInfo2);
		case VK_STRUCTURE_TYPE_IMAGE_SPARSE_MEMORY_REQUIREMENTS_INFO_2:
			return sizeof(VkImageSparseMemoryRequirementsInfo2);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_DEVICE_IMAGE_MEMORY_REQUIREMENTS:
			return sizeof(VkDeviceImageMemoryRequirements);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2:
			return sizeof(VkMemoryRequirements2);
		case VK_STRUCTURE_TYPE_SPARSE_IMAGE_MEMORY_REQUIREMENTS_2:
			return sizeof(VkSparseImageMemoryRequirements2);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES:
			return sizeof(VkPhysicalDevicePointClippingProperties);
		case VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS:
			return sizeof(VkMemoryDedicatedRequirements);
		case VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO:
			return sizeof(VkMemoryDedicatedAllocateInfo);
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO:
			return sizeof(VkImageViewUsageCreateInfo);
#endif
#if defined(VK_EXT_image_sliced_view_of_3d) && ((defined(VK_KHR_maintenance1) && defined(VK_KHR_get_physical_device_properties2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_SLICED_CREATE_INFO_EXT:
			return sizeof(VkImageViewSlicedCreateInfoEXT);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_DOMAIN_ORIGIN_STATE_CREATE_INFO:
			return sizeof(VkPipelineTessellationDomainOriginStateCreateInfo);
		case VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_INFO:
			return sizeof(VkSamplerYcbcrConversionInfo);
		case VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_CREATE_INFO:
			return sizeof(VkSamplerYcbcrConversionCreateInfo);
		case VK_STRUCTURE_TYPE_BIND_IMAGE_PLANE_MEMORY_INFO:
			return sizeof(VkBindImagePlaneMemoryInfo);
		case VK_STRUCTURE_TYPE_IMAGE_PLANE_MEMORY_REQUIREMENTS_INFO:
			return sizeof(VkImagePlaneMemoryRequirementsInfo);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_YCBCR_CONVERSION_FEATURES:
			return sizeof(VkPhysicalDeviceSamplerYcbcrConversionFeatures);
		case VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_IMAGE_FORMAT_PROPERTIES:
			return sizeof(VkSamplerYcbcrConversionImageFormatProperties);
#endif
#if defined(VK_AMD_texture_gather_bias_lod) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_TEXTURE_LOD_GATHER_FORMAT_PROPERTIES_AMD:
			return sizeof(VkTextureLODGatherFormatPropertiesAMD);
#endif
#if defined(VK_EXT_conditional_rendering) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_CONDITIONAL_RENDERING_BEGIN_INFO_EXT:
			return sizeof(VkConditionalRenderingBeginInfoEXT);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PROTECTED_SUBMIT_INFO:
			return sizeof(VkProtectedSubmitInfo);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_FEATURES:
			return sizeof(VkPhysicalDeviceProtectedMemoryFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROTECTED_MEMORY_PROPERTIES:
			return sizeof(VkPhysicalDeviceProtectedMemoryProperties);
		case VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2:
			return sizeof(VkDeviceQueueInfo2);
#endif
#if defined(VK_NV_fragment_coverage_to_color)
		case VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_TO_COLOR_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineCoverageToColorStateCreateInfoNV);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLER_FILTER_MINMAX_PROPERTIES:
			return sizeof(VkPhysicalDeviceSamplerFilterMinmaxProperties);
#endif
#if defined(VK_EXT_sample_locations) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_SAMPLE_LOCATIONS_INFO_EXT:
			return sizeof(VkSampleLocationsInfoEXT);
		case VK_STRUCTURE_TYPE_RENDER_PASS_SAMPLE_LOCATIONS_BEGIN_INFO_EXT:
			return sizeof(VkRenderPassSampleLocationsBeginInfoEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_SAMPLE_LOCATIONS_STATE_CREATE_INFO_EXT:
			return sizeof(VkPipelineSampleLocationsStateCreateInfoEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SAMPLE_LOCATIONS_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceSampleLocationsPropertiesEXT);
		case VK_STRUCTURE_TYPE_MULTISAMPLE_PROPERTIES_EXT:
			return sizeof(VkMultisamplePropertiesEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO:
			return sizeof(VkSamplerReductionModeCreateInfo);
#endif
#if defined(VK_EXT_blend_operation_advanced) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT);
#endif
#if defined(VK_EXT_multi_draw) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTI_DRAW_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceMultiDrawFeaturesEXT);
#endif
#if defined(VK_EXT_blend_operation_advanced) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_ADVANCED_STATE_CREATE_INFO_EXT:
			return sizeof(VkPipelineColorBlendAdvancedStateCreateInfoEXT);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES:
			return sizeof(VkPhysicalDeviceInlineUniformBlockFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES:
			return sizeof(VkPhysicalDeviceInlineUniformBlockProperties);
		case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK:
			return sizeof(VkWriteDescriptorSetInlineUniformBlock);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_INLINE_UNIFORM_BLOCK_CREATE_INFO:
			return sizeof(VkDescriptorPoolInlineUniformBlockCreateInfo);
#endif
#if defined(VK_NV_framebuffer_mixed_samples)
		case VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_MODULATION_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineCoverageModulationStateCreateInfoNV);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO:
			return sizeof(VkImageFormatListCreateInfo);
#endif
#if defined(VK_EXT_validation_cache)
		case VK_STRUCTURE_TYPE_VALIDATION_CACHE_CREATE_INFO_EXT:
			return sizeof(VkValidationCacheCreateInfoEXT);
		case VK_STRUCTURE_TYPE_SHADER_MODULE_VALIDATION_CACHE_CREATE_INFO_EXT:
			return sizeof(VkShaderModuleValidationCacheCreateInfoEXT);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES:
			return sizeof(VkPhysicalDeviceMaintenance3Properties);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES:
			return sizeof(VkPhysicalDeviceMaintenance4Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_PROPERTIES:
			return sizeof(VkPhysicalDeviceMaintenance4Properties);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_FEATURES:
			return sizeof(VkPhysicalDeviceMaintenance5Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_5_PROPERTIES:
			return sizeof(VkPhysicalDeviceMaintenance5Properties);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_6_FEATURES:
			return sizeof(VkPhysicalDeviceMaintenance6Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_6_PROPERTIES:
			return sizeof(VkPhysicalDeviceMaintenance6Properties);
#endif
#if defined(VK_KHR_maintenance7) && (defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_7_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceMaintenance7FeaturesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_7_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceMaintenance7PropertiesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LAYERED_API_PROPERTIES_LIST_KHR:
			return sizeof(VkPhysicalDeviceLayeredApiPropertiesListKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LAYERED_API_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceLayeredApiPropertiesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LAYERED_API_VULKAN_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceLayeredApiVulkanPropertiesKHR);
#endif
#if defined(VK_KHR_maintenance8) && (defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_8_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceMaintenance8FeaturesKHR);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_RENDERING_AREA_INFO:
			return sizeof(VkRenderingAreaInfo);
#endif
#if defined(VK_VERSION_1_1)
		case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT:
			return sizeof(VkDescriptorSetLayoutSupport);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES:
			return sizeof(VkPhysicalDeviceShaderDrawParametersFeatures);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES:
			return sizeof(VkPhysicalDeviceShaderFloat16Int8Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES:
			return sizeof(VkPhysicalDeviceFloatControlsProperties);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES:
			return sizeof(VkPhysicalDeviceHostQueryResetFeatures);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR) && (defined(VK_ANDROID_native_buffer))
		case VK_STRUCTURE_TYPE_NATIVE_BUFFER_ANDROID:
			return sizeof(VkNativeBufferANDROID);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_IMAGE_CREATE_INFO_ANDROID:
			return sizeof(VkSwapchainImageCreateInfoANDROID);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENTATION_PROPERTIES_ANDROID:
			return sizeof(VkPhysicalDevicePresentationPropertiesANDROID);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_DEVICE_QUEUE_GLOBAL_PRIORITY_CREATE_INFO:
			return sizeof(VkDeviceQueueGlobalPriorityCreateInfo);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GLOBAL_PRIORITY_QUERY_FEATURES:
			return sizeof(VkPhysicalDeviceGlobalPriorityQueryFeatures);
		case VK_STRUCTURE_TYPE_QUEUE_FAMILY_GLOBAL_PRIORITY_PROPERTIES:
			return sizeof(VkQueueFamilyGlobalPriorityProperties);
#endif
#if defined(VK_EXT_debug_utils)
		case VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT:
			return sizeof(VkDebugUtilsObjectNameInfoEXT);
		case VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT:
			return sizeof(VkDebugUtilsObjectTagInfoEXT);
		case VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT:
			return sizeof(VkDebugUtilsLabelEXT);
		case VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT:
			return sizeof(VkDebugUtilsMessengerCreateInfoEXT);
		case VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT:
			return sizeof(VkDebugUtilsMessengerCallbackDataEXT);
#endif
#if defined(VK_EXT_device_memory_report) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_MEMORY_REPORT_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceDeviceMemoryReportFeaturesEXT);
		case VK_STRUCTURE_TYPE_DEVICE_DEVICE_MEMORY_REPORT_CREATE_INFO_EXT:
			return sizeof(VkDeviceDeviceMemoryReportCreateInfoEXT);
		case VK_STRUCTURE_TYPE_DEVICE_MEMORY_REPORT_CALLBACK_DATA_EXT:
			return sizeof(VkDeviceMemoryReportCallbackDataEXT);
#endif
#if defined(VK_EXT_external_memory_host) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_IMPORT_MEMORY_HOST_POINTER_INFO_EXT:
			return sizeof(VkImportMemoryHostPointerInfoEXT);
		case VK_STRUCTURE_TYPE_MEMORY_HOST_POINTER_PROPERTIES_EXT:
			return sizeof(VkMemoryHostPointerPropertiesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_HOST_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceExternalMemoryHostPropertiesEXT);
#endif
#if defined(VK_EXT_conservative_rasterization) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceConservativeRasterizationPropertiesEXT);
#endif
#if defined(VK_KHR_calibrated_timestamps) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_CALIBRATED_TIMESTAMP_INFO_KHR:
			return sizeof(VkCalibratedTimestampInfoKHR);
#endif
#if defined(VK_AMD_shader_core_properties) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_AMD:
			return sizeof(VkPhysicalDeviceShaderCorePropertiesAMD);
#endif
#if defined(VK_AMD_shader_core_properties2) && (defined(VK_AMD_shader_core_properties))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_2_AMD:
			return sizeof(VkPhysicalDeviceShaderCoreProperties2AMD);
#endif
#if defined(VK_EXT_conservative_rasterization) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT:
			return sizeof(VkPipelineRasterizationConservativeStateCreateInfoEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES:
			return sizeof(VkPhysicalDeviceDescriptorIndexingFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES:
			return sizeof(VkPhysicalDeviceDescriptorIndexingProperties);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO:
			return sizeof(VkDescriptorSetLayoutBindingFlagsCreateInfo);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO:
			return sizeof(VkDescriptorSetVariableDescriptorCountAllocateInfo);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_LAYOUT_SUPPORT:
			return sizeof(VkDescriptorSetVariableDescriptorCountLayoutSupport);
		case VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2:
			return sizeof(VkAttachmentDescription2);
		case VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2:
			return sizeof(VkAttachmentReference2);
		case VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2:
			return sizeof(VkSubpassDescription2);
		case VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2:
			return sizeof(VkSubpassDependency2);
		case VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2:
			return sizeof(VkRenderPassCreateInfo2);
		case VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO:
			return sizeof(VkSubpassBeginInfo);
		case VK_STRUCTURE_TYPE_SUBPASS_END_INFO:
			return sizeof(VkSubpassEndInfo);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES:
			return sizeof(VkPhysicalDeviceTimelineSemaphoreFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_PROPERTIES:
			return sizeof(VkPhysicalDeviceTimelineSemaphoreProperties);
		case VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO:
			return sizeof(VkSemaphoreTypeCreateInfo);
		case VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO:
			return sizeof(VkTimelineSemaphoreSubmitInfo);
		case VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO:
			return sizeof(VkSemaphoreWaitInfo);
		case VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO:
			return sizeof(VkSemaphoreSignalInfo);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_DIVISOR_STATE_CREATE_INFO:
			return sizeof(VkPipelineVertexInputDivisorStateCreateInfo);
#endif
#if defined(VK_EXT_vertex_attribute_divisor) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceVertexAttributeDivisorPropertiesEXT);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES:
			return sizeof(VkPhysicalDeviceVertexAttributeDivisorProperties);
#endif
#if defined(VK_EXT_pci_bus_info) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PCI_BUS_INFO_PROPERTIES_EXT:
			return sizeof(VkPhysicalDevicePCIBusInfoPropertiesEXT);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR) && (defined(VK_ANDROID_external_memory_android_hardware_buffer) && (((defined(VK_KHR_sampler_ycbcr_conversion) && defined(VK_KHR_external_memory) && defined(VK_KHR_dedicated_allocation)) || defined(VK_VERSION_1_1)) && defined(VK_EXT_queue_family_foreign)))
		case VK_STRUCTURE_TYPE_IMPORT_ANDROID_HARDWARE_BUFFER_INFO_ANDROID:
			return sizeof(VkImportAndroidHardwareBufferInfoANDROID);
		case VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_USAGE_ANDROID:
			return sizeof(VkAndroidHardwareBufferUsageANDROID);
		case VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID:
			return sizeof(VkAndroidHardwareBufferPropertiesANDROID);
		case VK_STRUCTURE_TYPE_MEMORY_GET_ANDROID_HARDWARE_BUFFER_INFO_ANDROID:
			return sizeof(VkMemoryGetAndroidHardwareBufferInfoANDROID);
		case VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_ANDROID:
			return sizeof(VkAndroidHardwareBufferFormatPropertiesANDROID);
#endif
#if defined(VK_EXT_conditional_rendering) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_CONDITIONAL_RENDERING_INFO_EXT:
			return sizeof(VkCommandBufferInheritanceConditionalRenderingInfoEXT);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR) && (defined(VK_ANDROID_external_memory_android_hardware_buffer) && (((defined(VK_KHR_sampler_ycbcr_conversion) && defined(VK_KHR_external_memory) && defined(VK_KHR_dedicated_allocation)) || defined(VK_VERSION_1_1)) && defined(VK_EXT_queue_family_foreign)))
		case VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_ANDROID:
			return sizeof(VkExternalFormatANDROID);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES:
			return sizeof(VkPhysicalDevice8BitStorageFeatures);
#endif
#if defined(VK_EXT_conditional_rendering) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceConditionalRenderingFeaturesEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_MEMORY_MODEL_FEATURES:
			return sizeof(VkPhysicalDeviceVulkanMemoryModelFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES:
			return sizeof(VkPhysicalDeviceShaderAtomicInt64Features);
#endif
#if defined(VK_EXT_shader_atomic_float) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceShaderAtomicFloatFeaturesEXT);
#endif
#if defined(VK_EXT_shader_atomic_float2) && (defined(VK_EXT_shader_atomic_float))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT_2_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES:
			return sizeof(VkPhysicalDeviceVertexAttributeDivisorFeatures);
#endif
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_NV:
			return sizeof(VkQueueFamilyCheckpointPropertiesNV);
		case VK_STRUCTURE_TYPE_CHECKPOINT_DATA_NV:
			return sizeof(VkCheckpointDataNV);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES:
			return sizeof(VkPhysicalDeviceDepthStencilResolveProperties);
		case VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_DEPTH_STENCIL_RESOLVE:
			return sizeof(VkSubpassDescriptionDepthStencilResolve);
#endif
#if defined(VK_EXT_astc_decode_mode) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_ASTC_DECODE_MODE_EXT:
			return sizeof(VkImageViewASTCDecodeModeEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ASTC_DECODE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceASTCDecodeFeaturesEXT);
#endif
#if defined(VK_EXT_transform_feedback) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceTransformFeedbackFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceTransformFeedbackPropertiesEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_STREAM_CREATE_INFO_EXT:
			return sizeof(VkPipelineRasterizationStateStreamCreateInfoEXT);
#endif
#if defined(VK_NV_representative_fragment_test) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_REPRESENTATIVE_FRAGMENT_TEST_FEATURES_NV:
			return sizeof(VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV);
		case VK_STRUCTURE_TYPE_PIPELINE_REPRESENTATIVE_FRAGMENT_TEST_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineRepresentativeFragmentTestStateCreateInfoNV);
#endif
#if defined(VK_NV_scissor_exclusive) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXCLUSIVE_SCISSOR_FEATURES_NV:
			return sizeof(VkPhysicalDeviceExclusiveScissorFeaturesNV);
		case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_EXCLUSIVE_SCISSOR_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineViewportExclusiveScissorStateCreateInfoNV);
#endif
#if defined(VK_NV_corner_sampled_image) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CORNER_SAMPLED_IMAGE_FEATURES_NV:
			return sizeof(VkPhysicalDeviceCornerSampledImageFeaturesNV);
#endif
#if defined(VK_KHR_compute_shader_derivatives) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceComputeShaderDerivativesFeaturesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMPUTE_SHADER_DERIVATIVES_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceComputeShaderDerivativesPropertiesKHR);
#endif
#if defined(VK_NV_shader_image_footprint) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_FOOTPRINT_FEATURES_NV:
			return sizeof(VkPhysicalDeviceShaderImageFootprintFeaturesNV);
#endif
#if defined(VK_NV_dedicated_allocation_image_aliasing) && ((defined(VK_KHR_dedicated_allocation) && defined(VK_KHR_get_physical_device_properties2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEDICATED_ALLOCATION_IMAGE_ALIASING_FEATURES_NV:
			return sizeof(VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV);
#endif
#if defined(VK_NV_copy_memory_indirect) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COPY_MEMORY_INDIRECT_FEATURES_NV:
			return sizeof(VkPhysicalDeviceCopyMemoryIndirectFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COPY_MEMORY_INDIRECT_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceCopyMemoryIndirectPropertiesNV);
#endif
#if defined(VK_NV_memory_decompression) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_DECOMPRESSION_FEATURES_NV:
			return sizeof(VkPhysicalDeviceMemoryDecompressionFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_DECOMPRESSION_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceMemoryDecompressionPropertiesNV);
#endif
#if defined(VK_NV_shading_rate_image) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_SHADING_RATE_IMAGE_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineViewportShadingRateImageStateCreateInfoNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_FEATURES_NV:
			return sizeof(VkPhysicalDeviceShadingRateImageFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADING_RATE_IMAGE_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceShadingRateImagePropertiesNV);
#endif
#if defined(VK_HUAWEI_invocation_mask) && (defined(VK_KHR_ray_tracing_pipeline) && (defined(VK_KHR_synchronization2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INVOCATION_MASK_FEATURES_HUAWEI:
			return sizeof(VkPhysicalDeviceInvocationMaskFeaturesHUAWEI);
#endif
#if defined(VK_NV_shading_rate_image) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_COARSE_SAMPLE_ORDER_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineViewportCoarseSampleOrderStateCreateInfoNV);
#endif
#if defined(VK_NV_mesh_shader) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV:
			return sizeof(VkPhysicalDeviceMeshShaderFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceMeshShaderPropertiesNV);
#endif
#if defined(VK_EXT_mesh_shader) && (defined(VK_KHR_spirv_1_4) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceMeshShaderFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceMeshShaderPropertiesEXT);
#endif
#if defined(VK_NV_ray_tracing) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV:
			return sizeof(VkRayTracingShaderGroupCreateInfoNV);
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && ((defined(VK_KHR_spirv_1_4) || defined(VK_VERSION_1_2)) && defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR:
			return sizeof(VkRayTracingShaderGroupCreateInfoKHR);
#endif
#if defined(VK_NV_ray_tracing) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV:
			return sizeof(VkRayTracingPipelineCreateInfoNV);
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && ((defined(VK_KHR_spirv_1_4) || defined(VK_VERSION_1_2)) && defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR:
			return sizeof(VkRayTracingPipelineCreateInfoKHR);
#endif
#if defined(VK_NV_ray_tracing) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NV:
			return sizeof(VkGeometryTrianglesNV);
		case VK_STRUCTURE_TYPE_GEOMETRY_AABB_NV:
			return sizeof(VkGeometryAABBNV);
		case VK_STRUCTURE_TYPE_GEOMETRY_NV:
			return sizeof(VkGeometryNV);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_INFO_NV:
			return sizeof(VkAccelerationStructureInfoNV);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NV:
			return sizeof(VkAccelerationStructureCreateInfoNV);
		case VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NV:
			return sizeof(VkBindAccelerationStructureMemoryInfoNV);
#endif
#if defined(VK_KHR_acceleration_structure) && (((defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_deferred_host_operations))
		case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR:
			return sizeof(VkWriteDescriptorSetAccelerationStructureKHR);
#endif
#if defined(VK_NV_ray_tracing) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV:
			return sizeof(VkWriteDescriptorSetAccelerationStructureNV);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV:
			return sizeof(VkAccelerationStructureMemoryRequirementsInfoNV);
#endif
#if defined(VK_KHR_acceleration_structure) && (((defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_deferred_host_operations))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceAccelerationStructureFeaturesKHR);
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && ((defined(VK_KHR_spirv_1_4) || defined(VK_VERSION_1_2)) && defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceRayTracingPipelineFeaturesKHR);
#endif
#if defined(VK_KHR_ray_query) && ((defined(VK_KHR_spirv_1_4) || defined(VK_VERSION_1_2)) && defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceRayQueryFeaturesKHR);
#endif
#if defined(VK_KHR_acceleration_structure) && (((defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_deferred_host_operations))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceAccelerationStructurePropertiesKHR);
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && ((defined(VK_KHR_spirv_1_4) || defined(VK_VERSION_1_2)) && defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceRayTracingPipelinePropertiesKHR);
#endif
#if defined(VK_NV_ray_tracing) && ((defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_get_memory_requirements2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceRayTracingPropertiesNV);
#endif
#if defined(VK_KHR_ray_tracing_maintenance1) && (defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_MAINTENANCE_1_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR);
#endif
#if defined(VK_EXT_image_drm_format_modifier) && ((((defined(VK_KHR_bind_memory2) && defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_sampler_ycbcr_conversion)) || defined(VK_VERSION_1_1)) && defined(VK_KHR_image_format_list)) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_EXT:
			return sizeof(VkDrmFormatModifierPropertiesListEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_DRM_FORMAT_MODIFIER_INFO_EXT:
			return sizeof(VkPhysicalDeviceImageDrmFormatModifierInfoEXT);
		case VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_LIST_CREATE_INFO_EXT:
			return sizeof(VkImageDrmFormatModifierListCreateInfoEXT);
		case VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_EXPLICIT_CREATE_INFO_EXT:
			return sizeof(VkImageDrmFormatModifierExplicitCreateInfoEXT);
		case VK_STRUCTURE_TYPE_IMAGE_DRM_FORMAT_MODIFIER_PROPERTIES_EXT:
			return sizeof(VkImageDrmFormatModifierPropertiesEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_IMAGE_STENCIL_USAGE_CREATE_INFO:
			return sizeof(VkImageStencilUsageCreateInfo);
#endif
#if defined(VK_AMD_memory_overallocation_behavior)
		case VK_STRUCTURE_TYPE_DEVICE_MEMORY_OVERALLOCATION_CREATE_INFO_AMD:
			return sizeof(VkDeviceMemoryOverallocationCreateInfoAMD);
#endif
#if defined(VK_EXT_fragment_density_map) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceFragmentDensityMapFeaturesEXT);
#endif
#if defined(VK_EXT_fragment_density_map2) && (defined(VK_EXT_fragment_density_map))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceFragmentDensityMap2FeaturesEXT);
#endif
#if defined(VK_EXT_fragment_density_map_offset) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_EXT_fragment_density_map) && (defined(VK_KHR_create_renderpass2) || defined(VK_VERSION_1_2)) && (defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_OFFSET_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceFragmentDensityMapOffsetFeaturesEXT);
#endif
#if defined(VK_EXT_fragment_density_map) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceFragmentDensityMapPropertiesEXT);
#endif
#if defined(VK_EXT_fragment_density_map2) && (defined(VK_EXT_fragment_density_map))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_2_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceFragmentDensityMap2PropertiesEXT);
#endif
#if defined(VK_EXT_fragment_density_map_offset) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_EXT_fragment_density_map) && (defined(VK_KHR_create_renderpass2) || defined(VK_VERSION_1_2)) && (defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_DENSITY_MAP_OFFSET_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceFragmentDensityMapOffsetPropertiesEXT);
#endif
#if defined(VK_EXT_fragment_density_map) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_RENDER_PASS_FRAGMENT_DENSITY_MAP_CREATE_INFO_EXT:
			return sizeof(VkRenderPassFragmentDensityMapCreateInfoEXT);
#endif
#if defined(VK_EXT_fragment_density_map_offset) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_EXT_fragment_density_map) && (defined(VK_KHR_create_renderpass2) || defined(VK_VERSION_1_2)) && (defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering)))
		case VK_STRUCTURE_TYPE_RENDER_PASS_FRAGMENT_DENSITY_MAP_OFFSET_END_INFO_EXT:
			return sizeof(VkRenderPassFragmentDensityMapOffsetEndInfoEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES:
			return sizeof(VkPhysicalDeviceScalarBlockLayoutFeatures);
#endif
#if defined(VK_KHR_surface_protected_capabilities) && (defined(VK_VERSION_1_1) && defined(VK_KHR_get_surface_capabilities2))
		case VK_STRUCTURE_TYPE_SURFACE_PROTECTED_CAPABILITIES_KHR:
			return sizeof(VkSurfaceProtectedCapabilitiesKHR);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES:
			return sizeof(VkPhysicalDeviceUniformBufferStandardLayoutFeatures);
#endif
#if defined(VK_EXT_depth_clip_enable) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceDepthClipEnableFeaturesEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_DEPTH_CLIP_STATE_CREATE_INFO_EXT:
			return sizeof(VkPipelineRasterizationDepthClipStateCreateInfoEXT);
#endif
#if defined(VK_EXT_memory_budget) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceMemoryBudgetPropertiesEXT);
#endif
#if defined(VK_EXT_memory_priority) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceMemoryPriorityFeaturesEXT);
		case VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT:
			return sizeof(VkMemoryPriorityAllocateInfoEXT);
#endif
#if defined(VK_EXT_pageable_device_local_memory) && (defined(VK_EXT_memory_priority))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PAGEABLE_DEVICE_LOCAL_MEMORY_FEATURES_EXT:
			return sizeof(VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES:
			return sizeof(VkPhysicalDeviceBufferDeviceAddressFeatures);
#endif
#if defined(VK_EXT_buffer_device_address) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceBufferDeviceAddressFeaturesEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO:
			return sizeof(VkBufferDeviceAddressInfo);
		case VK_STRUCTURE_TYPE_BUFFER_OPAQUE_CAPTURE_ADDRESS_CREATE_INFO:
			return sizeof(VkBufferOpaqueCaptureAddressCreateInfo);
#endif
#if defined(VK_EXT_buffer_device_address) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_CREATE_INFO_EXT:
			return sizeof(VkBufferDeviceAddressCreateInfoEXT);
#endif
#if defined(VK_EXT_filter_cubic)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_IMAGE_FORMAT_INFO_EXT:
			return sizeof(VkPhysicalDeviceImageViewImageFormatInfoEXT);
		case VK_STRUCTURE_TYPE_FILTER_CUBIC_IMAGE_VIEW_IMAGE_FORMAT_PROPERTIES_EXT:
			return sizeof(VkFilterCubicImageViewImageFormatPropertiesEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGELESS_FRAMEBUFFER_FEATURES:
			return sizeof(VkPhysicalDeviceImagelessFramebufferFeatures);
		case VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO:
			return sizeof(VkFramebufferAttachmentsCreateInfo);
		case VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO:
			return sizeof(VkFramebufferAttachmentImageInfo);
		case VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO:
			return sizeof(VkRenderPassAttachmentBeginInfo);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES:
			return sizeof(VkPhysicalDeviceTextureCompressionASTCHDRFeatures);
#endif
#if defined(VK_NV_cooperative_matrix) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV:
			return sizeof(VkPhysicalDeviceCooperativeMatrixFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceCooperativeMatrixPropertiesNV);
		case VK_STRUCTURE_TYPE_COOPERATIVE_MATRIX_PROPERTIES_NV:
			return sizeof(VkCooperativeMatrixPropertiesNV);
#endif
#if defined(VK_EXT_ycbcr_image_arrays) && (defined(VK_KHR_sampler_ycbcr_conversion) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_IMAGE_ARRAYS_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceYcbcrImageArraysFeaturesEXT);
#endif
#if defined(VK_NVX_image_view_handle)
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_HANDLE_INFO_NVX:
			return sizeof(VkImageViewHandleInfoNVX);
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_ADDRESS_PROPERTIES_NVX:
			return sizeof(VkImageViewAddressPropertiesNVX);
#endif
#if defined(VK_USE_PLATFORM_GGP) && (defined(VK_GGP_frame_token) && (defined(VK_KHR_swapchain) && defined(VK_GGP_stream_descriptor_surface)))
		case VK_STRUCTURE_TYPE_PRESENT_FRAME_TOKEN_GGP:
			return sizeof(VkPresentFrameTokenGGP);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO:
			return sizeof(VkPipelineCreationFeedbackCreateInfo);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_EXT_full_screen_exclusive) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain)))
		case VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT:
			return sizeof(VkSurfaceFullScreenExclusiveInfoEXT);
#endif
#if (defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_EXT_full_screen_exclusive) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain)))) && (defined(VK_KHR_win32_surface))
		case VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_WIN32_INFO_EXT:
			return sizeof(VkSurfaceFullScreenExclusiveWin32InfoEXT);
#endif
#if defined(VK_USE_PLATFORM_WIN32_KHR) && (defined(VK_EXT_full_screen_exclusive) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain)))
		case VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_FULL_SCREEN_EXCLUSIVE_EXT:
			return sizeof(VkSurfaceCapabilitiesFullScreenExclusiveEXT);
#endif
#if defined(VK_NV_present_barrier) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2) && defined(VK_KHR_swapchain))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_BARRIER_FEATURES_NV:
			return sizeof(VkPhysicalDevicePresentBarrierFeaturesNV);
		case VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_PRESENT_BARRIER_NV:
			return sizeof(VkSurfaceCapabilitiesPresentBarrierNV);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_BARRIER_CREATE_INFO_NV:
			return sizeof(VkSwapchainPresentBarrierCreateInfoNV);
#endif
#if defined(VK_KHR_performance_query) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR:
			return sizeof(VkPhysicalDevicePerformanceQueryFeaturesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_PROPERTIES_KHR:
			return sizeof(VkPhysicalDevicePerformanceQueryPropertiesKHR);
		case VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_KHR:
			return sizeof(VkPerformanceCounterKHR);
		case VK_STRUCTURE_TYPE_PERFORMANCE_COUNTER_DESCRIPTION_KHR:
			return sizeof(VkPerformanceCounterDescriptionKHR);
		case VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_CREATE_INFO_KHR:
			return sizeof(VkQueryPoolPerformanceCreateInfoKHR);
		case VK_STRUCTURE_TYPE_ACQUIRE_PROFILING_LOCK_INFO_KHR:
			return sizeof(VkAcquireProfilingLockInfoKHR);
		case VK_STRUCTURE_TYPE_PERFORMANCE_QUERY_SUBMIT_INFO_KHR:
			return sizeof(VkPerformanceQuerySubmitInfoKHR);
#endif
#if (defined(VK_KHR_performance_query) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) && (defined(VKSC_VERSION_1_0))
		case VK_STRUCTURE_TYPE_PERFORMANCE_QUERY_RESERVATION_INFO_KHR:
			return sizeof(VkPerformanceQueryReservationInfoKHR);
#endif
#if defined(VK_EXT_headless_surface) && (defined(VK_KHR_surface))
		case VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT:
			return sizeof(VkHeadlessSurfaceCreateInfoEXT);
#endif
#if defined(VK_NV_coverage_reduction_mode) && (defined(VK_NV_framebuffer_mixed_samples) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COVERAGE_REDUCTION_MODE_FEATURES_NV:
			return sizeof(VkPhysicalDeviceCoverageReductionModeFeaturesNV);
		case VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_REDUCTION_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineCoverageReductionStateCreateInfoNV);
		case VK_STRUCTURE_TYPE_FRAMEBUFFER_MIXED_SAMPLES_COMBINATION_NV:
			return sizeof(VkFramebufferMixedSamplesCombinationNV);
#endif
#if defined(VK_INTEL_shader_integer_functions2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_FUNCTIONS_2_FEATURES_INTEL:
			return sizeof(VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL);
#endif
#if defined(VK_INTEL_performance_query)
		case VK_STRUCTURE_TYPE_INITIALIZE_PERFORMANCE_API_INFO_INTEL:
			return sizeof(VkInitializePerformanceApiInfoINTEL);
		case VK_STRUCTURE_TYPE_QUERY_POOL_PERFORMANCE_QUERY_CREATE_INFO_INTEL:
			return sizeof(VkQueryPoolPerformanceQueryCreateInfoINTEL);
		case VK_STRUCTURE_TYPE_PERFORMANCE_MARKER_INFO_INTEL:
			return sizeof(VkPerformanceMarkerInfoINTEL);
		case VK_STRUCTURE_TYPE_PERFORMANCE_STREAM_MARKER_INFO_INTEL:
			return sizeof(VkPerformanceStreamMarkerInfoINTEL);
		case VK_STRUCTURE_TYPE_PERFORMANCE_OVERRIDE_INFO_INTEL:
			return sizeof(VkPerformanceOverrideInfoINTEL);
		case VK_STRUCTURE_TYPE_PERFORMANCE_CONFIGURATION_ACQUIRE_INFO_INTEL:
			return sizeof(VkPerformanceConfigurationAcquireInfoINTEL);
#endif
#if defined(VK_KHR_shader_clock) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CLOCK_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceShaderClockFeaturesKHR);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES:
			return sizeof(VkPhysicalDeviceIndexTypeUint8Features);
#endif
#if defined(VK_NV_shader_sm_builtins) && (defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceShaderSMBuiltinsPropertiesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SM_BUILTINS_FEATURES_NV:
			return sizeof(VkPhysicalDeviceShaderSMBuiltinsFeaturesNV);
#endif
#if defined(VK_EXT_fragment_shader_interlock) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_INTERLOCK_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES:
			return sizeof(VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures);
		case VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_STENCIL_LAYOUT:
			return sizeof(VkAttachmentReferenceStencilLayout);
#endif
#if defined(VK_EXT_primitive_topology_list_restart) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVE_TOPOLOGY_LIST_RESTART_FEATURES_EXT:
			return sizeof(VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_STENCIL_LAYOUT:
			return sizeof(VkAttachmentDescriptionStencilLayout);
#endif
#if defined(VK_KHR_pipeline_executable_properties) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_EXECUTABLE_PROPERTIES_FEATURES_KHR:
			return sizeof(VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_INFO_KHR:
			return sizeof(VkPipelineInfoKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_PROPERTIES_KHR:
			return sizeof(VkPipelineExecutablePropertiesKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INFO_KHR:
			return sizeof(VkPipelineExecutableInfoKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_STATISTIC_KHR:
			return sizeof(VkPipelineExecutableStatisticKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_EXECUTABLE_INTERNAL_REPRESENTATION_KHR:
			return sizeof(VkPipelineExecutableInternalRepresentationKHR);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES:
			return sizeof(VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures);
#endif
#if defined(VK_EXT_texel_buffer_alignment) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXEL_BUFFER_ALIGNMENT_PROPERTIES:
			return sizeof(VkPhysicalDeviceTexelBufferAlignmentProperties);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES:
			return sizeof(VkPhysicalDeviceSubgroupSizeControlFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES:
			return sizeof(VkPhysicalDeviceSubgroupSizeControlProperties);
		case VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_REQUIRED_SUBGROUP_SIZE_CREATE_INFO:
			return sizeof(VkPipelineShaderStageRequiredSubgroupSizeCreateInfo);
#endif
#if defined(VK_HUAWEI_subpass_shading) && (((defined(VK_KHR_create_renderpass2) || defined(VK_VERSION_1_2)) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_SUBPASS_SHADING_PIPELINE_CREATE_INFO_HUAWEI:
			return sizeof(VkSubpassShadingPipelineCreateInfoHUAWEI);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_PROPERTIES_HUAWEI:
			return sizeof(VkPhysicalDeviceSubpassShadingPropertiesHUAWEI);
#endif
#if defined(VK_HUAWEI_cluster_culling_shader) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_CULLING_SHADER_PROPERTIES_HUAWEI:
			return sizeof(VkPhysicalDeviceClusterCullingShaderPropertiesHUAWEI);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_MEMORY_OPAQUE_CAPTURE_ADDRESS_ALLOCATE_INFO:
			return sizeof(VkMemoryOpaqueCaptureAddressAllocateInfo);
		case VK_STRUCTURE_TYPE_DEVICE_MEMORY_OPAQUE_CAPTURE_ADDRESS_INFO:
			return sizeof(VkDeviceMemoryOpaqueCaptureAddressInfo);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_FEATURES:
			return sizeof(VkPhysicalDeviceLineRasterizationFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINE_RASTERIZATION_PROPERTIES:
			return sizeof(VkPhysicalDeviceLineRasterizationProperties);
		case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO:
			return sizeof(VkPipelineRasterizationLineStateCreateInfo);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES:
			return sizeof(VkPhysicalDevicePipelineCreationCacheControlFeatures);
#endif
#if defined(VK_VERSION_1_2)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES:
			return sizeof(VkPhysicalDeviceVulkan11Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES:
			return sizeof(VkPhysicalDeviceVulkan11Properties);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES:
			return sizeof(VkPhysicalDeviceVulkan12Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES:
			return sizeof(VkPhysicalDeviceVulkan12Properties);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES:
			return sizeof(VkPhysicalDeviceVulkan13Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES:
			return sizeof(VkPhysicalDeviceVulkan13Properties);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES:
			return sizeof(VkPhysicalDeviceVulkan14Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_PROPERTIES:
			return sizeof(VkPhysicalDeviceVulkan14Properties);
#endif
#if defined(VK_AMD_pipeline_compiler_control)
		case VK_STRUCTURE_TYPE_PIPELINE_COMPILER_CONTROL_CREATE_INFO_AMD:
			return sizeof(VkPipelineCompilerControlCreateInfoAMD);
#endif
#if defined(VK_AMD_device_coherent_memory) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD:
			return sizeof(VkPhysicalDeviceCoherentMemoryFeaturesAMD);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TOOL_PROPERTIES:
			return sizeof(VkPhysicalDeviceToolProperties);
#endif
#if defined(VK_EXT_custom_border_color) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_SAMPLER_CUSTOM_BORDER_COLOR_CREATE_INFO_EXT:
			return sizeof(VkSamplerCustomBorderColorCreateInfoEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceCustomBorderColorPropertiesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceCustomBorderColorFeaturesEXT);
#endif
#if defined(VK_EXT_border_color_swizzle) && (defined(VK_EXT_custom_border_color))
		case VK_STRUCTURE_TYPE_SAMPLER_BORDER_COLOR_COMPONENT_MAPPING_CREATE_INFO_EXT:
			return sizeof(VkSamplerBorderColorComponentMappingCreateInfoEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BORDER_COLOR_SWIZZLE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceBorderColorSwizzleFeaturesEXT);
#endif
#if defined(VK_KHR_acceleration_structure) && (((defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_deferred_host_operations))
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR:
			return sizeof(VkAccelerationStructureGeometryTrianglesDataKHR);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_AABBS_DATA_KHR:
			return sizeof(VkAccelerationStructureGeometryAabbsDataKHR);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR:
			return sizeof(VkAccelerationStructureGeometryInstancesDataKHR);
#endif
#if defined(VK_NV_ray_tracing_linear_swept_spheres) && (defined(VK_KHR_ray_tracing_pipeline))
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_LINEAR_SWEPT_SPHERES_DATA_NV:
			return sizeof(VkAccelerationStructureGeometryLinearSweptSpheresDataNV);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_SPHERES_DATA_NV:
			return sizeof(VkAccelerationStructureGeometrySpheresDataNV);
#endif
#if defined(VK_KHR_acceleration_structure) && (((defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_deferred_host_operations))
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR:
			return sizeof(VkAccelerationStructureGeometryKHR);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR:
			return sizeof(VkAccelerationStructureBuildGeometryInfoKHR);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR:
			return sizeof(VkAccelerationStructureCreateInfoKHR);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR:
			return sizeof(VkAccelerationStructureDeviceAddressInfoKHR);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_VERSION_INFO_KHR:
			return sizeof(VkAccelerationStructureVersionInfoKHR);
		case VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR:
			return sizeof(VkCopyAccelerationStructureInfoKHR);
		case VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_TO_MEMORY_INFO_KHR:
			return sizeof(VkCopyAccelerationStructureToMemoryInfoKHR);
		case VK_STRUCTURE_TYPE_COPY_MEMORY_TO_ACCELERATION_STRUCTURE_INFO_KHR:
			return sizeof(VkCopyMemoryToAccelerationStructureInfoKHR);
#endif
#if defined(VK_KHR_ray_tracing_pipeline) && ((defined(VK_KHR_spirv_1_4) || defined(VK_VERSION_1_2)) && defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_INTERFACE_CREATE_INFO_KHR:
			return sizeof(VkRayTracingPipelineInterfaceCreateInfoKHR);
#endif
#if defined(VK_KHR_pipeline_library)
		case VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR:
			return sizeof(VkPipelineLibraryCreateInfoKHR);
#endif
#if defined(VK_KHR_object_refresh)
		case VK_STRUCTURE_TYPE_REFRESH_OBJECT_LIST_KHR:
			return sizeof(VkRefreshObjectListKHR);
#endif
#if defined(VK_EXT_extended_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceExtendedDynamicStateFeaturesEXT);
#endif
#if defined(VK_EXT_extended_dynamic_state2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceExtendedDynamicState2FeaturesEXT);
#endif
#if defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceExtendedDynamicState3FeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceExtendedDynamicState3PropertiesEXT);
#endif
#if defined(VK_QCOM_render_pass_transform)
		case VK_STRUCTURE_TYPE_RENDER_PASS_TRANSFORM_BEGIN_INFO_QCOM:
			return sizeof(VkRenderPassTransformBeginInfoQCOM);
#endif
#if defined(VK_QCOM_rotated_copy_commands) && (defined(VK_KHR_copy_commands2) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_COPY_COMMAND_TRANSFORM_INFO_QCOM:
			return sizeof(VkCopyCommandTransformInfoQCOM);
#endif
#if defined(VK_QCOM_render_pass_transform)
		case VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDER_PASS_TRANSFORM_INFO_QCOM:
			return sizeof(VkCommandBufferInheritanceRenderPassTransformInfoQCOM);
#endif
#if defined(VK_NV_partitioned_acceleration_structure) && (defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PARTITIONED_ACCELERATION_STRUCTURE_FEATURES_NV:
			return sizeof(VkPhysicalDevicePartitionedAccelerationStructureFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PARTITIONED_ACCELERATION_STRUCTURE_PROPERTIES_NV:
			return sizeof(VkPhysicalDevicePartitionedAccelerationStructurePropertiesNV);
		case VK_STRUCTURE_TYPE_PARTITIONED_ACCELERATION_STRUCTURE_FLAGS_NV:
			return sizeof(VkPartitionedAccelerationStructureFlagsNV);
		case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_PARTITIONED_ACCELERATION_STRUCTURE_NV:
			return sizeof(VkWriteDescriptorSetPartitionedAccelerationStructureNV);
		case VK_STRUCTURE_TYPE_PARTITIONED_ACCELERATION_STRUCTURE_INSTANCES_INPUT_NV:
			return sizeof(VkPartitionedAccelerationStructureInstancesInputNV);
		case VK_STRUCTURE_TYPE_BUILD_PARTITIONED_ACCELERATION_STRUCTURE_INFO_NV:
			return sizeof(VkBuildPartitionedAccelerationStructureInfoNV);
#endif
#if defined(VK_NV_device_diagnostics_config) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DIAGNOSTICS_CONFIG_FEATURES_NV:
			return sizeof(VkPhysicalDeviceDiagnosticsConfigFeaturesNV);
		case VK_STRUCTURE_TYPE_DEVICE_DIAGNOSTICS_CONFIG_CREATE_INFO_NV:
			return sizeof(VkDeviceDiagnosticsConfigCreateInfoNV);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_WORKGROUP_MEMORY_FEATURES:
			return sizeof(VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures);
#endif
#if defined(VK_KHR_shader_subgroup_uniform_control_flow) && (defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_UNIFORM_CONTROL_FLOW_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR);
#endif
#if defined(VK_KHR_robustness2) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceRobustness2FeaturesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceRobustness2PropertiesKHR);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ROBUSTNESS_FEATURES:
			return sizeof(VkPhysicalDeviceImageRobustnessFeatures);
#endif
#if defined(VK_KHR_workgroup_memory_explicit_layout) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_WORKGROUP_MEMORY_EXPLICIT_LAYOUT_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS) && (defined(VK_KHR_portability_subset) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR:
			return sizeof(VkPhysicalDevicePortabilitySubsetFeaturesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR:
			return sizeof(VkPhysicalDevicePortabilitySubsetPropertiesKHR);
#endif
#if defined(VK_EXT_4444_formats) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT:
			return sizeof(VkPhysicalDevice4444FormatsFeaturesEXT);
#endif
#if defined(VK_HUAWEI_subpass_shading) && (((defined(VK_KHR_create_renderpass2) || defined(VK_VERSION_1_2)) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_SHADING_FEATURES_HUAWEI:
			return sizeof(VkPhysicalDeviceSubpassShadingFeaturesHUAWEI);
#endif
#if defined(VK_HUAWEI_cluster_culling_shader) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_CULLING_SHADER_FEATURES_HUAWEI:
			return sizeof(VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CLUSTER_CULLING_SHADER_VRS_FEATURES_HUAWEI:
			return sizeof(VkPhysicalDeviceClusterCullingShaderVrsFeaturesHUAWEI);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_BUFFER_COPY_2:
			return sizeof(VkBufferCopy2);
		case VK_STRUCTURE_TYPE_IMAGE_COPY_2:
			return sizeof(VkImageCopy2);
		case VK_STRUCTURE_TYPE_IMAGE_BLIT_2:
			return sizeof(VkImageBlit2);
		case VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2:
			return sizeof(VkBufferImageCopy2);
		case VK_STRUCTURE_TYPE_IMAGE_RESOLVE_2:
			return sizeof(VkImageResolve2);
		case VK_STRUCTURE_TYPE_COPY_BUFFER_INFO_2:
			return sizeof(VkCopyBufferInfo2);
		case VK_STRUCTURE_TYPE_COPY_IMAGE_INFO_2:
			return sizeof(VkCopyImageInfo2);
		case VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2:
			return sizeof(VkBlitImageInfo2);
		case VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2:
			return sizeof(VkCopyBufferToImageInfo2);
		case VK_STRUCTURE_TYPE_COPY_IMAGE_TO_BUFFER_INFO_2:
			return sizeof(VkCopyImageToBufferInfo2);
		case VK_STRUCTURE_TYPE_RESOLVE_IMAGE_INFO_2:
			return sizeof(VkResolveImageInfo2);
#endif
#if defined(VK_EXT_shader_image_atomic_int64) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT);
#endif
#if defined(VK_KHR_fragment_shading_rate) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_create_renderpass2)) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_FRAGMENT_SHADING_RATE_ATTACHMENT_INFO_KHR:
			return sizeof(VkFragmentShadingRateAttachmentInfoKHR);
		case VK_STRUCTURE_TYPE_PIPELINE_FRAGMENT_SHADING_RATE_STATE_CREATE_INFO_KHR:
			return sizeof(VkPipelineFragmentShadingRateStateCreateInfoKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceFragmentShadingRateFeaturesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceFragmentShadingRatePropertiesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_KHR:
			return sizeof(VkPhysicalDeviceFragmentShadingRateKHR);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TERMINATE_INVOCATION_FEATURES:
			return sizeof(VkPhysicalDeviceShaderTerminateInvocationFeatures);
#endif
#if defined(VK_NV_fragment_shading_rate_enums) && (defined(VK_KHR_fragment_shading_rate))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_FEATURES_NV:
			return sizeof(VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_ENUMS_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceFragmentShadingRateEnumsPropertiesNV);
		case VK_STRUCTURE_TYPE_PIPELINE_FRAGMENT_SHADING_RATE_ENUM_STATE_CREATE_INFO_NV:
			return sizeof(VkPipelineFragmentShadingRateEnumStateCreateInfoNV);
#endif
#if defined(VK_KHR_acceleration_structure) && (((defined(VK_VERSION_1_1) && defined(VK_EXT_descriptor_indexing) && defined(VK_KHR_buffer_device_address)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_deferred_host_operations))
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR:
			return sizeof(VkAccelerationStructureBuildSizesInfoKHR);
#endif
#if defined(VK_EXT_image_2d_view_of_3d) && ((defined(VK_KHR_maintenance1) && defined(VK_KHR_get_physical_device_properties2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_2D_VIEW_OF_3D_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceImage2DViewOf3DFeaturesEXT);
#endif
#if defined(VK_EXT_image_sliced_view_of_3d) && ((defined(VK_KHR_maintenance1) && defined(VK_KHR_get_physical_device_properties2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_SLICED_VIEW_OF_3D_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT);
#endif
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_EXT_attachment_feedback_loop_layout))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ATTACHMENT_FEEDBACK_LOOP_DYNAMIC_STATE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT);
#endif
#if defined(VK_EXT_legacy_vertex_attributes) && (defined(VK_EXT_vertex_input_dynamic_state))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LEGACY_VERTEX_ATTRIBUTES_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceLegacyVertexAttributesFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LEGACY_VERTEX_ATTRIBUTES_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceLegacyVertexAttributesPropertiesEXT);
#endif
#if defined(VK_EXT_mutable_descriptor_type) && (defined(VK_KHR_maintenance3) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MUTABLE_DESCRIPTOR_TYPE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT);
		case VK_STRUCTURE_TYPE_MUTABLE_DESCRIPTOR_TYPE_CREATE_INFO_EXT:
			return sizeof(VkMutableDescriptorTypeCreateInfoEXT);
#endif
#if defined(VK_EXT_depth_clip_control) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_CONTROL_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceDepthClipControlFeaturesEXT);
#endif
#if defined(VK_EXT_zero_initialize_device_memory) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ZERO_INITIALIZE_DEVICE_MEMORY_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceZeroInitializeDeviceMemoryFeaturesEXT);
#endif
#if defined(VK_EXT_device_generated_commands) && (((defined(VK_KHR_buffer_device_address) || defined(VK_VERSION_1_2)) && defined(VK_KHR_maintenance5)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceDeviceGeneratedCommandsFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEVICE_GENERATED_COMMANDS_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceDeviceGeneratedCommandsPropertiesEXT);
		case VK_STRUCTURE_TYPE_GENERATED_COMMANDS_PIPELINE_INFO_EXT:
			return sizeof(VkGeneratedCommandsPipelineInfoEXT);
		case VK_STRUCTURE_TYPE_GENERATED_COMMANDS_SHADER_INFO_EXT:
			return sizeof(VkGeneratedCommandsShaderInfoEXT);
		case VK_STRUCTURE_TYPE_GENERATED_COMMANDS_MEMORY_REQUIREMENTS_INFO_EXT:
			return sizeof(VkGeneratedCommandsMemoryRequirementsInfoEXT);
		case VK_STRUCTURE_TYPE_INDIRECT_EXECUTION_SET_PIPELINE_INFO_EXT:
			return sizeof(VkIndirectExecutionSetPipelineInfoEXT);
		case VK_STRUCTURE_TYPE_INDIRECT_EXECUTION_SET_SHADER_LAYOUT_INFO_EXT:
			return sizeof(VkIndirectExecutionSetShaderLayoutInfoEXT);
		case VK_STRUCTURE_TYPE_INDIRECT_EXECUTION_SET_SHADER_INFO_EXT:
			return sizeof(VkIndirectExecutionSetShaderInfoEXT);
		case VK_STRUCTURE_TYPE_INDIRECT_EXECUTION_SET_CREATE_INFO_EXT:
			return sizeof(VkIndirectExecutionSetCreateInfoEXT);
		case VK_STRUCTURE_TYPE_GENERATED_COMMANDS_INFO_EXT:
			return sizeof(VkGeneratedCommandsInfoEXT);
		case VK_STRUCTURE_TYPE_WRITE_INDIRECT_EXECUTION_SET_PIPELINE_EXT:
			return sizeof(VkWriteIndirectExecutionSetPipelineEXT);
#endif
#if (defined(VK_EXT_device_generated_commands) && (((defined(VK_KHR_buffer_device_address) || defined(VK_VERSION_1_2)) && defined(VK_KHR_maintenance5)) || defined(VK_VERSION_1_3))) && (defined(VK_EXT_shader_object))
		case VK_STRUCTURE_TYPE_WRITE_INDIRECT_EXECUTION_SET_SHADER_EXT:
			return sizeof(VkWriteIndirectExecutionSetShaderEXT);
#endif
#if defined(VK_EXT_device_generated_commands) && (((defined(VK_KHR_buffer_device_address) || defined(VK_VERSION_1_2)) && defined(VK_KHR_maintenance5)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_CREATE_INFO_EXT:
			return sizeof(VkIndirectCommandsLayoutCreateInfoEXT);
		case VK_STRUCTURE_TYPE_INDIRECT_COMMANDS_LAYOUT_TOKEN_EXT:
			return sizeof(VkIndirectCommandsLayoutTokenEXT);
#endif
#if defined(VK_EXT_depth_clip_control) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_DEPTH_CLIP_CONTROL_CREATE_INFO_EXT:
			return sizeof(VkPipelineViewportDepthClipControlCreateInfoEXT);
#endif
#if defined(VK_EXT_depth_clamp_control) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLAMP_CONTROL_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceDepthClampControlFeaturesEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_DEPTH_CLAMP_CONTROL_CREATE_INFO_EXT:
			return sizeof(VkPipelineViewportDepthClampControlCreateInfoEXT);
#endif
#if defined(VK_EXT_vertex_input_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT);
#endif
#if defined(VK_NV_external_memory_rdma) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_RDMA_FEATURES_NV:
			return sizeof(VkPhysicalDeviceExternalMemoryRDMAFeaturesNV);
#endif
#if defined(VK_KHR_shader_relaxed_extended_instruction)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_RELAXED_EXTENDED_INSTRUCTION_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceShaderRelaxedExtendedInstructionFeaturesKHR);
#endif
#if (defined(VK_EXT_vertex_input_dynamic_state) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_dynamic_rendering)) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_VERTEX_INPUT_BINDING_DESCRIPTION_2_EXT:
			return sizeof(VkVertexInputBindingDescription2EXT);
		case VK_STRUCTURE_TYPE_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_2_EXT:
			return sizeof(VkVertexInputAttributeDescription2EXT);
#endif
#if defined(VK_EXT_color_write_enable) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceColorWriteEnableFeaturesEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_COLOR_WRITE_CREATE_INFO_EXT:
			return sizeof(VkPipelineColorWriteCreateInfoEXT);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_MEMORY_BARRIER_2:
			return sizeof(VkMemoryBarrier2);
		case VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2:
			return sizeof(VkImageMemoryBarrier2);
		case VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2:
			return sizeof(VkBufferMemoryBarrier2);
#endif
#if defined(VK_KHR_maintenance8) && (defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_MEMORY_BARRIER_ACCESS_FLAGS_3_KHR:
			return sizeof(VkMemoryBarrierAccessFlags3KHR);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_DEPENDENCY_INFO:
			return sizeof(VkDependencyInfo);
		case VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO:
			return sizeof(VkSemaphoreSubmitInfo);
		case VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO:
			return sizeof(VkCommandBufferSubmitInfo);
		case VK_STRUCTURE_TYPE_SUBMIT_INFO_2:
			return sizeof(VkSubmitInfo2);
#endif
#if (defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
		case VK_STRUCTURE_TYPE_QUEUE_FAMILY_CHECKPOINT_PROPERTIES_2_NV:
			return sizeof(VkQueueFamilyCheckpointProperties2NV);
		case VK_STRUCTURE_TYPE_CHECKPOINT_DATA_2_NV:
			return sizeof(VkCheckpointData2NV);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES:
			return sizeof(VkPhysicalDeviceSynchronization2Features);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_FEATURES:
			return sizeof(VkPhysicalDeviceHostImageCopyFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_IMAGE_COPY_PROPERTIES:
			return sizeof(VkPhysicalDeviceHostImageCopyProperties);
		case VK_STRUCTURE_TYPE_MEMORY_TO_IMAGE_COPY:
			return sizeof(VkMemoryToImageCopy);
		case VK_STRUCTURE_TYPE_IMAGE_TO_MEMORY_COPY:
			return sizeof(VkImageToMemoryCopy);
		case VK_STRUCTURE_TYPE_COPY_MEMORY_TO_IMAGE_INFO:
			return sizeof(VkCopyMemoryToImageInfo);
		case VK_STRUCTURE_TYPE_COPY_IMAGE_TO_MEMORY_INFO:
			return sizeof(VkCopyImageToMemoryInfo);
		case VK_STRUCTURE_TYPE_COPY_IMAGE_TO_IMAGE_INFO:
			return sizeof(VkCopyImageToImageInfo);
		case VK_STRUCTURE_TYPE_HOST_IMAGE_LAYOUT_TRANSITION_INFO:
			return sizeof(VkHostImageLayoutTransitionInfo);
		case VK_STRUCTURE_TYPE_SUBRESOURCE_HOST_MEMCPY_SIZE:
			return sizeof(VkSubresourceHostMemcpySize);
		case VK_STRUCTURE_TYPE_HOST_IMAGE_COPY_DEVICE_PERFORMANCE_QUERY:
			return sizeof(VkHostImageCopyDevicePerformanceQuery);
#endif
#if defined(VK_EXT_primitives_generated_query) && (defined(VK_EXT_transform_feedback))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRIMITIVES_GENERATED_QUERY_FEATURES_EXT:
			return sizeof(VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT);
#endif
#if defined(VK_EXT_legacy_dithering) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LEGACY_DITHERING_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceLegacyDitheringFeaturesEXT);
#endif
#if defined(VK_EXT_multisampled_render_to_single_sampled) && ((defined(VK_KHR_create_renderpass2) && defined(VK_KHR_depth_stencil_resolve)) || defined(VK_VERSION_1_2))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT);
		case VK_STRUCTURE_TYPE_SUBPASS_RESOLVE_PERFORMANCE_QUERY_EXT:
			return sizeof(VkSubpassResolvePerformanceQueryEXT);
		case VK_STRUCTURE_TYPE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_INFO_EXT:
			return sizeof(VkMultisampledRenderToSingleSampledInfoEXT);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_PROTECTED_ACCESS_FEATURES:
			return sizeof(VkPhysicalDevicePipelineProtectedAccessFeatures);
#endif
#if defined(VK_KHR_video_queue) && ((defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_QUEUE_FAMILY_VIDEO_PROPERTIES_KHR:
			return sizeof(VkQueueFamilyVideoPropertiesKHR);
		case VK_STRUCTURE_TYPE_QUEUE_FAMILY_QUERY_RESULT_STATUS_PROPERTIES_KHR:
			return sizeof(VkQueueFamilyQueryResultStatusPropertiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_PROFILE_LIST_INFO_KHR:
			return sizeof(VkVideoProfileListInfoKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_FORMAT_INFO_KHR:
			return sizeof(VkPhysicalDeviceVideoFormatInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_FORMAT_PROPERTIES_KHR:
			return sizeof(VkVideoFormatPropertiesKHR);
#endif
#if defined(VK_KHR_video_encode_quantization_map) && (defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_QUANTIZATION_MAP_CAPABILITIES_KHR:
			return sizeof(VkVideoEncodeQuantizationMapCapabilitiesKHR);
#endif
#if (defined(VK_KHR_video_encode_quantization_map) && (defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3)))) && (defined(VK_KHR_video_encode_h264))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_QUANTIZATION_MAP_CAPABILITIES_KHR:
			return sizeof(VkVideoEncodeH264QuantizationMapCapabilitiesKHR);
#endif
#if (defined(VK_KHR_video_encode_quantization_map) && (defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3)))) && (defined(VK_KHR_video_encode_h265))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_QUANTIZATION_MAP_CAPABILITIES_KHR:
			return sizeof(VkVideoEncodeH265QuantizationMapCapabilitiesKHR);
#endif
#if (defined(VK_KHR_video_encode_quantization_map) && (defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3)))) && (defined(VK_KHR_video_encode_av1))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_QUANTIZATION_MAP_CAPABILITIES_KHR:
			return sizeof(VkVideoEncodeAV1QuantizationMapCapabilitiesKHR);
#endif
#if defined(VK_KHR_video_encode_quantization_map) && (defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_VIDEO_FORMAT_QUANTIZATION_MAP_PROPERTIES_KHR:
			return sizeof(VkVideoFormatQuantizationMapPropertiesKHR);
#endif
#if (defined(VK_KHR_video_encode_quantization_map) && (defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3)))) && (defined(VK_KHR_video_encode_h265))
		case VK_STRUCTURE_TYPE_VIDEO_FORMAT_H265_QUANTIZATION_MAP_PROPERTIES_KHR:
			return sizeof(VkVideoFormatH265QuantizationMapPropertiesKHR);
#endif
#if (defined(VK_KHR_video_encode_quantization_map) && (defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3)))) && (defined(VK_KHR_video_encode_av1))
		case VK_STRUCTURE_TYPE_VIDEO_FORMAT_AV1_QUANTIZATION_MAP_PROPERTIES_KHR:
			return sizeof(VkVideoFormatAV1QuantizationMapPropertiesKHR);
#endif
#if defined(VK_KHR_video_queue) && ((defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_VIDEO_PROFILE_INFO_KHR:
			return sizeof(VkVideoProfileInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_CAPABILITIES_KHR:
			return sizeof(VkVideoCapabilitiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_SESSION_MEMORY_REQUIREMENTS_KHR:
			return sizeof(VkVideoSessionMemoryRequirementsKHR);
		case VK_STRUCTURE_TYPE_BIND_VIDEO_SESSION_MEMORY_INFO_KHR:
			return sizeof(VkBindVideoSessionMemoryInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_PICTURE_RESOURCE_INFO_KHR:
			return sizeof(VkVideoPictureResourceInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_REFERENCE_SLOT_INFO_KHR:
			return sizeof(VkVideoReferenceSlotInfoKHR);
#endif
#if defined(VK_KHR_video_decode_queue) && (defined(VK_KHR_video_queue) && (defined(VK_KHR_synchronization2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_CAPABILITIES_KHR:
			return sizeof(VkVideoDecodeCapabilitiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_USAGE_INFO_KHR:
			return sizeof(VkVideoDecodeUsageInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_INFO_KHR:
			return sizeof(VkVideoDecodeInfoKHR);
#endif
#if defined(VK_KHR_video_maintenance1) && (defined(VK_KHR_video_queue))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_MAINTENANCE_1_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceVideoMaintenance1FeaturesKHR);
#endif
#if defined(VK_KHR_video_maintenance2) && (defined(VK_KHR_video_queue))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_MAINTENANCE_2_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceVideoMaintenance2FeaturesKHR);
#endif
#if defined(VK_KHR_video_maintenance1) && (defined(VK_KHR_video_queue))
		case VK_STRUCTURE_TYPE_VIDEO_INLINE_QUERY_INFO_KHR:
			return sizeof(VkVideoInlineQueryInfoKHR);
#endif
#if defined(VK_KHR_video_decode_h264) && (defined(VK_KHR_video_decode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_PROFILE_INFO_KHR:
			return sizeof(VkVideoDecodeH264ProfileInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_CAPABILITIES_KHR:
			return sizeof(VkVideoDecodeH264CapabilitiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_SESSION_PARAMETERS_ADD_INFO_KHR:
			return sizeof(VkVideoDecodeH264SessionParametersAddInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_SESSION_PARAMETERS_CREATE_INFO_KHR:
			return sizeof(VkVideoDecodeH264SessionParametersCreateInfoKHR);
#endif
#if (defined(VK_KHR_video_maintenance2) && (defined(VK_KHR_video_queue))) && (defined(VK_KHR_video_decode_h264))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_INLINE_SESSION_PARAMETERS_INFO_KHR:
			return sizeof(VkVideoDecodeH264InlineSessionParametersInfoKHR);
#endif
#if defined(VK_KHR_video_decode_h264) && (defined(VK_KHR_video_decode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_PICTURE_INFO_KHR:
			return sizeof(VkVideoDecodeH264PictureInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_DPB_SLOT_INFO_KHR:
			return sizeof(VkVideoDecodeH264DpbSlotInfoKHR);
#endif
#if defined(VK_KHR_video_decode_h265) && (defined(VK_KHR_video_decode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PROFILE_INFO_KHR:
			return sizeof(VkVideoDecodeH265ProfileInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_CAPABILITIES_KHR:
			return sizeof(VkVideoDecodeH265CapabilitiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_SESSION_PARAMETERS_ADD_INFO_KHR:
			return sizeof(VkVideoDecodeH265SessionParametersAddInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_SESSION_PARAMETERS_CREATE_INFO_KHR:
			return sizeof(VkVideoDecodeH265SessionParametersCreateInfoKHR);
#endif
#if (defined(VK_KHR_video_maintenance2) && (defined(VK_KHR_video_queue))) && (defined(VK_KHR_video_decode_h265))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_INLINE_SESSION_PARAMETERS_INFO_KHR:
			return sizeof(VkVideoDecodeH265InlineSessionParametersInfoKHR);
#endif
#if defined(VK_KHR_video_decode_h265) && (defined(VK_KHR_video_decode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PICTURE_INFO_KHR:
			return sizeof(VkVideoDecodeH265PictureInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_DPB_SLOT_INFO_KHR:
			return sizeof(VkVideoDecodeH265DpbSlotInfoKHR);
#endif
#if defined(VK_KHR_video_decode_av1) && (defined(VK_KHR_video_decode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_PROFILE_INFO_KHR:
			return sizeof(VkVideoDecodeAV1ProfileInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_CAPABILITIES_KHR:
			return sizeof(VkVideoDecodeAV1CapabilitiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_SESSION_PARAMETERS_CREATE_INFO_KHR:
			return sizeof(VkVideoDecodeAV1SessionParametersCreateInfoKHR);
#endif
#if (defined(VK_KHR_video_maintenance2) && (defined(VK_KHR_video_queue))) && (defined(VK_KHR_video_decode_av1))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_INLINE_SESSION_PARAMETERS_INFO_KHR:
			return sizeof(VkVideoDecodeAV1InlineSessionParametersInfoKHR);
#endif
#if defined(VK_KHR_video_decode_av1) && (defined(VK_KHR_video_decode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_PICTURE_INFO_KHR:
			return sizeof(VkVideoDecodeAV1PictureInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_DPB_SLOT_INFO_KHR:
			return sizeof(VkVideoDecodeAV1DpbSlotInfoKHR);
#endif
#if defined(VK_KHR_video_queue) && ((defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_VIDEO_SESSION_CREATE_INFO_KHR:
			return sizeof(VkVideoSessionCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_SESSION_PARAMETERS_CREATE_INFO_KHR:
			return sizeof(VkVideoSessionParametersCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_SESSION_PARAMETERS_UPDATE_INFO_KHR:
			return sizeof(VkVideoSessionParametersUpdateInfoKHR);
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && (defined(VK_KHR_synchronization2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_SESSION_PARAMETERS_GET_INFO_KHR:
			return sizeof(VkVideoEncodeSessionParametersGetInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_SESSION_PARAMETERS_FEEDBACK_INFO_KHR:
			return sizeof(VkVideoEncodeSessionParametersFeedbackInfoKHR);
#endif
#if defined(VK_KHR_video_queue) && ((defined(VK_VERSION_1_1) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_VIDEO_BEGIN_CODING_INFO_KHR:
			return sizeof(VkVideoBeginCodingInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_END_CODING_INFO_KHR:
			return sizeof(VkVideoEndCodingInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_CODING_CONTROL_INFO_KHR:
			return sizeof(VkVideoCodingControlInfoKHR);
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && (defined(VK_KHR_synchronization2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_USAGE_INFO_KHR:
			return sizeof(VkVideoEncodeUsageInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_INFO_KHR:
			return sizeof(VkVideoEncodeInfoKHR);
#endif
#if defined(VK_KHR_video_encode_quantization_map) && (defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_QUANTIZATION_MAP_INFO_KHR:
			return sizeof(VkVideoEncodeQuantizationMapInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_QUANTIZATION_MAP_SESSION_PARAMETERS_CREATE_INFO_KHR:
			return sizeof(VkVideoEncodeQuantizationMapSessionParametersCreateInfoKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_ENCODE_QUANTIZATION_MAP_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceVideoEncodeQuantizationMapFeaturesKHR);
#endif
#if defined(VK_KHR_video_encode_queue) && (defined(VK_KHR_video_queue) && (defined(VK_KHR_synchronization2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_QUERY_POOL_VIDEO_ENCODE_FEEDBACK_CREATE_INFO_KHR:
			return sizeof(VkQueryPoolVideoEncodeFeedbackCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_QUALITY_LEVEL_INFO_KHR:
			return sizeof(VkVideoEncodeQualityLevelInfoKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_ENCODE_QUALITY_LEVEL_INFO_KHR:
			return sizeof(VkPhysicalDeviceVideoEncodeQualityLevelInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_QUALITY_LEVEL_PROPERTIES_KHR:
			return sizeof(VkVideoEncodeQualityLevelPropertiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_RATE_CONTROL_INFO_KHR:
			return sizeof(VkVideoEncodeRateControlInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_RATE_CONTROL_LAYER_INFO_KHR:
			return sizeof(VkVideoEncodeRateControlLayerInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_CAPABILITIES_KHR:
			return sizeof(VkVideoEncodeCapabilitiesKHR);
#endif
#if defined(VK_KHR_video_encode_h264) && (defined(VK_KHR_video_encode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_CAPABILITIES_KHR:
			return sizeof(VkVideoEncodeH264CapabilitiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_QUALITY_LEVEL_PROPERTIES_KHR:
			return sizeof(VkVideoEncodeH264QualityLevelPropertiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_SESSION_CREATE_INFO_KHR:
			return sizeof(VkVideoEncodeH264SessionCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_SESSION_PARAMETERS_ADD_INFO_KHR:
			return sizeof(VkVideoEncodeH264SessionParametersAddInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_SESSION_PARAMETERS_CREATE_INFO_KHR:
			return sizeof(VkVideoEncodeH264SessionParametersCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_SESSION_PARAMETERS_GET_INFO_KHR:
			return sizeof(VkVideoEncodeH264SessionParametersGetInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_SESSION_PARAMETERS_FEEDBACK_INFO_KHR:
			return sizeof(VkVideoEncodeH264SessionParametersFeedbackInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_DPB_SLOT_INFO_KHR:
			return sizeof(VkVideoEncodeH264DpbSlotInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_PICTURE_INFO_KHR:
			return sizeof(VkVideoEncodeH264PictureInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_PROFILE_INFO_KHR:
			return sizeof(VkVideoEncodeH264ProfileInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_NALU_SLICE_INFO_KHR:
			return sizeof(VkVideoEncodeH264NaluSliceInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_RATE_CONTROL_INFO_KHR:
			return sizeof(VkVideoEncodeH264RateControlInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_GOP_REMAINING_FRAME_INFO_KHR:
			return sizeof(VkVideoEncodeH264GopRemainingFrameInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H264_RATE_CONTROL_LAYER_INFO_KHR:
			return sizeof(VkVideoEncodeH264RateControlLayerInfoKHR);
#endif
#if defined(VK_KHR_video_encode_h265) && (defined(VK_KHR_video_encode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_CAPABILITIES_KHR:
			return sizeof(VkVideoEncodeH265CapabilitiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_QUALITY_LEVEL_PROPERTIES_KHR:
			return sizeof(VkVideoEncodeH265QualityLevelPropertiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_SESSION_CREATE_INFO_KHR:
			return sizeof(VkVideoEncodeH265SessionCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_SESSION_PARAMETERS_ADD_INFO_KHR:
			return sizeof(VkVideoEncodeH265SessionParametersAddInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_SESSION_PARAMETERS_CREATE_INFO_KHR:
			return sizeof(VkVideoEncodeH265SessionParametersCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_SESSION_PARAMETERS_GET_INFO_KHR:
			return sizeof(VkVideoEncodeH265SessionParametersGetInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_SESSION_PARAMETERS_FEEDBACK_INFO_KHR:
			return sizeof(VkVideoEncodeH265SessionParametersFeedbackInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_PICTURE_INFO_KHR:
			return sizeof(VkVideoEncodeH265PictureInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_NALU_SLICE_SEGMENT_INFO_KHR:
			return sizeof(VkVideoEncodeH265NaluSliceSegmentInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_RATE_CONTROL_INFO_KHR:
			return sizeof(VkVideoEncodeH265RateControlInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_GOP_REMAINING_FRAME_INFO_KHR:
			return sizeof(VkVideoEncodeH265GopRemainingFrameInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_RATE_CONTROL_LAYER_INFO_KHR:
			return sizeof(VkVideoEncodeH265RateControlLayerInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_PROFILE_INFO_KHR:
			return sizeof(VkVideoEncodeH265ProfileInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_H265_DPB_SLOT_INFO_KHR:
			return sizeof(VkVideoEncodeH265DpbSlotInfoKHR);
#endif
#if defined(VK_KHR_video_encode_av1) && (defined(VK_KHR_video_encode_queue))
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_CAPABILITIES_KHR:
			return sizeof(VkVideoEncodeAV1CapabilitiesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_QUALITY_LEVEL_PROPERTIES_KHR:
			return sizeof(VkVideoEncodeAV1QualityLevelPropertiesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_ENCODE_AV1_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceVideoEncodeAV1FeaturesKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_SESSION_CREATE_INFO_KHR:
			return sizeof(VkVideoEncodeAV1SessionCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_SESSION_PARAMETERS_CREATE_INFO_KHR:
			return sizeof(VkVideoEncodeAV1SessionParametersCreateInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_DPB_SLOT_INFO_KHR:
			return sizeof(VkVideoEncodeAV1DpbSlotInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_PICTURE_INFO_KHR:
			return sizeof(VkVideoEncodeAV1PictureInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_PROFILE_INFO_KHR:
			return sizeof(VkVideoEncodeAV1ProfileInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_RATE_CONTROL_INFO_KHR:
			return sizeof(VkVideoEncodeAV1RateControlInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_GOP_REMAINING_FRAME_INFO_KHR:
			return sizeof(VkVideoEncodeAV1GopRemainingFrameInfoKHR);
		case VK_STRUCTURE_TYPE_VIDEO_ENCODE_AV1_RATE_CONTROL_LAYER_INFO_KHR:
			return sizeof(VkVideoEncodeAV1RateControlLayerInfoKHR);
#endif
#if defined(VK_NV_inherited_viewport_scissor) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INHERITED_VIEWPORT_SCISSOR_FEATURES_NV:
			return sizeof(VkPhysicalDeviceInheritedViewportScissorFeaturesNV);
		case VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_VIEWPORT_SCISSOR_INFO_NV:
			return sizeof(VkCommandBufferInheritanceViewportScissorInfoNV);
#endif
#if defined(VK_EXT_ycbcr_2plane_444_formats) && (defined(VK_KHR_sampler_ycbcr_conversion) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_2_PLANE_444_FORMATS_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT);
#endif
#if defined(VK_EXT_provoking_vertex) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceProvokingVertexFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROVOKING_VERTEX_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceProvokingVertexPropertiesEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_PROVOKING_VERTEX_STATE_CREATE_INFO_EXT:
			return sizeof(VkPipelineRasterizationProvokingVertexStateCreateInfoEXT);
#endif
#if defined(VK_NVX_binary_import)
		case VK_STRUCTURE_TYPE_CU_MODULE_CREATE_INFO_NVX:
			return sizeof(VkCuModuleCreateInfoNVX);
		case VK_STRUCTURE_TYPE_CU_MODULE_TEXTURING_MODE_CREATE_INFO_NVX:
			return sizeof(VkCuModuleTexturingModeCreateInfoNVX);
		case VK_STRUCTURE_TYPE_CU_FUNCTION_CREATE_INFO_NVX:
			return sizeof(VkCuFunctionCreateInfoNVX);
		case VK_STRUCTURE_TYPE_CU_LAUNCH_INFO_NVX:
			return sizeof(VkCuLaunchInfoNVX);
#endif
#if defined(VK_EXT_descriptor_buffer) && (((((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_buffer_device_address) && defined(VK_EXT_descriptor_indexing)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceDescriptorBufferFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceDescriptorBufferPropertiesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_DENSITY_MAP_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceDescriptorBufferDensityMapPropertiesEXT);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_ADDRESS_INFO_EXT:
			return sizeof(VkDescriptorAddressInfoEXT);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT:
			return sizeof(VkDescriptorBufferBindingInfoEXT);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_PUSH_DESCRIPTOR_BUFFER_HANDLE_EXT:
			return sizeof(VkDescriptorBufferBindingPushDescriptorBufferHandleEXT);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_GET_INFO_EXT:
			return sizeof(VkDescriptorGetInfoEXT);
		case VK_STRUCTURE_TYPE_BUFFER_CAPTURE_DESCRIPTOR_DATA_INFO_EXT:
			return sizeof(VkBufferCaptureDescriptorDataInfoEXT);
		case VK_STRUCTURE_TYPE_IMAGE_CAPTURE_DESCRIPTOR_DATA_INFO_EXT:
			return sizeof(VkImageCaptureDescriptorDataInfoEXT);
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_CAPTURE_DESCRIPTOR_DATA_INFO_EXT:
			return sizeof(VkImageViewCaptureDescriptorDataInfoEXT);
		case VK_STRUCTURE_TYPE_SAMPLER_CAPTURE_DESCRIPTOR_DATA_INFO_EXT:
			return sizeof(VkSamplerCaptureDescriptorDataInfoEXT);
#endif
#if (defined(VK_EXT_descriptor_buffer) && (((((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_buffer_device_address) && defined(VK_EXT_descriptor_indexing)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CAPTURE_DESCRIPTOR_DATA_INFO_EXT:
			return sizeof(VkAccelerationStructureCaptureDescriptorDataInfoEXT);
#endif
#if defined(VK_EXT_descriptor_buffer) && (((((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_buffer_device_address) && defined(VK_EXT_descriptor_indexing)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_OPAQUE_CAPTURE_DESCRIPTOR_DATA_CREATE_INFO_EXT:
			return sizeof(VkOpaqueCaptureDescriptorDataCreateInfoEXT);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_FEATURES:
			return sizeof(VkPhysicalDeviceShaderIntegerDotProductFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_INTEGER_DOT_PRODUCT_PROPERTIES:
			return sizeof(VkPhysicalDeviceShaderIntegerDotProductProperties);
#endif
#if defined(VK_EXT_physical_device_drm) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRM_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceDrmPropertiesEXT);
#endif
#if defined(VK_KHR_fragment_shader_barycentric) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADER_BARYCENTRIC_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceFragmentShaderBarycentricPropertiesKHR);
#endif
#if defined(VK_NV_ray_tracing_motion_blur) && (defined(VK_KHR_ray_tracing_pipeline))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_MOTION_BLUR_FEATURES_NV:
			return sizeof(VkPhysicalDeviceRayTracingMotionBlurFeaturesNV);
#endif
#if defined(VK_NV_ray_tracing_validation)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_VALIDATION_FEATURES_NV:
			return sizeof(VkPhysicalDeviceRayTracingValidationFeaturesNV);
#endif
#if defined(VK_NV_ray_tracing_linear_swept_spheres) && (defined(VK_KHR_ray_tracing_pipeline))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_LINEAR_SWEPT_SPHERES_FEATURES_NV:
			return sizeof(VkPhysicalDeviceRayTracingLinearSweptSpheresFeaturesNV);
#endif
#if defined(VK_NV_ray_tracing_motion_blur) && (defined(VK_KHR_ray_tracing_pipeline))
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_MOTION_TRIANGLES_DATA_NV:
			return sizeof(VkAccelerationStructureGeometryMotionTrianglesDataNV);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MOTION_INFO_NV:
			return sizeof(VkAccelerationStructureMotionInfoNV);
#endif
#if defined(VK_NV_external_memory_rdma) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_MEMORY_GET_REMOTE_ADDRESS_INFO_NV:
			return sizeof(VkMemoryGetRemoteAddressInfoNV);
#endif
#if defined(VK_USE_PLATFORM_FUCHSIA) && (defined(VK_FUCHSIA_buffer_collection) && (defined(VK_FUCHSIA_external_memory) && (defined(VK_KHR_sampler_ycbcr_conversion) || defined(VK_VERSION_1_1))))
		case VK_STRUCTURE_TYPE_IMPORT_MEMORY_BUFFER_COLLECTION_FUCHSIA:
			return sizeof(VkImportMemoryBufferCollectionFUCHSIA);
		case VK_STRUCTURE_TYPE_BUFFER_COLLECTION_IMAGE_CREATE_INFO_FUCHSIA:
			return sizeof(VkBufferCollectionImageCreateInfoFUCHSIA);
		case VK_STRUCTURE_TYPE_BUFFER_COLLECTION_BUFFER_CREATE_INFO_FUCHSIA:
			return sizeof(VkBufferCollectionBufferCreateInfoFUCHSIA);
		case VK_STRUCTURE_TYPE_BUFFER_COLLECTION_CREATE_INFO_FUCHSIA:
			return sizeof(VkBufferCollectionCreateInfoFUCHSIA);
		case VK_STRUCTURE_TYPE_BUFFER_COLLECTION_PROPERTIES_FUCHSIA:
			return sizeof(VkBufferCollectionPropertiesFUCHSIA);
		case VK_STRUCTURE_TYPE_BUFFER_CONSTRAINTS_INFO_FUCHSIA:
			return sizeof(VkBufferConstraintsInfoFUCHSIA);
		case VK_STRUCTURE_TYPE_SYSMEM_COLOR_SPACE_FUCHSIA:
			return sizeof(VkSysmemColorSpaceFUCHSIA);
		case VK_STRUCTURE_TYPE_IMAGE_FORMAT_CONSTRAINTS_INFO_FUCHSIA:
			return sizeof(VkImageFormatConstraintsInfoFUCHSIA);
		case VK_STRUCTURE_TYPE_IMAGE_CONSTRAINTS_INFO_FUCHSIA:
			return sizeof(VkImageConstraintsInfoFUCHSIA);
		case VK_STRUCTURE_TYPE_BUFFER_COLLECTION_CONSTRAINTS_INFO_FUCHSIA:
			return sizeof(VkBufferCollectionConstraintsInfoFUCHSIA);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS) && (defined(VK_NV_cuda_kernel_launch))
		case VK_STRUCTURE_TYPE_CUDA_MODULE_CREATE_INFO_NV:
			return sizeof(VkCudaModuleCreateInfoNV);
		case VK_STRUCTURE_TYPE_CUDA_FUNCTION_CREATE_INFO_NV:
			return sizeof(VkCudaFunctionCreateInfoNV);
		case VK_STRUCTURE_TYPE_CUDA_LAUNCH_INFO_NV:
			return sizeof(VkCudaLaunchInfoNV);
#endif
#if defined(VK_EXT_rgba10x6_formats) && (defined(VK_KHR_sampler_ycbcr_conversion) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RGBA10X6_FORMATS_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_3:
			return sizeof(VkFormatProperties3);
#endif
#if (defined(VK_EXT_image_drm_format_modifier) && ((((defined(VK_KHR_bind_memory2) && defined(VK_KHR_get_physical_device_properties2) && defined(VK_KHR_sampler_ycbcr_conversion)) || defined(VK_VERSION_1_1)) && defined(VK_KHR_image_format_list)) || defined(VK_VERSION_1_2))) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_2_EXT:
			return sizeof(VkDrmFormatModifierPropertiesList2EXT);
#endif
#if (defined(VK_USE_PLATFORM_ANDROID_KHR) && (defined(VK_ANDROID_external_memory_android_hardware_buffer) && (((defined(VK_KHR_sampler_ycbcr_conversion) && defined(VK_KHR_external_memory) && defined(VK_KHR_dedicated_allocation)) || defined(VK_VERSION_1_1)) && defined(VK_EXT_queue_family_foreign)))) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_2_ANDROID:
			return sizeof(VkAndroidHardwareBufferFormatProperties2ANDROID);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO:
			return sizeof(VkPipelineRenderingCreateInfo);
		case VK_STRUCTURE_TYPE_RENDERING_INFO:
			return sizeof(VkRenderingInfo);
#endif
#if defined(VK_EXT_fragment_density_map_offset) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_EXT_fragment_density_map) && (defined(VK_KHR_create_renderpass2) || defined(VK_VERSION_1_2)) && (defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering)))
		case VK_STRUCTURE_TYPE_RENDERING_END_INFO_EXT:
			return sizeof(VkRenderingEndInfoEXT);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO:
			return sizeof(VkRenderingAttachmentInfo);
#endif
#if (defined(VK_KHR_fragment_shading_rate) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_create_renderpass2)) || defined(VK_VERSION_1_2))) && (defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering))
		case VK_STRUCTURE_TYPE_RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT_INFO_KHR:
			return sizeof(VkRenderingFragmentShadingRateAttachmentInfoKHR);
#endif
#if (defined(VK_EXT_fragment_density_map) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))) && (defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering))
		case VK_STRUCTURE_TYPE_RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT_INFO_EXT:
			return sizeof(VkRenderingFragmentDensityMapAttachmentInfoEXT);
#endif
#if defined(VK_VERSION_1_3)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES:
			return sizeof(VkPhysicalDeviceDynamicRenderingFeatures);
		case VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDERING_INFO:
			return sizeof(VkCommandBufferInheritanceRenderingInfo);
#endif
#if (defined(VK_AMD_mixed_attachment_samples)) && (defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering))
		case VK_STRUCTURE_TYPE_ATTACHMENT_SAMPLE_COUNT_INFO_AMD:
			return sizeof(VkAttachmentSampleCountInfoAMD);
#endif
#if (defined(VK_NVX_multiview_per_view_attributes) && (defined(VK_KHR_multiview) || defined(VK_VERSION_1_1))) && (defined(VK_VERSION_1_3) || defined(VK_KHR_dynamic_rendering))
		case VK_STRUCTURE_TYPE_MULTIVIEW_PER_VIEW_ATTRIBUTES_INFO_NVX:
			return sizeof(VkMultiviewPerViewAttributesInfoNVX);
#endif
#if defined(VK_EXT_image_view_min_lod) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_VIEW_MIN_LOD_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceImageViewMinLodFeaturesEXT);
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_MIN_LOD_CREATE_INFO_EXT:
			return sizeof(VkImageViewMinLodCreateInfoEXT);
#endif
#if defined(VK_EXT_rasterization_order_attachment_access) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT);
#endif
#if defined(VK_NV_linear_color_attachment) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LINEAR_COLOR_ATTACHMENT_FEATURES_NV:
			return sizeof(VkPhysicalDeviceLinearColorAttachmentFeaturesNV);
#endif
#if defined(VK_EXT_graphics_pipeline_library) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_pipeline_library))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT);
#endif
#if defined(VK_KHR_pipeline_binary) && (defined(VK_KHR_maintenance5) || defined(VK_VERSION_1_4))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_BINARY_FEATURES_KHR:
			return sizeof(VkPhysicalDevicePipelineBinaryFeaturesKHR);
		case VK_STRUCTURE_TYPE_DEVICE_PIPELINE_BINARY_INTERNAL_CACHE_CONTROL_KHR:
			return sizeof(VkDevicePipelineBinaryInternalCacheControlKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_BINARY_PROPERTIES_KHR:
			return sizeof(VkPhysicalDevicePipelineBinaryPropertiesKHR);
#endif
#if defined(VK_EXT_graphics_pipeline_library) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_pipeline_library))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_GRAPHICS_PIPELINE_LIBRARY_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceGraphicsPipelineLibraryPropertiesEXT);
		case VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT:
			return sizeof(VkGraphicsPipelineLibraryCreateInfoEXT);
#endif
#if defined(VK_VALVE_descriptor_set_host_mapping) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_SET_HOST_MAPPING_FEATURES_VALVE:
			return sizeof(VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_BINDING_REFERENCE_VALVE:
			return sizeof(VkDescriptorSetBindingReferenceVALVE);
		case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_HOST_MAPPING_INFO_VALVE:
			return sizeof(VkDescriptorSetLayoutHostMappingInfoVALVE);
#endif
#if defined(VK_EXT_nested_command_buffer) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_NESTED_COMMAND_BUFFER_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceNestedCommandBufferFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_NESTED_COMMAND_BUFFER_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceNestedCommandBufferPropertiesEXT);
#endif
#if defined(VK_EXT_shader_module_identifier) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_EXT_pipeline_creation_cache_control)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MODULE_IDENTIFIER_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MODULE_IDENTIFIER_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceShaderModuleIdentifierPropertiesEXT);
		case VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_MODULE_IDENTIFIER_CREATE_INFO_EXT:
			return sizeof(VkPipelineShaderStageModuleIdentifierCreateInfoEXT);
		case VK_STRUCTURE_TYPE_SHADER_MODULE_IDENTIFIER_EXT:
			return sizeof(VkShaderModuleIdentifierEXT);
#endif
#if defined(VK_EXT_image_compression_control) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_IMAGE_COMPRESSION_CONTROL_EXT:
			return sizeof(VkImageCompressionControlEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_COMPRESSION_CONTROL_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceImageCompressionControlFeaturesEXT);
		case VK_STRUCTURE_TYPE_IMAGE_COMPRESSION_PROPERTIES_EXT:
			return sizeof(VkImageCompressionPropertiesEXT);
#endif
#if defined(VK_EXT_image_compression_control_swapchain) && (defined(VK_EXT_image_compression_control))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_COMPRESSION_CONTROL_SWAPCHAIN_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_IMAGE_SUBRESOURCE_2:
			return sizeof(VkImageSubresource2);
		case VK_STRUCTURE_TYPE_SUBRESOURCE_LAYOUT_2:
			return sizeof(VkSubresourceLayout2);
#endif
#if defined(VK_EXT_subpass_merge_feedback) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_RENDER_PASS_CREATION_CONTROL_EXT:
			return sizeof(VkRenderPassCreationControlEXT);
		case VK_STRUCTURE_TYPE_RENDER_PASS_CREATION_FEEDBACK_CREATE_INFO_EXT:
			return sizeof(VkRenderPassCreationFeedbackCreateInfoEXT);
		case VK_STRUCTURE_TYPE_RENDER_PASS_SUBPASS_FEEDBACK_CREATE_INFO_EXT:
			return sizeof(VkRenderPassSubpassFeedbackCreateInfoEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBPASS_MERGE_FEEDBACK_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT);
#endif
#if defined(VK_EXT_opacity_micromap) && (defined(VK_KHR_acceleration_structure) && (defined(VK_KHR_synchronization2) || defined(VK_VERSION_1_3)))
		case VK_STRUCTURE_TYPE_MICROMAP_BUILD_INFO_EXT:
			return sizeof(VkMicromapBuildInfoEXT);
		case VK_STRUCTURE_TYPE_MICROMAP_CREATE_INFO_EXT:
			return sizeof(VkMicromapCreateInfoEXT);
		case VK_STRUCTURE_TYPE_MICROMAP_VERSION_INFO_EXT:
			return sizeof(VkMicromapVersionInfoEXT);
		case VK_STRUCTURE_TYPE_COPY_MICROMAP_INFO_EXT:
			return sizeof(VkCopyMicromapInfoEXT);
		case VK_STRUCTURE_TYPE_COPY_MICROMAP_TO_MEMORY_INFO_EXT:
			return sizeof(VkCopyMicromapToMemoryInfoEXT);
		case VK_STRUCTURE_TYPE_COPY_MEMORY_TO_MICROMAP_INFO_EXT:
			return sizeof(VkCopyMemoryToMicromapInfoEXT);
		case VK_STRUCTURE_TYPE_MICROMAP_BUILD_SIZES_INFO_EXT:
			return sizeof(VkMicromapBuildSizesInfoEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPACITY_MICROMAP_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceOpacityMicromapFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPACITY_MICROMAP_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceOpacityMicromapPropertiesEXT);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_TRIANGLES_OPACITY_MICROMAP_EXT:
			return sizeof(VkAccelerationStructureTrianglesOpacityMicromapEXT);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS) && (defined(VK_NV_displacement_micromap) && (defined(VK_EXT_opacity_micromap)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISPLACEMENT_MICROMAP_FEATURES_NV:
			return sizeof(VkPhysicalDeviceDisplacementMicromapFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISPLACEMENT_MICROMAP_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceDisplacementMicromapPropertiesNV);
		case VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_TRIANGLES_DISPLACEMENT_MICROMAP_NV:
			return sizeof(VkAccelerationStructureTrianglesDisplacementMicromapNV);
#endif
#if defined(VK_EXT_pipeline_properties) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PIPELINE_PROPERTIES_IDENTIFIER_EXT:
			return sizeof(VkPipelinePropertiesIdentifierEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_PROPERTIES_FEATURES_EXT:
			return sizeof(VkPhysicalDevicePipelinePropertiesFeaturesEXT);
#endif
#if defined(VK_AMD_shader_early_and_late_fragment_tests) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_EARLY_AND_LATE_FRAGMENT_TESTS_FEATURES_AMD:
			return sizeof(VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD);
#endif
#if defined(VK_EXT_external_memory_acquire_unmodified) && (defined(VK_KHR_external_memory) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_ACQUIRE_UNMODIFIED_EXT:
			return sizeof(VkExternalMemoryAcquireUnmodifiedEXT);
#endif
#if defined(VK_USE_PLATFORM_METAL_EXT) && (defined(VK_EXT_metal_objects))
		case VK_STRUCTURE_TYPE_EXPORT_METAL_OBJECT_CREATE_INFO_EXT:
			return sizeof(VkExportMetalObjectCreateInfoEXT);
		case VK_STRUCTURE_TYPE_EXPORT_METAL_OBJECTS_INFO_EXT:
			return sizeof(VkExportMetalObjectsInfoEXT);
		case VK_STRUCTURE_TYPE_EXPORT_METAL_DEVICE_INFO_EXT:
			return sizeof(VkExportMetalDeviceInfoEXT);
		case VK_STRUCTURE_TYPE_EXPORT_METAL_COMMAND_QUEUE_INFO_EXT:
			return sizeof(VkExportMetalCommandQueueInfoEXT);
		case VK_STRUCTURE_TYPE_EXPORT_METAL_BUFFER_INFO_EXT:
			return sizeof(VkExportMetalBufferInfoEXT);
		case VK_STRUCTURE_TYPE_IMPORT_METAL_BUFFER_INFO_EXT:
			return sizeof(VkImportMetalBufferInfoEXT);
		case VK_STRUCTURE_TYPE_EXPORT_METAL_TEXTURE_INFO_EXT:
			return sizeof(VkExportMetalTextureInfoEXT);
		case VK_STRUCTURE_TYPE_IMPORT_METAL_TEXTURE_INFO_EXT:
			return sizeof(VkImportMetalTextureInfoEXT);
		case VK_STRUCTURE_TYPE_EXPORT_METAL_IO_SURFACE_INFO_EXT:
			return sizeof(VkExportMetalIOSurfaceInfoEXT);
		case VK_STRUCTURE_TYPE_IMPORT_METAL_IO_SURFACE_INFO_EXT:
			return sizeof(VkImportMetalIOSurfaceInfoEXT);
		case VK_STRUCTURE_TYPE_EXPORT_METAL_SHARED_EVENT_INFO_EXT:
			return sizeof(VkExportMetalSharedEventInfoEXT);
		case VK_STRUCTURE_TYPE_IMPORT_METAL_SHARED_EVENT_INFO_EXT:
			return sizeof(VkImportMetalSharedEventInfoEXT);
#endif
#if defined(VK_EXT_non_seamless_cube_map) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_NON_SEAMLESS_CUBE_MAP_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_ROBUSTNESS_FEATURES:
			return sizeof(VkPhysicalDevicePipelineRobustnessFeatures);
		case VK_STRUCTURE_TYPE_PIPELINE_ROBUSTNESS_CREATE_INFO:
			return sizeof(VkPipelineRobustnessCreateInfo);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_ROBUSTNESS_PROPERTIES:
			return sizeof(VkPhysicalDevicePipelineRobustnessProperties);
#endif
#if defined(VK_QCOM_image_processing) && (defined(VK_KHR_format_feature_flags2) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_IMAGE_VIEW_SAMPLE_WEIGHT_CREATE_INFO_QCOM:
			return sizeof(VkImageViewSampleWeightCreateInfoQCOM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceImageProcessingFeaturesQCOM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_PROPERTIES_QCOM:
			return sizeof(VkPhysicalDeviceImageProcessingPropertiesQCOM);
#endif
#if defined(VK_QCOM_tile_properties) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TILE_PROPERTIES_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceTilePropertiesFeaturesQCOM);
		case VK_STRUCTURE_TYPE_TILE_PROPERTIES_QCOM:
			return sizeof(VkTilePropertiesQCOM);
#endif
#if defined(VK_QCOM_tile_memory_heap) && ((defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_get_physical_device_properties2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_TILE_MEMORY_BIND_INFO_QCOM:
			return sizeof(VkTileMemoryBindInfoQCOM);
#endif
#if defined(VK_SEC_amigo_profiling) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_AMIGO_PROFILING_FEATURES_SEC:
			return sizeof(VkPhysicalDeviceAmigoProfilingFeaturesSEC);
		case VK_STRUCTURE_TYPE_AMIGO_PROFILING_SUBMIT_INFO_SEC:
			return sizeof(VkAmigoProfilingSubmitInfoSEC);
#endif
#if defined(VK_EXT_attachment_feedback_loop_layout) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ATTACHMENT_FEEDBACK_LOOP_LAYOUT_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT);
#endif
#if defined(VK_EXT_device_address_binding_report) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_EXT_debug_utils))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ADDRESS_BINDING_REPORT_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceAddressBindingReportFeaturesEXT);
		case VK_STRUCTURE_TYPE_DEVICE_ADDRESS_BINDING_CALLBACK_DATA_EXT:
			return sizeof(VkDeviceAddressBindingCallbackDataEXT);
#endif
#if defined(VK_NV_optical_flow) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_format_feature_flags2) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPTICAL_FLOW_FEATURES_NV:
			return sizeof(VkPhysicalDeviceOpticalFlowFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_OPTICAL_FLOW_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceOpticalFlowPropertiesNV);
		case VK_STRUCTURE_TYPE_OPTICAL_FLOW_IMAGE_FORMAT_INFO_NV:
			return sizeof(VkOpticalFlowImageFormatInfoNV);
		case VK_STRUCTURE_TYPE_OPTICAL_FLOW_IMAGE_FORMAT_PROPERTIES_NV:
			return sizeof(VkOpticalFlowImageFormatPropertiesNV);
		case VK_STRUCTURE_TYPE_OPTICAL_FLOW_SESSION_CREATE_INFO_NV:
			return sizeof(VkOpticalFlowSessionCreateInfoNV);
		case VK_STRUCTURE_TYPE_OPTICAL_FLOW_SESSION_CREATE_PRIVATE_DATA_INFO_NV:
			return sizeof(VkOpticalFlowSessionCreatePrivateDataInfoNV);
		case VK_STRUCTURE_TYPE_OPTICAL_FLOW_EXECUTE_INFO_NV:
			return sizeof(VkOpticalFlowExecuteInfoNV);
#endif
#if defined(VK_EXT_device_fault) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FAULT_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceFaultFeaturesEXT);
		case VK_STRUCTURE_TYPE_DEVICE_FAULT_COUNTS_EXT:
			return sizeof(VkDeviceFaultCountsEXT);
		case VK_STRUCTURE_TYPE_DEVICE_FAULT_INFO_EXT:
			return sizeof(VkDeviceFaultInfoEXT);
#endif
#if defined(VK_EXT_pipeline_library_group_handles) && (defined(VK_KHR_ray_tracing_pipeline) && defined(VK_KHR_pipeline_library))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_LIBRARY_GROUP_HANDLES_FEATURES_EXT:
			return sizeof(VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT);
#endif
#if defined(VK_EXT_depth_bias_control) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_DEPTH_BIAS_INFO_EXT:
			return sizeof(VkDepthBiasInfoEXT);
		case VK_STRUCTURE_TYPE_DEPTH_BIAS_REPRESENTATION_INFO_EXT:
			return sizeof(VkDepthBiasRepresentationInfoEXT);
#endif
#if defined(VK_ARM_shader_core_builtins) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_BUILTINS_PROPERTIES_ARM:
			return sizeof(VkPhysicalDeviceShaderCoreBuiltinsPropertiesARM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_BUILTINS_FEATURES_ARM:
			return sizeof(VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM);
#endif
#if defined(VK_EXT_frame_boundary)
		case VK_STRUCTURE_TYPE_FRAME_BOUNDARY_EXT:
			return sizeof(VkFrameBoundaryEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAME_BOUNDARY_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceFrameBoundaryFeaturesEXT);
#endif
#if defined(VK_EXT_dynamic_rendering_unused_attachments) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_dynamic_rendering)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_UNUSED_ATTACHMENTS_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT);
#endif
#if defined(VK_EXT_surface_maintenance1) && (defined(VK_KHR_surface) && defined(VK_KHR_get_surface_capabilities2))
		case VK_STRUCTURE_TYPE_SURFACE_PRESENT_MODE_EXT:
			return sizeof(VkSurfacePresentModeEXT);
		case VK_STRUCTURE_TYPE_SURFACE_PRESENT_SCALING_CAPABILITIES_EXT:
			return sizeof(VkSurfacePresentScalingCapabilitiesEXT);
		case VK_STRUCTURE_TYPE_SURFACE_PRESENT_MODE_COMPATIBILITY_EXT:
			return sizeof(VkSurfacePresentModeCompatibilityEXT);
#endif
#if defined(VK_EXT_swapchain_maintenance1) && (defined(VK_KHR_swapchain) && defined(VK_EXT_surface_maintenance1) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SWAPCHAIN_MAINTENANCE_1_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_FENCE_INFO_EXT:
			return sizeof(VkSwapchainPresentFenceInfoEXT);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_MODES_CREATE_INFO_EXT:
			return sizeof(VkSwapchainPresentModesCreateInfoEXT);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_MODE_INFO_EXT:
			return sizeof(VkSwapchainPresentModeInfoEXT);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_SCALING_CREATE_INFO_EXT:
			return sizeof(VkSwapchainPresentScalingCreateInfoEXT);
		case VK_STRUCTURE_TYPE_RELEASE_SWAPCHAIN_IMAGES_INFO_EXT:
			return sizeof(VkReleaseSwapchainImagesInfoEXT);
#endif
#if defined(VK_EXT_depth_bias_control) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_BIAS_CONTROL_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceDepthBiasControlFeaturesEXT);
#endif
#if defined(VK_NV_ray_tracing_invocation_reorder) && (defined(VK_KHR_ray_tracing_pipeline))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_INVOCATION_REORDER_FEATURES_NV:
			return sizeof(VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_INVOCATION_REORDER_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceRayTracingInvocationReorderPropertiesNV);
#endif
#if defined(VK_NV_extended_sparse_address_space)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_SPARSE_ADDRESS_SPACE_FEATURES_NV:
			return sizeof(VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_SPARSE_ADDRESS_SPACE_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceExtendedSparseAddressSpacePropertiesNV);
#endif
#if defined(VK_LUNARG_direct_driver_loading)
		case VK_STRUCTURE_TYPE_DIRECT_DRIVER_LOADING_INFO_LUNARG:
			return sizeof(VkDirectDriverLoadingInfoLUNARG);
		case VK_STRUCTURE_TYPE_DIRECT_DRIVER_LOADING_LIST_LUNARG:
			return sizeof(VkDirectDriverLoadingListLUNARG);
#endif
#if defined(VK_QCOM_multiview_per_view_viewports) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_VIEWPORTS_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM);
#endif
#if defined(VK_KHR_ray_tracing_position_fetch) && (defined(VK_KHR_acceleration_structure))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_POSITION_FETCH_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_DEVICE_IMAGE_SUBRESOURCE_INFO:
			return sizeof(VkDeviceImageSubresourceInfo);
#endif
#if defined(VK_ARM_shader_core_properties) && (defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_ARM:
			return sizeof(VkPhysicalDeviceShaderCorePropertiesARM);
#endif
#if defined(VK_QCOM_multiview_per_view_render_areas)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PER_VIEW_RENDER_AREAS_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM);
		case VK_STRUCTURE_TYPE_MULTIVIEW_PER_VIEW_RENDER_AREAS_RENDER_PASS_BEGIN_INFO_QCOM:
			return sizeof(VkMultiviewPerViewRenderAreasRenderPassBeginInfoQCOM);
#endif
#if defined(VK_NV_low_latency)
		case VK_STRUCTURE_TYPE_QUERY_LOW_LATENCY_SUPPORT_NV:
			return sizeof(VkQueryLowLatencySupportNV);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_MEMORY_MAP_INFO:
			return sizeof(VkMemoryMapInfo);
		case VK_STRUCTURE_TYPE_MEMORY_UNMAP_INFO:
			return sizeof(VkMemoryUnmapInfo);
#endif
#if defined(VK_EXT_shader_object) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_dynamic_rendering)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceShaderObjectFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceShaderObjectPropertiesEXT);
		case VK_STRUCTURE_TYPE_SHADER_CREATE_INFO_EXT:
			return sizeof(VkShaderCreateInfoEXT);
#endif
#if defined(VK_EXT_shader_tile_image) && (defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TILE_IMAGE_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceShaderTileImageFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_TILE_IMAGE_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceShaderTileImagePropertiesEXT);
#endif
#if defined(VK_USE_PLATFORM_SCREEN_QNX) && (defined(VK_QNX_external_memory_screen_buffer) && (((defined(VK_KHR_sampler_ycbcr_conversion) && defined(VK_KHR_external_memory) && defined(VK_KHR_dedicated_allocation)) || defined(VK_VERSION_1_1)) && defined(VK_EXT_queue_family_foreign)))
		case VK_STRUCTURE_TYPE_IMPORT_SCREEN_BUFFER_INFO_QNX:
			return sizeof(VkImportScreenBufferInfoQNX);
		case VK_STRUCTURE_TYPE_SCREEN_BUFFER_PROPERTIES_QNX:
			return sizeof(VkScreenBufferPropertiesQNX);
		case VK_STRUCTURE_TYPE_SCREEN_BUFFER_FORMAT_PROPERTIES_QNX:
			return sizeof(VkScreenBufferFormatPropertiesQNX);
		case VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_QNX:
			return sizeof(VkExternalFormatQNX);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_MEMORY_SCREEN_BUFFER_FEATURES_QNX:
			return sizeof(VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX);
#endif
#if defined(VK_KHR_cooperative_matrix) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceCooperativeMatrixFeaturesKHR);
		case VK_STRUCTURE_TYPE_COOPERATIVE_MATRIX_PROPERTIES_KHR:
			return sizeof(VkCooperativeMatrixPropertiesKHR);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_KHR:
			return sizeof(VkPhysicalDeviceCooperativeMatrixPropertiesKHR);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS) && (defined(VK_AMDX_shader_enqueue) && (((defined(VK_KHR_synchronization2) && defined(VK_KHR_spirv_1_4) && defined(VK_EXT_extended_dynamic_state)) || defined(VK_VERSION_1_3)) && defined(VK_KHR_maintenance5) && defined(VK_KHR_pipeline_library)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ENQUEUE_PROPERTIES_AMDX:
			return sizeof(VkPhysicalDeviceShaderEnqueuePropertiesAMDX);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ENQUEUE_FEATURES_AMDX:
			return sizeof(VkPhysicalDeviceShaderEnqueueFeaturesAMDX);
		case VK_STRUCTURE_TYPE_EXECUTION_GRAPH_PIPELINE_CREATE_INFO_AMDX:
			return sizeof(VkExecutionGraphPipelineCreateInfoAMDX);
		case VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_NODE_CREATE_INFO_AMDX:
			return sizeof(VkPipelineShaderStageNodeCreateInfoAMDX);
		case VK_STRUCTURE_TYPE_EXECUTION_GRAPH_PIPELINE_SCRATCH_SIZE_AMDX:
			return sizeof(VkExecutionGraphPipelineScratchSizeAMDX);
#endif
#if defined(VK_AMD_anti_lag)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ANTI_LAG_FEATURES_AMD:
			return sizeof(VkPhysicalDeviceAntiLagFeaturesAMD);
		case VK_STRUCTURE_TYPE_ANTI_LAG_DATA_AMD:
			return sizeof(VkAntiLagDataAMD);
		case VK_STRUCTURE_TYPE_ANTI_LAG_PRESENTATION_INFO_AMD:
			return sizeof(VkAntiLagPresentationInfoAMD);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_BIND_MEMORY_STATUS:
			return sizeof(VkBindMemoryStatus);
#endif
#if defined(VK_QCOM_tile_memory_heap) && ((defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_get_physical_device_properties2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TILE_MEMORY_HEAP_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceTileMemoryHeapFeaturesQCOM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TILE_MEMORY_HEAP_PROPERTIES_QCOM:
			return sizeof(VkPhysicalDeviceTileMemoryHeapPropertiesQCOM);
#endif
#if (defined(VK_QCOM_tile_memory_heap) && ((defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_get_physical_device_properties2)) || defined(VK_VERSION_1_1))) && (defined(VK_QCOM_tile_properties))
		case VK_STRUCTURE_TYPE_TILE_MEMORY_SIZE_INFO_QCOM:
			return sizeof(VkTileMemorySizeInfoQCOM);
#endif
#if defined(VK_QCOM_tile_memory_heap) && ((defined(VK_KHR_get_memory_requirements2) && defined(VK_KHR_get_physical_device_properties2)) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_TILE_MEMORY_REQUIREMENTS_QCOM:
			return sizeof(VkTileMemoryRequirementsQCOM);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_BIND_DESCRIPTOR_SETS_INFO:
			return sizeof(VkBindDescriptorSetsInfo);
		case VK_STRUCTURE_TYPE_PUSH_CONSTANTS_INFO:
			return sizeof(VkPushConstantsInfo);
		case VK_STRUCTURE_TYPE_PUSH_DESCRIPTOR_SET_INFO:
			return sizeof(VkPushDescriptorSetInfo);
		case VK_STRUCTURE_TYPE_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_INFO:
			return sizeof(VkPushDescriptorSetWithTemplateInfo);
#endif
#if (defined(VK_KHR_maintenance6) && (defined(VK_VERSION_1_1))) && (defined(VK_EXT_descriptor_buffer))
		case VK_STRUCTURE_TYPE_SET_DESCRIPTOR_BUFFER_OFFSETS_INFO_EXT:
			return sizeof(VkSetDescriptorBufferOffsetsInfoEXT);
		case VK_STRUCTURE_TYPE_BIND_DESCRIPTOR_BUFFER_EMBEDDED_SAMPLERS_INFO_EXT:
			return sizeof(VkBindDescriptorBufferEmbeddedSamplersInfoEXT);
#endif
#if defined(VK_QCOM_filter_cubic_clamp) && ((defined(VK_EXT_filter_cubic)) && (defined(VK_VERSION_1_2) || defined(VK_EXT_sampler_filter_minmax)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUBIC_CLAMP_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceCubicClampFeaturesQCOM);
#endif
#if defined(VK_QCOM_ycbcr_degamma)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_YCBCR_DEGAMMA_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceYcbcrDegammaFeaturesQCOM);
		case VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_YCBCR_DEGAMMA_CREATE_INFO_QCOM:
			return sizeof(VkSamplerYcbcrConversionYcbcrDegammaCreateInfoQCOM);
#endif
#if defined(VK_QCOM_filter_cubic_weights) && (defined(VK_EXT_filter_cubic))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUBIC_WEIGHTS_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceCubicWeightsFeaturesQCOM);
		case VK_STRUCTURE_TYPE_SAMPLER_CUBIC_WEIGHTS_CREATE_INFO_QCOM:
			return sizeof(VkSamplerCubicWeightsCreateInfoQCOM);
		case VK_STRUCTURE_TYPE_BLIT_IMAGE_CUBIC_WEIGHTS_INFO_QCOM:
			return sizeof(VkBlitImageCubicWeightsInfoQCOM);
#endif
#if defined(VK_QCOM_image_processing2) && (defined(VK_QCOM_image_processing))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_2_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceImageProcessing2FeaturesQCOM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_PROCESSING_2_PROPERTIES_QCOM:
			return sizeof(VkPhysicalDeviceImageProcessing2PropertiesQCOM);
		case VK_STRUCTURE_TYPE_SAMPLER_BLOCK_MATCH_WINDOW_CREATE_INFO_QCOM:
			return sizeof(VkSamplerBlockMatchWindowCreateInfoQCOM);
#endif
#if defined(VK_NV_descriptor_pool_overallocation) && (defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_POOL_OVERALLOCATION_FEATURES_NV:
			return sizeof(VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV);
#endif
#if defined(VK_MSFT_layered_driver) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_LAYERED_DRIVER_PROPERTIES_MSFT:
			return sizeof(VkPhysicalDeviceLayeredDriverPropertiesMSFT);
#endif
#if defined(VK_NV_per_stage_descriptor_set) && (defined(VK_KHR_maintenance6) || defined(VK_VERSION_1_4))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PER_STAGE_DESCRIPTOR_SET_FEATURES_NV:
			return sizeof(VkPhysicalDevicePerStageDescriptorSetFeaturesNV);
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR) && (defined(VK_ANDROID_external_format_resolve) && (defined(VK_ANDROID_external_memory_android_hardware_buffer)))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FORMAT_RESOLVE_FEATURES_ANDROID:
			return sizeof(VkPhysicalDeviceExternalFormatResolveFeaturesANDROID);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_FORMAT_RESOLVE_PROPERTIES_ANDROID:
			return sizeof(VkPhysicalDeviceExternalFormatResolvePropertiesANDROID);
		case VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_RESOLVE_PROPERTIES_ANDROID:
			return sizeof(VkAndroidHardwareBufferFormatResolvePropertiesANDROID);
#endif
#if defined(VK_NV_low_latency2) && (defined(VK_VERSION_1_2) || defined(VK_KHR_timeline_semaphore))
		case VK_STRUCTURE_TYPE_LATENCY_SLEEP_MODE_INFO_NV:
			return sizeof(VkLatencySleepModeInfoNV);
		case VK_STRUCTURE_TYPE_LATENCY_SLEEP_INFO_NV:
			return sizeof(VkLatencySleepInfoNV);
		case VK_STRUCTURE_TYPE_SET_LATENCY_MARKER_INFO_NV:
			return sizeof(VkSetLatencyMarkerInfoNV);
		case VK_STRUCTURE_TYPE_GET_LATENCY_MARKER_INFO_NV:
			return sizeof(VkGetLatencyMarkerInfoNV);
		case VK_STRUCTURE_TYPE_LATENCY_TIMINGS_FRAME_REPORT_NV:
			return sizeof(VkLatencyTimingsFrameReportNV);
		case VK_STRUCTURE_TYPE_OUT_OF_BAND_QUEUE_TYPE_INFO_NV:
			return sizeof(VkOutOfBandQueueTypeInfoNV);
		case VK_STRUCTURE_TYPE_LATENCY_SUBMISSION_PRESENT_ID_NV:
			return sizeof(VkLatencySubmissionPresentIdNV);
		case VK_STRUCTURE_TYPE_SWAPCHAIN_LATENCY_CREATE_INFO_NV:
			return sizeof(VkSwapchainLatencyCreateInfoNV);
		case VK_STRUCTURE_TYPE_LATENCY_SURFACE_CAPABILITIES_NV:
			return sizeof(VkLatencySurfaceCapabilitiesNV);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS) && (defined(VK_NV_cuda_kernel_launch))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUDA_KERNEL_LAUNCH_FEATURES_NV:
			return sizeof(VkPhysicalDeviceCudaKernelLaunchFeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUDA_KERNEL_LAUNCH_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceCudaKernelLaunchPropertiesNV);
#endif
#if defined(VK_ARM_scheduling_controls) && (defined(VK_ARM_shader_core_builtins))
		case VK_STRUCTURE_TYPE_DEVICE_QUEUE_SHADER_CORE_CONTROL_CREATE_INFO_ARM:
			return sizeof(VkDeviceQueueShaderCoreControlCreateInfoARM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCHEDULING_CONTROLS_FEATURES_ARM:
			return sizeof(VkPhysicalDeviceSchedulingControlsFeaturesARM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCHEDULING_CONTROLS_PROPERTIES_ARM:
			return sizeof(VkPhysicalDeviceSchedulingControlsPropertiesARM);
#endif
#if defined(VK_IMG_relaxed_line_rasterization) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RELAXED_LINE_RASTERIZATION_FEATURES_IMG:
			return sizeof(VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG);
#endif
#if defined(VK_ARM_render_pass_striped) && (((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_synchronization2)) || defined(VK_VERSION_1_3))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RENDER_PASS_STRIPED_FEATURES_ARM:
			return sizeof(VkPhysicalDeviceRenderPassStripedFeaturesARM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RENDER_PASS_STRIPED_PROPERTIES_ARM:
			return sizeof(VkPhysicalDeviceRenderPassStripedPropertiesARM);
		case VK_STRUCTURE_TYPE_RENDER_PASS_STRIPE_INFO_ARM:
			return sizeof(VkRenderPassStripeInfoARM);
		case VK_STRUCTURE_TYPE_RENDER_PASS_STRIPE_BEGIN_INFO_ARM:
			return sizeof(VkRenderPassStripeBeginInfoARM);
		case VK_STRUCTURE_TYPE_RENDER_PASS_STRIPE_SUBMIT_INFO_ARM:
			return sizeof(VkRenderPassStripeSubmitInfoARM);
#endif
#if defined(VK_ARM_pipeline_opacity_micromap) && (defined(VK_EXT_opacity_micromap))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_OPACITY_MICROMAP_FEATURES_ARM:
			return sizeof(VkPhysicalDevicePipelineOpacityMicromapFeaturesARM);
#endif
#if defined(VK_KHR_shader_maximal_reconvergence) && (defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MAXIMAL_RECONVERGENCE_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceShaderMaximalReconvergenceFeaturesKHR);
#endif
#if defined(VK_VERSION_1_4)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_SUBGROUP_ROTATE_FEATURES:
			return sizeof(VkPhysicalDeviceShaderSubgroupRotateFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_EXPECT_ASSUME_FEATURES:
			return sizeof(VkPhysicalDeviceShaderExpectAssumeFeatures);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT_CONTROLS_2_FEATURES:
			return sizeof(VkPhysicalDeviceShaderFloatControls2Features);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES:
			return sizeof(VkPhysicalDeviceDynamicRenderingLocalReadFeatures);
		case VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_LOCATION_INFO:
			return sizeof(VkRenderingAttachmentLocationInfo);
		case VK_STRUCTURE_TYPE_RENDERING_INPUT_ATTACHMENT_INDEX_INFO:
			return sizeof(VkRenderingInputAttachmentIndexInfo);
#endif
#if defined(VK_KHR_shader_quad_control) && (((defined(VK_VERSION_1_1) && defined(VK_KHR_vulkan_memory_model)) || defined(VK_VERSION_1_2)) && defined(VK_KHR_shader_maximal_reconvergence))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_QUAD_CONTROL_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceShaderQuadControlFeaturesKHR);
#endif
#if defined(VK_NV_shader_atomic_float16_vector)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_FLOAT16_VECTOR_FEATURES_NV:
			return sizeof(VkPhysicalDeviceShaderAtomicFloat16VectorFeaturesNV);
#endif
#if defined(VK_EXT_map_memory_placed) && (defined(VK_KHR_map_memory2) || defined(VK_VERSION_1_4))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceMapMemoryPlacedFeaturesEXT);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_PROPERTIES_EXT:
			return sizeof(VkPhysicalDeviceMapMemoryPlacedPropertiesEXT);
		case VK_STRUCTURE_TYPE_MEMORY_MAP_PLACED_INFO_EXT:
			return sizeof(VkMemoryMapPlacedInfoEXT);
#endif
#if defined(VK_KHR_shader_bfloat16) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_BFLOAT16_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceShaderBfloat16FeaturesKHR);
#endif
#if defined(VK_NV_raw_access_chains)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAW_ACCESS_CHAINS_FEATURES_NV:
			return sizeof(VkPhysicalDeviceRawAccessChainsFeaturesNV);
#endif
#if defined(VK_NV_command_buffer_inheritance)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COMMAND_BUFFER_INHERITANCE_FEATURES_NV:
			return sizeof(VkPhysicalDeviceCommandBufferInheritanceFeaturesNV);
#endif
#if defined(VK_MESA_image_alignment_control) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ALIGNMENT_CONTROL_FEATURES_MESA:
			return sizeof(VkPhysicalDeviceImageAlignmentControlFeaturesMESA);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_ALIGNMENT_CONTROL_PROPERTIES_MESA:
			return sizeof(VkPhysicalDeviceImageAlignmentControlPropertiesMESA);
		case VK_STRUCTURE_TYPE_IMAGE_ALIGNMENT_CONTROL_CREATE_INFO_MESA:
			return sizeof(VkImageAlignmentControlCreateInfoMESA);
#endif
#if defined(VK_EXT_shader_replicated_composites)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_REPLICATED_COMPOSITES_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceShaderReplicatedCompositesFeaturesEXT);
#endif
#if defined(VK_EXT_present_mode_fifo_latest_ready) && (defined(VK_KHR_swapchain))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_MODE_FIFO_LATEST_READY_FEATURES_EXT:
			return sizeof(VkPhysicalDevicePresentModeFifoLatestReadyFeaturesEXT);
#endif
#if defined(VK_NV_cooperative_matrix2) && (defined(VK_KHR_cooperative_matrix))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_2_FEATURES_NV:
			return sizeof(VkPhysicalDeviceCooperativeMatrix2FeaturesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_2_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceCooperativeMatrix2PropertiesNV);
		case VK_STRUCTURE_TYPE_COOPERATIVE_MATRIX_FLEXIBLE_DIMENSIONS_PROPERTIES_NV:
			return sizeof(VkCooperativeMatrixFlexibleDimensionsPropertiesNV);
#endif
#if defined(VK_HUAWEI_hdr_vivid) && ((defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1)) && defined(VK_KHR_swapchain) && defined(VK_EXT_hdr_metadata))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HDR_VIVID_FEATURES_HUAWEI:
			return sizeof(VkPhysicalDeviceHdrVividFeaturesHUAWEI);
#endif
#if defined(VK_EXT_vertex_attribute_robustness) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_ROBUSTNESS_FEATURES_EXT:
			return sizeof(VkPhysicalDeviceVertexAttributeRobustnessFeaturesEXT);
#endif
#if defined(VK_KHR_depth_clamp_zero_one) && (defined(VK_KHR_get_physical_device_properties2) || defined(VK_VERSION_1_1))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLAMP_ZERO_ONE_FEATURES_KHR:
			return sizeof(VkPhysicalDeviceDepthClampZeroOneFeaturesKHR);
#endif
#if defined(VK_NV_cooperative_vector)
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_VECTOR_FEATURES_NV:
			return sizeof(VkPhysicalDeviceCooperativeVectorFeaturesNV);
		case VK_STRUCTURE_TYPE_COOPERATIVE_VECTOR_PROPERTIES_NV:
			return sizeof(VkCooperativeVectorPropertiesNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_VECTOR_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceCooperativeVectorPropertiesNV);
		case VK_STRUCTURE_TYPE_CONVERT_COOPERATIVE_VECTOR_MATRIX_INFO_NV:
			return sizeof(VkConvertCooperativeVectorMatrixInfoNV);
#endif
#if defined(VK_QCOM_tile_shading) && (defined(VK_QCOM_tile_properties) || defined(VK_KHR_get_physical_device_properties2))
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TILE_SHADING_FEATURES_QCOM:
			return sizeof(VkPhysicalDeviceTileShadingFeaturesQCOM);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TILE_SHADING_PROPERTIES_QCOM:
			return sizeof(VkPhysicalDeviceTileShadingPropertiesQCOM);
		case VK_STRUCTURE_TYPE_RENDER_PASS_TILE_SHADING_CREATE_INFO_QCOM:
			return sizeof(VkRenderPassTileShadingCreateInfoQCOM);
		case VK_STRUCTURE_TYPE_PER_TILE_BEGIN_INFO_QCOM:
			return sizeof(VkPerTileBeginInfoQCOM);
		case VK_STRUCTURE_TYPE_PER_TILE_END_INFO_QCOM:
			return sizeof(VkPerTileEndInfoQCOM);
		case VK_STRUCTURE_TYPE_DISPATCH_TILE_INFO_QCOM:
			return sizeof(VkDispatchTileInfoQCOM);
#endif
#if defined(VK_ENABLE_BETA_EXTENSIONS) && (defined(VK_NV_present_metering))
		case VK_STRUCTURE_TYPE_SET_PRESENT_CONFIG_NV:
			return sizeof(VkSetPresentConfigNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PRESENT_METERING_FEATURES_NV:
			return sizeof(VkPhysicalDevicePresentMeteringFeaturesNV);
#endif
#if defined(VK_NV_external_compute_queue)
		case VK_STRUCTURE_TYPE_EXTERNAL_COMPUTE_QUEUE_DEVICE_CREATE_INFO_NV:
			return sizeof(VkExternalComputeQueueDeviceCreateInfoNV);
		case VK_STRUCTURE_TYPE_EXTERNAL_COMPUTE_QUEUE_CREATE_INFO_NV:
			return sizeof(VkExternalComputeQueueCreateInfoNV);
		case VK_STRUCTURE_TYPE_EXTERNAL_COMPUTE_QUEUE_DATA_PARAMS_NV:
			return sizeof(VkExternalComputeQueueDataParamsNV);
		case VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_COMPUTE_QUEUE_PROPERTIES_NV:
			return sizeof(VkPhysicalDeviceExternalComputeQueuePropertiesNV);
#endif
		default:
			return 0;
		}
	}
}
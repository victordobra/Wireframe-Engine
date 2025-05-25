#include "VulkanRenderer.hpp"
#include "Info/EngineInfo.hpp"
#include "Core/Memory/Allocator.hpp"

namespace wfe {
	// Callback functions
	void* AllocationCallback(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) {
		// Allocate the memory
		return AllocMemory(size, alignment);
	}
	void* ReallocationCallback(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) {
		// Reallocate the memory
		return ReallocMemory(pOriginal, size, alignment);
	}
	void FreeCallback(void* pUserData, void* pMemory) {
		// Free the memory
		FreeMemory(pMemory);
	}

	// Constants
	const VkAllocationCallbacks VulkanRenderer::ALLOCATION_CALLBACKS {
		.pUserData = nullptr,
		.pfnAllocation = AllocationCallback,
		.pfnReallocation = ReallocationCallback,
		.pfnFree = FreeCallback,
		.pfnInternalAllocation = nullptr,
		.pfnInternalFree = nullptr
	};

	// Public functions
	VulkanRenderer::VulkanRenderer(Program* program) : program(program) {
		// Create the loader
		loader = new VulkanLoader();

		// Set the application info
		VkApplicationInfo appInfo {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = program->GetProgramInfo().programName,
			.applicationVersion = VK_MAKE_API_VERSION(0, program->GetProgramInfo().programVersionMajor, program->GetProgramInfo().programVersionMinor, program->GetProgramInfo().programVersionPatch),
			.pEngineName = WFE_ENGINE_NAME,
			.engineVersion = VK_MAKE_API_VERSION(0, WFE_ENGINE_VERSION_MAJOR, WFE_ENGINE_VERSION_MINOR, WFE_ENGINE_VERSION_PATCH),
			.apiVersion = VulkanInstance::DEFAULT_REQUIRED_INSTANCE_API_VERSION
		};

		// Create the instance
		instance = new VulkanInstance(loader, appInfo, program->GetLogger());

		// Create the surface
		surface = new VulkanSurface(instance, program->GetWindow());

		// Create the device
		device = new VulkanDevice(instance, surface);

		// Create the swap chain
		swapChain = new VulkanSwapChain(device, surface);
	}

	VulkanRenderer::~VulkanRenderer() {
		// Destroy all renderer components
		delete swapChain;
		delete device;
		delete surface;
		delete instance;
		delete loader;
	}
}
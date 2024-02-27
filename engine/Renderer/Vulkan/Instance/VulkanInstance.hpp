#pragma once

#include "Renderer/Vulkan/Loader/VulkanLoader.hpp"

#include <Core.hpp>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace wfe {
	/// @brief A wrapper for the Vulkan instance and a debug messenger, if requested.
	class VulkanInstance {
	public:
		/// @brief Gets the default Vulkan application info used when creating the instance.
		/// @return A struct containing the default Vulkan application info.
		static const VkApplicationInfo& GetDefaultApplicationInfo();
		/// @brief Gets the extensions required by the Vulkan instance implementation by default.
		/// @return A set containing the names of all Vulkan required extensions.
		static const set<const char_t*>& GetDefaultRequiredInstanceExtensions();
		/// @brief Gets the extensions optionally required by the Vulkan instance implementation by default.
		/// @return A set containing the names of all Vulkan optional extensions.
		static const set<const char_t*>& GetDefaultOptionalInstanceExtensions();
		/// @brief Gets the debug extensions required by the Vulkan instance implementation if debug is enabled.
		/// @return A set containing the names of all Vulkan required debug extensions.
		static const set<const char_t*>& GetDefaultRequiredInstanceDebugExtensions();
		/// @brief Gets the debug extensions used by the Vulkan instance implementation if supported and if debug is enabled.
		/// @return A set containing the names of all Vulkan optional debug extensions.
		static const set<const char_t*>& GetDefaultOptionalInstanceDebugExtensions();
		/// @brief Gets the validation layers required by the Vulkan instance implementation if debug is enabled.
		/// @return A set containing the names of all Vulkan required validation layers.
		static const set<const char_t*>& GetDefaultRequiredInstanceLayers();
		/// @brief Gets the validation layers used by the Vulkan instance implementation if supported and if debug is enabled.
		/// @return A set containing the names of all Vulkan optional validation layers.
		static const set<const char_t*>& GetDefaultOptionalInstanceLayers();

		/// @brief Checks if the given instance extensions are supported.
		/// @param loader The Vulkan loader whose function pointers to use, or nullptr if the function will use the static Vulkan functions.
		/// @param extensionNames A set containing the names of the instance extensions to check.
		/// @param supported An optional pointer to an array of booleans, in which every index will be set to true if the corresponding extension is supported, otherwise to false. 
		/// @return The number of supported instance extensions from the given set.
		static size_t CheckForExtensionSupport(const VulkanLoader* loader, const set<const char_t*>& extensionNames, bool8_t* supported = nullptr);
		/// @brief Checks if the given validation layers are supported.
		/// @param loader The Vulkan loader whose function pointers to use, or nullptr if the function will use the static Vulkan functions.
		/// @param layerNames A set containing the names of the validation layers to check.
		/// @param supported An optional pointer to an array of booleans, in which every index will be set to true if the corresponding layer is supported, otherwise to false.
		/// @return The number of supported validation layers from the given set.
		static size_t CheckForLayerSupport(const VulkanLoader* loader, const set<const char_t*>& layerNames, bool8_t* supported = nullptr);

		/// @brief Creates a Vulkan instance with the default specifications.
		/// @param loader The Vulkan loader whose function pointers to use, or nullptr if the instance will use the static Vulkan functions.
		/// @param debugEnabled True if debugging should be enabled if supported, otherwise false.
		/// @param logger The logger to use for debugging, if debugging is enabled.
		VulkanInstance(const VulkanLoader* loader, bool8_t debugEnabled, Logger* logger);
		/// @brief Creates a Vulkan instance with the given specifications.
		/// @param loader The Vulkan loader whose function pointers to use, or nullptr if the instance will use the static Vulkan functions.
		/// @param debugEnabled True if debugging should be enabled if supported, otherwise false.
		/// @param logger The logger to use for debugging, if debugging is enabled.
		/// @param appInfo The Vulkan application info to use when creating the instance.
		/// @param requiredExtensions The Vulkan extensions required for the creation of the Vulkan instance.
		/// @param optionalExtensions The Vulkan extensions that can be used, if supported, by the Vulkan instance.
		/// @param requiredDebugExtensions The Vulkan extensions required for debugging for the Vulkan instance.
		/// @param optionalDebugExtensions The Vulkan extensions that can be used, if supported, for debugging for the Vulkan instance.
		/// @param requiredLayers The Vulkan validation layers required for debugging for the Vulkan instance.
		/// @param optionalLayers The Vulkan validation layers that can be used, if supported, for debugging for the Vulkan instance.
		VulkanInstance(const VulkanLoader* loader, bool8_t debugEnabled, Logger* logger, const VkApplicationInfo& appInfo, const set<const char_t*>& requiredExtensions, const set<const char_t*>& optionalExtensions, const set<const char_t*>& requiredDebugExtensions, const set<const char_t*>& optionalDebugExtensions, const set<const char_t*>& requiredLayers, const set<const char_t*>& optionalLayers);
		VulkanInstance(const VulkanInstance&) = delete;
		VulkanInstance(VulkanInstance&&) noexcept = delete;

		VulkanInstance& operator=(const VulkanInstance&) = delete;
		VulkanInstance& operator=(VulkanInstance&&) = delete;

		/// @brief Gets the Vulkan function loader used by the instance.
		/// @return A pointer to the Vulkan loader used by the instance.
		const VulkanLoader* GetLoader() const;

		/// @brief Gets the Vulkan instance of the implementation.
		/// @return A handle to the Vulkan instance.
		VkInstance GetInstance();
		/// @brief Gets the Vulkan debug messenger for the instance.
		/// @return A handle to the Vulkan debug messenger, or VK_NULL_HANDLE if debugging is disabled.
		VkDebugUtilsMessengerEXT GetDebugMessenger();

		/// @brief Gets the Vulkan instance's enabled extensions.
		/// @return A set containing the names of all enabled extensions.
		const set<const char_t*>& GetInstanceExtensions() const;
		/// @brief Gets the Vulkan instance's enabled validation layers.
		/// @return A set containing the names of all enabled layers.
		const set<const char_t*>& GetInstanceLayers() const;
		/// @brief Gets the Vulkan instance's version.
		/// @return The 32-bit unsigned int representation to the version.
		uint32_t GetInstanceVersion() const;

		/// @brief Checks if debugging is enabled for the current instance.
		/// @return True if debugging is enabled, otherwise false.
		bool8_t IsDebuggingEnabled() const;

		/// @brief Destroys the Vulkan instance and its debug messenger, if it exists.
		~VulkanInstance();
	private:
		bool8_t CheckInstanceSupport(bool8_t& debugEnabled, const set<const char_t*>& requiredExtensions, const set<const char_t*>& optionalExtensions, const set<const char_t*>& requiredDebugExtensions, const set<const char_t*>& optionalDebugExtensions, const set<const char_t*>& requiredLayers, const set<const char_t*>& optionalLayers);
		void CreateInstance(bool8_t debugEnabled, Logger* logger, const VkApplicationInfo& appInfo);

		const VulkanLoader* loader;

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;

		set<const char_t*> extensions;
		set<const char_t*> layers;
		uint32_t version;
	};
}
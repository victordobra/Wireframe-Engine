#pragma once

#include "Assets/General/TextFile.hpp"

#include "Core/Assets/Asset.hpp"
#include "Core/Assets/AssetManager.hpp"
#include "Core/Assets/AssetType.hpp"
#include "Core/Debug/Logger.hpp"
#include "Core/Management/Event.hpp"
#include "Core/Management/TypeName.hpp"
#include "Core/Parsers/WFEONParser.hpp"
#include "Core/Types/BuildInfo.hpp"
#include "Core/Types/Defines.hpp"

#include "Info/EngineInfo.hpp"

#include "Main/Program.hpp"

#include "Platform/Input.hpp"
#include "Platform/Window.hpp"

#include "Vulkan/Instance/VulkanInstance.hpp"
#include "Vulkan/Instance/VulkanSurface.hpp"
#include "Vulkan/Loader/VulkanLoader.hpp"
#include "Vulkan/VulkanRenderer.hpp"
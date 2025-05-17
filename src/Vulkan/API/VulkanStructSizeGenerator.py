import re
import urllib.request
import xml.etree.ElementTree as ET

VK_XML_PATH = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

class Type:
	def __init__(self, name, values, requirements):
		self.name = name
		self.values = values
		self.requirements = requirements

platforms = { }
types = { }

def FormatCommandRequirements(require):
	return re.sub(r"[a-zA-Z0-9_]+", lambda req: "defined({0})".format(req.group(0)), require).replace(",", " || ").replace("+", " && ")

def ParseXML():
	# Open the XML tree
	tree = ET.parse(urllib.request.urlopen(VK_XML_PATH))
	root = tree.getroot()

	# Loop through all platforms
	for platform in root.findall("platforms/platform"):
		# Add the platform to the map
		platforms[platform.get("name")] = platform.get("protect")

	# Loop through all types
	for type in root.findall("types/type"):
		# Skip the current type if not from the Vulkan API
		if type.get("api") is not None and type.get("api") != "vulkan":
			continue

		# Skip the current type if not a struct
		if (type.get("category") is None) or type.get("category") != "struct":
			continue

		# Get the type's name
		typeName = type.get("name")
		if not typeName:
			continue
			
		# Find the type's sType
		for member in type.findall("member"):
			# If it has the "value" attribute set, it must be the sType member
			valuesStr = member.get("values")
			if valuesStr is not None:
				# Create the new type
				values = valuesStr.split(",")
				type = Type(typeName, values, "")

				# Add the type to all maps
				types[typeName] = type
	
	# Loop through all Vulkan feature roots
	for featureRoot in root.findall("feature"):
		# Remove all types if the current feature is from the Vulkan API
		api = featureRoot.get("api")
		if "vulkan" not in api.split(","):
			for type in featureRoot.findall("require/type"):
				# Check if the type was saved
				if type.get("name") in types:
					# Erase the type
					types.pop(type.get("name"))

			continue
		
		# Set the version requirements for every command
		for type in featureRoot.findall("require/type"):
			if type.get("name") in types:
				# Format the requirement as a C++ defined macro
				types[type.get("name")].requirements = "defined({0})".format(featureRoot.get("name"))
	
	# Loop through all Vulkan extensions
	for extension in root.findall("extensions/extension"):
		# Get the current extension's dependencies
		extDependencies = extension.get("depends")
		if extDependencies is not None:
			extDependencies = "defined({0}) && ({1})".format(extension.get("name"), FormatCommandRequirements(extDependencies))
		else:
			extDependencies = "defined({0})".format(extension.get("name"))
		
		# Add the platform's dependency, if valid
		platform = extension.get("platform")
		if platform is not None:
			extDependencies = "defined({0}) && ({1})".format(platforms[platform], extDependencies)
		
		# Loop through the extension's different requirements
		for requirements in extension.findall("require"):
			# Get the current requirement's dependencies
			dependencies = requirements.get("depends")
			if dependencies is not None:
				dependencies = "({0}) && ({1})".format(extDependencies, FormatCommandRequirements(dependencies))
			else:
				dependencies = extDependencies
			
			# Set the new dependencies for every type
			for type in requirements.findall("type"):
				# Get the current command's name
				typeName = type.get("name")
				if typeName not in types:
					continue

				if types[typeName].requirements != "":
					types[typeName].requirements = "({0}) || ({1})".format(types[typeName].requirements, dependencies)
				else:
					types[typeName].requirements = dependencies

def GenerateHeader():
	# Open the header file
	headerFile = open("VulkanStructSize.hpp", "w")

	# Write the header's start
	headerFile.write("""#pragma once

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
""")

	# Write all sType cases
	prevType = None
	for type in types.values():
		if prevType is None:
			# Write the first type's requirements
			headerFile.write("#if {0}\n".format(type.requirements))
		elif prevType.requirements != type.requirements:
			# Write the current command's requirements
			headerFile.write("#endif\n#if {0}\n".format(type.requirements))
		
		# Set the new previous type
		prevType = type
		
		# Write the current type's cases
		for value in type.values:
			headerFile.write("\t\tcase {0}:\n\t\t\treturn sizeof({1});\n".format(value, type.name))
	
	# End the header file
	headerFile.write("""#endif
		default:
			return 0;
		}
	}
}""")

	# Close the header file
	headerFile.close()
		

if __name__ == "__main__":
	ParseXML()
	GenerateHeader()
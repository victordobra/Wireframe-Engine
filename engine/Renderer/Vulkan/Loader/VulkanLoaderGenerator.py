import re
import urllib.request
import xml.etree.ElementTree as ET

VK_XML_PATH = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

class Param:
	def __init__(self, name, type, fullName):
		self.name = name
		self.type = type
		self.fullName = fullName

class Command:
	def __init__(self, name, returnType, params):
		self.name = name
		self.returnType = returnType
		self.params = params
		self.requirements = ""
		self.coreCommand = False
		self.deviceCommand = False

class Type:
	def __init__(self, name, parents):
		self.name = name
		self.parents = parents

commands = {}
types = {}
tags = []

def FormatCommandRequirements(require):
	return re.sub(r"[a-zA-Z0-9_]+", lambda req: "defined({0})".format(req.group(0)), require).replace(",", " || ").replace("+", " && ")

def IsTypeDescendant(name, base):
	# Check if the base was already reached
	if name == base:
		return True
	
	# Check if the current type does not exist
	if not types.get(name):
		return False

	# Check if the current types does not have a parent
	if not types[name].parents:
		return False
	
	# Loop through the type's parents and check if one of them is the descendant
	for parent in types[name].parents.split(","):
		if IsTypeDescendant(parent, base):
			return True
	return False


def ParseXML():
	# Open the XML tree
	tree = ET.parse(urllib.request.urlopen(VK_XML_PATH))
	root = tree.getroot()

	# Loop through all Vulkan types
	for type in root.findall("types/type"):
		if (not type.get("api")) or type.get("api") == "vulkan":
			if type.get("name"):
				types[type.get("name")] = Type(type.get("name"), type.get("parents"))
			elif type.find("name"):
				types[type.find("name").text] = Type(type.find("name").text, type.get("parents"))
	
	# Add all Vulkan tags to an array
	for tag in root.findall("tags/tag"):
		tags.append(tag.get("name"))
	
	# Loop through all commands
	for command in root.findall("commands/command"):
		# Skip the current command variant if it is not from the Vulkan API
		if command.get("api") and command.get("api") != "vulkan":
			continue

		# Check if the current command is an alias
		if command.get("alias"):
			# Get the command's name and its alias' name
			commandName = command.get("name")
			commandAlias = command.get("alias")

			# Get the original command
			originalCommand = commands[commandAlias]

			# Get the original command's tag
			originalTag = ""
			for tag in tags:
				if commandAlias.endswith(tag):
					originalTag = tag
					break
			
			# Get the current command's tag
			commandTag = ""
			for tag in tags:
				if commandName.endswith(tag):
					commandTag = tag
					break
			
			if originalTag == "" and commandTag != "":
				# Set the current command's parameters as default
				command = Command(commandName, originalCommand.returnType, originalCommand.params.copy())

				# Try to add the tag to the command's return type
				if types.get(command.returnType + commandTag):
					command.returnType += commandTag
				
				# Try to add the tag to every parameter of the command
				for i in range(0, len(command.params)):
					if types.get(command.params[i].type + commandTag):
						# Add the tag to the type and the full name
						newType = command.params[i].type + commandTag
						command.params[i].fullName = command.params[i].fullName.replace(command.params[i].type, newType, 1)
						command.params[i].type = newType

				# Add the command to the dictionary
				commands[commandName] = command
			elif commandTag != originalTag:
				# Set the current command's parameters as default
				command = Command(commandName, originalCommand.returnType, originalCommand.params.copy())

				# Replace the tag in the command's return type
				command.returnType = command.returnType.replace(originalTag, commandTag, 1)

				# Replace the tag in every parameter of the command
				for i in range(0, len(command.params)):
					command.params[i].type = command.params[i].type.replace(originalTag, commandTag, 1)
					command.params[i].fullName = command.params[i].fullName.replace(originalTag, commandTag, 1)

				# Add the command to the dictionary
				commands[commandName] = command
			else:
				# Save the current command with its parameters
				commands[commandName] = Command(commandName, originalCommand.returnType, originalCommand.params.copy())

			continue

		# Get the command's name and return value
		commandName = command.find("proto").find("name").text
		commandReturnType = command.find("proto").find("type").text

		# Get the current command's parameters
		commandParams = []
		for commandParam in command.findall("param"):
			# Skip the param if it has to valid structure or if it is not from the Vulkan API
			if commandParam.find("type") == None or commandParam.find("name") == None or (commandParam.get("api") and commandParam.get("api") != "vulkan"):
				continue

			# Add the current parameter to the array
			paramName = commandParam.find("name").text
			paramType = commandParam.find("type").text

			paramFullName = ET.tostring(commandParam, encoding = "unicode", method = "text")
			paramFullName = re.sub(r"\s+", " ", paramFullName)
			paramFullName = paramFullName[:paramFullName.find("\n")]

			commandParams.append(Param(paramName, paramType, paramFullName))
		
		# Add the current command to the dictionary
		commands[commandName] = Command(commandName, commandReturnType, commandParams)
	
	# Loop through all Vulkan feature roots
	for featureRoot in root.findall("feature"):
		# Remove all commands if the current feature is from the Vulkan API
		api = featureRoot.get("api")
		if "vulkan" not in api.split(","):
			for command in featureRoot.findall("require/command"):
				# Erase the current command from the array
				commands.pop(command.get("name"))
			continue
		
		# Set the version requirements for every command
		for command in featureRoot.findall("require/command"):
			# Format the requirement as a C++ defined macro
			commands[command.get("name")].requirements = "defined({0})".format(featureRoot.get("name"))
	
	# Loop through all Vulkan extensions
	for extension in root.findall("extensions/extension"):
		# Get the current extension's dependencies
		extDependencies = extension.get("depends")
		if extDependencies:
			extDependencies = "defined({0}) && ({1})".format(extension.get("name"), FormatCommandRequirements(extDependencies))
		else:
			extDependencies = "defined({0})".format(extension.get("name"))
		
		# Loop through the extension's different requirements
		for requirements in extension.findall("require"):
			# Get the current requirement's dependencies
			dependencies = requirements.get("depends")
			if dependencies:
				dependencies = "({0}) && ({1})".format(extDependencies, FormatCommandRequirements(dependencies))
			else:
				dependencies = extDependencies
			
			# Set the new dependencies for every command
			for command in requirements.findall("command"):
				# Get the current command's name
				commandName = command.get("name")

				if commands[commandName].requirements != "":
					commands[commandName].requirements = "({0}) || ({1})".format(commands[commandName].requirements, dependencies)
				else:
					commands[commandName].requirements = dependencies
	
	# Categorize all device, instance and core commands
	for commandName in commands:
		commands[commandName].coreCommand = commands[commandName].requirements == "defined(VK_VERSION_1_0)"
		commands[commandName].deviceCommand = IsTypeDescendant(commands[commandName].params[0].type, "VkDevice")

def GenerateHeader():
	# Open the header file
	headerFile = open("VulkanLoader.hpp", "w")

	# Write the header's start
	headerFile.write("""#pragma once

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

""")

	# Write all command function declarations
	prevCommand = None
	for command in commands.values():
		if prevCommand == None:
			# Write the first command's requirements
			headerFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			headerFile.write("#endif\n")
			headerFile.write("#if {0}\n".format(command.requirements))
		
		# Set the new previous command
		prevCommand = command

		# Write the current command's declaration
		headerFile.write("\t\tVKAPI_ATTR {0} VKAPI_CALL {1}(".format(command.returnType, command.name))

		# Write every parameter
		for i in range(0, len(command.params)):
			# Write the parameter
			headerFile.write(command.params[i].fullName)
			if i != len(command.params) - 1:
				headerFile.write(", ")

		# End the function declaration
		headerFile.write(") const;\n")
	headerFile.write("#endif\n")
	
	# Write the variable declaration start
	headerFile.write("\tprivate:\n\t\t// Vulkan function pointers\n")

	# Write all command function pointer variables
	prevCommand = None
	for command in commands.values():
		if prevCommand == None:
			# Write the first command's requirements
			headerFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			headerFile.write("#endif\n")
			headerFile.write("#if {0}\n".format(command.requirements))

		# Set the new previous command
		prevCommand = command

		# Write the current command's variable
		headerFile.write("\t\tPFN_{0} pfn_{0};\n".format(command.name))
	headerFile.write("#endif\n")

	# End the header file
	headerFile.write("\t};\n}\n")

	# Close the header file
	headerFile.close()

def GenerateSource():
	# Open the source file
	sourceFile = open("VulkanLoader.cpp", "w")

	# Begin the source file
	sourceFile.write("""#include \"VulkanLoader.hpp\"

namespace wfe {
	// Internal variables
	static atomic_size_t loaderCount = 0;
	static DynamicLib vulkanLib;

	// Static function pointers
""")

	# Write the static function pointers
	prevCommand = None
	for command in commands.values():
		if prevCommand == None:
			# Write the first command's requirements
			sourceFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			sourceFile.write("#endif\n")
			sourceFile.write("#if {0}\n".format(command.requirements))

		# Set the new previous command
		prevCommand = command

		# Write the current command's variable
		sourceFile.write("\tstatic PFN_{0} staticPfn_{0};\n".format(command.name))
	sourceFile.write("#endif\n")

	# Write the static function pointer loader
	sourceFile.write("""
	// Internal helper functions
	static void LoadStaticFunctionPointers() {
#if defined(WFE_PLATFORM_WINDOWS)
		if(!vulkanLib.LoadLib("vulkan-1.dll"))
			throw Renderer::UnsupportedAPIException(\"Failed to load Vulkan dynamic lib!\");
#elif defined(WFE_PLATFORM_LINUX)
		if(!vulkanLib.LoadLib("libvulkan.so"))
			throw Renderer::UnsupportedAPIException(\"Failed to load Vulkan dynamic lib!\");
#endif

""")

	prevCommand = None
	for command in commands.values():
		if prevCommand == None:
			# Write the first command's requirements
			sourceFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			sourceFile.write("#endif\n")
			sourceFile.write("#if {0}\n".format(command.requirements))

		# Set the new previous command
		prevCommand = command

		# Write the current command's load
		sourceFile.write("\t\tstaticPfn_{0} = (PFN_{0})vulkanLib.LoadFunc(\"{0}\");\n".format(command.name))
	sourceFile.write("#endif\n\t}\n")

	# Write the loader's load functions
	sourceFile.write("""
	// Public functions
	VulkanLoader::VulkanLoader() {
		// Load the static function pointers if no loaders were previously created
		if(!loaderCount++)
			LoadStaticFunctionPointers();
		
		// Set the function pointers
""")

	prevCommand = None
	for command in commands.values():
		if prevCommand == None:
			# Write the first command's requirements
			sourceFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			sourceFile.write("#endif\n")
			sourceFile.write("#if {0}\n".format(command.requirements))

		# Set the new previous command
		prevCommand = command

		# Write the current command's load
		sourceFile.write("\t\tpfn_{0} = staticPfn_{0};\n".format(command.name))
	sourceFile.write("#endif\n\t}\n")

	sourceFile.write("""
	void VulkanLoader::LoadInstanceFunctions(VkInstance instance) {
""")

	prevCommand = None
	for command in commands.values():
		# Skip the current command if it is a device command
		if command.deviceCommand:
			continue

		if prevCommand == None:
			# Write the first command's requirements
			sourceFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			sourceFile.write("#endif\n")
			sourceFile.write("#if {0}\n".format(command.requirements))

		# Set the new previous command
		prevCommand = command

		# Write the current command's load
		sourceFile.write("\t\tpfn_{0} = (PFN_{0})vkGetInstanceProcAddr(instance, \"{0}\");\n".format(command.name))
	sourceFile.write("#endif\n\t}")

	sourceFile.write("""
	void VulkanLoader::LoadDeviceFunctions(VkDevice device) {
""")

	prevCommand = None
	for command in commands.values():
		# Skip the current command if it is not a device command
		if not command.deviceCommand:
			continue

		if prevCommand == None:
			# Write the first command's requirements
			sourceFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			sourceFile.write("#endif\n")
			sourceFile.write("#if {0}\n".format(command.requirements))

		# Set the new previous command
		prevCommand = command

		# Write the current command's load
		sourceFile.write("\t\tpfn_{0} = (PFN_{0})vkGetDeviceProcAddr(device, \"{0}\");\n".format(command.name))
	sourceFile.write("#endif\n\t}\n")

	# Write the loader's destructor
	sourceFile.write("""
	VulkanLoader::~VulkanLoader() {
		// Free the dynamic lib if no loaders are left
		if(!--loaderCount)
			vulkanLib.FreeLib();
	}

""")

	# Write all loader command definitions
	prevCommand = None
	for command in commands.values():
		if prevCommand == None:
			# Write the first command's requirements
			sourceFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			sourceFile.write("#endif\n")
			sourceFile.write("#if {0}\n".format(command.requirements))
		
		# Set the new previous command
		prevCommand = command

		# Write the current command's declaration
		sourceFile.write("\tVKAPI_ATTR {0} VKAPI_CALL VulkanLoader::{1}(".format(command.returnType, command.name))

		# Write every parameter
		for i in range(0, len(command.params)):
			# Write the parameter
			sourceFile.write(command.params[i].fullName)
			if i != len(command.params) - 1:
				sourceFile.write(", ")

		# End the function declaration
		sourceFile.write(") const {\n\t\t")

		# Write the static function pointer call
		sourceFile.write("if(!this || !pfn_{0})".format(command.name) + " {\n\t\t\t")
		if command.returnType != "void":
			sourceFile.write("return ")
		sourceFile.write("staticPfn_{0}(".format(command.name))

		# Write every parameter
		for i in range(0, len(command.params)):
			# Write the parameter
			sourceFile.write(command.params[i].name)
			if i != len(command.params) - 1:
				sourceFile.write(", ")
		sourceFile.write(");\n\t\t")

		# End the static function pointer call if
		if command.returnType == "void":
			sourceFile.write("\treturn;\n\t\t")
		sourceFile.write("}\n\t\t")

		# Write the function pointer call
		if command.returnType != "void":
			sourceFile.write("return ")
		sourceFile.write("pfn_{0}(".format(command.name))

		# Write every parameter
		for i in range(0, len(command.params)):
			# Write the parameter
			sourceFile.write(command.params[i].name)
			if i != len(command.params) - 1:
				sourceFile.write(", ")
		sourceFile.write(");\n\t}\n")
	sourceFile.write("#endif\n")

	# End the source file's namespace
	sourceFile.write("}\n\n")

	# Write the static function definitions
	prevCommand = None
	for command in commands.values():
		if prevCommand == None:
			# Write the first command's requirements
			sourceFile.write("#if {0}\n".format(command.requirements))
		elif command.requirements != prevCommand.requirements:
			# Write the current command's requirements
			sourceFile.write("#endif\n")
			sourceFile.write("#if {0}\n".format(command.requirements))
		
		# Set the new previous command
		prevCommand = command

		# Write the current command's declaration
		sourceFile.write("VKAPI_ATTR {0} VKAPI_CALL {1}(".format(command.returnType, command.name))

		# Write every parameter
		for i in range(0, len(command.params)):
			# Write the parameter
			sourceFile.write(command.params[i].fullName)
			if i != len(command.params) - 1:
				sourceFile.write(", ")

		# End the function declaration
		sourceFile.write(") {\n\t")

		# Write the function pointer call
		if command.returnType != "void":
			sourceFile.write("return ")
		sourceFile.write("wfe::staticPfn_{0}(".format(command.name))

		# Write every parameter
		for i in range(0, len(command.params)):
			# Write the parameter
			sourceFile.write(command.params[i].name)
			if i != len(command.params) - 1:
				sourceFile.write(", ")
		sourceFile.write(");\n}\n")
	sourceFile.write("#endif\n")

	# Close the source file
	sourceFile.close()

if __name__ == "__main__":
	ParseXML()
	GenerateHeader()
	GenerateSource()
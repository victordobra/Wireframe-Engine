import re
import urllib.request
import xml.etree.ElementTree as ET

VK_XML_PATH = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

class Parameter:
	def __init__(self, type, name, fullName):
		self.type = type
		self.name = name
		self.fullName = fullName

class Command:
	def __init__(self, name, returnType, params):
		self.name = name
		self.returnType = returnType
		self.params = params
		self.requirements = ""
		self.deviceCommand = False
	def GetDeclarationText(self, namePref):
		# Write the command's return type and name
		declaration = "VKAPI_ATTR {0} VKAPI_CALL {1}{2}(".format(self.returnType, namePref, self.name)

		# Write the command's parameters
		for param in self.params:
			declaration += param.fullName + ", "
		declaration = declaration[:-2] + ")"

		return declaration
	def GetCallText(self, namePref):
		# Write the command's name and a return statement, if required
		call = ("return " if self.returnType != "void" else "") + namePref + self.name + "("

		# Write the command's paramaters
		for param in self.params:
			call += param.name + ", "
		call = call[:-2] + ");"

		return call

def CheckAPIExclusive(api):
	# Check is the command/type/tag is part of the core API and not extensions like vulkansc
	return api is None or api == "vulkan" or api == "vulkan,vulkanbase"
def CheckAPIInclusive(api):
	return "vulkan" in api.split(",")
def NameReplaceTag(name, oldTag, newTag):
	return name[:len(name) - len(oldTag)] + newTag

def FormatRequirements(require):
	if require is not None:
		return re.sub(r"[a-zA-Z0-9_]+", lambda req: "defined({0})".format(req.group(0)), require).replace(",", " || ").replace("+", " && ")
	else:
		return ""
def MergeRequirements(require1, require2, operator):
	if require2 != "":
		return "({0}) {1} ({2})".format(require1, operator, require2)
	else:
		return require1

def WriteRequirementIfs(file, prevCommand, command):
	if prevCommand is None:
		# Write the first command's requirements
		file.write("#if {0}\n".format(command.requirements))
	elif prevCommand.requirements != command.requirements:
		# Write the current command's requirements
		file.write("#endif\n#if {0}\n".format(command.requirements))

class CommandSet:
	def __init__(self):
		self.commands = {}
		self.typeParents = {}
		self.tags = []

	def GetNameTag(self, name):
		# Loop through all tags and check if they are a prefix
		for tag in self.tags:
			if name.endswith(tag):
				return tag
			
		# No tag is present; return an empty string
		return ""
	def CommandReplaceTag(self, command, newName, oldTag, newTag):
		newCommand = Command(newName, command.returnType, command.params.copy())

		if oldTag == newTag:
			return newCommand

		# Replace the tag in the command return type
		if newCommand.returnType.endswith(oldTag):
			newType = NameReplaceTag(newCommand.returnType, oldTag, newTag)
			newCommand.returnType = newType if newType in self.typeParents else newCommand.returnType
		
		# Replace the tag in all parameter types
		for param in newCommand.params:
			if not param.type.endswith(oldTag):
				continue

			newType = NameReplaceTag(param.type, oldTag, newTag)
			if newType in self.typeParents:
				param.fullName = param.fullName.replace(param.type, newType, 1)
				param.type = newType

		return newCommand
	def IsTypeDescendant(self, type, root):
		if type == root:
			return True

		for parent in self.typeParents[type]:
			if self.IsTypeDescendant(parent, root):
				return True
		return False
	
	def ParseTypes(self, root):
		# Loop through all types
		for type in root.findall("types/type"):
			# Skip the type if it is invalid
			if not CheckAPIExclusive(type.get("api")) or (type.get("name") is None and type.find("name") is None):
				continue

			# Get the type's name and parents
			name = type.get("name") if type.get("name") is not None else type.find("name").text
			parents = type.get("parents").split(",") if type.get("parents") is not None else []

			# Add the type to the dictionary
			self.typeParents[name] = parents
	def ParseTags(self, root):
		# Add all tags to the array
		for tag in root.findall("tags/tag"):
			self.tags.append(tag.get("name"))
	def ParseCommands(self, root):
		# Loop through all commands
		for command in root.findall("commands/command"):
			if not CheckAPIExclusive(command.get("api")):
				continue

			# Check if the current command is an alias
			if command.get("alias") is not None:
				# Get the command's name and its alias
				commandName = command.get("name")
				commandAlias = command.get("alias")

				# Get the current and alias tags
				nameTag = self.GetNameTag(commandName)
				aliasTag = self.GetNameTag(commandAlias)

				# Get the original command object and replace all tags
				oldCommand = self.commands[commandAlias]
				self.commands[commandName] = self.CommandReplaceTag(oldCommand, commandName, aliasTag, nameTag)
			else:
				# Get the command's name and return type
				commandName = command.find("proto").find("name").text
				commandReturnType = command.find("proto").find("type").text

				# Get the current command's parameters
				commandParams = []
				for commandParam in command.findall("param"):
					# Skip the parameter if it has an invalid structure or API
					if commandParam.find("type") is None or commandParam.find("name") is None or not CheckAPIExclusive(commandParam.get("api")):
						continue

					# Get the parameter's data
					paramType = commandParam.find("type").text
					paramName = commandParam.find("name").text

					# Format the parameter's full name
					paramFullName = ET.tostring(commandParam, encoding="unicode", method="text")
					paramFullName = re.sub(r"\s+", " ", paramFullName)
					paramFullName = paramFullName[:paramFullName.find("\n")]

					# Add the parameter to the array
					commandParams.append(Parameter(paramType, paramName, paramFullName))

				# Add the command to the dictionary
				self.commands[commandName] = Command(commandName, commandReturnType, commandParams)
	def ParseFeatures(self, root):
		# Loop through all feature roots
		for featureRoot in root.findall("feature"):
			# Get the feature name
			featureName = featureRoot.get("name")
			featureName = re.sub(r"VK_(BASE|COMPUTE|GRAPHICS)_VERSION_", "VK_VERSION_", featureName)

			if CheckAPIInclusive(featureRoot.get("api")):
				# Set all commands to require the feature
				for command in featureRoot.findall("require/command"):
					commandName = command.get("name")
					self.commands[commandName].requirements = "defined({0})".format(featureName)
			else:
				# Remove all associated commands
				for command in featureRoot.findall("require/command"):
					self.commands.pop(command.get("name"))
	def ParseExtensions(self, root):
		# Loop through all Vulkan extensions
		for extension in root.findall("extensions/extension"):
			# Get the extension's dependencies
			extDependencies = MergeRequirements("defined({0})".format(extension.get("name")), FormatRequirements(extension.get("depends")), "&&")

			# Loop through the extension's different requirements
			for requirements in extension.findall("require"):
				# Get the current requirement's dependencies
				dependencies = MergeRequirements(extDependencies, FormatRequirements(requirements.get("depends")), "&&")

				# Set the new dependencies for every command
				for command in requirements.findall("command"):
					commandName = command.get("name")
					self.commands[commandName].requirements = MergeRequirements(dependencies, self.commands[commandName].requirements, "||")
	def IdentifyDeviceCommands(self):
		for command in self.commands.values():
			command.deviceCommand = self.IsTypeDescendant(command.params[0].type, "VkDevice")

def ParseXML():
	# Create an empty command set
	commandSet = CommandSet()

	# Open the XML tree
	tree = ET.parse(urllib.request.urlopen(VK_XML_PATH))
	root = tree.getroot()

	# Parse the XML file
	commandSet.ParseTypes(root)
	commandSet.ParseTags(root)
	commandSet.ParseCommands(root)
	commandSet.ParseFeatures(root)
	commandSet.ParseExtensions(root)
	commandSet.IdentifyDeviceCommands()

	return commandSet

def GenerateHeader(commandSet):
	# Open the header file
	headerFile = open("VulkanLoader.hpp", "w")

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
	for command in commandSet.commands.values():
		# Write the requirement ifs
		WriteRequirementIfs(headerFile, prevCommand, command)
		prevCommand = command

		# Write the current command's declaration
		headerFile.write("\t\t" + command.GetDeclarationText("") + " const;\n")
	headerFile.write("#endif\n")

	# Write the variable declaration start
	headerFile.write("\tprivate:\n\t\t// Vulkan function pointers\n")

	# Write all command function pointer variables
	prevCommand = None
	for command in commandSet.commands.values():
		# Write the requirement ifs
		WriteRequirementIfs(headerFile, prevCommand, command)
		prevCommand = command

		# Write the current command's variable
		headerFile.write("\t\tPFN_{0} pfn_{0};\n".format(command.name))
	headerFile.write("#endif\n")

	# End the header file
	headerFile.write("\t};\n}\n")

	# Close the header file
	headerFile.close()

def GenerateSource(commandSet):
	# Open the source file
	sourceFile = open("VulkanLoader.cpp", "w")

	# Begin the source file
	sourceFile.write("""#include \"VulkanLoader.hpp\"
#include <stdexcept>

#if defined(WFE_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#elif defined(WFE_PLATFORM_LINUX)
#include <dlfcn.h>
#endif

namespace wfe {
	// Library access macros
#if defined(WFE_PLATFORM_WINDOWS)
#define WFE_LIBRARY_TYPE HMODULE
#define WFE_OPEN_LIBRARY(libName) LoadLibraryA(libName)
#define WFE_LIBRARY_FUNC(lib, func) GetProcAddress(lib, func)
#define WFE_CLOSE_LIBRARY(lib) FreeLibrary(lib)
#elif defined(WFE_PLATFORM_LINUX)
#define WFE_LIBRARY_TYPE void*
#define WFE_OPEN_LIBRARY(libName) dlopen(libName, RTLD_NOW)
#define WFE_LIBRARY_FUNC(lib, func) dlsym(lib, func)
#define WFE_CLOSE_LIBRARY(lib) dlclose(lib)
#endif
	
	// Constants
#if defined(WFE_PLATFORM_WINDOWS)
	static const char* const LIBRARY_NAME = "vulkan-1.dll";
#elif defined(WFE_PLATFORM_LINUX)
	static const char* const LIBRARY_NAME = "libvulkan.so";
#endif
				  
	// Internal variables
	static atomic_size_t loaderCount = 0;
	static WFE_LIBRARY_TYPE vulkanLib;

	// Static function pointers
""")

	# Write the static function pointers
	prevCommand = None
	for command in commandSet.commands.values():
		# Write the requirement ifs
		WriteRequirementIfs(sourceFile, prevCommand, command)
		prevCommand = command

		# Write the current command's variable
		sourceFile.write("\tstatic PFN_{0} staticPfn_{0};\n".format(command.name))
	sourceFile.write("#endif\n")

	# Write the static function pointer loader
	sourceFile.write("""
	// Internal helper functions
	static void LoadStaticFunctionPointers() {
		vulkanLib = WFE_OPEN_LIBRARY(LIBRARY_NAME);
		if(!vulkanLib)
			throw std::runtime_error(\"Failed to load Vulkan dynamic lib!\");

""")

	prevCommand = None
	for command in commandSet.commands.values():
		# Write the requirement ifs
		WriteRequirementIfs(sourceFile, prevCommand, command)
		prevCommand = command

		# Write the current command's load
		sourceFile.write("\t\tstaticPfn_{0} = (PFN_{0})WFE_LIBRARY_FUNC(vulkanLib, \"{0}\");\n".format(command.name))
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
	for command in commandSet.commands.values():
		# Write the requirement ifs
		WriteRequirementIfs(sourceFile, prevCommand, command)
		prevCommand = command

		# Write the current command's load
		sourceFile.write("\t\tpfn_{0} = staticPfn_{0};\n".format(command.name))
	sourceFile.write("#endif\n\t}\n")

	sourceFile.write("""
	void VulkanLoader::LoadInstanceFunctions(VkInstance instance) {
""")

	prevCommand = None
	for command in commandSet.commands.values():
		# Skip the current command if it is a device command
		if not command.deviceCommand:
			# Write the requirement ifs
			WriteRequirementIfs(sourceFile, prevCommand, command)
			prevCommand = command

			# Write the current command's load
			sourceFile.write("\t\tpfn_{0} = (PFN_{0})vkGetInstanceProcAddr(instance, \"{0}\");\n".format(command.name))
	sourceFile.write("#endif\n\t}")

	sourceFile.write("""
	void VulkanLoader::LoadDeviceFunctions(VkDevice device) {
""")

	prevCommand = None
	for command in commandSet.commands.values():
		# Skip the current command if it is not a device command
		if command.deviceCommand:
			# Write the requirement ifs
			WriteRequirementIfs(sourceFile, prevCommand, command)
			prevCommand = command

			# Write the current command's load
			sourceFile.write("\t\tpfn_{0} = (PFN_{0})vkGetDeviceProcAddr(device, \"{0}\");\n".format(command.name))
	sourceFile.write("#endif\n\t}\n")

	# Write the loader's destructor
	sourceFile.write("""
	VulkanLoader::~VulkanLoader() {
		// Free the dynamic lib if no loaders are left
		if(!--loaderCount)
			WFE_CLOSE_LIBRARY(vulkanLib);
	}

""")

	# Write all loader command definitions
	prevCommand = None
	for command in commandSet.commands.values():
		# Write the requirement ifs
		WriteRequirementIfs(sourceFile, prevCommand, command)
		prevCommand = command

		# Write the current command's declaration
		sourceFile.write("\t" + command.GetDeclarationText("VulkanLoader::") + " const {\n")

		# Write the function pointer calls
		sourceFile.write("\t\tif(pfn_{0})".format(command.name) + " {\n")
		sourceFile.write("\t\t\t" + command.GetCallText("pfn_") + "\n")
		sourceFile.write("\t\t} else {\n")
		sourceFile.write("\t\t\t" + command.GetCallText("staticPfn_") + "\n")
		sourceFile.write("\t\t}\n\t}\n")
	sourceFile.write("#endif\n")

	# End the source file's namespace
	sourceFile.write("}\n\n")

	# Write the static function definitions
	prevCommand = None
	for command in commandSet.commands.values():
		# Write the requirement ifs
		WriteRequirementIfs(sourceFile, prevCommand, command)
		prevCommand = command

		# Write the static function's definition
		sourceFile.write(command.GetDeclarationText("") + " {\n")
		sourceFile.write("\t" + command.GetCallText("wfe::staticPfn_") + "\n")
		sourceFile.write("}\n")
	sourceFile.write("#endif\n")

	# Close the source file
	sourceFile.close()

if __name__ == "__main__":
	commandSet = ParseXML()
	GenerateHeader(commandSet)
	GenerateSource(commandSet)

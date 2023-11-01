# Import the ElementTree XML library
import re
import urllib.request
import xml.etree.ElementTree as ET

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
		self.core = False
		self.instance = False

def IsTypeDescendant(types, name, base):
	# Check if the base was already reached
	if name == base:
		return True
	# Check if the current type does not exist
	type = types.get("name")
	if not type:
		return False
	# Check if the current type doesn't have any parents
	parents = type.get("parents")
	if not parents:
		return False
	# Loop through all parents, checking if the given type is the descendant
	for parent in parents.split(","):
		if IsTypeDescendant(types, parent, base):
			return True
	return False

def FormCommandRequirement(require):
	return "defined({0})".format(require.group(0))
def FormCommandRequirements(require):
	# Add a defined condition to every requirement and replace every logical operator
	return re.sub(r"[a-zA-Z0-9_]+", FormCommandRequirement, require).replace(",", " || ").replace("+", " && ")

def WriteCommandDefinition(command, outFile):
	# Write the command's definition
	outFile.write("VKAPI_ATTR {0} VKAPI_CALL {1}(".format(command.returnType, command.name))

	# Write the command's parameters
	for i in range(0, len(command.params)):
		# Write the parameter's full name
		outFile.write(command.params[i].fullName)

		# Write a comma if this is not the last parameter
		if i != len(command.params) - 1:
			outFile.write(", ")
	
	# Write the function body
	if command.returnType == "void":
		outFile.write(") {\n\tpfn_" + command.name + "(")
	else:
		outFile.write(") {\n\treturn pfn_" + command.name + "(")
	
	# Write the called pointer's parameters
	for i in range(0, len(command.params)):
		# Write the parameter's name
		outFile.write(command.params[i].name)

		# Write a comma if this is not the last parameter
		if i != len(command.params) - 1:
			outFile.write(", ")
	
	# End the function body
	outFile.write(");\n}\n")
def WriteCoreCommandLoad(command, outFile):
	# Write the command's load
	outFile.write("""
		pfn_{0} = (PFN_{0})vulkanLib.LoadFunc(\"{0}\");
		if(!pfn_{0}) """.format(command.name) + "{" + """
			// Free the library and exit the function
			vulkanLib.FreeLib();
			return false;
		}
""")
def WriteInstanceCommandLoad(command, outFile):
	# Write the command's load
	outFile.write("\t\tpfn_{0} = (PFN_{0})vkGetInstanceProcAddr(instance, \"{0}\");\n".format(command.name))
def WriteDeviceCommandLoad(command, outFile):
	# Write the command's load
	outFile.write("\t\tpfn_{0} = (PFN_{0})vkGetDeviceProcAddr(device, \"{0}\");\n".format(command.name))

def main():
	# Open the XML tree and output file
	vkXmlPath = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

	tree = ET.parse(urllib.request.urlopen(vkXmlPath))
	outFile = open("Loader.cpp", "w")

	# Find the Vulkan command definition root
	root = tree.getroot()

	# Loop through all commands
	commands = []

	for command in root.findall("commands/command"):
		# Skip the current command if it is not from the Vulkan API
		if command.get("api") and command.get("api") != "vulkan":
			continue

		# Check if the current command is an alias
		if command.get("alias"):
			# Get the current command's name and alias
			commandName = command.get("name")
			commandAlias = command.get("alias")

			# Find the matching command
			for i in range(0, len(commands)):
				if commands[i].name == commandAlias:
					# Add the new command to the array and exit the loop
					commands.append(Command(commandName, commands[i].returnType, commands[i].params))
					break
			continue

		# The current command is original; get the command's name and return value
		commandName = command.find("proto").find("name").text
		commandReturnType = command.find("proto").find("type").text

		# Get the current command's parameters
		commandParams = []
		for commandParam in command.findall("param"):
			# Skip the param if it has no valid structure
			if commandParam.find("type") == None or commandParam.find("name") == None:
				continue

			# Skip the param if it is not from the Vulkan API
			if commandParam.get("api") and commandParam.get("api") != "vulkan":
				continue

			# Add the current parameter to the array
			paramFullName = ET.tostring(commandParam, encoding = "unicode", method = "text")
			paramFullName = re.sub(r"\s+", " ", paramFullName)

			param = Param(commandParam.find("name").text, commandParam.find("type").text, paramFullName)
			param.fullName = param.fullName[:param.fullName.find('\n')]
			commandParams.append(param)
		
		# Add the current command to the array
		commands.append(Command(commandName, commandReturnType, commandParams))
	
	# Loop through all Vulkan feature definition roots
	for featureRoot in root.findall("feature"):
		# Remove all commands from the given feature root if it is not from the Vulkan API
		api = featureRoot.get("api")
		if "vulkan" not in api.split(","):
			for requirements in featureRoot.findall("require"):
				for command in requirements.findall("command"):
					# Get the current command's name
					commandName = command.get("name")

					# Remove the command with the same name from the array
					for i in range(0, len(commands)):
						if commands[i].name == commandName:
							# Remove the command from the array and exit the loop
							commands.pop(i)
							break
			
			# Move on to the next feature root
			continue

		# Loop through all commands
		for requirements in featureRoot.findall("require"):
			# Get the current requirement's depends
			for command in requirements.findall("command"):
				# Get the current command's name
				commandName = command.get("name")

				# Add the current requirement to the version
				for i in range(0, len(commands)):
					if commands[i].name == commandName:
						# Set the current command's requirements
						commands[i].requirements = "defined({0})".format(featureRoot.get("name"))
						break
	
	# Loop through every Vulkan extension
	for extension in root.iter("extension"):
		# Get the current extension's dependencies
		extensionDepends = extension.get("depends")
		if extensionDepends:
			extensionDepends = "defined({0}) && ({1})".format(extension.get("name"), FormCommandRequirements(extensionDepends))
		else:
			extensionDepends = "defined({0})".format(extension.get("name"))
		extensionType = extension.get("type")

		# Loop through the extension's commands
		for requirements in extension.iter("require"):
			# Get the current require's dependencies
			depends = requirements.get("depends")
			if depends:
				# Merge the extension depends with the current depends, if it exists
				depends = FormCommandRequirements(depends)
				depends = "({0}) && ({1})".format(extensionDepends, depends)
			else:
				depends = extensionDepends

			for command in requirements.iter("command"):
				# Get the current command's name
				commandName = command.get("name")

				# Remove the command with the same name from the array
				for i in range(0, len(commands)):
					if commands[i].name == commandName:
						# Add the current dependencies to the command, if they exist
						if commands[i].requirements != "":
							commands[i].requirements = "({0}) || ({1})".format(commands[i].requirements, depends)
						else:
							commands[i].requirements = depends

						# Check if the current command can be an instance command
						if extensionType == "instance":
							commands[i].instance = True

						break
	
	# Loop through all types and add them to a dictionary
	types = {}

	for type in root.findall("types/type"):
		types[type.get("name")] = type

	# Check for all device, instance and core commands
	for i in range(0, len(commands)):
		# Check if the current command is a core command
		if commands[i].requirements == "defined(VK_VERSION_1_0)":
			commands[i].core = True
		# Check if the current command is an instance or device command
		commands[i].instance = not IsTypeDescendant(types, commands[i].params[0].type, "VkDevice")
	
	# Start writing to the file
	outFile.write("""#include \"Loader.hpp\"

#if defined(WFE_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(WFE_PLATFORM_LINUX)
#define VK_USE_PLATFORM_XLIB_KHR
#include <X11/Xlib.h>
#endif

#include <vulkan/vulkan.h>

""")
	
	# Write the variables for every function pointer in the file
	for i in range(0, len(commands)):
		# Check if the current command is the first command
		if i == 0:
			# Write the first command's requirements
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
		elif commands[i].requirements != commands[i - 1].requirements:
			# Write the current command's requirements
			outFile.write("#endif\n")
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
		# Write the current command's variable
		outFile.write("static PFN_{0} pfn_{0};\n".format(commands[i].name))
	outFile.write("#endif\n\n")

	# Write the Vulkan function definitions
	for i in range(0, len(commands)):
		# Check if the current command is the first command
		if i == 0:
			# Write the first command's requirements
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
		elif commands[i].requirements != commands[i - 1].requirements:
			# Write the current command's requirements
			outFile.write("#endif\n")
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
		# Write the current command's definition
		WriteCommandDefinition(commands[i], outFile)
	outFile.write("#endif\n\n")

	# Write the Vulkan core function loaders
	outFile.write("""namespace wfe {
	// Variables
	static DynamicLib vulkanLib;
	
	// Public functions
	bool8_t LoadVulkanCoreFunctions() {
		// Try to load the Vulkan dynamic lib
#if defined(WFE_PLATFORM_WINDOWS)
		bool8_t libLoaded = vulkanLib.LoadLib(\"vulkan-1.dll\");
#elif defined(WFE_PLATFORM_LINUX)
		bool8_t libLoaded = vulkanLib.LoadLib(\"libvulkan.so\");
#endif
		if(!libLoaded)
			return false;
		
		// Load all Vulkan core functions
""")
	firstCommand = True
	prevIndex = 0
	for i in range(0, len(commands)):
		# Skip the current command if it is not a core command
		if not commands[i].core:
			continue

		# Check if the current command is the first command
		if firstCommand:
			# Write the first command's requirements
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
			firstCommand = False
		elif commands[i].requirements != commands[prevIndex].requirements:
			# Write the current command's requirements
			outFile.write("#endif\n")
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
		# Write the command's load
		WriteCoreCommandLoad(commands[i], outFile)
		prevIndex = i
	
	# Write the Vulkan instance function loaders
	outFile.write("""#endif

		return true;
	}
	void LoadVulkanInstanceFunctions(VkInstance instance) {
""")
	firstCommand = True
	prevIndex = 0
	for i in range(0, len(commands)):
		# Skip the current command if it is not an instance command
		if not commands[i].instance:
			continue

		# Check if the current command is the first command
		if firstCommand:
			# Write the first command's requirements
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
			firstCommand = False
		elif commands[i].requirements != commands[prevIndex].requirements:
			# Write the current command's requirements
			outFile.write("#endif\n")
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
		# Write the command's load
		WriteInstanceCommandLoad(commands[i], outFile)
		prevIndex = i

	# Write the Vulkan device function loaders
	outFile.write("""#endif
	}
	void LoadVulkanDeviceFunctions(VkDevice device) {
""")
	firstCommand = True
	prevIndex = 0
	for i in range(0, len(commands)):
		# Skip the current command if it is not a device command
		if commands[i].instance:
			continue

		# Check if the current command is the first command
		if firstCommand:
			# Write the first command's requirements
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
			firstCommand = False
		elif commands[i].requirements != commands[prevIndex].requirements:
			# Write the current command's requirements
			outFile.write("#endif\n")
			outFile.write("#if {0}\n".format(commands[i].requirements, outFile))
		# Write the command's load
		WriteDeviceCommandLoad(commands[i], outFile)
		prevIndex = i
	
	# End the file
	outFile.write("""#endif
	}
	void FreeVulkanFunctions() {
		// Free the Vulkan dynamic library
		vulkanLib.FreeLib();
	}
}
""")

	
	outFile.close()

if __name__ == "__main__":
	main()
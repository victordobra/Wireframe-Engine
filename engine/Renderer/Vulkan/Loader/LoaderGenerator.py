# Import the ElementTree XML library
import xml.etree.ElementTree as ET

class Param:
	def __init__(self, name, fullName):
		self.name = name
		self.fullName = fullName

class Command:
	def __init__(self, name, returnType, params):
		self.name = name
		self.returnType = returnType
		self.params = params

commands = []
windowsCommands = []
linuxCommands = []

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
def WriteCommandLoad(command, outFile):
	# Write the command's load
	outFile.write("""
		pfn_{0} = (PFN_{0})vulkanLib.LoadFunc(\"{0}\");
		if(!pfn_{0})
			return false;
""".format(command.name))

def main():
	# Open the XML tree and output file
	tree = ET.parse("vk.xml")
	outFile = open("Loader.cpp", "w")

	# Find the Vulkan command definition root
	root = tree.getroot()
	commandsRoot = list(root.iter("commands"))[0]

	# Loop through all commands
	for command in commandsRoot.iter("command"):
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
		for commandParam in command.iter("param"):
			# Skip the param if it has no valid structure
			if commandParam.find("type") == None or commandParam.find("name") == None:
				continue

			# Skip the param if it is not from the Vulkan API
			if commandParam.get("api") and commandParam.get("api") != "vulkan":
				continue

			# Add the current parameter to the array
			param = Param(commandParam.find("name").text, ET.tostring(commandParam, encoding = "unicode", method = "text"))
			param.fullName = param.fullName[:param.fullName.find('\n')]
			commandParams.append(param)
		
		# Add the current command to the array
		commands.append(Command(commandName, commandReturnType, commandParams))
	
	# Loop through all Vulkan feature definition roots
	for featureRoot in root.iter("feature"):
		# Move on to the next feature root if the current root is the Vulkan 1.0 feature root
		if featureRoot.get("name") == "VK_VERSION_1_0":
			continue
		
		# Loop through all commands
		for requirements in featureRoot.iter("require"):
			for command in requirements.iter("command"):
				# Get the current command's name
				commandName = command.get("name")

				# Remove the command with the same name from the array
				for i in range(0, len(commands)):
					if commands[i].name == commandName:
						# Remove the command from the array and exit the loop
						commands.pop(i)
						break
	
	# Loop through every Vulkan extension
	for extension in root.iter("extension"):
		# Move on to the next extension if the current extension is a WSI extension
		extensionName = extension.get("name")
		if extensionName == "VK_KHR_surface" or extensionName == "VK_KHR_swapchain":
			continue

		# Loop through the extension's commands
		for requirements in extension.iter("require"):
			for command in requirements.iter("command"):
				# Get the current command's name
				commandName = command.get("name")

				# Remove the command with the same name from the array
				for i in range(0, len(commands)):
					if commands[i].name == commandName:
						# Move the command to the appropriate array if the current extension is a platform-specific WSI extension
						if extensionName == "VK_KHR_win32_surface":
							windowsCommands.append(commands[i])
						elif extensionName == "VK_KHR_xlib_surface":
							linuxCommands.append(commands[i])
						
						# Remove the command from the array and exit the loop
						commands.pop(i)
						break
	
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
	for command in commands:
		outFile.write("static PFN_{0} pfn_{0};\n".format(command.name))
	
	# Write the variables for the platform-specific functions
	outFile.write("\n#if defined(WFE_PLATFORM_WINDOWS)\n")
	for command in windowsCommands:
		outFile.write("static PFN_{0} pfn_{0};\n".format(command.name))
	outFile.write("#elif defined(WFE_PLATFORM_LINUX)\n")
	for command in linuxCommands:
		outFile.write("static PFN_{0} pfn_{0};\n".format(command.name))
	outFile.write("#endif\n\n")

	# Write the Vulkan function definitions
	outFile.write("extern \"C\" {\n\n")

	for command in commands:
		WriteCommandDefinition(command, outFile)
	
	# Write the Vulkan platform-specific function definitions
	outFile.write("\n#if defined(WFE_PLATFORM_WINDOWS)\n")
	for command in windowsCommands:
		WriteCommandDefinition(command, outFile)
	outFile.write("#elif defined(WFE_PLATFORM_LINUX)\n")
	for command in linuxCommands:
		WriteCommandDefinition(command, outFile)
	outFile.write("#endif\n\n}\n")

	# Write the Vulkan loader function
	outFile.write("""
namespace wfe {
	// Internal variables
	DynamicLib vulkanLib;
	
	// Public functions
	bool8_t LoadVulkanFunctions() {
		// Load the Vulkan dynamic lib
#if defined(WFE_PLATFORM_WINDOWS)
		bool8_t libLoaded = vulkanLib.LoadLib(\"vulkan-1.dll\");
#elif defined(WFE_PLATFORM_LINUX)
		bool8_t libLoaded = vulkanLib.LoadLib(\"libvulkan.so\");
#endif

		if(!libLoaded)
			return false;
""")

	# Write the Vulkan command loads
	for command in commands:
		WriteCommandLoad(command, outFile)
	
	# Write the Vulkan platform-specific function definitions
	outFile.write("\n#if defined(WFE_PLATFORM_WINDOWS)")
	for command in windowsCommands:
		WriteCommandLoad(command, outFile)
	outFile.write("#elif defined(WFE_PLATFORM_LINUX)")
	for command in linuxCommands:
		WriteCommandLoad(command, outFile)
	outFile.write("#endif\n")

	# End the Vulkan loader function
	outFile.write("""
		WFE_LOG_INFO(\"Loaded Vulkan functions.\");

		return true;
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
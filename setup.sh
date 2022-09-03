sudo apt update

# Installing Vulkan
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.224-focal.list https://packages.lunarg.com/vulkan/1.3.224/lunarg-vulkan-1.3.224-focal.list
sudo apt update
sudo apt install vulkan-sdk
echo "--------------------------------------------INSTALLED VULKAN"

# Installing X11/xcb libraries
sudo apt install xcb
sudo apt install xorg-dev
sudo apt install libx11-xcb-dev
sudo apt install libxkbcommon-dev
sudo apt install libxcb-xfixes0-dev
echo "--------------------------------------------INSTALLED X11/XCB LIBRARIES"
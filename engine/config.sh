# install list
# - boost++
# - Vulkan SDK

git submodule add https://github.com/glfw/glfw.git
sudo apt-get install libboost-all-dev
wget -c https://vulkan.lunarg.com/sdk/home#sdk/downloadConfirm/1.1.92.1/linux/vulkansdk-linux-x86_64-1.1.92.1.tar.gz -O - | tar -xz

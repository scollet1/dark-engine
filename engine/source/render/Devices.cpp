#include "../../includes/render/RenderMgr.hpp"

bool						RenderMgr::createLogicalDevice() {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
	printf("fuck u\n");

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {
			indices.graphicsFamily.second,
			indices.presentFamily.second
	};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	printf("fuck u\n");
	

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.sampleRateShading = VK_TRUE; // enable sample shading feature for the device

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}
	printf("fuck u\n");

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(_device, indices.graphicsFamily.first, 0, &graphicsQueue);
	vkGetDeviceQueue(_device, indices.presentFamily.first, 0, &presentQueue);

	return (!!_device);
}

bool									RenderMgr::checkDeviceExtensionSupport(
		VkPhysicalDevice device) {
	printf("checking device extension support\n");
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	printf("%s%d%s extensions supported\n", ANSI_COLOR_BLUE, extensionCount, ANSI_COLOR_RESET);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(
			device, nullptr,
			&extensionCount,
			availableExtensions.data()
			);
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtensions) {
		// printf("%s extension\n", extension.extensionName); // no touchy // why though? we'll never know
		requiredExtensions.erase(extension.extensionName);
	}
	printf("required extensions cache %s empty\n", requiredExtensions.empty()?"is":"is not");
	// return requiredExtensions.empty();
	return (true);
}

bool	RenderMgr::isDeviceSuitable(VkPhysicalDevice device) {
	printf("checking device suitability\n");

	QueueFamilyIndices indices = findQueueFamilies(device);
	
	bool extensionsSupported = checkDeviceExtensionSupport(device);
	printf("extensions supported?             | %s%s%s\n",
		extensionsSupported? TRUE_STR:FALSE_STR,
		extensionsSupported? "TRUE":"FALSE",
		ANSI_COLOR_RESET);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() &&
							!swapChainSupport.presentModes.empty();
	}
	// TODO : OFFSETS!!!
	printf("swap chain adequate?              | %s%s%s\n",
		swapChainAdequate? TRUE_STR:FALSE_STR,
		swapChainAdequate? "TRUE":"FALSE",
		ANSI_COLOR_RESET);

	VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() &&
    		extensionsSupported &&
    		swapChainAdequate &&
    		supportedFeatures.samplerAnisotropy;
    //	return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
//		   deviceFeatures.geometryShader);
}

// TODO
//int										RenderMgr::rateDeviceSuitability(VkPhysicalDevice device) {
//	int score = 0;
//
//	// Discrete GPUs have a significant performance advantage
//	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
//		score += 1000;
//	}
//
//	// Maximum possible size of textures affects graphics quality
//	VkPhysicalDeviceFeatures deviceFeatures = {};
//	score += deviceProperties.limits.maxImageDimension2D;
//
//	// Application can't function without geometry shaders
//	if (!deviceFeatures.geometryShader) {
//		return 0;
//	}
//
//	return score;
//}

bool									RenderMgr::pickPhysicalDevice() {
//	std::multimap<int, VkPhysicalDevice> candidates;
	uint32_t deviceCount = 0;

	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
	if (deviceCount == 0)
		throw std::runtime_error("failed to find GPUs with Vulkan support!");

	printf("%s%d%s physical device%s supported on this system\n",
			ANSI_COLOR_BLUE, deviceCount, ANSI_COLOR_RESET, deviceCount > 1? "s":"");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

	printf("%sexploring physical devices%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            msaaSamples = getMaxUsableSampleCount();
            break;
        }
    }

	// printf("physical device is a thing?       | %s%s%s\n",
		// (physicalDevice == VK_NULL_HANDLE)? FALSE_STR:TRUE_STR,
		// (physicalDevice == VK_NULL_HANDLE)? "FALSE":"TRUE",
		// ANSI_COLOR_RESET);

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
	return (!!physicalDevice);
//	for (const auto& device : devices) {
//		int score = rateDeviceSuitability(device);
//		candidates.insert(std::make_pair(score, device));
//	}

	// Check if the best candidate is suitable at all
//	if (candidates.rbegin()->first > 0) {
//		physicalDevice = candidates.rbegin()->second;
//	} else {
//		throw std::runtime_error("failed to find a suitable GPU!");
//	}
}
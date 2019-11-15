#include "../../includes/Render/RenderManager.hpp"

VkSurfaceFormatKHR		RenderManager::chooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& availableFormats
) {
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	// TODO : return priority of available formats
	return (availableFormats[0]);
}

SwapChainSupportDetails	RenderManager::querySwapChainSupport(VkPhysicalDevice device) {
	SwapChainSupportDetails details;
	uint32_t formatCount;
	uint32_t presentModeCount;
//	bool swapChainAdequate = false;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return (details);
}

bool						RenderManager::createSwapChain() {
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
	uint32_t imageCount;

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	// TODO : make dynamic for stereoscopic 3D
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	uint32_t queueFamilyIndices[] = {
		indices.graphicsFamily.first,
		indices.presentFamily.first
	};

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(_device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_device, swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	return (SUCCESS);
}

VkExtent2D						RenderManager::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(dark_engine->get_window(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};
		actualExtent.width = std::max(
				capabilities.minImageExtent.width,
				std::min(
						capabilities.maxImageExtent.width,
						actualExtent.width));
		actualExtent.height = std::max(
				capabilities.minImageExtent.height,
				std::min(
						capabilities.maxImageExtent.height,
						actualExtent.height
						));
		return actualExtent;
	}
}

VkPresentModeKHR				RenderManager::chooseSwapPresentMode(
		const std::vector<VkPresentModeKHR> availablePresentModes
) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		} else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

bool	RenderManager::cleanupSwapChain() {
    vkDestroyImageView(_device, colorImageView, nullptr);
    vkDestroyImage(_device, colorImage, nullptr);
    vkFreeMemory(_device, colorImageMemory, nullptr);

	for (auto framebuffer : swapChainFramebuffers) {
	    vkDestroyFramebuffer(_device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(_device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

	vkDestroyPipeline(_device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(_device, pipelineLayout, nullptr);
	vkDestroyRenderPass(_device, renderPass, nullptr);

	for (auto imageView : swapChainImageViews) {
	    vkDestroyImageView(_device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(_device, swapChain, nullptr);
	
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		vkDestroyBuffer(_device, uniformBuffers[i], nullptr);
		vkFreeMemory(_device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorPool(_device, descriptorPool, nullptr);

    return (SUCCESS);
}

bool RenderManager::createDescriptorSets() {
	std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(swapChainImages.size());
    if (vkAllocateDescriptorSets(_device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < swapChainImages.size(); i++) {
    	printf("%i\n", i);
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture_image_views[0];
        imageInfo.sampler = textureSampler;

        std::array<VkWriteDescriptorSet, 4> descriptorWrites = {};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
    return SUCCESS;
}

// TODO : continue rendering while generating
//			new swap chain
bool	RenderManager::recreateSwapChain() {
	int width = 0, height = 0;
    
    while (width == 0 && height == 0) { // note && ||
        glfwGetFramebufferSize(dark_engine->get_window(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(_device);

    cleanupSwapChain();
    printf("create swap re\n");
    createSwapChain();
    printf("done create swap re\n");

    create_image_views();
    createRenderPass();
    createGraphicsPipeline();
    createColorResources();
    createDepthResources();
    createFrameBuffers();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    return (SUCCESS);
}
#include "../../includes/Render/RenderManager.hpp"

void RenderManager::create_image(
	uint32_t width, uint32_t height, uint32_t mipLevels,
	VkSampleCountFlagBits numSamples, VkFormat format,
	VkImageTiling tiling, VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties, VkImage& image,
	VkDeviceMemory& imageMemory
) {
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = numSamples;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(_device, image, imageMemory, 0);
}

VkImageView RenderManager::create_image_view(
	VkImage image, VkFormat format,
	VkImageAspectFlags aspectFlags,
	uint32_t mipLevels
) {
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void RenderManager::create_image_views() {
	swapChainImageViews.resize(swapChainImages.size());

	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i] = create_image_view(
			swapChainImages[i], swapChainImageFormat,
			VK_IMAGE_ASPECT_COLOR_BIT, 1
		);
	}
}

VkImage RenderManager::create_texture_image(Texture texture) {
	VkDeviceSize imageSize = texture.width * texture.height * 4;
	VkImage texture_image;
	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texture.width, texture.height)))) + 1;

	VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

	createBuffer(
		imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory
	);

	void* data;
	vkMapMemory(_device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, texture.pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(_device, stagingBufferMemory);

	stbi_image_free(texture.pixels);

	create_image(
		texture.width, texture.height, mipLevels,
		VK_SAMPLE_COUNT_1_BIT,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
		VK_IMAGE_USAGE_TRANSFER_DST_BIT |
		VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		texture_image, textureImageMemory
	);

    transitionImageLayout(
    	texture_image,
    	VK_FORMAT_R8G8B8A8_UNORM,
    	VK_IMAGE_LAYOUT_UNDEFINED,
    	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    	mipLevels
    );
    copyBufferToImage(
    	stagingBuffer, texture_image,
    	static_cast<uint32_t>(texture.width),
    	static_cast<uint32_t>(texture.height)
    );

    //transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps
    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    vkFreeMemory(_device, stagingBufferMemory, nullptr);

	return texture_image;
}

VkImageView RenderManager::create_texture_image_view(VkImage texture_image) {
	return create_image_view(
		texture_image, VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_ASPECT_COLOR_BIT, 1
	);
}

void RenderManager::create_texture_image_views() {
	VkImageView texture_image_view;
	std::vector<VkImage>::iterator i;

	for (i = texture_images.begin(); i != texture_images.end(); i++) {
		texture_image_view = create_texture_image_view(*i);
		texture_image_views.push_back(texture_image_view);
	}
}

void RenderManager::destroy_texture_images() {
	std::vector<VkImage>::iterator i;

	for (i = texture_images.begin(); i != texture_images.end(); i++) {
	    vkDestroyImage(_device, *i, nullptr);
	}
}


#include "../../includes/Render/RenderManager.hpp"

/*
 * BIG TODO: split these functions into appropriate files
 * AND KEEP THEM THERE!!!
 */

void RenderManager::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(_device, commandPool, 1, &commandBuffer);
}

VkCommandBuffer RenderManager::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void RenderManager::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
}

void RenderManager::transitionImageLayout(
	VkImage image, VkFormat format,
	VkImageLayout oldLayout,
	VkImageLayout newLayout,
	uint32_t mipLevels
) {

	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
	    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	    if (hasStencilComponent(format)) {
	        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	    }
	} else {
	    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
	    barrier.srcAccessMask = 0;
	    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	} else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
	    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	} else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
	    barrier.srcAccessMask = 0;
	    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	} else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
	    barrier.srcAccessMask = 0;
	    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	    destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	} else {
	    throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
	    commandBuffer,
	    sourceStage, destinationStage,
	    0,
	    0, nullptr,
	    0, nullptr,
	    1, &barrier
	);

	endSingleTimeCommands(commandBuffer);
}

bool RenderManager::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);

	return (SUCCESS);
}

uint32_t RenderManager::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
	    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
	        return i;
	    }
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void RenderManager::device_wait_idle() {
	vkDeviceWaitIdle(_device);
}

bool RenderManager::createBuffer(
	VkDeviceSize size, VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties, VkBuffer& buffer,
	VkDeviceMemory& bufferMemory
) {
    
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
        return (FAILURE);
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    int status;
    if ((status = vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory)) != VK_SUCCESS) {
    	std::cout << status << std::endl;
        throw std::runtime_error("failed to allocate buffer memory!");
        return (FAILURE);
    }

    vkBindBufferMemory(_device, buffer, bufferMemory, 0);

    return (SUCCESS);
}

void RenderManager::compile_vertices(std::vector<Vertex> obj_vertices) {
	std::vector<Vertex>::iterator i;

	for (i = obj_vertices.begin(); i != obj_vertices.end(); i++) {
		vertices.push_back(*i);
	}
}

void RenderManager::compile_indices(std::vector<uint32_t> obj_indices) {
	std::vector<uint32_t>::iterator i;

	for (i = obj_indices.begin(); i != obj_indices.end(); i++) {
		indices.push_back(*i);
	}
}

void RenderManager::compile_objects(std::vector<Object> objects) {
	Texture *obj_texture;
	std::vector<Object>::iterator i;
	std::vector<Vertex> obj_vertices;
	std::vector<uint32_t> obj_indices;

	for (i = objects.begin(); i != objects.end(); i++) {
		obj_vertices = (*i).get_vertices();
		compile_vertices(obj_vertices);
		obj_indices = (*i).get_indices();
		compile_indices(obj_indices);
		obj_texture = (*i).get_texture();
		texture_images.push_back(create_texture_image(obj_texture));
	}
}

bool RenderManager::createVertexBuffer() {
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    
	createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory
	);
	
	void* data;
	vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t) bufferSize);
	vkUnmapMemory(_device, stagingBufferMemory);

	createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer, vertexBufferMemory
	);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    vkFreeMemory(_device, stagingBufferMemory, nullptr);

	return (SUCCESS);
}

bool RenderManager::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(_device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

    return (SUCCESS);
}

std::vector<char> RenderManager::readFile(const std::string& filename) {
	// TODO : move to utils
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	std::cout << "opening file " << filename << " for reading" << std::endl;
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}
	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return (buffer);
}

//void							RenderManager::createSemaphores() {
//	VkSemaphoreCreateInfo semaphoreInfo = {};
//
//	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
//	if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, imageAvailableSemaphores) != VK_SUCCESS ||
//		vkCreateSemaphore(_device, &semaphoreInfo, nullptr, renderFinishedSemaphores) != VK_SUCCESS) {
//		throw std::runtime_error("failed to create semaphores!");
//	}
//}

bool RenderManager::createCommandBuffers() {
	commandBuffers.resize(swapChainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

	if (vkAllocateCommandBuffers(_device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
	    throw std::runtime_error("failed to allocate command buffers!");
	}

	for (size_t i = 0; i < commandBuffers.size(); i++) {
	    VkCommandBufferBeginInfo beginInfo = {};
	    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
	        throw std::runtime_error("failed to begin recording command buffer!");
	    }

	    VkRenderPassBeginInfo renderPassInfo = {};
	    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	    renderPassInfo.renderPass = renderPass;
	    renderPassInfo.framebuffer = swapChainFramebuffers[i];
	    renderPassInfo.renderArea.offset = {0, 0};
	    renderPassInfo.renderArea.extent = swapChainExtent;

	    std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

	    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	        VkBuffer vertexBuffers[] = {vertexBuffer};
	        VkDeviceSize offsets[] = {0};
	        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
	        vkCmdBindIndexBuffer(
	        	commandBuffers[i], indexBuffer,
	        	0, VK_INDEX_TYPE_UINT32
	        );
	        vkCmdBindDescriptorSets(
	        	commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
	        	pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr
	        );

		// printf("%i %i\n", indices.size(), i);
	        vkCmdDrawIndexed(
	        	commandBuffers[i],
	        	/*
	        	BIG TODO: indices hand-off from the
	        	current scene

	        	renderMgr will have access to the scene
	        	graph, but ONLY the currently instanced
	        	scene
	        	*/
	        	static_cast<uint32_t>(indices.size()),
	        	1, 0, 0, 0
	        );
	    vkCmdEndRenderPass(commandBuffers[i]);

	    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
	        throw std::runtime_error("failed to record command buffer!");
	    }
	}

	return (SUCCESS);
}

bool							RenderManager::createCommandPool() {
	printf("creating command pools\n");
	QueueFamilyIndices			qfi = findQueueFamilies(physicalDevice);
	VkCommandPoolCreateInfo		poolInfo = {};
	
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = qfi.graphicsFamily.second;
	poolInfo.flags = 0; // Optional
	
	if (vkCreateCommandPool(_device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
		return(FAILURE);
	}

	return (SUCCESS);
}

bool							RenderManager::createFrameBuffers() {
	printf("creating frame buffers\n");

	swapChainFramebuffers.resize(swapChainImageViews.size());
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 3> attachments = {
            colorImageView,
            depthImageView,
            swapChainImageViews[i]
        };

	    VkFramebufferCreateInfo framebufferInfo = {};
	    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	    framebufferInfo.renderPass = renderPass;
	    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	    framebufferInfo.pAttachments = attachments.data();
	    framebufferInfo.width = swapChainExtent.width;
	    framebufferInfo.height = swapChainExtent.height;
	    framebufferInfo.layers = 1;

	    if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
	        throw std::runtime_error("failed to create framebuffer!");
	    }
	}

	return (SUCCESS);
}

bool							RenderManager::createRenderPass() {
	printf("creating render pass\n");
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = msaaSamples;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = findDepthFormat();
    depthAttachment.samples = msaaSamples;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve = {};
    colorAttachmentResolve.format = swapChainImageFormat;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentResolveRef = {};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 3> attachments = {
    	colorAttachment,
    	depthAttachment,
    	colorAttachmentResolve
	};

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(_device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}

	printf("render pass created\n");
	return (!!renderPass);
}

bool RenderManager::createGraphicsPipeline() {
	// iterate
	auto vertShaderCode = readFile(TEST_VERT_FILE);
	auto fragShaderCode = readFile(TEST_FRAG_FILE);
	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vertShaderStageInfo, fragShaderStageInfo
	};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapChainExtent.width;
	viewport.height = (float) swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.rasterizationSamples = msaaSamples;
    multisampling.sampleShadingEnable = VK_TRUE; // enable sample shading in the pipeline
    multisampling.minSampleShading = .2f; // min fraction for sample shading; closer to one is smoother

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
	    throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
	    throw std::runtime_error("failed to create graphics pipeline!");
	}

	vkDestroyShaderModule(_device, fragShaderModule, nullptr);
	vkDestroyShaderModule(_device, vertShaderModule, nullptr);

	return (!!graphicsPipeline);
}

QueueFamilyIndices				RenderManager::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;
	int i = 0;
	int ioffs = i?0:1;
	uint32_t queueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	indices.graphicsFamily.second = false;
	indices.presentFamily.second = false;
	
	printf("enumerating %d queue famil%s\n",
		queueFamilyCount, queueFamilyCount > 1?"ies":"y");

	for (const auto& queueFamily : queueFamilies) {
		
		printf("queue %d/%d\nqueue flags: %d\n",
				i+ioffs, queueFamily.queueCount, queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT);

		if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
			// printf("setting gfam as %d\n", i);
			indices.graphicsFamily.first = i;
			indices.graphicsFamily.second = true;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			// printf("setting pfam as %d\n", i);
			indices.presentFamily.first = i;
			indices.presentFamily.second = true;
		}

		printf("presentation support?             | %s%s%s\n",
			presentSupport?TRUE_STR:FALSE_STR,
			presentSupport?"TRUE":"FALSE",
			ANSI_COLOR_RESET);
		// printf("presentation family?              | %s%s%s\n",
			// indices.presentFamily.value()?TRUE_STR:FALSE_STR,
			// indices.presentFamily.value()?"TRUE":"FALSE",
			// ANSI_COLOR_RESET);
		// printf("graphics family?                  | %s%s%s\n",
			// indices.graphicsFamily.value()?TRUE_STR:FALSE_STR,
			// indices.graphicsFamily.value()?"TRUE":"FALSE",
			// ANSI_COLOR_RESET);
		if (indices.isComplete()) {
			printf("indices complete with graphics family iter %d\n", i);
			break;
		}

		i++;
	}

	return (indices);
}
																// TODO : more verbose var names
bool RenderManager::createInstance(const char *title, const char *name) {
	printf("ooof\n");
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}
	printf("ooof\n");

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = title;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = name;
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	printf("ooof\n");

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	printf("ooof\n");

	auto extensions = getRequiredExtensions();
	printf("ooof\n");
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	printf("ooof\n");

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}
	printf("ooof7\n");

	if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
	printf("ooof\n");
	return (!!_instance);
}

void RenderManager::updateUniformBuffer(uint32_t currentImage) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};

	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(_device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(_device, uniformBuffersMemory[currentImage]);
}

void	RenderManager::draw_frame() {
	bool framebufferResized;
	framebufferResized = dark_engine->get_frame_buffer_resized();

	vkWaitForFences(
		_device, 1, &inFlightFences[currentFrame],
		VK_TRUE, std::numeric_limits<uint64_t>::max()
	);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(
		_device, swapChain, std::numeric_limits<uint64_t>::max(),
		imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex
	);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
	    recreateSwapChain();
	    return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
	    throw std::runtime_error("failed to acquire swap chain image!");
	}

	updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(_device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(
		graphicsQueue, 1, &submitInfo,
		inFlightFences[currentFrame]) != VK_SUCCESS
	) {
	    throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
	    framebufferResized = false;
	    recreateSwapChain();
	} else if (result != VK_SUCCESS) {
	    throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


bool RenderManager::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(_device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(_device, stagingBuffer, nullptr);
    vkFreeMemory(_device, stagingBufferMemory, nullptr);

    return (SUCCESS);
}

bool RenderManager::createUniformBuffers() {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(swapChainImages.size());
	uniformBuffersMemory.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++) {
	    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}

	return (SUCCESS);
}

bool RenderManager::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.pImmutableSamplers = nullptr;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
	    throw std::runtime_error("failed to create descriptor set layout!");
	}

	return (SUCCESS);
}

void RenderManager::generateMipmaps(
	VkImage image, VkFormat imageFormat,
	int32_t texWidth, int32_t texHeight,
	uint32_t mipLevels
	) {
	
     // Check if image format supports linear blitting
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties);

        if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
            throw std::runtime_error("texture image format does not support linear blitting!");
        }

        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        int32_t mipWidth = texWidth;
        int32_t mipHeight = texHeight;

        for (uint32_t i = 1; i < mipLevels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            VkImageBlit blit = {};
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer,
                image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
	}
         barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
            0, nullptr,
            0, nullptr,
            1, &barrier);   
    endSingleTimeCommands(commandBuffer);
}

bool RenderManager::createDescriptorPool() {
    std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

    if (vkCreateDescriptorPool(_device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

	return (SUCCESS);
}

bool RenderManager::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat RenderManager::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
	for (VkFormat format : candidates) {
    	VkFormatProperties props;
    	vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
		    return format;
		} else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
		    return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat RenderManager::findDepthFormat() {
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

void RenderManager::createColorResources() {
    VkFormat colorFormat = swapChainImageFormat;

    create_image(swapChainExtent.width, swapChainExtent.height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
    colorImageView = create_image_view(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

    transitionImageLayout(colorImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1);
}

bool RenderManager::createDepthResources() {
	VkFormat depthFormat = findDepthFormat();
	create_image(
		swapChainExtent.width, swapChainExtent.height,
		1, msaaSamples, depthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		depthImage, depthImageMemory
	);
	depthImageView = create_image_view(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
	transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
	return SUCCESS;
}

VkSampleCountFlagBits RenderManager::getMaxUsableSampleCount() {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}

bool RenderManager::createTextureSampler() {
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 1;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0; // Optional
    samplerInfo.maxLod = static_cast<float>(mipLevels);
    samplerInfo.mipLodBias = 0; // Optional

	if (vkCreateSampler(_device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
	    throw std::runtime_error("failed to create texture sampler!");
	}

    return true;
}

// TODO : create queue of DESTRUCTION
bool    RenderManager::_Destroy() {
	cleanupSwapChain();
       
	vkDestroySampler(_device, textureSampler, nullptr);
    vkDestroyImageView(_device, textureImageView, nullptr);
    destroy_texture_images();
    vkFreeMemory(_device, textureImageMemory, nullptr);
    vkDestroyDescriptorSetLayout(_device, descriptorSetLayout, nullptr);

	vkDestroyBuffer(_device, indexBuffer, nullptr);
	vkFreeMemory(_device, indexBufferMemory, nullptr);

	vkDestroyBuffer(_device, vertexBuffer, nullptr);
	vkFreeMemory(_device, vertexBufferMemory, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
	    vkDestroySemaphore(_device, renderFinishedSemaphores[i], nullptr);
	    vkDestroySemaphore(_device, imageAvailableSemaphores[i], nullptr);
	    vkDestroyFence(_device, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(_device, commandPool, nullptr);

	vkDestroyDevice(_device, nullptr);

	if (enableValidationLayers) {
	    DestroyDebugUtilsMessengerEXT(_instance, debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(_instance, surface, nullptr);
	vkDestroyInstance(_instance, nullptr);

	return (SUCCESS);
}

bool RenderManager::initialize_vulkan(const char *title, const char *name) {
	if (createInstance(title, name) == FAILURE)	// instance
		return (FAILURE);
	if (setupDebugCallback() == FAILURE)
		return (FAILURE);	// callback
	if (createSurface() == FAILURE)
		return (FAILURE);	// surface
	if (pickPhysicalDevice() == FAILURE)
		return (FAILURE);	// physical device
	if (createLogicalDevice() == FAILURE)
		return (FAILURE);	// logical device
}

bool    RenderManager::_Init(const char *title, const char *name) {
    initialize_vulkan(title, name);
	initialize_swap_chain();

	if (createRenderPass() == FAILURE)
		return (FAILURE);	// render pass
	if (createDescriptorSetLayout() == FAILURE)
		return (FAILURE);
	if (createGraphicsPipeline() == FAILURE)
		return (FAILURE); // pipeline
	if (createCommandPool() == FAILURE)
		return (FAILURE);
    createColorResources();
 	if (createDepthResources() == FAILURE)
 		return (FAILURE);
	if (createFrameBuffers() == FAILURE)
		return (FAILURE);	// frame buffers

	create_texture_images();

    create_texture_image_views();
    if (createTextureSampler() == FAILURE)
    	return (FAILURE);
	if (createVertexBuffer() == FAILURE)
		return (FAILURE);
	if (createIndexBuffer() == FAILURE)
		return (FAILURE);
	if (createUniformBuffers() == FAILURE)
		return (FAILURE);
	if (createDescriptorPool() == FAILURE)
		return (FAILURE);
	if (createDescriptorSets() == FAILURE)
		return (FAILURE);
	if (createCommandBuffers() == FAILURE)
		return (FAILURE);	// command buffers
	if (createSyncObjects() == FAILURE)
		return (FAILURE);

//	createSemaphores(); // semaphores
	// init ...
	// vertex buffers
	// sync objects
	return (true);
}

bool    RenderManager::_Run() {
	// moved to engine
	return SUCCESS;
}

bool	RenderManager::_Test() {
	printf("entering main test loop\n");
	_Run();
	return (SUCCESS);
}
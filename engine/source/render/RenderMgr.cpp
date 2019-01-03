#include "../../includes/render/RenderMgr.hpp"

/*
 * BIG TODO: split these functions into appropriate files
 * AND KEEP THEM THERE!!!
 */

bool    RenderMgr::_Init(const char *title, const char *name) {
	// TODO : make dynamic
	if (getScreenRes() == FAILURE)
		return (FAILURE);
	if (initWindow(name) == FAILURE)
		return (FAILURE);
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
	if (createSwapChain() == FAILURE)
		return (FAILURE);	// swap chain
	if (createImageViews() == FAILURE)
		return (FAILURE);	// image views
	if (createRenderPass() == FAILURE)
		return (FAILURE);	// render pass
	if (createGraphicsPipeline() == FAILURE)
		return (FAILURE); // pipeline
	createFramebuffers();	// frame buffers
	createCommandPool();	// command pool
	createCommandBuffers();	// command buffers
//	createSemaphores(); // semaphores
	// init ...
	// vertex buffers
	// sync objects
	return (true);
}

std::vector<char>				RenderMgr::readFile(const std::string& filename) {
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

//void							RenderMgr::createSemaphores() {
//	VkSemaphoreCreateInfo semaphoreInfo = {};
//
//	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
//	if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, imageAvailableSemaphores) != VK_SUCCESS ||
//		vkCreateSemaphore(_device, &semaphoreInfo, nullptr, renderFinishedSemaphores) != VK_SUCCESS) {
//		throw std::runtime_error("failed to create semaphores!");
//	}
//}

bool							RenderMgr::createCommandBuffers() {
	commandBuffers.resize(swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo = {};
	size_t	i;

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

	if (vkAllocateCommandBuffers(_device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	for (i = 0; i < commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = swapChainExtent;

		VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
	return (SUCCESS);
}

bool							RenderMgr::createCommandPool() {
	printf("creating command pools\n");
	QueueFamilyIndices			qfi = findQueueFamilies(physicalDevice);
	VkCommandPoolCreateInfo		poolInfo = {};

	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = qfi.graphicsFamily.second;
	poolInfo.flags = 0; // Optional
	if (vkCreateCommandPool(_device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
	return (SUCCESS);
}

bool							RenderMgr::createFramebuffers() {
	printf("creating frame buffers\n");

	swapChainFramebuffers.resize(swapChainImgViews.size());
	for (size_t i = 0; i < swapChainImgViews.size(); i++) {
		VkImageView attachments[] = {
				swapChainImgViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
	return (SUCCESS);
}

bool							RenderMgr::createRenderPass() {
	printf("creating render pass\n");
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(_device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
	return (!!renderPass);
}

bool							RenderMgr::createGraphicsPipeline() {
	/*
	 * TODO : break this into different functions
	 */
	/*
	 * TODO : dynamic path
	 */
	auto vertShaderCode = readFile("/home/samurai/stunning-system/engine/source/render/shaders/vert.spv");
	auto fragShaderCode = readFile("/home/samurai/stunning-system/engine/source/render/shaders/frag.spv");
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	VkViewport viewport = {};
	VkRect2D scissor = {};
	VkPipelineViewportStateCreateInfo viewportState = {};
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
//	VkDynamicState dynamicStates[] = {
//			VK_DYNAMIC_STATE_VIEWPORT,
//			VK_DYNAMIC_STATE_LINE_WIDTH
//	};
//	VkPipelineDynamicStateCreateInfo dynamicState = {};

	vertShaderModule = createShaderModule(vertShaderCode);
	fragShaderModule = createShaderModule(fragShaderCode);

	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {
		vertShaderStageInfo,
		fragShaderStageInfo
	};

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	/*
	 * TODO : dynamic, obvs
	 */

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapChainExtent.width;
	viewport.height = (float) swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;

	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

//	printf("where we at\n"); // TODO : remove

//	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//	dynamicState.dynamicStateCount = 2;
//	dynamicState.pDynamicStates = dynamicStates;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	printf("creating graphics pipeline layout\n");

	if (vkCreatePipelineLayout(
			_device, &pipelineLayoutInfo,
			nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo;
	memset(&pipelineInfo, 0, sizeof(pipelineInfo));
	
	VkPipelineCacheCreateInfo pipelineCache;
	memset(&pipelineCache, 0, sizeof(pipelineCache));

	pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    if (vkCreatePipelineCache(_device, &pipelineCache, NULL, &_pipelineCache) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline cache!");

	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	printf("creating graphics pipelines\n");
	printf("device initialized?               | %s%s%s\n",
		_device?TRUE_STR:FALSE_STR,
		_device?"TRUE":"FALSE",
		ANSI_COLOR_RESET);

	try {
		if (vkCreateGraphicsPipelines(
				_device, _pipelineCache,
				1, &pipelineInfo, nullptr,
				&graphicsPipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}
	} catch (std::exception& e) {
    	std::cerr << e.what() << std::endl;
    	return (FAILURE);
	}

	printf("%sgraphics pipeline created%s\n", TRUE_STR, ANSI_COLOR_RESET);

//	printf("where we at\n"); // TODO : remove


	/*
	 *
	 */

	vkDestroyShaderModule(_device, fragShaderModule, nullptr);
	vkDestroyShaderModule(_device, vertShaderModule, nullptr);

	return (!!graphicsPipeline);
}

QueueFamilyIndices				RenderMgr::findQueueFamilies(VkPhysicalDevice device) {
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
			// printf("indices complete with graphics family iter %d\n", i);
			break;
		}
		++i;
	}

	return indices;
}
																// TODO : more verbose var names
bool									RenderMgr::createInstance(const char *title, const char *name) {
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = title;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = name;
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
	return (!!_instance);
}


bool	RenderMgr::initWindow(const char *title) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	printf("creating draw window\n");
	return ((_window = glfwCreateWindow(
		_screen->width - 100,
		_screen->height - 100,
		title, nullptr, nullptr))
	);
}

bool    RenderMgr::_Run() {
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
		drawFrame();
	}
	return (SUCCESS);
}

bool    RenderMgr::_Destroy() {
	// TODO : create queue of destruction ... bitchin'
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(_instance, callback, nullptr);
	}
	size_t i;
	for (i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(_device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_device, inFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(_device, commandPool, nullptr);
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(_device, framebuffer, nullptr);
	}
	vkDestroyPipeline(_device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(_device, pipelineLayout, nullptr);
	vkDestroyRenderPass(_device, renderPass, nullptr);
	for (auto imageView : swapChainImgViews) {
		vkDestroyImageView(_device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(_device, swapChain, nullptr);
	vkDestroyDevice(_device, nullptr);
	vkDestroySurfaceKHR(_instance, surface, nullptr);
	vkDestroyInstance(_instance, nullptr);
	glfwDestroyWindow(_window);
	glfwTerminate();
	return (true);
}

bool	RenderMgr::getScreenRes() {
//	// Get a handle to the desktop window
//	const HWND hDesktop;
//
//	hDesktop = GetDesktopWindow();
//	// Get the size of screen to the variable desktop
//	GetWindowRect(hDesktop, &desktop);
//	// The top left corner will have coordinates (0,0)
//	// and the bottom right corner will have coordinates
//	// (horizontal, vertical)
	_display = XOpenDisplay(NULL);
	_screen = DefaultScreenOfDisplay(_display);
	return (true);
}

void	RenderMgr::drawFrame() {
	printf("drawing frame\n");
	vkWaitForFences(_device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
	vkResetFences(_device, 1, &inFlightFences[currentFrame]);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(
			_device, swapChain,
			std::numeric_limits<uint64_t>::max(),
			imageAvailableSemaphores[currentFrame],
			VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {
			imageAvailableSemaphores[currentFrame]
	};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
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

	vkQueuePresentKHR(presentQueue, &presentInfo);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

bool	RenderMgr::_Test() {
	printf("entering main test loop\n");
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();
		drawFrame();
	}
	return (SUCCESS);
}
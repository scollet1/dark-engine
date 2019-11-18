#ifndef RENDERMGR_HPP
#define RENDERMGR_HPP

#include "../dark.hpp"
#include "../vertex/vertex.hpp"
#include "../texture/texture.hpp"
#include "../Assets/Object.hpp"
#include "../Engine/DarkEngine.hpp"

/*
 * TODO : conditional platform checks
 */

#ifndef TEST_VERT_FILE
#define TEST_VERT_FILE "/home/samurai/dark-engine/engine/source/Render/shaders/vert.spv"
#endif

#ifndef TEST_FRAG_FILE
#define TEST_FRAG_FILE "/home/samurai/dark-engine/engine/source/Render/shaders/frag.spv"
#endif

//#include "wtypes.h"
#include <chrono>

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct	QueueFamilyIndices {
	std::pair<uint32_t, bool> graphicsFamily;
	std::pair<uint32_t, bool> presentFamily;

	bool isComplete() {
		return (graphicsFamily.second && presentFamily.second);
	}
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class DarkEngine;

class RenderManager {
public:
	RenderManager() {};
	RenderManager(DarkEngine *dark_engine) : dark_engine(dark_engine) {};
	~RenderManager() {};
	RenderManager(const RenderManager &rhs) {*this = rhs;}
	RenderManager& operator=(const RenderManager &rhs) {
		*this = rhs; return (*this);
	}

	bool    _Init(const char *title, const char *name);
	bool    _Run();
	bool    _Destroy();
	bool	_Test();

	bool	initWindow(const char *title);
	/*
	 * BIG TODO :
	 * vulkan instance layer
	 * init everything
	 * draw window
	 * repeat lol
	 * cleanup();
	 */
	// drawFrame();

	/* Getters */
	// TODO : preprocessor statements for debugging

	/* Setters */

	void draw_frame();
	void device_wait_idle();

private:
	// TODO : move to utils.cpp
	static std::vector<char>				readFile(const std::string& filename);
	bool initialize_swap_chain();
	bool initialize_vulkan(const char *title, const char *name);
	bool									createInstance(const char *title, const char *name);
	bool									createGraphicsPipeline();
	bool									createRenderPass();
	bool									createFrameBuffers();
	bool									createCommandPool();
	bool									createCommandBuffers();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	bool									createSurface();
	bool									createSwapChain();
	bool 									cleanupSwapChain();
	bool 									recreateSwapChain();
	VkSurfaceFormatKHR						chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	SwapChainSupportDetails					querySwapChainSupport(VkPhysicalDevice device);
	VkPresentModeKHR						chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D								chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void									createSemaphores();
	bool createUniformBuffers();
	bool createDescriptorPool();
	void updateUniformBuffer(uint32_t currentImage);
	VkShaderModule							createShaderModule(const std::vector<char>& code);
	bool createDescriptorSetLayout();
	bool createDescriptorSets();
	VkSampleCountFlagBits getMaxUsableSampleCount();
	void create_texture_image_views();
	bool create_texture_images();
	bool 									createIndexBuffer();
	bool									pickPhysicalDevice();
	int										rateDeviceSuitability(VkPhysicalDevice device);
	bool									isDeviceSuitable(VkPhysicalDevice device);
	bool									checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool									createLogicalDevice();
	bool 									createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	QueueFamilyIndices						findQueueFamilies(VkPhysicalDevice device);
	bool									checkValidationLayerSupport();
	std::vector<const char*>				getRequiredExtensions();
	bool 									createSyncObjects();
	bool									setupDebugCallback();
	bool 									createVertexBuffer();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	bool createDepthResources();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	VkCommandBuffer beginSingleTimeCommands();

	bool createTextureSampler();
	
	void create_swap_chain_image_views();
	void create_image_views();
	VkImage create_texture_image(Texture *texture);
	VkImageView create_texture_image_view(VkImage texture_image);
	VkImageView create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void create_image(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void destroy_texture_images();

	void compile_vertices(std::vector<Vertex> obj_vertices);
	void compile_indices(std::vector<uint32_t> obj_indices);
	bool compile_objects();

	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	bool hasStencilComponent(VkFormat format);

	void createColorResources();
	VkFormat findDepthFormat();

	bool copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	uint32_t 								findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
			);
	VkResult								CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pCallback
			);
	static void								DestroyDebugUtilsMessengerEXT(
			VkInstance instance,
			VkDebugUtilsMessengerEXT callback,
			const VkAllocationCallbacks* pAllocator
			);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	/* Platform specs */
//	RECT						_desktop;
	DarkEngine *dark_engine;

    VkInstance							_instance;
    VkDebugUtilsMessengerEXT 			callback;
    VkSurfaceKHR 						surface;

    VkPhysicalDevice 					physicalDevice = VK_NULL_HANDLE;
    VkDevice 							_device;
    VkQueue 							graphicsQueue;
    VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
    VkQueue 							presentQueue;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
    VkDebugUtilsMessengerEXT debugMessenger;
	std::vector<VkImage> texture_images;
	VkDeviceMemory textureImageMemory;

    VkSwapchainKHR 						swapChain;
    std::vector<VkImage> 				swapChainImages;
    VkFormat 							swapChainImageFormat;
    VkExtent2D 							swapChainExtent;
    std::vector<VkImageView> 			swapChainImageViews;
    std::vector<VkFramebuffer> 			swapChainFramebuffers;
    VkImage colorImage;
    
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;
    VkImageView textureImageView;
    std::vector<VkImageView> texture_image_views;
    uint32_t mipLevels;
	VkSampler textureSampler;

	std::vector<Vertex> vertices; // combine vertices
    std::vector<uint32_t> indices; // combine indices

	VkDeviceMemory vertexBufferMemory;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	VkDescriptorSetLayout descriptorSetLayout;
    VkRenderPass 						renderPass;
    VkPipelineLayout 					pipelineLayout;
    VkPipeline 							graphicsPipeline;

    VkCommandPool 						commandPool;
    std::vector<VkCommandBuffer> 		commandBuffers;
	VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkSemaphore> 			imageAvailableSemaphores;
    std::vector<VkSemaphore> 			renderFinishedSemaphores;
    std::vector<VkFence> 				inFlightFences;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    size_t 								currentFrame = 0;
    bool 								framebufferResized = false;

	/* Vulkan specs */
	// TODO : Specify members as class _vars

	// handles

    VkPipelineCache						_pipelineCache;

	// NOTE : I no longer know what this means
	// TODO : topo image object wrapper for Vk ???

	const std::vector<const char*> validationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
};

#endif // RENDERMGR_H
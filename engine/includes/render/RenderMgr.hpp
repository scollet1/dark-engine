#include "../topo.hpp"
#include <optional>
#include <boost/optional.hpp>
/*
 * TODO : conditional platform checks bro
 */
//#include "wtypes.h"
#include <X11/Xlib.h>
#include <chrono>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif 

#include <stb_image.h>

#ifndef SCRNWOFFS
#define SCRNWOFFS -100
#endif

#ifndef SCRNHOFFS
#define SCRNHOFFS - 100
#endif

#ifndef TEST_VERT_FILE
#define TEST_VERT_FILE "/home/samurai/dark-engine/engine/source/render/shaders/vert.spv"
#endif

#ifndef TEST_FRAG_FILE
#define TEST_FRAG_FILE "/home/samurai/dark-engine/engine/source/render/shaders/frag.spv"
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

#include <GLFW/glfw3.h>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct	QueueFamilyIndices {
	std::pair<uint32_t,bool> graphicsFamily;
	std::pair<uint32_t,bool> presentFamily;

	bool isComplete() {
		// printf("returning completed indices\n");
		printf("graphics family is complete?      | %s%s%s\n",
		graphicsFamily.second? TRUE_STR:FALSE_STR,	
		graphicsFamily.second? "TRUE":"FALSE",
		ANSI_COLOR_RESET);
		printf("presentation family is complete?  | %s%s%s\n",
		presentFamily.second? TRUE_STR:FALSE_STR,
		presentFamily.second? "TRUE":"FALSE",
		ANSI_COLOR_RESET);
		// return (graphicsFamily.has_value() && presentFamily.has_value());
		return (graphicsFamily.second && presentFamily.second);
	}
};

/*
TODO : move this shit into another file ok
Vertex.h/cpp
*/
#include <glm/glm.hpp>

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		// BIG TODO : needs reworking dood
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return (bindingDescription);
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

class RenderMgr {
public:
	RenderMgr() {};
	~RenderMgr() {};
	RenderMgr(const RenderMgr &rhs) {*this = rhs;};
	RenderMgr& operator=(const RenderMgr &rhs) {
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
	bool									getScreenRes();
	// TODO : preprocessor statements for debugging
	uint32_t								getScreenWidth() {
			// return (800);}
		return (_screen->width + SCRNWOFFS);}
	uint32_t								getScreenHeight() {
			// return (400);}
		return (_screen->height + SCRNHOFFS);}

	/* Setters */
	void									setFrameBufferResized(bool stat) {
		framebufferResized = stat;
	}

private:
	// TODO : move to utils.cpp
	static std::vector<char>				readFile(const std::string& filename);

	void									drawFrame();

	bool									createInstance(const char *title, const char *name);
	bool									createGraphicsPipeline();
	bool									createRenderPass();
	bool									createFramebuffers();
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
	bool									createImageViews();
	void									createSemaphores();
	static void								framebufferResizeCallback(GLFWwindow* window, int width, int height);
	bool createUniformBuffers();
	bool createDescriptorPool();
	void updateUniformBuffer(uint32_t currentImage);
	VkShaderModule							createShaderModule(const std::vector<char>& code);
	bool createDescriptorSetLayout();
	bool createDescriptorSets();
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
void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
VkCommandBuffer beginSingleTimeCommands();
bool createTextureImageView();
bool createTextureSampler();
	bool createTextureImage();
VkImageView createImageView(VkImage image, VkFormat format);
void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	bool  									copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	uint32_t 								findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static VKAPI_ATTR VkBool32 VKAPI_CALL 	debugCallback(
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


	/* Platform specs */
//	RECT						_desktop;
	Display								*_display;
	Screen								*_screen;
	int									_width;
	int									_height;

	/* GLFW specs */
	GLFWwindow* 						_window;

    VkInstance							_instance;
    VkDebugUtilsMessengerEXT 			callback;
    VkSurfaceKHR 						surface;

    // BIG TODO : MAJOR NEEDS BIG PIPELINE TO INITIALIZE AND PROCESS;;;
    const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

    VkPhysicalDevice 					physicalDevice = VK_NULL_HANDLE;
    VkDevice 							_device;
    VkQueue 							graphicsQueue;
    VkQueue 							presentQueue;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
    VkDebugUtilsMessengerEXT debugMessenger;
	VkDeviceMemory vertexBufferMemory;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
    VkSwapchainKHR 						swapChain;
    std::vector<VkImage> 				swapChainImages;
    VkFormat 							swapChainImageFormat;
    VkExtent2D 							swapChainExtent;
    std::vector<VkImageView> 			swapChainImageViews;
    std::vector<VkFramebuffer> 			swapChainFramebuffers;
    VkImageView textureImageView;
VkSampler textureSampler;

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
    size_t 								currentFrame = 0;
    bool 								framebufferResized = false;

	/* Vulkan specs */
	// TODO : Specify members as class _vars

	// handles

    VkPipelineCache						_pipelineCache;

	// NOTE : I no longer know what this means
	// TODO : topo image object wrapper for Vk ???

	const std::vector<const char*>		validationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
	};
	const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

};
#include "../topo.hpp"
#include <optional>
#include <boost/optional.hpp>
/*
 * TODO : conditional platform checks bro
 */
//#include "wtypes.h"
#include <X11/Xlib.h>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

#include <GLFW/glfw3.h>

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
		return (graphicsFamily.second && presentFamily.second);
	}
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
	uint32_t								getScreenWidth() {return (_screen->width);}
	uint32_t								getScreenHeight() {return (_screen->height);}

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

	bool									createSurface();
	bool									createSwapChain();
	VkSurfaceFormatKHR						chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	SwapChainSupportDetails					querySwapChainSupport(VkPhysicalDevice device);
	VkPresentModeKHR						chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D								chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool									createImageViews();
	void									createSemaphores();

	VkShaderModule							createShaderModule(const std::vector<char>& code);

	bool									pickPhysicalDevice();
	int										rateDeviceSuitability(VkPhysicalDevice device);
	bool									isDeviceSuitable(VkPhysicalDevice device);
	bool									checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool									createLogicalDevice();

	QueueFamilyIndices						findQueueFamilies(VkPhysicalDevice device);
	bool									checkValidationLayerSupport();
	std::vector<const char*>				getRequiredExtensions();

	bool									setupDebugCallback();
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
	GLFWwindow							*_window;

	/* Vulkan specs */
	// TODO : Specify members as class _vars
	VkPhysicalDevice					physicalDevice;
	VkDevice							_device;
	VkInstance							_instance;
	VkSurfaceKHR						surface;

	VkSwapchainKHR						swapChain;
	VkExtent2D							swapChainExtent;
	VkFormat							swapChainImageFormat;

	// handles
	std::vector<VkImage>				swap_chain;
	std::vector<VkImage>				swapChainImages;
	std::vector<VkImageView>			swapChainImgViews;
	std::vector<VkFramebuffer>			swapChainFramebuffers;

	VkRenderPass						renderPass;
	VkPipelineLayout					pipelineLayout;
	VkCommandPool						commandPool;
	std::vector<VkCommandBuffer>		commandBuffers; // handle
    VkPipelineCache						_pipelineCache;
	VkPipeline							graphicsPipeline;

	std::vector<VkSemaphore>			imageAvailableSemaphores;
	std::vector<VkSemaphore>			renderFinishedSemaphores;
	std::vector<VkFence>				inFlightFences;

	VkQueue								presentQueue;
	VkQueue								graphicsQueue;

	VkDebugUtilsMessengerEXT			callback;

	size_t								currentFrame = 0;

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
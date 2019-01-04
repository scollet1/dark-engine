#include "../../includes/render/RenderMgr.hpp"

VkResult RenderMgr::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void									RenderMgr::DestroyDebugUtilsMessengerEXT(
	VkInstance instance,
	VkDebugUtilsMessengerEXT callback,	
	const VkAllocationCallbacks* pAllocator) {
	
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
	vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL RenderMgr::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	std::cerr << pUserData << std::endl;
	std::cerr << messageSeverity << std::endl;
	std::cerr << messageType << std::endl;
	return VK_FALSE;
}

bool									RenderMgr::setupDebugCallback() {
	if (!enableValidationLayers) return (FAILURE);

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = debugCallback;

	if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}
	return (SUCCESS);
}
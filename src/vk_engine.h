// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <memory>
#include <vk_types.h>
#include "deletion_queue.h"
#include "vk_descriptors.h"
#include "vk_loader.h"

constexpr unsigned int FRAME_OVERLAP = 2;

struct FrameData {
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	VkSemaphore _swapchainSemaphore, _renderSemaphore;
	VkFence _renderFence;
	DeletionQueue _deletionQueue;
};

struct ComputePushConstants {
	glm::vec4 data1;
	glm::vec4 data2;
	glm::vec4 data3;
	glm::vec4 data4;
};

struct ComputeEffect {
	const char* name;
	VkPipeline pipeline;
	VkPipelineLayout layout;

	ComputePushConstants data;
};

class VulkanEngine {
public:
	DescriptorAllocator globalDescriptorAllocator;

	FrameData _frames[FRAME_OVERLAP];
	FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };

	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	DeletionQueue _mainDeletionQueue;

	bool _isInitialized{ false };
	int _frameNumber {0};
	bool stop_rendering{ false };
	VkExtent2D _windowExtent{ 1700 , 900 };

	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debug_messenger;
	VkPhysicalDevice _chosenGPU;
	VkDevice _device;
	VkSurfaceKHR _surface;

	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

	VmaAllocator _allocator;

	AllocatedImage _drawImage;
	VkExtent2D _drawExtent;

	VkDescriptorSet _drawImageDescriptors;
	VkDescriptorSetLayout _drawImageDescriptorLayout;

	VkPipeline _gradientPipeline;
	VkPipelineLayout _gradientPipelineLayout;

	// Immediate submit structures
	VkFence _immFence;
	VkCommandBuffer _immCommandBuffer;
	VkCommandPool _immCommandPool;

	VkPipelineLayout _trianglePipelineLayout;
	VkPipeline _trianglePipeline;

	VkPipelineLayout _meshPipelineLayout;
	VkPipeline _meshPipeline;

	std::vector<std::shared_ptr<MeshAsset>> testMeshes;

	GPUMeshBuffers rectangle;

	std::vector<ComputeEffect> backgroundEffects;
	int currentBackgroundEffect{ 0 };

	struct SDL_Window* _window{ nullptr };

	static VulkanEngine& Get();

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();

	GPUMeshBuffers uploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

private:
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_sync_structures();
	void init_descriptors();
	void init_pipelines();
	void init_background_pipelines();
	void init_imgui();
	void init_triangle_pipeline();
	void init_mesh_pipeline();
	void init_default_data();

	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();
	void draw_background(VkCommandBuffer cmd);
	void draw_imgui(VkCommandBuffer cmd, VkImageView targetImageView);
	void draw_geometry(VkCommandBuffer cmd);

	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
	void destroy_buffer(const AllocatedBuffer& buffer);


	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
};

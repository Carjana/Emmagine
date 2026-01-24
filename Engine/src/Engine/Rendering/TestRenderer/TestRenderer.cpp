#include "TestRenderer.h"

#include "SDL3/SDL_assert.h"
#include "SDL3/SDL_filesystem.h"


namespace Emma
{
	SDL_GPUShader* LoadShader(SDL_GPUDevice* device, const char* shaderFilename, Uint32 samplerCount,
		Uint32 uniformBufferCount, Uint32 storageBufferCount, Uint32 storageTextureCount)
	{
	// Auto-detect the shader stage from the file name for convenience
	SDL_GPUShaderStage stage;
	if (SDL_strstr(shaderFilename, ".vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (SDL_strstr(shaderFilename, ".frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		SDL_Log("Invalid shader stage!");
		return nullptr;
	}

	char fullPath[256];
	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char *entrypoint;

	const char *BasePath = "D:/OtherProjects/Emmagine/Engine/";

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/SPIRV/%s.spv", BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	} else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/MSL/%s.msl", BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main0";
	} else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/DXIL/%s.dxil", BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	} else {
		SDL_Log("%s", "Unrecognized backend shader format!");
		return nullptr;
	}

	size_t codeSize;
	void* code = SDL_LoadFile(fullPath, &codeSize);
	if (code == nullptr)
	{
		SDL_Log("Failed to load shader from disk! %s", fullPath);
		return nullptr;
	}

	SDL_GPUShaderCreateInfo shaderInfo{};
		shaderInfo.code = (Uint8 *)code;
		shaderInfo.code_size = codeSize;
		shaderInfo.entrypoint = entrypoint;
		shaderInfo.format = format;
		shaderInfo.stage = stage;
		shaderInfo.num_samplers = samplerCount;
		shaderInfo.num_uniform_buffers = uniformBufferCount;
		shaderInfo.num_storage_buffers = storageBufferCount;
		shaderInfo.num_storage_textures = storageTextureCount;

	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == nullptr)
	{
		SDL_Log("Failed to create shader!");
		SDL_free(code);
		return nullptr;
	}

	SDL_free(code);
	return shader;
}

	SDL_Surface* LoadSDLImage(const char* imageFilename, int desiredChannels)
	{
		char fullPath[256];
		SDL_Surface *result;
		SDL_PixelFormat format;

		const char *BasePath = "D:/OtherProjects/Emmagine/Engine/";

		SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Images/%s", BasePath, imageFilename);

		result = SDL_LoadBMP(fullPath);
		if (result == nullptr)
		{
			SDL_Log("Failed to load BMP: %s", SDL_GetError());
			return nullptr;
		}

		if (desiredChannels == 4)
		{
			format = SDL_PIXELFORMAT_ABGR8888;
		}
		else
		{
			SDL_assert(!"Unexpected desiredChannels");
			SDL_DestroySurface(result);
			return nullptr;
		}
		if (result->format != format)
		{
			SDL_Surface *next = SDL_ConvertSurface(result, format);
			SDL_DestroySurface(result);
			result = next;
		}

		return result;
	}

	void TestRenderer::Init()
	{
		Instance = EmmaApplication::GetInstance();
		Context = Instance->mainWindow->Context;

		SDL_GPUShader *vertexShader = LoadShader(Context->Device, "TexturedQuad.vert",
		//SDL_GPUShader *vertexShader = LoadShader(Context->Device, "PositionColor.vert",
			0,0,0,0);
		CORE_ASSERT_MESSAGE(vertexShader, SDL_GetError());

		//SDL_GPUShader *fragmentShader = LoadShader(Context->Device, "SolidColor.frag",
		SDL_GPUShader *fragmentShader = LoadShader(Context->Device, "TexturedQuad.frag",
			1,0,0,0);
		CORE_ASSERT_MESSAGE(fragmentShader, SDL_GetError());

		SDL_Surface *imageData = LoadSDLImage("ravioli.bmp", 4);
		CORE_ASSERT_MESSAGE(imageData, SDL_GetError());

		// pipeline
		SDL_GPUColorTargetDescription colorTargetDescription = {};
		colorTargetDescription.format = SDL_GetGPUSwapchainTextureFormat(Context->Device, Context->Window);
		SDL_GPUColorTargetDescription colorTargetDescriptions[] = {colorTargetDescription};

		SDL_GPUGraphicsPipelineTargetInfo pipelineTargetInfo = {};
		pipelineTargetInfo.color_target_descriptions = colorTargetDescriptions;
		pipelineTargetInfo.num_color_targets = 1;

		SDL_GPUVertexAttribute vertexAttributes[] =
		{
		{	0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, 0},
		{1, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,  sizeof(vec3)}
		};

		SDL_GPUVertexBufferDescription vertexBufferDescriptions[] =
		{
		{0, sizeof(TempVertex), SDL_GPU_VERTEXINPUTRATE_VERTEX}
		};

		SDL_GPUVertexInputState vertexInputState = {};
		vertexInputState.vertex_attributes = vertexAttributes;
		vertexInputState.num_vertex_attributes = ArrayCount(vertexAttributes);
		vertexInputState.vertex_buffer_descriptions = vertexBufferDescriptions;
		vertexInputState.num_vertex_buffers = ArrayCount(vertexBufferDescriptions);

		SDL_GPUGraphicsPipelineCreateInfo pipelineInfo {};
		pipelineInfo.vertex_shader = vertexShader;
		pipelineInfo.fragment_shader = fragmentShader;
		pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

		pipelineInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
		pipelineInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;

		pipelineInfo.target_info = pipelineTargetInfo;

		pipelineInfo.vertex_input_state = vertexInputState;

		Pipeline = SDL_CreateGPUGraphicsPipeline(Instance->mainWindow->Context->Device, &pipelineInfo);
		CORE_ASSERT_MESSAGE(Pipeline, SDL_GetError());

		SDL_ReleaseGPUShader(Context->Device, vertexShader);
		SDL_ReleaseGPUShader(Context->Device, fragmentShader);

		// Sampler

		// PointClamp
		SDL_GPUSamplerCreateInfo pointClampInfo = {};
		pointClampInfo.min_filter = SDL_GPU_FILTER_NEAREST;
		pointClampInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
		pointClampInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
		pointClampInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		pointClampInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		pointClampInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

		// PointWrap
		SDL_GPUSamplerCreateInfo pointWrapInfo = {};
		pointWrapInfo.min_filter = SDL_GPU_FILTER_NEAREST;
		pointWrapInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
		pointWrapInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
		pointWrapInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
		pointWrapInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
		pointWrapInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;

		// LinearClamp
		SDL_GPUSamplerCreateInfo linearClampInfo = {};
		linearClampInfo.min_filter = SDL_GPU_FILTER_LINEAR;
		linearClampInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
		linearClampInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
		linearClampInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		linearClampInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		linearClampInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

		// Linear Wrap
		SDL_GPUSamplerCreateInfo linearWrapInfo = {};
		linearWrapInfo.min_filter = SDL_GPU_FILTER_LINEAR;
		linearWrapInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
		linearWrapInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
		linearWrapInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
		linearWrapInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
		linearWrapInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;

		// AnisotropicClamp
		SDL_GPUSamplerCreateInfo anisotropicClampInfo = {};
		anisotropicClampInfo.min_filter = SDL_GPU_FILTER_LINEAR;
		anisotropicClampInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
		anisotropicClampInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
		anisotropicClampInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		anisotropicClampInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		anisotropicClampInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		anisotropicClampInfo.enable_anisotropy = true;
		anisotropicClampInfo.max_anisotropy = 4;

		// AnisotropicWrap
		SDL_GPUSamplerCreateInfo anisotropicWrapInfo = {};
		anisotropicWrapInfo.min_filter = SDL_GPU_FILTER_LINEAR;
		anisotropicWrapInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
		anisotropicWrapInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
		anisotropicWrapInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
		anisotropicWrapInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
		anisotropicWrapInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
		anisotropicWrapInfo.enable_anisotropy = true;
		anisotropicWrapInfo.max_anisotropy = 4;

		Samplers[0] = SDL_CreateGPUSampler(Context->Device, &pointClampInfo);
		Samplers[1] = SDL_CreateGPUSampler(Context->Device, &pointWrapInfo);
		Samplers[2] = SDL_CreateGPUSampler(Context->Device, &linearClampInfo);
		Samplers[3] = SDL_CreateGPUSampler(Context->Device, &linearWrapInfo);
		Samplers[4] = SDL_CreateGPUSampler(Context->Device, &anisotropicClampInfo);
		Samplers[5] = SDL_CreateGPUSampler(Context->Device, &anisotropicWrapInfo);

		SDL_GPUTextureCreateInfo textureCreateInfo {};
		textureCreateInfo.type = SDL_GPU_TEXTURETYPE_2D;
		textureCreateInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
		textureCreateInfo.width = imageData->w;
		textureCreateInfo.height = imageData->h;
		textureCreateInfo.layer_count_or_depth = 1;
		textureCreateInfo.num_levels = 1;
		textureCreateInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
		Texture = SDL_CreateGPUTexture(Context->Device, &textureCreateInfo);
		CORE_ASSERT_MESSAGE(Texture, SDL_GetError());
		SDL_SetGPUTextureName(Context->Device, Texture, "RavioliTexture");

		// Vertices and Vertex Buffer

		TempVertex vertices[]
		{
			// triangle
			// {{-0.5f,-0.5f,0.0f}, {255,0,0,255}},
			// {{0.5f,-0.5f,0.0f}, {0,255,0,255}},
			// {{0.0f,0.5f,0.0f}, {0,0,255,255}},

			// rect with index buffer
			{{-0.5f, -0.5f, 0.0f}, {0.0f,2.0f}},	// botLeft
			{{-0.5f, 0.5f, 0.0f}, {0.0f,0.0f}},	// topLeft
			{{0.5f, 0.5f, 0.0f}, {1.0f,0.0f}},	// topRight
			{{0.5f, -0.5f, 0.0f}, {1.0f,2.0f}},	// botRight
		};

		Uint16 indices[]
		{
			0, 1, 2,	// first triangle
			2, 3, 0		// second triangle
		};

		// create vertex Buffer
		SDL_GPUBufferCreateInfo vertexBufferCreateInfo {};
		vertexBufferCreateInfo.size = sizeof(vertices);
		vertexBufferCreateInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;

		VertexBuffer = SDL_CreateGPUBuffer(Context->Device, &vertexBufferCreateInfo);
		CORE_ASSERT_MESSAGE(VertexBuffer, SDL_GetError());
		SDL_SetGPUBufferName(Context->Device, VertexBuffer, "VertexBuffer");

		// create index Buffer
		SDL_GPUBufferCreateInfo indexBufferCreateInfo {};
		indexBufferCreateInfo.size = sizeof(indices);
		indexBufferCreateInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;

		IndexBuffer = SDL_CreateGPUBuffer(Context->Device, &indexBufferCreateInfo);
		CORE_ASSERT_MESSAGE(IndexBuffer, SDL_GetError());
		SDL_SetGPUBufferName(Context->Device, IndexBuffer, "IndexBuffer");

		// transfer Buffer
		SDL_GPUTransferBufferCreateInfo vertexTransferBufferCreateInfo {};
		vertexTransferBufferCreateInfo.size = sizeof(vertices) + sizeof(indices);
		vertexTransferBufferCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;

		// create vertex transferbuffer and map memory to it
		SDL_GPUTransferBuffer *vertexTransferBuffer = SDL_CreateGPUTransferBuffer(Context->Device, &vertexTransferBufferCreateInfo);
		CORE_ASSERT_MESSAGE(vertexTransferBuffer, SDL_GetError());
		Uint8 *mappedMemory = (Uint8 *)SDL_MapGPUTransferBuffer(Context->Device, vertexTransferBuffer, false);
		CORE_ASSERT_MESSAGE(mappedMemory != nullptr, SDL_GetError());

		SDL_memcpy(mappedMemory, vertices, sizeof(vertices));
		// ReSharper disable once CppDFANullDereference
		SDL_memcpy(mappedMemory + sizeof(vertices), indices, sizeof(indices));

		SDL_UnmapGPUTransferBuffer(Context->Device, vertexTransferBuffer);

		// create texture transferbuffer and map memory to it
		SDL_GPUTransferBufferCreateInfo textureTransferBufferCreateInfo {};
		textureTransferBufferCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
		textureTransferBufferCreateInfo.size = imageData->w * imageData->h * sizeof(Uint32);

		SDL_GPUTransferBuffer *textureTransferBuffer = SDL_CreateGPUTransferBuffer(Context->Device, &textureTransferBufferCreateInfo);

		Uint8* textureTransferPtr = (Uint8 *)SDL_MapGPUTransferBuffer(Context->Device, textureTransferBuffer, false);

		SDL_memcpy(textureTransferPtr, imageData->pixels, imageData->w * imageData->h * sizeof(Uint32));

		SDL_UnmapGPUTransferBuffer(Context->Device, textureTransferBuffer);

		// upload buffer to GPU
		SDL_GPUCommandBuffer *cmdBuffer = SDL_AcquireGPUCommandBuffer(Context->Device);
		CORE_ASSERT_MESSAGE(cmdBuffer, SDL_GetError());

		SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuffer);

		SDL_GPUTransferBufferLocation vertexBufferLocation {};
		vertexBufferLocation.offset = 0;
		vertexBufferLocation.transfer_buffer = vertexTransferBuffer;

		SDL_GPUBufferRegion vertexGPUBufferRegion {};
		vertexGPUBufferRegion.buffer = VertexBuffer;
		vertexGPUBufferRegion.offset = 0;
		vertexGPUBufferRegion.size = sizeof(vertices);

		SDL_UploadToGPUBuffer(copyPass, &vertexBufferLocation, &vertexGPUBufferRegion, false);

		SDL_GPUTransferBufferLocation indexBufferLocation {};
		indexBufferLocation.transfer_buffer = vertexTransferBuffer;
		indexBufferLocation.offset = sizeof(vertices);

		SDL_GPUBufferRegion indexGPUBufferRegion {};
		indexGPUBufferRegion.buffer = IndexBuffer;
		indexGPUBufferRegion.offset = 0;
		indexGPUBufferRegion.size = sizeof(indices);

		SDL_UploadToGPUBuffer(copyPass, &indexBufferLocation, &indexGPUBufferRegion, false);

		SDL_GPUTextureTransferInfo textureSourceInfo {};
		textureSourceInfo.transfer_buffer = textureTransferBuffer;
		textureSourceInfo.offset = 0;

		SDL_GPUTextureRegion textureGPURegionInfo {};
		textureGPURegionInfo.texture = Texture;
		textureGPURegionInfo.w = imageData->w;
		textureGPURegionInfo.h = imageData->h;
		textureGPURegionInfo.d = 1;

		SDL_UploadToGPUTexture(copyPass, &textureSourceInfo, &textureGPURegionInfo, false);

		SDL_EndGPUCopyPass(copyPass);

		CORE_ASSERT_FUNC(SDL_SubmitGPUCommandBuffer(cmdBuffer), SDL_GetError());

		SDL_ReleaseGPUTransferBuffer(Context->Device, vertexTransferBuffer);
		SDL_DestroySurface(imageData);
		SDL_ReleaseGPUTransferBuffer(Context->Device, textureTransferBuffer);
	}

	void TestRenderer::Run()
	{
		SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(Context->Device);

		SDL_GPUTexture *swapChainTexture;
		CORE_ASSERT_FUNC(SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, Context->Window, &swapChainTexture, nullptr, nullptr), SDL_GetError());
		if (swapChainTexture)
		{
			SDL_GPUColorTargetInfo colorTargetInfo {};
			colorTargetInfo.texture = swapChainTexture;
			colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
			colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
			colorTargetInfo.clear_color = SDL_FColor{1.0f,0.0f,1.0f,1.0f};

			SDL_GPUColorTargetInfo colorTargetInfos[] =
			{
				colorTargetInfo
			};

			SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, colorTargetInfos, 1, nullptr);

			SDL_BindGPUGraphicsPipeline(renderPass, Pipeline);


			SDL_GPUBufferBinding bufferBindings[]
			{
				{VertexBuffer, 0}
			};

			SDL_BindGPUVertexBuffers(renderPass, 0, bufferBindings, ArrayCount(bufferBindings));

			SDL_GPUBufferBinding indexBufferBinding;
			indexBufferBinding.buffer = IndexBuffer;
			indexBufferBinding.offset = 0;

			SDL_BindGPUIndexBuffer(renderPass, &indexBufferBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

			SDL_GPUTextureSamplerBinding samplerBindings[]
			{
				{Texture, Samplers[1]}
			};

			SDL_BindGPUFragmentSamplers(renderPass, 0, samplerBindings, ArrayCount(samplerBindings));

			SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, 0, 0);

			SDL_EndGPURenderPass(renderPass);
		}

		SDL_SubmitGPUCommandBuffer(commandBuffer);
	}
}

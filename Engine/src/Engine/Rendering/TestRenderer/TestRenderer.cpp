#include "TestRenderer.h"

#include "Engine/Rendering/EmmaVertex.h"
#include "SDL3/SDL_assert.h"
#include "SDL3_shadercross/SDL_shadercross.h"


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
		SDL_ShaderCross_Init();

		Pipeline = new GraphicsPipeline();
		GraphicsPipelineCreateInfo info = {};
		ShaderLoadInfo vertexShaderLoadInfo = {};
		vertexShaderLoadInfo.entryPoint = "main";
		vertexShaderLoadInfo.absolutePath = "D:/OtherProjects/Emmagine/Engine/Content/Shaders/Source/TexturedQuad.vert.hlsl";
		vertexShaderLoadInfo.shaderStage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;

		ShaderLoadInfo fragmentShaderLoadInfo = {};
		fragmentShaderLoadInfo.entryPoint = "main";
		fragmentShaderLoadInfo.absolutePath = "D:/OtherProjects/Emmagine/Engine/Content/Shaders/Source/TexturedQuad.frag.hlsl";
		fragmentShaderLoadInfo.shaderStage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;

		info.VertexShaderInfo = vertexShaderLoadInfo;
		info.FragmentShaderInfo = fragmentShaderLoadInfo;
		info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(Context->Device, Context->Window);
		EmmaVertex::FillVertexBufferLayout(info.VertexBufferLayout);
		Pipeline->CreateGraphicsPipeline(Context, &info);


		SDL_Surface *imageData = LoadSDLImage("ravioli.bmp", 4);
		CORE_ASSERT_MESSAGE(imageData, SDL_GetError());
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

		EmmaVertex vertices[]
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

		VertexBuffer = new Emma::VertexBuffer();
		VertexBuffer->Create(Context, sizeof(vertices));
		VertexBuffer->UploadDataToBuffer(vertices, sizeof(vertices));

		IndexBuffer = new Emma::IndexBuffer();
		IndexBuffer->Create(Context, sizeof(indices));
		IndexBuffer->UploadDataToBuffer(indices, ArrayCount(indices));

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
			colorTargetInfo.clear_color = SDL_FColor{0.2f,0.1f,0.3f,1.0f};

			SDL_GPUColorTargetInfo colorTargetInfos[] =
			{
				colorTargetInfo
			};
			CORE_ASSERT_MESSAGE(Pipeline->numColorTargets == ArrayCount(colorTargetInfos), "This must match!");
			SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, colorTargetInfos, Pipeline->numColorTargets, nullptr);

			Pipeline->Bind(renderPass);

			VertexBuffer->Bind(renderPass);

			IndexBuffer->Bind(renderPass);

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

	void TestRenderer::Shutdown()
	{
		VertexBuffer->ReleaseBuffer();
		IndexBuffer->ReleaseBuffer();
		Pipeline->ReleasePipeline();

		for (SDL_GPUSampler *Sampler: Samplers)
		{
			SDL_ReleaseGPUSampler(Context->Device, Sampler);
		}

		SDL_ReleaseGPUTexture(Context->Device, Texture);
	}
}

#pragma once
#include "ShapeHandler.h"

class LineHandler : public ShapeHandler
{
public:
	LineHandler(const std::string lineHandler, const size_t maxLineCount);
	virtual ~LineHandler();
	void Shutdown();

	void BeginBatch();
	void EndBatch();
	void Flush();

	void DrawLine(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]);

	const Stats& GetStats();
	void ResetStats();
private:
	GLuint quadVA = 0;
	GLuint quadVB = 0;
	GLuint quadIB = 0;

	//Indices to draw when flushed
	const uint32_t IndexOffset = 2;
	const uint32_t VertexOffset = 2;

	const size_t MaxLineCount = 0;
	const size_t MaxVertexCount = 0;
	const size_t MaxIndexCount = 0;

	//Points beginning of the buffer. This is in CPU
	Vertex* quadBuffer = nullptr;
	//Where we are up to
	Vertex* quadBufferPtr = nullptr;

	//Use openGL code to Query the driver, find how many Texture slots the GPU have
	static const size_t MaxTextures = 32;

	//Which Texture slot holds to what OpenGL ID
	std::array<uint32_t, MaxTextures> TextureSlots;
	uint32_t TextureSlotIndex = 1;
	GLuint WhiteTexture = 0;
	uint32_t WhiteTextureSlot = 0;
	uint32_t indexCount = 0;

	struct Stats RenderStats;
};
#pragma once
#include "ShapeHandler.h"

class PointHandler : public ShapeHandler
{
public:
	PointHandler(const std::string quadShape, const size_t maxVertexCount);
	virtual ~PointHandler();

	void Shutdown();

	void BeginBatch();
	void EndBatch();
	void Flush();

	void PlotPoint(const glm::vec3& positions, const glm::vec4& color, const glm::vec2 TexIndices[]);

	const Stats& GetStats();
	void ResetStats();
private:
	GLuint quadVA = 0;
	GLuint quadVB = 0;

	//Vertices to draw when flushed
	const size_t MaxVertexCount = 0;

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
	uint32_t vertexCount = 0;

	struct Stats RenderStats;
};
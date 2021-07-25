#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GLCore.h>
#include <array>
#include <glad\glad.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexIndex;
};

struct Stats
{
	uint32_t DrawCount = 0;
	uint32_t QuadCount = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
};

class PointHandler
{
public:
	PointHandler(const std::string quadShape, const size_t maxVertexCount);
	~PointHandler();
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
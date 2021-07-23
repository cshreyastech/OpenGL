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

class QuadShape
{
public:
	QuadShape(const std::string quadShape, int* indicesSequence, const GLenum mode, const int indexOffset, const int vertexOffset, const size_t maxQuadCount);
	~QuadShape();

	void DrawQuad(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]);
	void Flush();

	const Stats& GetStats();
	void ResetStats();
public:
	const GLenum Mode;
	GLuint quadVA = 0;
	GLuint quadVB = 0;
	GLuint quadIB = 0;

	//Indices to draw when flushed
	const uint32_t IndexOffset = 0;
	const uint32_t VertexOffset = 0;

	const size_t MaxQuadCount = 0;
	const size_t MaxVertexCount = 0;
	const size_t MaxIndexCount;

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
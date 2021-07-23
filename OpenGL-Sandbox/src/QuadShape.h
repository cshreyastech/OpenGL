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

class QuadShape
{
public:
	QuadShape(const std::string QuadShape, int IndexOffset, int VertexOffset, const size_t MaxQuadCount);
	~QuadShape();

	void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

public:
	GLuint quadVA = 0;
	GLuint quadVB = 0;
	GLuint quadIB = 0;

	//Indices to draw when flushed
	uint32_t indexOffset = 0;
	uint32_t vertexOffset = 0;

	const size_t maxQuadCount = 0;
	const size_t maxVertexCount = 0;
	const size_t maxIndexCount;

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
};
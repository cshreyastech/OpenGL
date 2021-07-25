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

class ShapeHandler
{
public:
	virtual ~ShapeHandler() = default;

	virtual void Shutdown() = 0;

	virtual void BeginBatch() = 0;
	virtual void EndBatch() = 0;
	virtual void Flush() = 0;

	//void DrawQuad(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]);

	virtual const Stats& GetStats() = 0 ;
	virtual void ResetStats() = 0;
};
#pragma once
#include "ShapeHandler.h"

class QuadHandler : public ShapeHandler
{
public:
	QuadHandler(const std::string quadHandler, const int* indexSequence, const GLenum type,
		const uint32_t indexOffset, const uint32_t vertexOffset, const size_t maxQuadCount);
	virtual ~QuadHandler();
	void Flush();

	void DrawQuad(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]);
};
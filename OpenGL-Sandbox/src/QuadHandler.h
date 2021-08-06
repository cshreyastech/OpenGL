#pragma once
#include "ShapeHandler.h"

class QuadHandler : public ShapeHandler
{
public:
	QuadHandler(Isolines::Lines id, const int* indexSequence, const GLenum type,
		const uint32_t indexOffset, const uint32_t vertexOffset, const size_t maxQuadCount);
	virtual ~QuadHandler();
	
	void Flush() override;
	void Draw(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]) override;
};
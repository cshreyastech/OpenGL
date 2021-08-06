#pragma once
#include "ShapeHandler.h"

class PointHandler : public ShapeHandler
{
public:
	PointHandler(Isolines::Lines id, const GLenum type, const uint32_t indexOffset,
		const uint32_t vertexOffset, const size_t maxQuadCount);
	virtual ~PointHandler();

	void Flush() override;
	void Draw(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]) override;
};
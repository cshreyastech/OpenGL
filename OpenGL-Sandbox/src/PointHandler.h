#pragma once
#include "ShapeHandler.h"

class PointHandler : public ShapeHandler
{
public:
	PointHandler(const std::string quadShape, const GLenum type, const uint32_t indexOffset, 
		const uint32_t vertexOffset, const size_t maxQuadCount);
	virtual ~PointHandler();

	void Flush();

	void PlotPoint(const glm::vec3& positions, const glm::vec4& color, const glm::vec2 TexIndices[]);
};
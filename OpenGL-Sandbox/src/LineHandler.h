#pragma once
#include "ShapeHandler.h"

class LineHandler : public ShapeHandler
{
public:
	LineHandler(const std::string quadHandler, const int* indexSequence, const GLenum type,
		const uint32_t indexOffset, const uint32_t vertexOffset, const size_t maxQuadCount);
	virtual ~LineHandler();
	void Flush();

	void DrawLine(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]);

};
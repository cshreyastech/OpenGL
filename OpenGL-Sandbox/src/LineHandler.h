#pragma once
#include "ShapeHandler.h"

class LineHandler : public ShapeHandler
{
public:
	LineHandler(Isolines::Lines id, std::vector<int>& indexSequence, const GLenum type,
		const uint32_t indexOffset, const uint32_t vertexOffset, const size_t maxLineCount);
	virtual ~LineHandler();

	void Flush() override;
	void Draw(const std::vector<glm::vec3> positions, const glm::vec4& color, const std::vector<glm::vec2> TexIndices) override;
};
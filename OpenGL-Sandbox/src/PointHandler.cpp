#include "PointHandler.h"



PointHandler::PointHandler(Isolines::Lines id, const GLenum type, const uint32_t indexOffset,
	const uint32_t vertexOffset, const size_t maxPointCount)
	:ShapeHandler(id, type, indexOffset, vertexOffset, maxPointCount)
{
}

PointHandler::~PointHandler()
{
}

void PointHandler::Flush()
{
	ShapeHandler::FlushArray();
}

void PointHandler::Draw(const std::vector<glm::vec3> positions, const glm::vec4& color, const std::vector<glm::vec2> TexIndices)
{
	ShapeHandler::PlotPoint(positions, color, TexIndices);
}
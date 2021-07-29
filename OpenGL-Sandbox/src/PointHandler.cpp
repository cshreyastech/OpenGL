#include "PointHandler.h"
#include <GLCore.h>
#include <array>
#include <glad\glad.h>


PointHandler::PointHandler(const std::string quadShape, const GLenum type, const uint32_t indexOffset,
	const uint32_t vertexOffset, const size_t maxPointCount)
	:ShapeHandler(quadShape, type, indexOffset, vertexOffset, maxPointCount)
{
}

PointHandler::~PointHandler()
{
}

void PointHandler::PlotPoint(const glm::vec3& positions, const glm::vec4& color, const glm::vec2 TexIndices[])
{
	ShapeHandler::PlotPoint(positions, color, TexIndices);
}

void PointHandler::Flush()
{
	ShapeHandler::FlushArray();
}

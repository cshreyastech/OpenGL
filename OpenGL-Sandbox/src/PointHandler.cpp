#include "PointHandler.h"
#include <GLCore.h>
#include <array>
#include <glad\glad.h>


PointHandler::PointHandler(ShapeHandlers id, const GLenum type, const uint32_t indexOffset,
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

void PointHandler::Draw(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
{
	ShapeHandler::PlotPoint(positions, color, TexIndices);
}
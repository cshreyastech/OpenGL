#include "QuadHandler.h"
#include <GLCore.h>
#include <array>
#include <glad\glad.h>

QuadHandler::QuadHandler(ShapeHandlers id, const int* indexSequence,
	const GLenum type, const uint32_t indexOffset, const uint32_t vertexOffset, 
	const size_t maxQuadCount)
	:ShapeHandler(id, indexSequence, type, indexOffset, vertexOffset, maxQuadCount)
{
}

QuadHandler::~QuadHandler()
{
}

void QuadHandler::Flush()
{
	ShapeHandler::FlushElements(GL_TRIANGLES);
}

void QuadHandler::Draw(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
{
	ShapeHandler::DrawShape(positions, color, TexIndices);
}
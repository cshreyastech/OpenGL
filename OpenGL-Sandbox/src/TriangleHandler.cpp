#include "TriangleHandler.h"


TriangleHandler::TriangleHandler(Isolines::Lines id, std::vector<int> indexSequence,
	const GLenum type, const uint32_t indexOffset, const uint32_t vertexOffset, 
	const size_t maxTriangleCount)
	:ShapeHandler(id, indexSequence, type, indexOffset, vertexOffset, maxTriangleCount)
{
}

TriangleHandler::~TriangleHandler()
{
}

void TriangleHandler::Flush()
{
	ShapeHandler::FlushElements(GL_TRIANGLES);
}

void TriangleHandler::Draw(const std::vector<glm::vec3> positions, const glm::vec4& color, const std::vector<glm::vec2> TexIndices/*const glm::vec2 TexIndices[]*/)
{
	ShapeHandler::DrawShape(positions, color, TexIndices);
}
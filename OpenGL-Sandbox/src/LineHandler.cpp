#include "LineHandler.h"
#include <GLCore.h>
#include <array>
#include <glad\glad.h>

LineHandler::LineHandler(const std::string lineHandler, const int* indexSequence, const GLenum type,
	const uint32_t indexOffset, const uint32_t vertexOffset, const size_t maxLineCount)
	:ShapeHandler(lineHandler, indexSequence, type, indexOffset, vertexOffset, maxLineCount)
{

}

LineHandler::~LineHandler()
{
}

void LineHandler::DrawLine(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
{
	ShapeHandler::DrawShape(positions, color, TexIndices);
	//if (indexCount >= MaxIndexCount)
	//{
	//	EndBatch();
	//	Flush();
	//	BeginBatch();
	//}

	//float textureIndex = 0.0f;

	//for (int i = 0; i < VertexOffset; i++)
	//{
	//	quadBufferPtr->Position = positions[i];
	//	quadBufferPtr->Color = color;
	//	quadBufferPtr->TexCoords = TexIndices[i];
	//	quadBufferPtr->TexIndex = textureIndex;
	//	quadBufferPtr++;
	//}

	//indexCount += IndexOffset;

	//RenderStats.QuadCount++;
	//RenderStats.VertexCount = RenderStats.QuadCount * VertexOffset;
	//RenderStats.IndexCount = RenderStats.QuadCount * IndexOffset;
}

void LineHandler::Flush()
{
	ShapeHandler::FlushElements(GL_LINES);
}
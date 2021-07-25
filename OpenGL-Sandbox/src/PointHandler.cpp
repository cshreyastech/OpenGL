#include "PointHandler.h"
#include <GLCore.h>
#include <array>
#include <glad\glad.h>

PointHandler::PointHandler(const std::string quadShape, const size_t maxVertexCount)
	: MaxVertexCount(maxVertexCount)
	{
		quadBuffer = new Vertex[MaxVertexCount];

		glCreateVertexArrays(1, &quadVA);
		glBindVertexArray(quadVA);

		glCreateBuffers(1, &quadVB);
		glBindBuffer(GL_ARRAY_BUFFER, quadVB);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexArrayAttrib(quadVA, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

		glEnableVertexArrayAttrib(quadVA, 1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

		glEnableVertexArrayAttrib(quadVA, 2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

		glEnableVertexArrayAttrib(quadVA, 3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexIndex));

		
		//1 x 1 white texture
		glCreateTextures(GL_TEXTURE_2D, 1, &WhiteTexture);
		glBindTexture(GL_TEXTURE_2D, WhiteTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		uint32_t color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &color);

		TextureSlots[0] = WhiteTexture;
		//memory clear, alternatively use memset
		for (size_t i = 1; i < MaxTextures; i++)
			TextureSlots[i] = 0;
	}

PointHandler::~PointHandler()
{
}

void PointHandler::Shutdown()
{
	glDeleteVertexArrays(1, &quadVA);
	glDeleteBuffers(1, &quadVB);
	glDeleteTextures(1, &WhiteTexture);

	delete[] quadBuffer;
}

void PointHandler::BeginBatch()
{
	// Resets point to the begining
	// pointer keeps traks of where we copying the data to 
	// in the cpu side vertex buffer
	quadBufferPtr = quadBuffer;
}

void PointHandler::EndBatch()
{
	GLsizeiptr size = (uint8_t*)quadBufferPtr - (uint8_t*)quadBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, quadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, quadBuffer);
}

void PointHandler::PlotPoint(const glm::vec3& positions, const glm::vec4& color, const glm::vec2 TexIndices[])
{
	if (vertexCount >= MaxVertexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float textureIndex = 0.0f;

	quadBufferPtr->Position = positions;
	quadBufferPtr->Color = color;
	quadBufferPtr->TexCoords = TexIndices[0];
	quadBufferPtr->TexIndex = textureIndex;
	quadBufferPtr++;

	vertexCount++;

	RenderStats.QuadCount++;

	/*std::cout << "Inside PointHandler::PlotPoint: positions.size()" << positions.size() << std::endl;
	for (int i = 0; i < positions.size(); i++)
	{
		quadBufferPtr->Position = positions[i];
		quadBufferPtr->Color = color;
		quadBufferPtr->TexCoords = TexIndices[i];
		quadBufferPtr->TexIndex = textureIndex;
		quadBufferPtr++;
		textureIndex++;

		vertexCount++;
	}*/
	

	
	RenderStats.VertexCount = RenderStats.QuadCount;
}

void PointHandler::Flush()
{
	for (uint32_t i = 0; i < TextureSlotIndex; i++)
		glBindTextureUnit(i, TextureSlots[i]);

	glBindVertexArray(quadVA);
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, vertexCount);

	RenderStats.DrawCount++;
	vertexCount = 0;
	TextureSlotIndex = 1;
}

const Stats& PointHandler::GetStats()
{
	return RenderStats;
}

void PointHandler::ResetStats()
{
	memset(&RenderStats, 0, sizeof(Stats));
}
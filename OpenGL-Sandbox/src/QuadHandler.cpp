#include "QuadHandler.h"
#include <GLCore.h>
#include <array>
#include <glad\glad.h>


QuadHandler::QuadHandler(const std::string quadHandler, const size_t maxQuadCount)
	: MaxQuadCount(maxQuadCount),
		MaxVertexCount(maxQuadCount * VertexOffset), MaxIndexCount(MaxQuadCount * IndexOffset)
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

		int indicesSequence[6] = { 0, 1, 2, 2, 3, 0 };

		uint32_t* indices;
		indices = new uint32_t[MaxIndexCount];
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxIndexCount; i += IndexOffset)
		{
			for (int j = 0; j < IndexOffset; j++)
				indices[i + j] = indicesSequence[j] + offset;

			offset += VertexOffset;
		}

		//Sending to GPU
		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MaxIndexCount, indices, GL_STATIC_DRAW);


		delete[] indices;
		
		TextureSlots[0] = WhiteTexture;
		//memory clear, alternatively use memset
		for (size_t i = 1; i < MaxTextures; i++)
			TextureSlots[i] = 0;

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

QuadHandler::~QuadHandler()
{
}

void QuadHandler::Shutdown()
{
	glDeleteVertexArrays(1, &quadVA);
	glDeleteBuffers(1, &quadVB);
	glDeleteBuffers(1, &quadIB);
	glDeleteTextures(1, &WhiteTexture);

	delete[] quadBuffer;
}

void QuadHandler::BeginBatch()
{
	// Resets point to the begining
	// pointer keeps traks of where we copying the data to 
	// in the cpu side vertex buffer
	quadBufferPtr = quadBuffer;
}

void QuadHandler::EndBatch()
{
	GLsizeiptr size = (uint8_t*)quadBufferPtr - (uint8_t*)quadBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, quadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, quadBuffer);
}

void QuadHandler::DrawQuad(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
{
	if (indexCount >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float textureIndex = 0.0f;

	for (int i = 0; i < VertexOffset; i++)
	{
		quadBufferPtr->Position = positions[i];
		quadBufferPtr->Color = color;
		quadBufferPtr->TexCoords = TexIndices[i];
		quadBufferPtr->TexIndex = textureIndex;
		quadBufferPtr++;
	}

	indexCount += IndexOffset;

	RenderStats.QuadCount++;
	RenderStats.VertexCount = RenderStats.QuadCount * VertexOffset;
	RenderStats.IndexCount = RenderStats.QuadCount * IndexOffset;
}

void QuadHandler::Flush()
{
	for (uint32_t i = 0; i < TextureSlotIndex; i++)
		glBindTextureUnit(i, TextureSlots[i]);

	glBindVertexArray(quadVA);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

	RenderStats.DrawCount++;
	indexCount = 0;
	TextureSlotIndex = 1;
}

const Stats& QuadHandler::GetStats()
{
	return RenderStats;
}

void QuadHandler::ResetStats()
{
	memset(&RenderStats, 0, sizeof(Stats));
}
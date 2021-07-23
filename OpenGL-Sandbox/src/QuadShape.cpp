#include "QuadShape.h"
#include <GLCore.h>
#include <array>
#include <glad\glad.h>


QuadShape::QuadShape(const std::string QuadShape, int IndexOffset, int VertexOffset, const size_t MaxQuadCount)
	: indexOffset(IndexOffset), vertexOffset(VertexOffset), maxQuadCount(MaxQuadCount),
		maxVertexCount(maxQuadCount * vertexOffset), maxIndexCount(maxQuadCount * indexOffset)
	{
		quadBuffer = new Vertex[maxVertexCount];

		glCreateVertexArrays(1, &quadVA);
		glBindVertexArray(quadVA);


		glCreateBuffers(1, &quadVB);
		glBindBuffer(GL_ARRAY_BUFFER, quadVB);
		glBufferData(GL_ARRAY_BUFFER, maxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexArrayAttrib(quadVA, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

		glEnableVertexArrayAttrib(quadVA, 1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

		glEnableVertexArrayAttrib(quadVA, 2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

		glEnableVertexArrayAttrib(quadVA, 3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexIndex));

		uint32_t *indices;
		indices = new uint32_t[maxIndexCount];

		uint32_t offset = 0;
		for (size_t i = 0; i < maxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		/*std::cout << "NEW: "
			<< "MaxIndexCount: " << maxIndexCount
			<< ", sizeof(indices): " << sizeof(indices)
			<< ", sizeof(uint32_t): " << sizeof(uint32_t) << std::endl;*/


		//Sending to GPU
		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * maxIndexCount, indices, GL_STATIC_DRAW);


		delete[] indices;


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

QuadShape::~QuadShape()
{

}

void QuadShape::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	/*if (s_Data.IndexCount >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}*/
	float textureIndex = 0.0f;

	quadBufferPtr->Position = { position.x, position.y, 0.0f };
	quadBufferPtr->Color = color;
	quadBufferPtr->TexCoords = { 0.0f, 0.0f };
	quadBufferPtr->TexIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
	quadBufferPtr->Color = color;
	quadBufferPtr->TexCoords = { 1.0f, 0.0f };
	quadBufferPtr->TexIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
	quadBufferPtr->Color = color;
	quadBufferPtr->TexCoords = { 1.0f, 1.0f };
	quadBufferPtr->TexIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
	quadBufferPtr->Color = color;
	quadBufferPtr->TexCoords = { 0.0f, 1.0f };
	quadBufferPtr->TexIndex = textureIndex;
	quadBufferPtr++;

	indexCount += indexOffset;
	//s_Data.RenderStats.QuadCount++;
	//s_Data.RenderStats.VertexCount = s_Data.RenderStats.QuadCount * 4;
	//s_Data.RenderStats.IndexCount = s_Data.RenderStats.QuadCount * 6;
}

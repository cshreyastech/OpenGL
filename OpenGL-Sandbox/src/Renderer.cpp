#include "Renderer.h"

#include <GLCore.h>
#include <array>
#include <glad\glad.h>


Renderer* Renderer::s_Instance = nullptr;

Renderer::Renderer()
{
	if (!s_Instance)
	{
		// Initialize core
		//Log::Init();
	}

	GLCORE_ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

}

Renderer::~Renderer()
{
}


void Renderer::Init()
{
	quadShape = new QuadShape("SquareQuad", 6, 4, 20);
}

void Renderer::Shutdown()
{
	glDeleteVertexArrays(1, &quadShape->quadVA);
	glDeleteBuffers(1, &quadShape->quadVB);
	glDeleteBuffers(1, &quadShape->quadIB);

	glDeleteTextures(1, &quadShape->WhiteTexture);

	delete[] quadShape->quadBuffer;
}

void Renderer::BeginBatch()
{
	// Resets point to the begining
	// pointer keeps traks of where we copying the data to 
	// in the cpu side vertex buffer
	quadShape->quadBufferPtr = quadShape->quadBuffer;
}

void Renderer::EndBatch()
{
	GLsizeiptr size = (uint8_t*)quadShape->quadBufferPtr - (uint8_t*)quadShape->quadBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, quadShape->quadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, quadShape->quadBuffer);
}

void Renderer::Flush()
{
	for (uint32_t i = 0; i < quadShape->TextureSlotIndex; i++)
		glBindTextureUnit(i, quadShape->TextureSlots[i]);

	glBindVertexArray(quadShape->quadVA);
	glDrawElements(GL_TRIANGLES, quadShape->indexCount, GL_UNSIGNED_INT, nullptr);

	RenderStats.DrawCount++;

	quadShape->indexCount = 0;
	quadShape->TextureSlotIndex = 1;
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (quadShape->indexCount >= quadShape->maxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}
	quadShape->DrawQuad(position, size, color);

	RenderStats.QuadCount++;
	RenderStats.IndexCount = RenderStats.QuadCount * 6;
	RenderStats.VertexCount = RenderStats.QuadCount * 4;
}


const Stats& Renderer::GetStats()
{
	return RenderStats;
}

void Renderer::ResetStats()
{
	memset(&RenderStats, 0, sizeof(Stats));
}
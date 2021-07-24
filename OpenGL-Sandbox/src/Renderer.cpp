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
	quadShape = new QuadShape("SquareQuad", new int[6]{ 0, 1, 2, 2, 3, 0 }, GL_TRIANGLES, 6, 4, 1000);
}

void Renderer::Shutdown()
{
	quadShape->Shutdown();
}

void Renderer::BeginBatch()
{
	quadShape->BeginBatch();
}

void Renderer::EndBatch()
{
	quadShape->EndBatch();
}

void Renderer::Flush()
{
	quadShape->Flush();
}

void Renderer::DrawQuad(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
{
	quadShape->DrawQuad(positions, color, TexIndices);
}


const Stats& Renderer::GetStats()
{
	return quadShape->GetStats();
}

void Renderer::ResetStats()
{
	quadShape->ResetStats();
}
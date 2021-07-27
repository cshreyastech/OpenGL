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
	//quadHandler = new QuadHandler("SquareQuad", 1000);
	pointHandler = new PointHandler("PlotPoints", 100);
	lineHandler = new LineHandler("DrawLine", 100);
}

void Renderer::Shutdown()
{
	//quadHandler->Shutdown();
	pointHandler->Shutdown();
	lineHandler->Shutdown();
}

void Renderer::BeginBatch()
{
	//quadHandler->BeginBatch();
	pointHandler->BeginBatch();
	lineHandler->BeginBatch();
}

void Renderer::EndBatch()
{
	//quadHandler->EndBatch();
	pointHandler->EndBatch();
	lineHandler->EndBatch();
}

void Renderer::Flush()
{
	//quadHandler->Flush();
	pointHandler->Flush();
	lineHandler->Flush();
}

void Renderer::DrawQuad(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
{
	quadHandler->DrawQuad(positions, color, TexIndices);
	
}

void Renderer::PlotPoints(const glm::vec3& positions, const glm::vec4& color, const glm::vec2 TexIndices[])
{
	pointHandler->PlotPoint(positions, color, TexIndices);
}

void Renderer::DrawLine(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
{
	lineHandler->DrawLine(positions, color, TexIndices);
}


const Stats& Renderer::GetStats()
{
	//return quadHandler->GetStats();
	//return pointHandler->GetStats();
	return lineHandler->GetStats();
}

void Renderer::ResetStats()
{
	//quadHandler->ResetStats();
	pointHandler->ResetStats();
	lineHandler->ResetStats();
}
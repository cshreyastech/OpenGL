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
	quadHandler = new QuadHandler("SquareQuad", new int[6]{ 0, 1, 2, 2, 3, 0 }, GL_TRIANGLES, 6, 4, 2);
	pointHandler = new PointHandler("PlotPoints", GL_POINTS, 1, 1, 3);
	lineHandler = new LineHandler("DrawLine", new int[2]{ 0, 1 }, GL_LINES,2, 2, 2);
}

void Renderer::Shutdown()
{
	quadHandler->Shutdown();
	pointHandler->Shutdown();
	lineHandler->Shutdown();
}

void Renderer::BeginBatch()
{
	quadHandler->BeginBatch();
	pointHandler->BeginBatch();
	lineHandler->BeginBatch();
}

void Renderer::EndBatch()
{
	quadHandler->EndBatch();
	pointHandler->EndBatch();
	lineHandler->EndBatch();
}

void Renderer::Flush()
{
	quadHandler->Flush();
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
	quadHandler->ResetStats();
	pointHandler->ResetStats();
	lineHandler->ResetStats();
}
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
	shapeHandlerMap[ShapeHandlers::Quad] =
		new QuadHandler(ShapeHandlers::Quad,
			new int[6]{ 0, 1, 2, 2, 3, 0 }, GL_TRIANGLES, 6, 4, 2);

	shapeHandlerMap[ShapeHandlers::Line] =
		new LineHandler(ShapeHandlers::Line,
			new int[2]{ 0, 1 }, GL_LINES, 2, 2, 2);

	shapeHandlerMap[ShapeHandlers::Point] =
		new PointHandler(ShapeHandlers::Point, GL_POINTS, 1, 1, 3);



}

void Renderer::Shutdown()
{
	for (shapeHandlerMapItr = shapeHandlerMap.begin();
		shapeHandlerMapItr != shapeHandlerMap.end();
		shapeHandlerMapItr++)
	{
		shapeHandlerMapItr->second->Shutdown();
	}
	
}

void Renderer::BeginBatch()
{
	for (shapeHandlerMapItr = shapeHandlerMap.begin();
		shapeHandlerMapItr != shapeHandlerMap.end();
		shapeHandlerMapItr++)
	{
		shapeHandlerMapItr->second->BeginBatch();
	}
}

void Renderer::EndBatch()
{
	for (shapeHandlerMapItr = shapeHandlerMap.begin();
		shapeHandlerMapItr != shapeHandlerMap.end();
		shapeHandlerMapItr++)
	{
		shapeHandlerMapItr->second->EndBatch();
	}
}

void Renderer::Flush()
{
	for (shapeHandlerMapItr = shapeHandlerMap.begin();
		shapeHandlerMapItr != shapeHandlerMap.end();
		shapeHandlerMapItr++)
	{
		shapeHandlerMapItr->second->Flush();
	}
}

void Renderer::Draw(ShapeHandlers handler, const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
{
	shapeHandlerMap[handler]->Draw(positions, color, TexIndices);
}

const std::vector<Stats> Renderer::GetStats()
{
	std::vector<Stats> Statuses;

	for (shapeHandlerMapItr = shapeHandlerMap.begin();
		shapeHandlerMapItr != shapeHandlerMap.end();
		shapeHandlerMapItr++)
	{
		const Stats stats = shapeHandlerMapItr->second->GetStats();
		Statuses.emplace_back(stats);
	}

	return Statuses;
}

void Renderer::ResetStats()
{
	for (shapeHandlerMapItr = shapeHandlerMap.begin();
		shapeHandlerMapItr != shapeHandlerMap.end();
		shapeHandlerMapItr++)
	{
		shapeHandlerMapItr->second->ResetStats();
	}
}
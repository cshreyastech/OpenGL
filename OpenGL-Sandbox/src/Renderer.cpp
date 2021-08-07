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
	const int batchSize = 10000;
	shapeHandlerMap[Isolines::Lines::Point] =
		new PointHandler(Isolines::Lines::Point, GL_POINTS, 1, 1, batchSize);

	/*shapeHandlerMap[ShapeHandlers::Quad] =
		new QuadHandler(ShapeHandlers::Quad,
			new int[6]{ 0, 1, 2, 2, 3, 0 }, GL_TRIANGLES, 6, 4, 1000);*/

	for (const Isolines::Lines line : Isolines::allLines)
	{
		if (line == Isolines::Lines::Ten) continue;

		shapeHandlerMap[line] =
			new LineHandler(line, new int[2]{ 0, 1 }, GL_LINES, 2, 2, batchSize);
	}

	shapeHandlerMap[Isolines::Lines::Ten] =
		new LineHandler(Isolines::Lines::Ten, new int[6]{ 0, 1, 1, 2, 2, 3 }, GL_LINES, 6, 4, batchSize);
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

void Renderer::Draw(Isolines::Lines handler, const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[])
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
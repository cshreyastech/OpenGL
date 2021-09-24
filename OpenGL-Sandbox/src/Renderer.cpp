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
	const int batchSize = 10;
	shapeHandlerMap[Isosurface::Facet::Point] =
		new PointHandler(Isosurface::Facet::Point, GL_POINTS, 1, 1, batchSize);


	for (const Isosurface::Facet facet : Isosurface::allFacet)
	{
		const ContourFacet contourLineProperites = MarchingSquare::ContourFacetProperties(facet);
		shapeHandlerMap[facet] =
			new TriangleHandler(facet, contourLineProperites.indexSequence, 
				GL_TRIANGLES, contourLineProperites.indexSequence.size(),
				contourLineProperites.vertexOffset, batchSize);
	}
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

void Renderer::Draw(Isosurface::Facet handler, const std::vector<glm::vec3> positions, const glm::vec4& color, const std::vector<glm::vec2> TexIndices)
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
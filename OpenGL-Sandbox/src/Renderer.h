#pragma once

#include <glm/glm.hpp>
#include "PointHandler.h"
#include "TriangleHandler.h"
#include "MarchingSquare.h"
#include <vector>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init();
	void Shutdown();

	void BeginBatch();
	void EndBatch();
	void Flush();

	void Draw(Isosurface::Facet handler, const std::vector<glm::vec3> positions, const glm::vec4& color, const std::vector<glm::vec2> TexIndices);

	const std::vector<Stats> GetStats();
	void ResetStats();

private:
	static Renderer* s_Instance;

	std::unordered_map<Isosurface::Facet, ShapeHandler*> shapeHandlerMap;
	std::unordered_map<Isosurface::Facet, ShapeHandler*>::iterator shapeHandlerMapItr;
};


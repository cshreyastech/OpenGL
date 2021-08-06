#pragma once

#include <glm/glm.hpp>
#include "QuadHandler.h"
#include "PointHandler.h"
#include "LineHandler.h"
#include "Handlers.h"
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

	void Draw(Isolines::Lines handler, const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]);

	const std::vector<Stats> GetStats();
	void ResetStats();

private:
	static Renderer* s_Instance;
	QuadHandler* quadHandler;
	PointHandler* pointHandler;
	LineHandler* lineHandler;

	std::unordered_map<Isolines::Lines, ShapeHandler*> shapeHandlerMap;
	std::unordered_map<Isolines::Lines, ShapeHandler*>::iterator shapeHandlerMapItr;
};


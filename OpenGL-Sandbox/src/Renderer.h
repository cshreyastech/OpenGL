#pragma once

#include <glm/glm.hpp>
#include "QuadHandler.h"
#include "PointHandler.h"
#include "LineHandler.h"

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

	//Flat colored Quad
	void DrawQuad(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]);
	void PlotPoints(const glm::vec3& positions, const glm::vec4& color, const glm::vec2 TexIndices[]);
	void DrawLine(const glm::vec3 positions[], const glm::vec4& color, const glm::vec2 TexIndices[]);


	const Stats& GetStats();
	void ResetStats();

private:
	static Renderer* s_Instance;
	QuadHandler* quadHandler;
	PointHandler* pointHandler;
	LineHandler* lineHandler;
};
#pragma once

#include <glm/glm.hpp>
#include "QuadShape.h"


struct Stats
{
	uint32_t DrawCount = 0;
	uint32_t QuadCount = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
};

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
	void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);



	const Stats& GetStats();
	void ResetStats();

private:
	//Renderer() {}
	static Renderer* s_Instance;
	//QuadShape quadShape;
	QuadShape* quadShape;

	struct Stats RenderStats;
};
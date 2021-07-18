#pragma once

#include <glm/glm.hpp>

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	//Flat colored Quad
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

	//Textured Quad
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID);

	struct Stats
	{
		uint32_t DrawCount = 0;
		uint32_t QuadCount = 0;
	};

	static const Stats& GetStats();
	static void ResetStats();
};
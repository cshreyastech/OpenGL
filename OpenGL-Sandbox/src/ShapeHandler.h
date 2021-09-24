#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GLCore.h>
#include <array>
#include <glad\glad.h>
#include "MarchingSquare.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexIndex;
};

struct Stats
{
	int ID;
	uint32_t DrawCount = 0;
	uint32_t QuadCount = 0;
	uint32_t VertexCount = 0;
	uint32_t IndexCount = 0;
};

class ShapeHandler
{
public:
	ShapeHandler(Isosurface::Facet id, std::vector<int>& indexSequence, const GLenum type,
		const uint32_t indexOffset, const uint32_t vertexOffset, const size_t maxShapeCount);

	ShapeHandler(Isosurface::Facet id, const GLenum type, const uint32_t indexOffset,
		const uint32_t vertexOffset, const size_t maxShapeCount);
	virtual ~ShapeHandler() = default;

	virtual void Draw(const std::vector<glm::vec3> positions, const glm::vec4& color, const std::vector<glm::vec2> TexIndices) = 0;
	
	void DrawShape(const std::vector<glm::vec3> positions, const glm::vec4& color, const std::vector<glm::vec2> TexIndices);
	void PlotPoint(const std::vector<glm::vec3> positions, const glm::vec4& color, const std::vector<glm::vec2> TexIndices);

	void Shutdown();

	const GLenum GetType() { return Type; }
	void BeginBatch();
	void EndBatch();
	void FlushElements(GLenum type) ;
	void FlushArray();
	virtual void Flush() = 0;

	const Stats& GetStats();
	void ResetStats();

private:
	Isosurface::Facet ID = Isosurface::Facet::Zero;
	GLuint quadVA = 0;
	GLuint quadVB = 0;
	GLuint quadIB = 0;

	const uint32_t IndexOffset = 0;
	const uint32_t VertexOffset = 0;

	const size_t MaxShapeCount = 0;
	const size_t MaxVertexCount = 0;
	const size_t MaxIndexCount = 0;
	const GLenum Type = 0;

	std::vector<int>& IndexSequence;
	//Points beginning of the buffer. This is in CPU
	Vertex* shapeBuffer = nullptr;
	//Where we are up to
	Vertex* shapeBufferPtr = nullptr;

	//Use openGL code to Query the driver, find how many Texture slots the GPU have
	static const size_t MaxTextures = 32;

	//Which Texture slot holds to what OpenGL ID
	std::array<uint32_t, MaxTextures> TextureSlots;
	uint32_t TextureSlotIndex = 1;
	GLuint WhiteTexture = 0;
	uint32_t WhiteTextureSlot = 0;
	uint32_t indexCount = 0;
	uint32_t vertexCount = 0;

	struct Stats RenderStats;
private:
	void CreateVA();
	void CreateVB();
	void EnableVA();
	void CreateIB();
	void TextureSlotReset();
};
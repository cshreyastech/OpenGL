#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <memory>
#include<glm/glm.hpp>

#include "Renderer.h"
#include "grid/Grid.h"

class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	void GenerateContour();
	void RenderContour();
	int GetState(int a, int b, int c, int d);
	void GeneratePoints(glm::vec3 position, float decimalCode) const;
	void DrawFacet(glm::vec3 edges[4], glm::vec3 contorPoints[4], int facetIndex) const;
private:
	Grid* grid = nullptr;

	const float highestISOvalue = 10.0f/*16.0f*/;
	const float lowestISOvalue = 0.0f/*-34.0f*/;
	const float isoLevel = /*0.0f*/ 5.0f;

	const float quad_size = 0.25f;
	const float rows = 10.0f;
	const float cols = 10.0f;
	int nRows = 2 * rows / quad_size + 1;
	int nCols = 2 * cols / quad_size + 1;

	std::unique_ptr<GLCore::Utils::Shader> m_Shader;
	GLCore::Utils::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	Renderer* s_Instance = nullptr;	
};
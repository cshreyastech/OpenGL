#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <memory>
#include<glm/glm.hpp>

#include "Renderer.h"

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
	//void GenerateQuads();
	//void GeneratePoints();
	void SandboxLayer::GeneratePoints(float x, float y, float decimalCode) const;
	void GenerateLines(float x, float y, Isolines::Lines line) const;

private:
	std::unique_ptr<GLCore::Utils::Shader> m_Shader;
	GLCore::Utils::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	Renderer* s_Instance = nullptr;
	float** contour;
};
#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <memory>
#include<glm/glm.hpp>

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
	std::unique_ptr<GLCore::Utils::Shader> m_Shader;
	GLuint m_QuadVA = 0, m_QuadVB = 0, m_QuadIB = 0;
	GLCore::Utils::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	GLuint m_ChernoTex, m_HazelTex;

	//float m_QuadPosition[2] = { -1.5f, -0.5f };
	//glm::vec2 m_QuadPosition(-1.5f, -0.5f);
	glm::vec2 m_QuadPosition = { -1.5f, -0.5f };
};
#include "SandboxLayer.h"
#include <stb_image/stb_image.h>


using namespace GLCore;
using namespace GLCore::Utils;


//float rez = 10.0f;
const float quad_size = 1.0f;
//const float rows = 100.0f / quad_size;
//const float cols = 100.0f / quad_size;

const float rows = 10.0f;
const float cols = 10.0f;

SandboxLayer::SandboxLayer()
	: Layer("Sandbox"), m_CameraController(16.0f / 9.0f)
{
}

SandboxLayer::~SandboxLayer()
{
}

static GLuint LoadTexture(const std::string& path)
{
	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb);

	GLuint textureID;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);
	return textureID;
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();
	m_Shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles(
		"assets/shaders/shader.glsl.vert",
		"assets/shaders/shader.glsl.frag"
	));

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	s_Instance = new Renderer();
	s_Instance->Init();
}

void SandboxLayer::OnDetach()
{
	s_Instance->Shutdown();
}

void SandboxLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
	if (event.GetEventType() == EventType::WindowResize)
	{
		WindowResizeEvent& e = (WindowResizeEvent&)event;
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
	}
}

static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	GLCORE_PROFILE_FUNCTION();

	// Update
	{
		GLCORE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	
	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_Shader->GetRendererID());

	{ 
			GLCORE_PROFILE_SCOPE("MVP");
			const auto& vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
			SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProj", vp);
			SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	}

	{
		GLCORE_PROFILE_SCOPE("Renderer Draw");
		s_Instance->ResetStats();
		s_Instance->BeginBatch();


		//GenerateQuads();
		GeneratePoints();

		const glm::vec2 TexIndicesPoints[] = {
			{ 0.0f, 0.0f },
			{ 0.0f, 1.0f }
		};

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float midd_size = quad_size / 2.0f;
		float row = rows - quad_size;
		float col = cols - quad_size;
		/*glm::vec3 positions[] = {
						{ -row + midd_size, -col + quad_size, 0.0f },
						{ -row + quad_size, -col + midd_size, 0.0f }
					};*/

		/*const glm::vec3 positions[] = {
						glm::vec3(-5.0f, -5.0f, 0.0f),
						glm::vec3( 5.0f,  5.0f, 0.0f)
		};*/

		/*glm::vec3 temp = positions[0];
		std::cout << "temp: " << temp[0] << std::endl;*/
		//s_Instance->DrawLine(positions, color, TexIndicesPoints);

		/*positions[0] = glm::vec3(-row + quad_size, -col + midd_size, 0.0f);
		positions[1] = glm::vec3(-row + midd_size,			   -col, 0.0f);
		s_Instance->DrawLine(positions, color, TexIndicesPoints);*/

		/*const glm::vec3 positions[] = {
						{ -10.0f + midd_size,			  -10.0f, 0.0f },
						{			  -10.0f, -10.0f + midd_size, 0.0f },
		};*/

		//s_Instance->DrawLine(positions, color, TexIndicesPoints);

		for (float y = -rows; y < rows; y += quad_size)
		{
			for (float x = -cols; x < cols; x += quad_size)
			{
				const glm::vec3 positions[] = {
					{ x + midd_size, y + quad_size, 0.0f },
					{ x + quad_size, y + midd_size, 0.0f }
				};
				/*std::cout << x + midd_size << ", " << y + quad_size << " : " 
						  << x + quad_size << ", " << y + midd_size << std::endl;*/
				s_Instance->DrawLine(positions, color, TexIndicesPoints);
			}
		}
		//std::cout << "--------------------" << std::endl;
		s_Instance->EndBatch();
		s_Instance->Flush();
	}
}

void SandboxLayer::OnImGuiRender()
{
	GLCORE_PROFILE_FUNCTION();

	ImGui::Begin("Controls");
	ImGui::Text("Quads: %d", s_Instance->GetStats().QuadCount);
	ImGui::Text("DrawCount: %d", s_Instance->GetStats().DrawCount);
	ImGui::Text("VertexCount: %d", s_Instance->GetStats().VertexCount);
	ImGui::Text("IndexCount: %d", s_Instance->GetStats().IndexCount);
	ImGui::End();
}

void SandboxLayer::GenerateQuads()
{
	const glm::vec2 TexIndices[] = {
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f }
	};

	
	for (float y = -rows; y < rows; y += quad_size)
	{
		for (float x = -cols; x < cols; x += quad_size)
		{
			glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, 1.0f };

			const glm::vec3 positions[] = {
				{			  x,			 y, 0.0f },
				{ x + quad_size,			 y, 0.0f },
				{ x + quad_size, y + quad_size, 0.0f },
				{			  x, y + quad_size, 0.0f }
			};

			s_Instance->DrawQuad(positions, color, TexIndices);
			//s_Instance->DrawLine(positions, color, TexIndices);
		}
	}
}

void SandboxLayer::GeneratePoints()
{
	const glm::vec2 TexIndicesPoints[] = {
	{ 0.0f, 0.0f }
	};

	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (float y = -rows; y <= rows; y += quad_size)
	{
		for (float x = -cols; x <= cols; x += quad_size)
		{
			s_Instance->PlotPoints(glm::vec3(x, y, 0.0f), color, TexIndicesPoints);
		}
	}
}

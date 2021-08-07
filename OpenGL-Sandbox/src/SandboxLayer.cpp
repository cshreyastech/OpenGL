#include "SandboxLayer.h"
#include <stb_image/stb_image.h>

using namespace GLCore;
using namespace GLCore::Utils;



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

	GenerateContour();
}

void SandboxLayer::OnDetach()
{
	s_Instance->Shutdown();
	delete[] contour;
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
		RenderContour();
		//GenerateQuads();
		

		s_Instance->EndBatch();
		s_Instance->Flush();
	}
}

void SandboxLayer::OnImGuiRender()
{
	GLCORE_PROFILE_FUNCTION();
	ImGui::Begin("Controls");
	ImGui::Columns(5, "Dashboard", true);
	ImGui::Text("Shape");
	ImGui::NextColumn();
	ImGui::Text("TotalCount");
	ImGui::NextColumn();
	ImGui::Text("DrawCount");
	ImGui::NextColumn();
	ImGui::Text("VertexCount");
	ImGui::NextColumn();
	ImGui::Text("IndexCount");
	ImGui::NextColumn();
	

	std::vector<Stats> States = s_Instance->GetStats();

	for (const Stats& stats : States)
	{
		ImGui::Text("%d", stats.ID);
		ImGui::NextColumn();
		ImGui::Text("%d", stats.QuadCount);
		ImGui::NextColumn();
		ImGui::Text("%d", stats.DrawCount);
		ImGui::NextColumn();
		ImGui::Text("%d", stats.VertexCount);
		ImGui::NextColumn();
		ImGui::Text("%d", stats.IndexCount);
		ImGui::NextColumn();
	}

	ImGui::End();
}

void SandboxLayer::GenerateContour()
{

	std::cout << "nRows: " << nRows << ", nCols: " << nCols << std::endl;
	contour = new float* [nRows];

	for (int i = 0; i < nRows; i++)
		contour[i] = new float[nCols];

	for (int row = 0; row < nRows; row++)
	{
		for (int col = 0; col < nCols; col++)
		{
			contour[row][col] = rand() % 2;
		}
	}
}

void SandboxLayer::RenderContour()
{
	int row = 0, col = 0;
	for (float y = -rows; y <= rows; y += quad_size)
	{
		col = 0;
		for (float x = -cols; x <= cols; x += quad_size)
		{
			GeneratePoints(x, y, contour[row][col]);
			
			col++;
		}
		row++;
	}

	float x = -rows, y = -cols;
	for (int row = 0; row < nRows - 1; row++)
	{
		x = -cols;
		for (int col = 0; col < nCols - 1; col++)
		{
			int isoLine = GetState(contour[row + 1][col], contour[row + 1][col + 1],
							contour[row][col + 1], contour[row][col]);
			GenerateLines(x, y, Isolines::LineByIndex(isoLine));
			x += quad_size;
		}

		y += quad_size;
	}

}

int SandboxLayer::GetState(int a, int b, int c, int d)
{
	return a * 8 + b * 4 + c * 2 + d * 1;
}

//void SandboxLayer::GenerateQuads()
//{
//	const glm::vec2 TexIndices[] = {
//	{ 0.0f, 0.0f },
//	{ 1.0f, 0.0f },
//	{ 1.0f, 1.0f },
//	{ 0.0f, 1.0f }
//	};
//
//	
//	for (float y = -rows; y < rows; y += quad_size)
//	{
//		for (float x = -cols; x < cols; x += quad_size)
//		{
//			glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, 1.0f };
//
//			const glm::vec3 positions[] = {
//				{			  x,			 y, 0.0f },
//				{ x + quad_size,			 y, 0.0f },
//				{ x + quad_size, y + quad_size, 0.0f },
//				{			  x, y + quad_size, 0.0f }
//			};
//
//			s_Instance->Draw(Isolines::Lines::Quad, positions, color, TexIndices);
//		}
//	}
//}

void SandboxLayer::GeneratePoints(float x, float y, float decimalCode) const
{
	const glm::vec2 TexIndicesPoints[] = {
		{ 0.0f, 0.0f }
	};

	const glm::vec3 positions[] = {
					{ x, y, 0.0f }
	};

	glm::vec4 color = { decimalCode, decimalCode, decimalCode, 1.0f };
	s_Instance->Draw(Isolines::Lines::Point, positions, color, TexIndicesPoints);
}

void SandboxLayer::GenerateLines(float x, float y, Isolines::Lines line) const
{
	const glm::vec2 TexIndicesPoints[] = {
			{ 0.0f, 0.0f },
			{ 0.0f, 1.0f }
	};

	const glm::vec2 TexIndicesPointsTen[] = {
			{ 0.5f, 0.5f },
			{ 0.0f, 0.5f },
			{ 0.5f, 1.0f },
			{ 1.0f, 0.5f }
	};

	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	float midd_size = quad_size / 2.0f;
	float row = rows - quad_size;
	float col = cols - quad_size;

	glm::vec3 positions[2];
	glm::vec3 positionsTen[4];

		/*
				a
			d		b
			.	c
		*/
		const glm::vec3 a = { x + midd_size, y + quad_size, 0.0f };
		const glm::vec3 b = { x + quad_size, y + midd_size, 0.0f };
		const glm::vec3 c = { x + midd_size,			 y, 0.0f };
		const glm::vec3 d = {			  x, y + midd_size, 0.0f };

			
		switch (line)
		{
			case Isolines::Lines::One:
				positions[0] = c;
				positions[1] = d;
				break;

			case Isolines::Lines::Two:
				positions[0] = b;
				positions[1] = c;
				break;

			case Isolines::Lines::Three:
				positions[0] = d;
				positions[1] = b;
				break;

			case Isolines::Lines::Four:
				positions[0] = a;
				positions[1] = b;
				break;

			case Isolines::Lines::Five:
				positions[0] = d;
				positions[1] = a;

				s_Instance->Draw(line, positions, color, TexIndicesPoints);

				positions[0] = b;
				positions[1] = c;
				break;

			case Isolines::Lines::Six:
				positions[0] = a;
				positions[1] = c;
				break;

			case Isolines::Lines::Seven:
				positions[0] = d;
				positions[1] = a;
				break;

			case Isolines::Lines::Eight:
				positions[0] = d;
				positions[1] = a;
				break;

			case Isolines::Lines::Nine:
				positions[0] = a;
				positions[1] = c;
				break;

			case Isolines::Lines::Ten:
				positionsTen[0] = c;
				positionsTen[1] = d;
				positionsTen[2] = a;
				positionsTen[3] = b;

				break;

			case Isolines::Lines::Eleven:
				positions[0] = a;
				positions[1] = b;
				break;

			case Isolines::Lines::Tweleve:
				positions[0] = d;
				positions[1] = b;
				break;

			case Isolines::Lines::Thirteen:
				positions[0] = b;
				positions[1] = c;
				break;

			case Isolines::Lines::Fourteen:
				positions[0] = c;
				positions[1] = d;
				break;

			default:
				break;
		};
			
		if (line == Isolines::Lines::Zero || line == Isolines::Lines::Fifteen)
			return;
		if(line == Isolines::Lines::Ten)
			s_Instance->Draw(line, positionsTen, color, TexIndicesPointsTen);
		else
			s_Instance->Draw(line, positions, color, TexIndicesPoints);
}

#pragma once
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

	grid = Grid::Instance(quad_size, lowestISOvalue, highestISOvalue);
	grid->GenerateIsoValues(lowestISOvalue, highestISOvalue);
	GenerateContour();

	//grid->PrintFromHead();
	//grid->CleanUp();
	
	
}

void SandboxLayer::OnDetach()
{
	s_Instance->Shutdown();
	//delete[] contour;
	grid->CleanUp();
	delete grid;
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

		//GenerateContour();
		RenderContour();
		//GenerateQuads();
		//GenerateTriangles();

		s_Instance->EndBatch();
		s_Instance->Flush();
	}
}

void SandboxLayer::OnImGuiRender()
{
	GLCORE_PROFILE_FUNCTION();

	//ImGui::SliderFloat("Quad Size", &quad_size, 0.2, 20.0, "%0.3f");

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
	//srand((unsigned)time(NULL));
	std::cout << "nHorizontalPoints: " << nRows << ", nVerticalPoints: " << nCols << std::endl;
	
	
	Node* head = grid->Head();
	
	Node* temp = head;
	Node* rowRunner = head;
	Node* colRunner = head;

	while (colRunner != nullptr)
	{
		rowRunner = colRunner;
		while (rowRunner != nullptr)
		{
			int facetIndex = 0;

			if (rowRunner->surfaceVal[0] < isoLevel) facetIndex |= 1;
			if (rowRunner->surfaceVal[1] < isoLevel) facetIndex |= 2;
			if (rowRunner->surfaceVal[2] < isoLevel) facetIndex |= 4;
			if (rowRunner->surfaceVal[3] < isoLevel) facetIndex |= 8;
			//std::cout << std::endl << rowRunner->cellID << ", " << facetIndex << std::endl;

			temp = rowRunner;
			rowRunner = rowRunner->right;
			
		}
		colRunner = colRunner->above;
	}
}

void SandboxLayer::RenderContour()
{

	// Draw points
	Node* head = grid->Head();

	Node* temp = head;
	Node* rowRunner = head;
	Node* colRunner = head;

	while (colRunner != nullptr)
	{
		rowRunner = colRunner;

		while (rowRunner != nullptr)
		{
			// head
			if (rowRunner == head)
			{
				for (int i = 0; i < 4; i++)
				{
					GeneratePoints(rowRunner->edgePositions[i], 
						rowRunner->surfaceVal[i] < isoLevel);
				}
			}
			// Bottom most Row right of head
			else if (rowRunner->below == nullptr && rowRunner->above != nullptr &&
				rowRunner->left != nullptr)
			{
				for (int i = 1; i < 3; i++)
				{
					GeneratePoints(rowRunner->edgePositions[i],
						rowRunner->surfaceVal[i] < isoLevel);
				}

			}
			// Left most Column top of head
			else if (rowRunner->below != nullptr && rowRunner->right != nullptr &&
				rowRunner->left == nullptr)
			{
				for (int i = 2; i < 4; i++)
				{
					GeneratePoints(rowRunner->edgePositions[i],
						rowRunner->surfaceVal[i] < isoLevel);
				}
			}
			//All other nodes
			else
			{
				GeneratePoints(rowRunner->edgePositions[2],
				rowRunner->surfaceVal[2] < isoLevel);
			}
			//std::cout << rowRunner->cellID << ", ";
			temp = rowRunner;
			rowRunner = rowRunner->right;
		}
		//std::cout << std::endl;

		colRunner = colRunner->above;
	}


	// Draw facet
	temp = head;
	rowRunner = head;
	colRunner = head;

	while (colRunner != nullptr)
	{
		rowRunner = colRunner;
		while (rowRunner != nullptr)
		{
			int facetIndex = 0;
			
			for (int i = 0; i < 4; i++)
			{
				if (rowRunner->surfaceVal[i] < isoLevel)
					facetIndex |= 1 << i;
			}

			//float surface_point = quad_size / 2.0f;

			float xN = MarchingSquare::InterpolateIntersectionPoint(isoLevel,
				rowRunner->edgePositions[3][0], rowRunner->edgePositions[2][0],
				rowRunner->surfaceVal[3], rowRunner->surfaceVal[2]);

			float yE = MarchingSquare::InterpolateIntersectionPoint(isoLevel, 
				rowRunner->edgePositions[0][1], rowRunner->edgePositions[3][1], 
				rowRunner->surfaceVal[0], rowRunner->surfaceVal[3]);
			float yW = MarchingSquare::InterpolateIntersectionPoint(isoLevel, 
				rowRunner->edgePositions[1][1], rowRunner->edgePositions[2][1], 
				rowRunner->surfaceVal[1], rowRunner->surfaceVal[2]);
			float xS = MarchingSquare::InterpolateIntersectionPoint(isoLevel, 
				rowRunner->edgePositions[0][0], rowRunner->edgePositions[1][0], 
				rowRunner->surfaceVal[0], rowRunner->surfaceVal[1]);
			
			//	/*
			//		D	N   C
			//		E		W
			//		A	S   B
			//	*/
			// A -> B -> C -> D
			// N -> E -> W -> S
			float xA = rowRunner->edgePositions[0][0];
			float yA = rowRunner->edgePositions[0][1];
			float xB = rowRunner->edgePositions[1][0];
			float yC = rowRunner->edgePositions[2][1];

			glm::vec3 surfaceValues[4] =
			{
				{ xN, yC, 0.0f },
				{ xA, yE, 0.0f },
				{ xB, yW, 0.0f },
				{ xS, yA, 0.0f }
			};
			DrawFacet(rowRunner->edgePositions, surfaceValues, facetIndex);
			
			temp = rowRunner;
			rowRunner = rowRunner->right;

		}
		colRunner = colRunner->above;
	}
}

void SandboxLayer::GeneratePoints(glm::vec3 position, float decimalCode) const
{
	std::vector<glm::vec2> TexIndicesPoints =
	{
		{ 0.0f, 0.0f }
	};

	const std::vector<glm::vec3> positions = 
	{
		position
	};
	glm::vec4 color = { decimalCode, decimalCode, decimalCode, 1.0f };
	s_Instance->Draw(Isosurface::Facet::Point, positions, color, TexIndicesPoints);
}

void SandboxLayer::DrawFacet(glm::vec3 edges[4], glm::vec3 surfaceValues[4], int facetIndex) const
{
	if (facetIndex == 0) return;
	
	glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
	Isosurface::Facet facetID = Isosurface::FacetByIndex(facetIndex);
	const ContourFacet contourFacetProperites = MarchingSquare::ContourFacetProperties(facetID);
	std::vector<glm::vec2> texIndicesPoints = contourFacetProperites.texIndicesPoints;
	std::vector<glm::vec3> positions = MarchingSquare::FacetCoordinates(edges, surfaceValues, facetID);


	s_Instance->Draw(facetID, positions, color, texIndicesPoints);
}

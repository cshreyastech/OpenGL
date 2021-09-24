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

	GenerateContour();
}

void SandboxLayer::OnDetach()
{
	s_Instance->Shutdown();
	//delete[] contour;
	delete[] GRID;
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
	std::cout << "nRows: " << nRows << ", nCols: " << nCols << std::endl;


	GRID = new GRIDPOINT* [nRows];
	for (int i = 0; i < nRows; i++)
		GRID[i] = new GRIDPOINT[nCols];


	int row = 0, col = 0;
	for (float y = -rows; y <= rows; y += quad_size)
	{
		col = 0;
		for (float x = -cols; x <= cols; x += quad_size)
		{
			const float isoSurfaceValue = lowestISOvalue + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (highestISOvalue - (lowestISOvalue))));
			GRIDPOINT gridPoint;
			gridPoint.edgePosition = { x, y, 0.0f };
			gridPoint.isoSurfaceValue = isoSurfaceValue;
			GRID[row][col] = gridPoint;
			col++;
		}
		row++;
	}

	//GRID[0][0].isoSurfaceValue = lowestISOvalue;
	//GRID[0][1].isoSurfaceValue = lowestISOvalue;
	//GRID[1][1].isoSurfaceValue = lowestISOvalue;
	//GRID[1][0].isoSurfaceValue = lowestISOvalue;

	//for (int row = 0; row < nRows; row++)
	//{
	//	for (int col = 0; col < nCols; col++)
	//	{
	//		const float isoSurfaceValue = lowestISOvalue + static_cast <float> (rand()) /
	//			(static_cast <float> (RAND_MAX / (highestISOvalue - (lowestISOvalue))));
	//		GRIDPOINT gridPoint;
	//		gridPoint.edgePosition = { row, col };
	//		gridPoint.isoSurfaceValue = isoSurfaceValue;
	//		GRID[row][col] = gridPoint;
	//		std::cout << row << ", " << col << ", " << isoSurfaceValue << std::endl;
	//	}
	//}







	//contour = new float* [nRows];
	//for (int i = 0; i < nRows; i++)
	//	contour[i] = new float[nCols];

	//for (int row = 0; row < nRows; row++)
	//{
	//	for (int col = 0; col < nCols; col++)
	//	{
	//		contour[row][col] = rand() % 2;

	//		std::cout << GRID[row][col].isoSurfaceValue << ", ";
	//	}
	//	std::cout << std::endl;
	//}


	//for (int row = 0; row < nRows - 1; row++)
	//{
	//	for (int col = 0; col < nCols - 1; col++)
	//	{
	//		int cubeindex = 0;

	//		if (GRID[row][col].isoSurfaceValue < isoLevel) cubeindex		 |= 1;
	//		if (GRID[row][col + 1].isoSurfaceValue < isoLevel) cubeindex	 |= 2;
	//		if (GRID[row + 1][col + 1].isoSurfaceValue < isoLevel) cubeindex |= 4;
	//		if (GRID[row + 1][col].isoSurfaceValue < isoLevel) cubeindex	 |= 8;

	//		std::cout << "cubeindex: " << cubeindex << ", ";
	//	}
	//	std::cout << std::endl;
	//}


}

void SandboxLayer::RenderContour()
{
	//int row = 0, col = 0;
	//for (float y = -rows; y <= rows; y += quad_size)
	//{
	//	col = 0;
	//	for (float x = -cols; x <= cols; x += quad_size)
	//	{
	//		GeneratePoints(x, y, contour[row][col]);
	//		col++;
	//	}
	//	row++;
	//}


	for (int row = 0; row < nRows - 1; row++)
	{
		for (int col = 0; col < nCols - 1; col++)
		{
			int facetIndex = 0;
			GRIDPOINT p1 = GRID[row][col];
			GRIDPOINT p2 = GRID[row][col + 1];
			GRIDPOINT p4 = GRID[row + 1][col + 1];
			GRIDPOINT p8 = GRID[row + 1][col];

			if (p1.isoSurfaceValue < isoLevel) facetIndex |= 1;
			if (p2.isoSurfaceValue < isoLevel) facetIndex |= 2;
			if (p4.isoSurfaceValue < isoLevel) facetIndex |= 4;
			if (p8.isoSurfaceValue < isoLevel) facetIndex |= 8;

			//std::cout << "facetIndex: " << facetIndex << ", ";

			GeneratePoints(GRID[row][col].edgePosition, (float)facetIndex / 15.0f);

			/*
				D	N   C
				E		W
				A	S   B
			*/

			/*const glm::vec3 A = { x,			 y, 0.0f };
			const glm::vec3 B = { x + quad_size,			 y, 0.0f };
			const glm::vec3 C = { x + quad_size, y + quad_size, 0.0f };
			const glm::vec3 D = { x, y + quad_size, 0.0f };

			const glm::vec3 N = { x + surface_point, y + quad_size, 0.0f };
			const glm::vec3 E = { x, y + surface_point, 0.0f };
			const glm::vec3 W = { x + quad_size, y + surface_point, 0.0f };
			const glm::vec3 S = { x + surface_point,			 y, 0.0f };*/

			const glm::vec3 A = p1.edgePosition;
			const glm::vec3 B = p2.edgePosition;
			const glm::vec3 C = p4.edgePosition;
			const glm::vec3 D = p8.edgePosition;

			float surface_point = quad_size / 2.0f;
			const glm::vec3 N = { A[0] + surface_point,					D[1], 0.0f };
			const glm::vec3 E = {				  A[0], A[1] + surface_point, 0.0f };
			const glm::vec3 W = {				  B[0], B[1] + surface_point, 0.0f };
			const glm::vec3 S = { A[0] + surface_point,					A[1], 0.0f };

			//GenerateTriangles(1, 1, Isosurface::FacetByIndex(facetIndex));
			GenerateTriangles(A, B, C, D, N, E, W, S, Isosurface::FacetByIndex(facetIndex));
		}
		std::cout << std::endl;
	}


	//float x = -rows, y = -cols;
	//for (int row = 0; row < nRows - 1; row++)
	//{
	//	x = -cols;
	//	for (int col = 0; col < nCols - 1; col++)
	//	{

	//		int isoLine = GetState(contour[row + 1][col], contour[row + 1][col + 1],
	//						contour[row][col + 1], contour[row][col]);
	//		std::cout <<
	//			contour[row + 1][col] << ", " <<
	//			contour[row + 1][col + 1] << ", " <<
	//			contour[row][col + 1] << ", " <<
	//			contour[row][col] << ", " <<
	//			isoLine << 
	//			std::endl;

	//		//GenerateLines(x, y, Isolines::LineByIndex(isoLine));
	//		//GenerateTriangles(x, y, Isolines::LineByIndex(isoLine));
	//		x += quad_size;
	//	}
	//	y += quad_size;
	//}
	//std::cout << "-------------" << std::endl;

}

//int SandboxLayer::GetState(int a, int b, int c, int d)
//{
//	return a * 8 + b * 4 + c * 2 + d * 1;
//}

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


void SandboxLayer::GenerateTriangles(const glm::vec3 A, const glm::vec3 B, const glm::vec3 C, 
	const glm::vec3 D, const glm::vec3 N, const glm::vec3 E, const glm::vec3 W, const glm::vec3 S, 
	Isosurface::Facet facet) const
{

	//float surface_point = quad_size / 2.0f;
	//float row = rows - quad_size;
	//float col = cols - quad_size;
	
	/*
		D	N   C
		E		W
		A	S   B
	*/

	//const glm::vec3 A = {			  x,			 y, 0.0f };
	//const glm::vec3 B = { x + quad_size,			 y, 0.0f };
	//const glm::vec3 C = { x + quad_size, y + quad_size, 0.0f };
	//const glm::vec3 D = {			  x, y + quad_size, 0.0f };

	//const glm::vec3 N = { x + surface_point, y + quad_size, 0.0f };
	//const glm::vec3 E = {			  x, y + surface_point, 0.0f };
	//const glm::vec3 W = { x + quad_size, y + surface_point, 0.0f };
	//const glm::vec3 S = { x + surface_point,			 y, 0.0f };

	glm::vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };

	std::vector<glm::vec3> positions;
	const ContourFacet contourFacetProperites = MarchingSquare::ContourFacetProperties(facet);
	std::vector<glm::vec2> texIndicesPoints = contourFacetProperites.texIndicesPoints;

	switch (facet)
	{
	case Isosurface::Facet::Zero:
		return;
		break;

	case Isosurface::Facet::One:
		positions.emplace_back(A);
		positions.emplace_back(S);
		positions.emplace_back(E);
		break;

	case Isosurface::Facet::Two:
		positions.emplace_back(B);
		positions.emplace_back(W);
		positions.emplace_back(S);
		break;

	case Isosurface::Facet::Three:
		positions.emplace_back(A);
		positions.emplace_back(B);
		positions.emplace_back(W);
		positions.emplace_back(E);
		break;

	case Isosurface::Facet::Four:
		positions.emplace_back(C);
		positions.emplace_back(N);
		positions.emplace_back(W);
		break;

	case Isosurface::Facet::Five:
		positions.emplace_back(A);
		positions.emplace_back(S);
		positions.emplace_back(W);
		positions.emplace_back(C);
		positions.emplace_back(N);
		positions.emplace_back(E);
		break;

	case Isosurface::Facet::Six:
		positions.emplace_back(B);
		positions.emplace_back(C);
		positions.emplace_back(N);
		positions.emplace_back(S);
		break;

	case Isosurface::Facet::Seven:
		positions.emplace_back(A);
		positions.emplace_back(B);
		positions.emplace_back(C);
		positions.emplace_back(N);
		positions.emplace_back(E);
		break;

	case Isosurface::Facet::Eight:
		positions.emplace_back(D);
		positions.emplace_back(E);
		positions.emplace_back(N);
		break;

	case Isosurface::Facet::Nine:
		positions.emplace_back(D);
		positions.emplace_back(A);
		positions.emplace_back(S);
		positions.emplace_back(N);
		break;

	case Isosurface::Facet::Ten:
		positions.emplace_back(B);
		positions.emplace_back(W);
		positions.emplace_back(N);
		positions.emplace_back(D);
		positions.emplace_back(E);
		positions.emplace_back(S);
		break;

	case Isosurface::Facet::Eleven:
		positions.emplace_back(D);
		positions.emplace_back(A);
		positions.emplace_back(B);
		positions.emplace_back(W);
		positions.emplace_back(N);
		break;

	case Isosurface::Facet::Tweleve:
		positions.emplace_back(C);
		positions.emplace_back(D);
		positions.emplace_back(E);
		positions.emplace_back(W);
		break;

	case Isosurface::Facet::Thirteen:
		positions.emplace_back(C);
		positions.emplace_back(D);
		positions.emplace_back(A);
		positions.emplace_back(S);
		positions.emplace_back(W);
		break;

	case Isosurface::Facet::Fourteen:
		positions.emplace_back(B);
		positions.emplace_back(C);
		positions.emplace_back(D);
		positions.emplace_back(E);
		positions.emplace_back(S);
		break;

	case Isosurface::Facet::Fifteen:
		positions.emplace_back(A);
		positions.emplace_back(B);
		positions.emplace_back(C);
		positions.emplace_back(D);
		break;

	default:
		break;
	};

	s_Instance->Draw(facet, positions, color, texIndicesPoints);
}
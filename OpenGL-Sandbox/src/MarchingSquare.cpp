#include "MarchingSquare.h"

float MarchingSquare::InterpolateIntersectionPoint(float isoLevel, float p1, float p2, float surfaceVal1, float surfaceVal2)
{
	float delta = surfaceVal2 - surfaceVal1;

	if (delta == 0) return 0.5f;

	return p1 + (isoLevel - surfaceVal1) * (p2 - p1) / delta;
}

std::vector<glm::vec3> MarchingSquare::FacetCoordinates(glm::vec3 edges[4], glm::vec3 surfaceValues[4], Isosurface::Facet facetID)
{
/*
	D	N   C
	E		W
	A	S   B
*/
	const glm::vec3 A = edges[0];
	const glm::vec3 B = edges[1];
	const glm::vec3 C = edges[2];
	const glm::vec3 D = edges[3];

	const glm::vec3 N = surfaceValues[0];
	const glm::vec3 E = surfaceValues[1];
	const glm::vec3 W = surfaceValues[2];
	const glm::vec3 S = surfaceValues[3];

	std::vector<glm::vec3> positions;

	switch (facetID)
	{
	case Isosurface::Facet::Zero:
		return std::vector<glm::vec3>();
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

	return positions;
}

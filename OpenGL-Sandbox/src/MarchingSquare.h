#pragma once
#include <glm/glm.hpp>
#include <GLCore.h>
#include <array>
#include <glad\glad.h>
#include <unordered_map>
#include<glm/glm.hpp>

namespace Isosurface{

	enum class Facet
	{
		Zero = 0,
		One, Two, Three, Four, Five,
		Six, Seven, Eight, Nine, Ten,
		Eleven, Tweleve, Thirteen, Fourteen, Fifteen,
		Point
	};

	static const Facet allFacet[] = {
		Facet::One, Facet::Two, Facet::Three, Facet::Four, Facet::Five,
		Facet::Six, Facet::Seven, Facet::Eight, Facet::Nine, Facet::Ten,
		Facet::Eleven, Facet::Tweleve, Facet::Thirteen, Facet::Fourteen,
		Facet::Fifteen
	};

	static const Facet FacetByIndex(int i) { return static_cast<Facet>(i); };
}

struct GRIDPOINT {
	glm::vec3 edgePosition;	//position of each corner of the grid in world space
	float isoSurfaceValue;	//value of the function at this grid corner
};

struct ContourFacet
{
	std::vector<int> indexSequence;
	uint32_t vertexOffset;
	std::vector<glm::vec2> texIndicesPoints;
};

class MarchingSquare
{
public:
	static const inline ContourFacet& ContourFacetProperties(Isosurface::Facet facet)
	{
		return contourFacetMap_.at(facet);
	}

	static float InterpolateIntersectionPoint(float isoLevel, float p1, float p2, float surfaceVal1, float surfaceVal2);
	static std::vector<glm::vec3> FacetCoordinates(glm::vec3 edges[4], glm::vec3 surfaceValues[4], Isosurface::Facet facetID);
private:
	MarchingSquare();

private:
	static const inline std::unordered_map<Isosurface::Facet, ContourFacet> contourFacetMap_ =
	{
		{
			Isosurface::Facet::Zero,
			{
				std::vector<int>{ }, 0,
				std::vector<glm::vec2>
				{
				}
			}
		},
		{
			Isosurface::Facet::One,
			{ 
				std::vector<int>{ 0, 1, 2, }, 3,
				std::vector<glm::vec2>
				{ 
					{ 0.0f, 0.0f }, 
					{ 1.0f, 0.0f }, 
					{ 1.0f, 1.0f } 
				}   
			} 
		},
		{
			Isosurface::Facet::Two,
			{
				std::vector<int>{ 0, 1, 2 }, 3,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f }
				}
			}
		},
		{
			Isosurface::Facet::Three,
			{
				std::vector<int>{ 0, 1, 2, 2, 3, 0 }, 4,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.0f, 1.0f }
				}
			}
		},
		{
			Isosurface::Facet::Four,
			{
				std::vector<int>{ 0, 1, 2 }, 3,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f }
				}
			}
		},
		{   
			Isosurface::Facet::Five, 
			{ 
				std::vector<int>{ 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 }, 6,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 0.5f, 0.0f },
					{ 1.0f, 0.5f },
					{ 1.0f, 1.0f },
					{ 0.5f, 1.0f },
					{ 0.0f, 0.5f }
				}
			} 
		},
		{
			Isosurface::Facet::Six,
			{
				std::vector<int>{ 0, 1, 2, 2, 3, 0 }, 4,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.0f, 1.0f }
				}
			}
		},
		{
			Isosurface::Facet::Seven,
			{
				std::vector<int>{ 0, 1, 2, 0, 2, 3, 0, 3, 4 }, 5,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.5f, 1.0f },
					{ 0.0f, 0.5f }
				}
			}
		},
		{
			Isosurface::Facet::Eight,
			{
				std::vector<int>{ 0, 1, 2 }, 3,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f }
				}
			}
		},
		{
			Isosurface::Facet::Nine,
			{
				std::vector<int>{ 0, 1, 2, 2, 3, 0 }, 4,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.0f, 1.0f }
				}
			}
		},
		{
			Isosurface::Facet::Ten,
			{
				std::vector<int>{ 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 }, 6,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 0.5f, 0.0f },
					{ 1.0f, 0.5f },
					{ 1.0f, 1.0f },
					{ 0.5f, 1.0f },
					{ 0.0f, 0.5f }
				}
			}
		},
		{
			Isosurface::Facet::Eleven,
			{
				std::vector<int>{ 0, 1, 2, 0, 2, 3, 0, 3, 4 }, 5,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.5f, 1.0f },
					{ 0.0f, 0.5f }
				}
			}
		},
		{
			Isosurface::Facet::Tweleve,
			{
				std::vector<int>{ 0, 1, 2, 2, 3, 0 }, 4,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.0f, 1.0f }
				}
			}
		},
		{
			Isosurface::Facet::Thirteen,
			{
				std::vector<int>{ 0, 1, 2, 0, 2, 3, 0, 3, 4 }, 5,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.5f, 1.0f },
					{ 0.0f, 0.5f }
				}
			}
		},
		{
			Isosurface::Facet::Fourteen,
			{
				std::vector<int>{ 0, 1, 2, 0, 2, 3, 0, 3, 4 }, 5,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.5f, 1.0f },
					{ 0.0f, 0.5f }
				}
			}
		},
		{
			Isosurface::Facet::Fifteen,
			{
				std::vector<int>{ 0, 1, 2, 2, 3, 0 }, 4,
				std::vector<glm::vec2>
				{
					{ 0.0f, 0.0f },
					{ 1.0f, 0.0f },
					{ 1.0f, 1.0f },
					{ 0.0f, 1.0f }
				}
			}
		}
	};
};
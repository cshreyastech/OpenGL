#pragma once
#include <glm/glm.hpp>
#include <GLCore.h>
#include <array>
#include <glad\glad.h>
#include <unordered_map>
#include<glm/glm.hpp>

namespace Isolines {

	enum class Lines
	{
		Zero = 0,
		One, Two, Three, Four, Five,
		Six, Seven, Eight, Nine, Ten,
		Eleven, Tweleve, Thirteen, Fourteen, Fifteen,
		Point
	};

	static const Lines allLines[] = {
		Lines::One, Lines::Two, Lines::Three, Lines::Four, Lines::Five,
		Lines::Six, Lines::Seven, Lines::Eight, Lines::Nine, Lines::Ten,
		Lines::Eleven, Lines::Tweleve, Lines::Thirteen, Lines::Fourteen,
		Lines::Fifteen
	};

	static const Lines LineByIndex(int i) { return static_cast<Lines>(i); };
}

struct ContourLines
{
	std::vector<int> indexSequence;
	uint32_t vertexOffset;
	std::vector<glm::vec2> texIndicesPoints;
};

class MarchingSquare
{
public:
	static const inline ContourLines& ContourLineProperties(Isolines::Lines line)
	{
		return contourLinesMap_.at(line);
	}

private:
	MarchingSquare();

private:
	static const inline std::unordered_map<Isolines::Lines, ContourLines> contourLinesMap_ =
	{
		{
			Isolines::Lines::Zero,
			{
				std::vector<int>{ }, 0,
				std::vector<glm::vec2>
				{
				}
			}
		},
		{
			Isolines::Lines::One,
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
			Isolines::Lines::Two,
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
			Isolines::Lines::Three,
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
			Isolines::Lines::Four,
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
			Isolines::Lines::Five, 
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
			Isolines::Lines::Six,
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
			Isolines::Lines::Seven,
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
			Isolines::Lines::Eight,
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
			Isolines::Lines::Nine,
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
			Isolines::Lines::Ten,
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
			Isolines::Lines::Eleven,
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
			Isolines::Lines::Tweleve,
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
			Isolines::Lines::Thirteen,
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
			Isolines::Lines::Fourteen,
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
			Isolines::Lines::Fifteen,
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
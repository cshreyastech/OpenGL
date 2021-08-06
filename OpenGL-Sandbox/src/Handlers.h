#pragma once

//enum class ShapeHandlers
//{
//	None = 0,
//	Point, Line, Quad
//};

namespace Isolines {

	enum class Lines
	{
		None = 0,
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
}
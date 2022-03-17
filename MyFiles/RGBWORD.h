#pragma once
class RGBWORD
{
public:
	WORD b;
	WORD g;
	WORD r;

public:
	RGBWORD() : b(0), g(0), r(0) {}
	RGBWORD(WORD gray) : b(gray), g(gray), r(gray) {}
	RGBWORD(const WORD& _r, const WORD& _g, const WORD& _b) : b(-b), g(_g), r(_r) {}
};


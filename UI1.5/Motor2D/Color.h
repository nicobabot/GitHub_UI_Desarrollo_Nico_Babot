#ifndef __COLOR_H__
#define __COLOR_H__

struct Color
{
	//This struct is for RGB colors from 255 to 0, not from 1.0 to 0.0

	int r, g, b, a;

	Color() : r(0), g(0), b(0), a(100)
	{}

	Color(int r, int g, int b, int a = 100) : r(r), g(g), b(b), a(a)
	{}

	void Set(int r, int g, int b, int a = 100)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	/*
	int* operator & ()
	{
		return (int*)this;
	}
	*/

	int operator () (int i)
	{
		int ret = 0;
		if (i >= 0 && i <= 2) {
			switch (i) {
			case 0:
				ret = r;
				break;
			case 1:
				ret = g;
				break;
			case 2:
				ret = b;
				break;
			}
		}
		return ret;
	}
};

extern Color Red;
extern Color Orange;
extern Color Yellow;
extern Color Lime;
extern Color Green;
extern Color LightMediumSeaGreeen;
extern Color Cyan;
extern Color Blue;
extern Color DarkBlue;
extern Color Violet;
extern Color Purple;
extern Color Magenta;
extern Color Black;
extern Color Grey;
extern Color White;

#endif // __COLOR_H__
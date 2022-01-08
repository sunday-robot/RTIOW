#pragma once
class color
{
public:
	double r;
	double g;
	double b;
	color() {}
	color(double r, double g, double b) : r(r), g(g), b(b) {}
	color(const color& c) : r(c.r), g(c.g), b(c.b) {}
};

inline color operator+(const color& c1, const color& c2) {
	return color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}

inline color operator*(const color& c1, const color& c2) {
	return color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}

inline color operator*(double t, const color& c) {
	return color(t * c.r, t * c.g, t * c.b);
}

inline color operator*(const color& c, double t) {
	return t * c;
}

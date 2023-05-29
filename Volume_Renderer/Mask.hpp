#ifndef Mask_hpp
#define Mask_hpp

#include "Header.hpp"

#define MAX_CELL_SHAPES (16)

struct Circle {
	int x, y, sqRadius;
};
struct Cell {
	int N;
	Circle circles[MAX_CELL_SHAPES];
	
	bool PosMasked(const int &x, const int &y) const;
};
class Mask {
public:
	void Init(const int &_width, const int &_height, const int &_maxShapeRadius=40);
	~Mask();
	
	void Clear() const;
	void Add(const Circle &circle) const;
	
	bool PosMasked(const int &x, const int &y) const;
	
	void Draw() const;
	
private:
	int maxShapeRadius;
	int width, height;
	int rows, columns;
	
	Cell *cells = nullptr; // row-major 2d array of cells
	
	vec2 XRtoScreen(const vec2 &v) const;
};

#endif /* Mask_hpp */

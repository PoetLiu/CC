#include "IRectangle.h"
#include <iostream>

class RectangleImp1 : public IRectangle {
public:
	RectangleImp1():m_length(1), m_width(1), m_color(0x00FFEC4D) {}
	virtual ~RectangleImp1() {}
	virtual float GetLength() const { return m_length; }
	virtual void SetLength(float newLength) { m_length = newLength; }
	virtual float GetWidth() const { return m_width; }
	virtual void SetWidth(float newWidth) { m_width = newWidth; }
	virtual RGB GetColor() const { return m_color; }
	virtual void SetColor(RGB newColor) { m_color = newColor; }
	virtual float CalculateArea() const { return m_length * m_width; }
	virtual void Draw() { std::cout << "RectangleImp1::Draw()" << std::endl; }

private:
	float m_length;
	float m_width;
	RGB m_color;
};

IRectangle * IRectangle::CreateRectangle()
{
	return new(std::nothrow) RectangleImp1;
}


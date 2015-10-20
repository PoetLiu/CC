#ifndef __IRECTANGLE_H__
#define __IRECTANGLE_H__

typedef unsigned long RGB;

class IRectangle {
public:
	virtual ~IRectangle() {}
	virtual float GetLength() const = 0;
	virtual void SetLength(float newLength) = 0;
	virtual float GetWidth() const = 0;
	virtual void SetWidth(float newWidth) = 0;
	virtual RGB GetColor() const = 0;
	virtual void SetColor(RGB newColor) = 0;
	virtual float CalculateArea() const = 0;
	virtual void Draw() = 0;
	static IRectangle* CreateRectangle();
	void Destroy() { delete this; }
};

#endif

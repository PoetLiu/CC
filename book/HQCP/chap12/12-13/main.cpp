#include <iostream>
#include <stdlib.h>
#include "IRectangle.h"

using namespace std;
int main(void)
{
	IRectangle *pRect = IRectangle::CreateRectangle();

	if (pRect == NULL)
		exit(-1);
	pRect->SetLength(100.5);
	pRect->SetWidth(35);
	pRect->SetColor(0x00FFFFFF);
	cout << "Length:" << pRect->GetLength() << endl;
	cout << "Witdh:" << pRect->GetWidth() << endl;
	cout << "Area:" << pRect->CalculateArea() << endl;
	cout << "Color:" << pRect->GetColor() << endl;
	pRect->Draw();
	pRect->Destroy();
	pRect	= NULL;

	return 0;
}

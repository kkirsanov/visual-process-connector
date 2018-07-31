#pragma once


typedef vector<int> vecI;
typedef vector<vecI> multivecI;

typedef vector<RGBQUAD> vecC;
typedef vector<vecC> multivecC;

class Mask{
public:
	multivecI mask;
	string name;
	int divisor;
	int bais;
	void resize(int size);
	Mask(int size);
	void Serialize(CArchive& ar);
	~Mask();
};

typedef vector<Mask*> MaskPalette;

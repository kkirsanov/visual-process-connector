#include "StdAfx.h"
#include "Mask.h"


void Mask::resize(int size){
	mask.clear();
	for(int i=0;i<size;i++)
		mask.push_back(vecI(size));
}
Mask::Mask(int size){
	resize(size);
	
	divisor = 0;
	bais	= 0;
};

void Mask::Serialize(CArchive& ar){
	if(ar.IsStoring()){
		ar<<divisor;
		ar<<bais;
		ar<<mask.size();
		for(int y=0;y<mask.size();y++){
			for(int x=0;x<mask.size();x++){
				ar<<mask.at(x).at(y);
			}
		}
	}else{
		ar>>divisor;
		ar>>bais;
		int size;
		ar >>size;
		resize(size);

		for(int y=0;y<size;y++){
			for(int x=0;x<size;x++){
				int val;
				ar>>val;
				mask.at(x).at(y) = val;
			}
		}
	}
}
		

Mask::~Mask(){
	mask.clear();
};

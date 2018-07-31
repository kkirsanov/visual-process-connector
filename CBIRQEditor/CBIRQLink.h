#pragma once

#include "CBIRQElement.h"


#include <boost/iterator/counting_iterator.hpp>
#include <functional>
#include <numeric>


class CCBIRQLink{
	friend class CCBIRQElement;
public:
	CCBIRQLink(){
		start=0;
		end=0;
		startPin=NULL;
		endPin=NULL;
		startpinnum=0;
		endpinnum=0;
		Description = "D";
	}
	UINT start, end;	
	OutPin *startPin;
	InPin *endPin;
	CString Description;
	int startpinnum, endpinnum;

	void CCBIRQLink::Serialize(CArchive &ar)
	{
		if (ar.IsStoring()){
			ar << start;
			ar << end;
			ar << endpinnum;
			ar << startpinnum;
			ar << Description;

		}else{
			ar >> start;
			ar >> end;
			ar >> endpinnum;
			ar >> startpinnum;
			ar >> Description;
		}
	};

};
#pragma once

#ifndef DSUTILS_H_
#define DSUTILS_H_

#include "dshow.h"
#include "qedit.h" //for CLSID_SampleGrabber
#include "uuids.h" //for CLSID_SampleGrabber

namespace dsutils{

	#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
	#endif;
	
	#ifndef FILE_FILTER_TEXT
	#define FILE_FILTER_TEXT  TEXT("Video Files (*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v)\0*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v\0")
	#endif;

	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	HRESULT InitCaptureGraphBuilder(
		IGraphBuilder **ppGraph,  // Receives the pointer.
		ICaptureGraphBuilder2 **ppBuild  // Receives the pointer.
	);
	HRESULT ConnectFilters(
		IGraphBuilder *pGraph, // Filter Graph Manager.
		IPin *pOut,            // Output pin on the upstream filter.
		IBaseFilter *pDest)    // Downstream filter.
		;
	HRESULT ConnectFilters(
		IGraphBuilder *pGraph, 
		IBaseFilter *pSrc, 
		IBaseFilter *pDest
	);

	HRESULT ConnectFilters(
		IGraphBuilder *pGraph, // Filter Graph Manager.
		IPin *pOut,            // Output pin on the upstream filter.
		IBaseFilter *pDest,		// Downstream filter.
		AM_MEDIA_TYPE *pMT)    
		;
	HRESULT ConnectFilters(
		IGraphBuilder *pGraph, 
		IBaseFilter *pSrc, 
		IBaseFilter *pDest,
		AM_MEDIA_TYPE *pMT
	);

	HRESULT GetUnconnectedPin(
		IBaseFilter *pFilter,   // Pointer to the filter.
		PIN_DIRECTION PinDir,   // Direction of the pin to find.
		IPin **ppPin)           // Receives a pointer to the pin.
		;
	HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
	void RemoveFromRot(DWORD pdwRegister);
	HRESULT AddFilterByCLSID( IGraphBuilder *pGraph, const GUID& clsid,  LPCWSTR wszName, IBaseFilter **ppF);
};
#endif
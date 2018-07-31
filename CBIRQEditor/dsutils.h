#ifndef DSUTILS_H_
#define DSUTILS_H_

#include "dshow.h"
#include "qedit.h" //for CLSID_SampleGrabber
#include "uuids.h" //for CLSID_SampleGrabber

namespace ds_utils{
	void Msg(TCHAR *szFormat, ...)	{
		TCHAR szBuffer[1024];  // Large buffer for long filenames or URLs
		const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
		const int LASTCHAR = NUMCHARS - 1;

		// Format the input string
		va_list pArgs;
		va_start(pArgs, szFormat);

		// Use a bounded buffer size to prevent buffer overruns.  Limit count to
		// character size minus one to allow for a NULL terminating character.
		//(void)StringCchVPrintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
		va_end(pArgs);

		// Ensure that the formatted string is NULL-terminated
		szBuffer[LASTCHAR] = TEXT('\0');

		// Display a message box with the formatted string
		MessageBox(NULL, szBuffer, TEXT("PlayWnd Sample"), MB_OK);
	}

	#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }


	#define JIF(x) if (FAILED(hr=(x))) \
		{Msg(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n\0"), hr); return hr;}

	#define LIF(x) if (FAILED(hr=(x))) \
		{Msg(TEXT("FAILED(hr=0x%x) in ") TEXT(#x) TEXT("\n\0"), hr);}

	#define FILE_FILTER_TEXT  TEXT("Video Files (*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v)\0*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v\0")


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
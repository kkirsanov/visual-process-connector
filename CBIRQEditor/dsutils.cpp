#include "dsutils.h"

using namespace ds_utils;

HRESULT ConnectFilters(
		IGraphBuilder *pGraph, 
		IBaseFilter *pSrc, 
		IBaseFilter *pDest)
	{
		if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL))
		{
			return E_POINTER;
		}

		// Find an output pin on the first filter.
		IPin *pOut = 0;
		HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
		if (FAILED(hr)) 
		{
			return hr;
		}
		hr = ConnectFilters(pGraph, pOut, pDest);
		pOut->Release();
		return hr;
	}

	HRESULT ConnectFilters(
		IGraphBuilder *pGraph, // Filter Graph Manager.
		IPin *pOut,            // Output pin on the upstream filter.
		IBaseFilter *pDest)    // Downstream filter.
	{
		if ((pGraph == NULL) || (pOut == NULL) || (pDest == NULL))
			return E_POINTER;
		
		#ifdef debug
			PIN_DIRECTION PinDir;
			pOut->QueryDirection(&PinDir);
			_ASSERTE(PinDir == PINDIR_OUTPUT);
		#endif

		//Find an input pin on the downstream filter.
		IPin *pIn = 0;
		HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
		if (FAILED(hr))
		{
			return hr;
		}
		// Try to connect them.
		hr = pGraph->Connect(pOut, pIn);
		pIn->Release();
		return hr;
	}

HRESULT AddFilterByCLSID( IGraphBuilder *pGraph, const GUID& clsid,  LPCWSTR wszName, IBaseFilter **ppF){
	if (!pGraph || ! ppF) return E_POINTER;
	*ppF = 0;
	IBaseFilter *pF = 0;
	HRESULT hr = CoCreateInstance(clsid, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, reinterpret_cast<void**>(&pF));
	if (SUCCEEDED(hr)){
		hr = pGraph->AddFilter(pF, wszName);
		if (SUCCEEDED(hr))
			*ppF = pF;
		else
			pF->Release();
	};
	return hr;
}

IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
	BOOL bFound = FALSE;
	IEnumPins *pEnum;
	IPin *pPin;
	// Begin by enumerating all the pins on a filter
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
		return NULL;
	}
	// Now look for a pin that matches the direction characteristic.
	// When we've found it, we'll return with it.
	while(pEnum->Next(1, &pPin, 0) == S_OK)
	{
		PIN_DIRECTION PinDirThis;
		pPin->QueryDirection(&PinDirThis);
		if (bFound = (PinDir == PinDirThis))
			break;
		pPin->Release();
	}
	pEnum->Release();
	return (bFound ? pPin : NULL);

}

HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
	
	/*
	IMoniker * pMoniker;
    IRunningObjectTable *pROT;
    if (FAILED(GetRunningObjectTable(0, &pROT))) {
        return E_FAIL;
    }
    //WCHAR wsz[256];
    //wsprintf(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());

	CHAR wsz[256];
    sprintf(wsz, "FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
	char *z = "!";
    HRESULT hr = CreateItemMoniker(z, wsz, &pMoniker);
    if (SUCCEEDED(hr)) {
        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
            pMoniker, pdwRegister);
        pMoniker->Release();
    }
    pROT->Release();
	*/
	HRESULT hr=S_OK;
    return hr;
}

void RemoveFromRot(DWORD pdwRegister)
{
    IRunningObjectTable *pROT;
    if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
        pROT->Revoke(pdwRegister);
        pROT->Release();
    }
}



HRESULT GetUnconnectedPin(
    IBaseFilter *pFilter,   // Pointer to the filter.
    PIN_DIRECTION PinDir,   // Direction of the pin to find.
    IPin **ppPin)           // Receives a pointer to the pin.
{
    *ppPin = 0;
    IEnumPins *pEnum = 0;
    IPin *pPin = 0;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }
    while (pEnum->Next(1, &pPin, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;
        pPin->QueryDirection(&ThisPinDir);
        if (ThisPinDir == PinDir)
        {
            IPin *pTmp = 0;
            hr = pPin->ConnectedTo(&pTmp);
            if (SUCCEEDED(hr))  // Already connected, not the pin we want.
            {
                pTmp->Release();
            }
            else  // Unconnected, this is the pin we want.
            {
                pEnum->Release();
                *ppPin = pPin;
                return S_OK;
            }
        }
        pPin->Release();
    }
    pEnum->Release();
    // Did not find a matching pin.
    return E_FAIL;
}

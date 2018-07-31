#pragma once

#include "Blocksock.h"

class Server
{
public:
	Server(DWORD ip, UINT port);
	void Listen();
	int Start(PImage pImage,Counter counter, UINT port=0);	
	void* Recive(int &type, Counter &counter);
	BOOL working;
public:
	~Server(void);
private:
	UINT port;
	DWORD ip;
	CBlockingSocket  sockRecv;
	CBlockingSocket  sockServer;
};

//============================================================================
// Name        : server.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "server.hpp"

using namespace WebSocket;
class WebSocketServer
{
private:
	int intPorta;
	bool boolExecutando;
	//TcpListener tcplWebSocketServer;
	//Thread threadEsperaSocketCliente;

	void error(const char *msg)
	{
		perror(msg);
		exit(1);
	}


public:
	WebSocketServer()
	{
		intPorta = 0;
		boolExecutando = false;
	}

	bool isBoolExecutando() const {
		return boolExecutando;
	}

	int getIntPorta() const {
		return intPorta;
	}

	void setIntPorta(int intPorta) {
		if (!boolExecutando)
		{
			this->intPorta = intPorta;
		}

	}

	void Start()
	{
		if (!boolExecutando)
		{
			//tcplWebSocketServer = new TcpListener(IPAddress.Any, intPorta);
			//threadEsperaSocketCliente = new Thread(EsperarSocketCliente);
			this.OnBeforeStart();
			//tcplWebSocketServer.Start();
			this.OnAfterStart();
			boolExecutando = true;
			//threadEsperaSocketCliente.Start();
		}
	}
/*
	void Stop()
	{
		if (boolExecutando)
		{
			boolExecutando = false;
			threadEsperaSocketCliente.Abort();
			this.OnBeforeStop();
			tcplWebSocketServer.Stop();
			this.OnAfterStop();
			threadEsperaSocketCliente = null;
			tcplWebSocketServer = null;
		}
	}

private:
	void EsperarSocketCliente()
	{
		while (boolExecutando)
		{
			new Thread(OnAcceptSocketClienteInterno).Start(tcplWebSocketServer.AcceptSocket());
		}
	}

	void OnAcceptSocketClienteInterno(object o)
	{
		OnAcceptSocketCliente((Socket)o);
	}
*/
public:
	//virtual void OnAcceptSocketCliente(Socket sockCliente);
	virtual void OnBeforeStart();
	virtual void OnAfterStart();
	virtual void OnBeforeStop();
	virtual void OnAfterStop();
	virtual ~WebSocketServer(); //não sei pq precisa..
};

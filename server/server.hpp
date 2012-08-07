/*
 * server.hpp
 *
 *  Created on: Aug 4, 2012
 *      Author: ricardo
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

namespace WebSocket
{
    class WebSocketServer
    {
    private:
    	int intPorta;
        bool boolExecutando;
        //TcpListener tcplWebSocketServer;
        //Thread threadEsperaSocketCliente;

        void error(const char *msg);

    public:
    	bool isBoolExecutando();
    	int getIntPorta();
    	void setIntPorta(int intPorta);
    	void Start();
        void Stop();

    private:
        void EsperarSocketCliente();
//        void OnAcceptSocketClienteInterno(object o);

    public:
        //virtual void OnAcceptSocketCliente(Socket sockCliente);
    	virtual void OnBeforeStart();
        virtual void OnAfterStart();
        virtual void OnBeforeStop();
        virtual void OnAfterStop();
        virtual ~WebSocketServer(); //não sei pq precisa..
    };


}



#endif /* SERVER_HPP_ */

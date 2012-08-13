/*
 * SimpleWebSocketServer.cpp
 *
 *  Created on: Aug 8, 2012
 *      Author: ricardo, the wizard
 */

#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include "SHA1.h"			  // For SHA-1 encoding
#include "base64.h"
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>


using namespace std;

void test(){

}

inline const unsigned int rol(const unsigned int num, const unsigned int cnt)
{
	return((num << cnt) | (num >> (32-cnt)));
}

void innerHash(unsigned int *result, unsigned int *w)
{
	unsigned int save[5];
	save[0]=result[0];
	save[1]=result[1];
	save[2]=result[2];
	save[3]=result[3];
	save[4]=result[4];

	#define a result[0]
	#define b result[1]
	#define c result[2]
	#define d result[3]
	#define e result[4]

	int j=0;
	#define sha1macro(func,val) \
		{const unsigned int t = rol(a, 5)+(func)+e+val+w[j]; \
		e = d; d = c; \
		c = rol(b, 30); \
		b = a; a = t;}
	while(j<16)
	{
		sha1macro((b&c)|(~b&d),0x5A827999)
		j++;
	}
	while(j<20)
	{
		w[j] = rol((w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16]), 1);
		sha1macro((b&c)|(~b&d),0x5A827999)
		j++;
	}
	while(j<40)
	{
		w[j] = rol((w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16]), 1);
		sha1macro(b^c^d,0x6ED9EBA1)
		j++;
	}
	while(j<60)
	{
		w[j] = rol((w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16]), 1);
		sha1macro((b&c)|(b&d)|(c&d),0x8F1BBCDC)
		j++;
	}
	while(j<80)
	{
		w[j] = rol((w[j-3] ^ w[j-8] ^ w[j-14] ^ w[j-16]), 1);
		sha1macro(b^c^d,0xCA62C1D6)
		j++;
	}
	#undef sha1macro
	#undef a
	#undef b
	#undef c
	#undef d
	#undef e

	result[0]+=save[0];
	result[1]+=save[1];
	result[2]+=save[2];
	result[3]+=save[3];
	result[4]+=save[4];
}

void calc(const void *src, const int bytelength, unsigned char *hash)
	{
		// Init the result array, and create references to the five unsigned integers for better readabillity.
		unsigned int result[5]={0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0};

		const unsigned char *sarray=(const unsigned char*)src;
		// The variables
		unsigned int w[80];
		int j,i,i1;
		j=0;
		// Loop through all complete 64byte blocks.
		for(i=0,i1=64; i<=(bytelength-64); i=i1,i1+=64)
		{
			int k=0;
			for(j=i;j<i1;j+=4)
			{
				// This line will swap endian on big endian and keep endian on little endian.
				w[k++]=(unsigned int)sarray[j+3]|(((unsigned int)sarray[j+2])<<8)|(((unsigned int)sarray[j+1])<<16)|(((unsigned int)sarray[j])<<24);
			}
			innerHash(result,w);
		}
		// fill in reminder
		i1=bytelength-i;
		memset(w,0,sizeof(unsigned int)*16);
		for(j=0;j<i1;j++)
		{
			w[j>>2]|=(unsigned int)sarray[j+i]<<((3-(j&3))<<3);
		}
		w[j>>2]|=0x80<<((3-(j&3))<<3);
		if(i1>=56)
		{
			innerHash(result,w);
			memset(w,0,sizeof(unsigned int)*16);
		}
		w[15]=bytelength<<3;
		innerHash(result,w);
		// Store hash in result pointer, and make sure we get in in the correct order on both endian models.
		for(i=20;--i>=0;)
		{
			hash[i]=(result[i>>2]>>(((3-i)&0x3)<<3))&0xFF;
		}
	}

	void toHexString(const unsigned char *hash, char *hexstring)
	{
		const char tab[]={"0123456789abcdef"};
		for(int i=20;--i>=0;)
		{
			hexstring[i<<1]=tab[(hash[i]>>4)&0xF];
			hexstring[(i<<1)+1]=tab[hash[i]&0xF];
		}
		hexstring[40]=0;
	}




const unsigned int RCVBUFSIZE = 32;    // Size of receive buffer


std::string replace(const std::string s, const std::string toBeReplaced, const std::string toBePut) {
	size_t pos;
	string result = string(s);
	while ((pos = result.find(toBeReplaced)) != std::string::npos)
	{
		result = result.replace(pos, toBeReplaced.length(), toBePut.c_str());
	}
	return result;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}



void HandleTCPClient(TCPSocket *sock = NULL); // TCP client handling function

int main(int argc, char *argv[]) {

	unsigned short echoServPort = 8080;  // First arg: local port

	if (argc >= 2) {                     // Test for correct number of arguments
		echoServPort = atoi(argv[1]);  // First arg: local port
	}

	cout << "Listening at port " << echoServPort << endl;

	try {
		TCPServerSocket servSock(echoServPort);     // Server Socket object

		for (;;) {   // Run forever
			HandleTCPClient(servSock.accept());  // Wait for a client to connect
			//break;
		}
	} catch (SocketException &e) {
		cerr << e.what() << endl;
		exit(1);
	}
	// NOT REACHED

	return 0;
}

// TCP client handling function
void HandleTCPClient(TCPSocket *sock) {

	cout << "Handling client ";
	try {
		cout << sock->getForeignAddress() << ":";
	} catch (const SocketException & e) {
		cerr << "Unable to get foreign address" << endl;
	}
	try {
		cout << sock->getForeignPort();
	} catch (const SocketException & e) {
		cerr << "Unable to get foreign port" << endl;
	}
	cout << endl;

	// Send received string and receive again until the end of transmission
	char echoBuffer[RCVBUFSIZE];
	unsigned int recvMsgSize;

	stringstream buffer;

	while ((recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE)) > 0) { // Zero means
		// end of transmission
		int pos;
		if ((pos = string(echoBuffer).find("\r\n\r\n")) != string::npos)
		{
			echoBuffer[pos] = 0;
			buffer << echoBuffer;
			break;
		}

		buffer << echoBuffer;

	}

	//buffer << "Handling client 127.0.0.1:39210\r\nGET / HTTP/1.1\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nHost: localhost:8080\r\nOrigin: null\r\nSec-WebSocket-Key: 6H3KOPvZDqggL+krxcLVrA=\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Extensions: x-webkit-deflate-frame";
	cout << buffer.str() << endl;
	
	string msgRcv = string(buffer.str());
	msgRcv = replace(msgRcv, "\r", "");
	vector<string> lines = split(msgRcv, '\n');

	string key;
	for (vector<string>::iterator i = lines.begin(); i != lines.end(); i++)
	{
		size_t pos;
		string line = (string) *i;
		if (line.find("Sec-WebSocket-Key") != string::npos)
		{
			pos = line.find(":") + 2;
			key = line.substr(pos);
		}
	}

	cout << "rcvkey:" << key << endl;
    key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11\0";
    cout << "anskey:" << key << endl;

    unsigned char answer1[256];
    for(int i = 0; i < 256; i++) answer1[i] = 0;

    cout << "clear: " << answer1 << endl;

    calc((const void *)key.c_str(), (unsigned int)key.length(), answer1);
    cout << "baseX: " << answer1 << endl;
    cout << "base64: " << base64_encode(answer1, 20) << endl;

 /*
    SHA1 sha = new SHA1CryptoServiceProvider();
    return Convert.ToBase64String(sha.ComputeHash(Encoding.UTF8.GetBytes((String)secWebSocketKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11")));
}
   */

	string answer2 =
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: " + base64_encode(answer1, 20) + "\r\n\r\n";
		//"Sec-WebSocket-Protocol: chat\r\n\r\n";

	cout << endl <<  answer2 << endl;
	sock->send(answer2.c_str(), strlen(answer2.c_str()));


	while((recvMsgSize = sock->recv(buffer, RCVBUFSIZE)) > 0) {
	  cout << buffer << endl;
	  cout.flush();
	}
	  


    delete sock;
    return;

}



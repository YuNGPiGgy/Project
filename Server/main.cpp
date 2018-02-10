#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include <windows.h>
#include <process.h>

using namespace std;

webSocket server;

/* called when a client connects */
void openHandler(int clientID){
    ostringstream os;
    os << "Stranger " << clientID << " has joined.";

    vector<int> clientIDs = server.getClientIDs();
	int channelNum = server.getClientPort(clientID);
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID && server.getClientPort(clientIDs[i]) == channelNum)
            server.wsSend(clientIDs[i], os.str());
    }
    server.wsSend(clientID, "Welcome!");
}


/* called when a client disconnects */
void closeHandler(int clientID){
    ostringstream os;
    os << "Stranger " << clientID << " has left.";

    vector<int> clientIDs = server.getClientIDs();
	int channelNum = server.getClientPort(clientID);
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID && server.getClientPort(clientIDs[i]) == channelNum)
            server.wsSend(clientIDs[i], os.str());
    }
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){			//check which port the client is in and only send message to that group of clients
    ostringstream os;
    os << "Stranger " << clientID << " says: " << message;

    vector<int> clientIDs = server.getClientIDs();
	int channelNum = server.getClientPort(clientID);
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID && server.getClientPort(clientIDs[i]) == channelNum)
            server.wsSend(clientIDs[i], os.str());
    }
}

/* called once per select() loop */
void periodicHandler(){
    static time_t next = time(NULL) + 10;
    time_t current = time(NULL);
    if (current >= next){
        ostringstream os;
		//Deprecated ctime API in Windows 10
		char timecstring[26];
		ctime_s(timecstring, sizeof(timecstring), &current);
		string timestring(timecstring);
        timestring = timestring.substr(0, timestring.size() - 1);
        os << timestring;

        vector<int> clientIDs = server.getClientIDs();
        for (int i = 0; i < clientIDs.size(); i++)
            server.wsSend(clientIDs[i], os.str());

        next = time(NULL) + 10;
    }
}


int main(int argc, char *argv[]){

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);

    /* start the chatroom server, listen to ip '127.0.0.1' and ports '8000'-'8003' */
	int ports[] = { 8000, 8001, 8002, 8003 };

	server.startServer(ports);

    return 1;
}

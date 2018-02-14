#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include <windows.h>
#include <process.h>
#include "pong50926235.h"

using namespace std;

webSocket server;
map<int, string> clientID_username_map;
Pong pong;


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

/***********************************************************************
 *  Parses strings in the format :
 *  " username | ballPosX | ballPosY | ballDirX | ballDirY | paddleTop | INPUT_KEYS_STRING"
 *  the INPUT_KEYS_STRING will be a string of key characters (i.e. w or W or s or S) since last update packet
 * @param clientID
 * @param message
 ***********************************************************************/
void parseStringUpdatePacket(int clientID, string message){
    vector<string> tokens = split(message);
    if(!clientID_username_map.contains(clientID, tokens.at(0))){
        clientID_username_map.insert(clientID, tokens.at(0));
    }

    pong.updateBall()

}
vector<string> split(string toSplit){
    stringstream ss($toSplit);
    string item;
    vector<string> tokens;
    while(getline(ss, item, '|')){
        tokens.push_back(item);
    }
    return tokens;
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

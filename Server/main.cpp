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
Pong pong(600, 800);

void parseStringUpdatePacket(int clientID, string message);
vector<string> split(string toSplit);


/* called when a client connects */
void openHandler(int clientID){
    ostringstream os;
    os << "Stranger " << clientID << " has joined.";

    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID)
            server.wsSend(clientIDs[i], os.str());
    }
    server.wsSend(clientID, "Welcome!");
}


/* called when a client disconnects */
void closeHandler(int clientID){
    ostringstream os;
    os << "Stranger " << clientID << " has left.";

    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID )
            server.wsSend(clientIDs[i], os.str());
    }
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){			//check which port the client is in and only send message to that group of clients
    ostringstream os;

    //vector<int> clientIDs = server.getClientIDs();
	//int channelNum = server.getClientPort(clientID);
    parseStringUpdatePacket(clientID, message);
}

/* called once per select() loop */
void periodicHandler(){
    static time_t next = time(NULL) + 5;
    time_t current = time(NULL);
    if (current >= next){
        ostringstream os;

        os << pong.getGameState();

        vector<int> clientIDs = server.getClientIDs();
        for (int i = 0; i < clientIDs.size(); i++)
            server.wsSend(clientIDs[i], os.str());

        next = time(NULL) + 5;
    }
}

/***********************************************************************
 *  Parses strings in the format :
 *  " username | ballPosX | ballPosY | ballDirX | ballDirY | paddleTop | INPUT_KEYS_STRING"
 *  the INPUT_KEYS_STRING will be a string of key characters (i.e. w or W or s or S) since last update packet
 *
 *  will send input data to pong object for simulation through methods:
 *  updateBall(double, double, double, double)
 *  updatePaddle(double)
 *  updateInputs(string)
 *
 ***********************************************************************/
void parseStringUpdatePacket(int clientID, string message){
    vector<string> tokens = split(message);
    if(!clientID_username_map.count(clientID)){
        clientID_username_map.insert(pair<int, string>(clientID, tokens.at(0)));
    }

    double ballXpos = stoi(tokens[1]);
    double ballYpos = stoi(tokens[2]);
    double ballXdir = stoi(tokens[3]);
    double ballYdir = stoi(tokens[4]);
    double paddleTop = stoi(tokens[5]);

    pong.updateBall(ballXpos, ballYpos, ballXdir, ballYdir);

    pong.updatePaddle(paddleTop);

    pong.updateInputs(tokens[6]);
}

/***********************************************************************
 * Split string into tokenized vector<string>
 ***********************************************************************/
vector<string> split(string toSplit){
    stringstream ss(toSplit);
    string item;
    vector<string> tokens;
    while(getline(ss, item, '|')){
        tokens.push_back(item);
    }
    return tokens;
}

int main(int argc, char *argv[])
{

	/* set event handler */
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
    server.setPeriodicHandler(periodicHandler);


    /* start the chatroom server, listen to ip '127.0.0.1' and ports '8000'-'8003' */
//	int ports[] = { 8000, 8001, 8002, 8003 };

	server.startServer(8000);

    return 1;
}

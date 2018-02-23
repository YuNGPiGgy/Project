#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include <windows.h>
#include <process.h>
#include "pong50926235.h"
#define INTERVAL_MS 10

using namespace std;

webSocket server;
map<int, string> clientID_username_map;
Pong pong(600, 800);
int playerCount = 0;

void parseStringUpdatePacket(int clientID, string message);
vector<string> split(string toSplit);


/* called when a client connects */
void openHandler(int clientID){

    server.wsSend(clientID, to_string(playerCount));
}


/* called when a client disconnects */
void closeHandler(int clientID){
	pong.pause();    

    vector<int> clientIDs = server.getClientIDs();
    for (int i = 0; i < clientIDs.size(); i++){
        if (clientIDs[i] != clientID )
            server.wsSend(clientIDs[i], "pause");
    }
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){			//check which port the client is in and only send message to that group of clients
   // ostringstream os;

    //vector<int> clientIDs = server.getClientIDs();
	//int channelNum = server.getClientPort(clientID);
    parseStringUpdatePacket(clientID, message);
}

/* called once per select() loop */
int interval_clocks = CLOCKS_PER_SEC * INTERVAL_MS / 1000;

void periodicHandler(){
    static clock_t next = clock() + interval_clocks;
    clock_t current = clock();
    if (current >= next){

        vector<int> clientIDs = server.getClientIDs();
        for (int i = 0; i < clientIDs.size(); i++)
            server.wsSend(clientIDs[i], pong.getGameState());

        next = time(NULL) + 5;
    }
}

/***********************************************************************
 *  Parses strings in the format (without spaces):
 *  " username | ballPosX | ballPosY | ballDirX | ballDirY | paddleTop | paddleLeft | INPUT_KEYS_STRING"
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
    if(!clientID_username_map.count(clientID) && playerCount <= 4){
        clientID_username_map.insert(pair<int, string>(clientID, tokens.at(0)));
		playerCount++;
    }
	Pong::PLAYER player = static_cast<Pong::PLAYER>(stoi(tokens.at(0)));

    int ballXpos = stoi(tokens[1]);
    int ballYpos = stoi(tokens[2]);
    int ballXdir = stoi(tokens[3]);
    int ballYdir = stoi(tokens[4]);
    int paddleTop = stoi(tokens[5]); //will use later < VV 
	int paddleLeft = stoi(tokens[6]);

    //pong.updateBall(ballXpos, ballYpos, ballXdir, ballYdir);

    pong.updateInputs(player, tokens[7]);
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
    //server.setPeriodicHandler(periodicHandler);


    /* start the chatroom server, listen to ip '127.0.0.1' and ports '8000'-'8003' */
    int ports[] = { 8000, 8001, 8002, 8003 };

	server.startServer(ports);

    return 1;
}
